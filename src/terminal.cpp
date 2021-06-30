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
            send("\n");
        }
    } else if (ch >= ' ') {
        buffer[bufferUsed++] = ch;

        if (bufferUsed >= BufferSize - 1) {
            send("Cmd: Buffer too long\n");
            bufferUsed = 0;
        }
    } else if (ch == 8) {
        if (bufferUsed > 0) {
            --bufferUsed;
        }
    }
}

void Terminal::processCommand(char *cmd) {
    const char *command = strtok(cmd, " ");
    const char *param = strtok(nullptr, " ");

    if (strcmp(command, "mem") == 0) {
        send("Heap: %d, PSHeap: %d\n", ESP.getFreeHeap(), ESP.getFreePsram());
    } else if (strcmp(command, "log") == 0) {
        if (param) {
            if (strcmp(param, "on") == 0) {
                const char *param2 = strtok(nullptr, " ");
                send("Logging on\n");
                logger.enable(true);
                if (param2) {
                    uint8_t level = atoi(param2);
                    if (level == 0) {
                        level = 2;
                    }
                    logger.setLevel(level);
                }
            } else if (strcmp(param, "off") == 0) {
                send("Logging off\n");
                logger.enable(false);
            }
        } else {
            send("Logging: %s %d\n", logger.isEnabled()? "on" : "off", logger.getLevel());
        }
    } else if (strcmp(command, "dir") == 0) {
        doDir(param);
    } else if (strcmp(command, "cat") == 0) {
        doCat(param);
    } else if (strcmp(command, "wifi") == 0) {
        if (param) {
            const char *param2 = strtok(nullptr, " ");
            if (param2) {
                WifiSSID = param;
                WifiPassword = param2;
                WiFi.disconnect();
                send("Changed Wifi Connection parameters\n");
                IniFile::save(SETTINGS_FILENAME);
            }
        } else {
            if (WiFi.status() != WL_CONNECTED) {
                send("Wifi not connected -%s\n", WifiSSID.c_str());
            } else {
                send("Wifi connected - %s - %s dB - %s\n",
                WifiSSID.c_str(), WiFi.RSSI(), WiFi.localIP().toString().c_str());
            }
        }
    } else {
        send("Unknown command: %s\n", command);
    }
}

void Terminal::send(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vsnprintf(sendBuffer, SendBufferSize, fmt, args);
    va_end(args);
    internalSend(sendBuffer);
}

void Terminal::doDir(const char *param) {
    File root = SD.open("/");
    if (!root) {
        send("Failed to open directory\n");
        return;
    }
    File file = root.openNextFile();
    while (file) {
        if (file.isDirectory()) {
            send("%-40.40s <DIR>\n", file.name());
        } else {
            send("%-40.40s %d\n", file.name(), file.size());
        }
        file = root.openNextFile();        
    }
}

void Terminal::doCat(const char *param) {
   File file = SD.open(param);
   if (!file) {
       send("Failed to open file\n");
       return;
   } 
   while (file.available()) {
       send("%c", file.read());
   }
   file.close();
}