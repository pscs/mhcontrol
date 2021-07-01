#include "terminal.h"

#include <Arduino.h>
//#include <dirent.h>
#include "FS.h"
#include "SD.h"
#include <WiFi.h>
#include "Controller.h"
#include "inifile.h"
#include "config.h"

#include "logger.h"

const uint16_t SendBufferSize = 1000;
const uint16_t BufferSize = 100;

Terminal::Terminal() {

}

Terminal::~Terminal() {
    free(buffer);
    free(sendBuffer);
}

void Terminal::initialise() {
    bufferSize = BufferSize;
    buffer = reinterpret_cast<char *>(ps_malloc(BufferSize));
    sendBuffer = reinterpret_cast<char *>(ps_malloc(SendBufferSize));
}

void Terminal::processCharacter(char ch) {
    if (ch == '\n') {
        if (bufferUsed > 0) {
            buffer[bufferUsed] = 0;
            processCommand(buffer);
            bufferUsed = 0;
        } else {
            print("\r\n");
        }
        print(getPrompt());
    } else if (ch >= ' ') {
        buffer[bufferUsed++] = ch;

        if (bufferUsed >= bufferSize - 1) {
            print("Cmd: Buffer too long\r\n");
            bufferUsed = 0;
        }
    } else if (ch == 8) {
        if (bufferUsed > 0) {
            --bufferUsed;
        }
    }
    yield();
}

void Terminal::processCommand(char *cmd) {
    auditLogger.print(LOG_TERMINAL, LOG_INFO, "Terminal (%c) command: %s\n", getType(), cmd);

    const char *command = strtok(cmd, " ");
    const char *param = strtok(nullptr, " ");

    if (strcmp(command, "mem") == 0) {
        printf("Heap: %d, PSHeap: %d\r\n", ESP.getFreeHeap(), ESP.getFreePsram());
    } else if (strcmp(command, "log") == 0) {
        if (param) {
            if (strcmp(param, "on") == 0) {
                const char *param2 = strtok(nullptr, " ");
                print("Logging on\r\n");
                logger.enable(true);
                if (param2) {
                    uint8_t level = atoi(param2);
                    if (level == 0) {
                        level = 2;
                    }
                    logger.setLevel(level);
                    IniFile::save(SETTINGS_FILENAME);
                }
            } else if (strcmp(param, "off") == 0) {
                print("Logging off\r\n");
                logger.enable(false);
                IniFile::save(SETTINGS_FILENAME);
            }
        } else {
            printf("Logging: %s level %d\r\n", logger.isEnabled()? "on" : "off", logger.getLevel());
        }
    } else if (strcmp(command, "help") == 0) {
        print("dir - directory listing of SD card\r\n");
        print("cat <filename> - display contents of file from SD card\r\n");
        print("quit - exit Telnet session\r\n");
        print("reboot - reboot\r\n");
        print("wifi - show Wifi details\r\n");
        print("wifi <ssid> <password> - change Wifi connection\r\n");
        print("log - show main logging details\r\n");
        print("log off - turn off main logging\r\n");
        print("log on [<level 0-5>] - turn on main logging, optionally set log level\r\n");
        print("mem - show free memory\r\n");
    }
    } else if (strcmp(command, "dir") == 0) {
        doDir(param);
    } else if (strcmp(command, "cat") == 0) {
        if (param) {
            doCat(param);
        }
    } else if (strcmp(command, "reboot") == 0) {
        ESP.restart();
    } else if ((strcmp(command, "quit") == 0) && (getType() == 'T')) {
        doQuit();
    } else if (strcmp(command, "wifi") == 0) {
        if (param) {
            const char *param2 = strtok(nullptr, " ");
            if (param2) {
                WifiSSID = param;
                WifiPassword = param2;
                WiFi.disconnect();
                print("Changed Wifi Connection parameters\r\n");
                IniFile::save(SETTINGS_FILENAME);
            }
        } else {
            if (WiFi.status() != WL_CONNECTED) {
                printf("Wifi not connected -%s\r\n", WifiSSID.c_str());
            } else {
                printf("Wifi connected - %s - %d dB - %s\r\n",
                    WiFi.SSID().c_str(), WiFi.RSSI(), WiFi.localIP().toString().c_str());
            }
        }
    } else {
        printf("Unknown command: %s\r\n", command);
    }
}

void Terminal::printf(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vsnprintf(sendBuffer, SendBufferSize, fmt, args);
    va_end(args);
    internalSend(sendBuffer);
}

void Terminal::doDir(const char *param) {
    File root = SD.open("/");
    if (!root) {
        print("Failed to open directory\r\n");
        return;
    }
    File file = root.openNextFile();
    while (file) {
        if (file.isDirectory()) {
            printf("%-40.40s <DIR>\r\n", file.name());
        } else {
            printf("%-40.40s %d\r\n", file.name(), file.size());
        }
        file = root.openNextFile();    
        yield();    
    }
}

void Terminal::doCat(const char *param) {
    char filename[strlen(param) + 2];
    if (param[0] != '/') {
        snprintf(filename, strlen(param) + 2, "/%s", param);
    } else {
        strcpy(filename, param);
    }

    File file = SD.open(filename);
    if (!file) {
        print("Failed to open file\r\n");
        return;
    } 
    char buffer[1000];
    int sz = 0;
    while (file.available()) {
        char ch = file.read();
        if (ch == '\n') {
            buffer[sz++] = '\r';
            buffer[sz++] = ch;
        } else {
            buffer[sz++] = ch;
        }
        if (sz >= 997) {
            buffer[sz] = 0;
            print(buffer);
            sz = 0;
            yield();
        }
    }
    buffer[sz] = 0;
    print(buffer);
    file.close();
}