#include "terminal.h"

#include <Arduino.h>
//#include <dirent.h>
#include "FS.h"
#include "SD.h"
#include <WiFi.h>
#include "Controller.h"
#include "inifile.h"
#include "config.h"
#include "ksenergy_battery.h"
#include "victron_smartsolar.h"
#include "victron_B2B.h"
#include "victron_mainscharger.h"

#include "logger.h"
#include "telnet_terminal.h"

const uint16_t SendBufferSize = 1000;
const uint16_t BufferSize = 100;
const uint16_t LastCommandBufferSize = BufferSize;

struct Setting {
    const char *name;
    void (*set)(const char *value);
    void (*show)(char *value, int maxLength);
};

Setting settings[] {
    {
        "password", 
        [](const char *value) {telnetTerminal.setPassword(value);},
        [](char *value, int) {strcpy(value, "<hidden>");}
    },/*
    {
        "log",
        [](const char *value) {
            if (strcmp(value, "off") == 0) {
                logger.enable(false);
            } else {
                logger.enable(true);
                logger.setLevel(atoi(value));
            }
        },
        [](char *value, int size) {snprintf(value, size, "logging is %s - level %d", logger.isEnabled() ? "on": "off", logger.getLevel());}
    },*/
    {
        "log",
        [](const char *value) {
            if (strcmp(value, "off") == 0) {
                logger.enable(false);
            } else if (strcmp(value, "on") == 0) {
                logger.enable(true);
            } else if (isdigit(value[0])) {
                logger.setAllLevels(atoi(value));
            } else {
                char *str = strdup(value);
                const char *name = strtok(str, " ");
                const char *levelStr = strtok(nullptr, " ");
                if (levelStr) {
                    int8_t level = atoi(levelStr);
                    if (strcmp(name, "general") == 0) {
                        logger.setLevel(LOG_GENERAL, level);
                    } else {
                        for (int i = LOG_GENERAL + 1; i < NUM_LOG_SOURCES; i++) {
                            if (strcmp(logger.getComponentName(static_cast<LogSource>(i)), name) == 0) {
                                logger.setLevel(static_cast<LogSource>(i), level);
                            }
                        }
                    }
                }
                free(str);
            }
        },
        [](char *value, int size) {
                strcpy(value, logger.isEnabled() ? "enabled" : "disabled");
                for (int i = 0; i < NUM_LOG_SOURCES; i++) {
                    int len = strlen(value);
                    const char *name = (i == 0) ? "general" : logger.getComponentName(static_cast<LogSource>(i));
                    snprintf(value + len, size - len, "\r\n  %s: %s(%d)",
                        name , logger.getLevelName(static_cast<LogSource>(i)), logger.getLevel(static_cast<LogSource>(i)));
                }
            }
    },
    {
        "log.maxsize",
        [](const char *value) {
            int32_t size = atoi(value);
            if (size == 0) {
                size = 100000;
            }
            logger.setMaxSize(size);
        ;},
        [](char *value, int size) {snprintf(value, size, "%d bytes", logger.getMaxSize());}
    },
    {
        "log.filename",
        [](const char *value) {logger.setFilename(value);},
        [](char *value, int size) {snprintf(value, size, "%s", logger.getFilename().c_str());}
    },
    {
        "log.serial",
        [](const char *value) {logger.setSerialOut(strcmp(value, "off"));},
        [](char *value, int size) {strcpy(value, logger.getSerialOut() ? "on" : "off");}
    },
    {
        "screentimeout",
        [](const char *value) {screenTimeout = atoi(value);},
        [](char *value, int size) { 
        if (screenTimeout < 30) {
                strcpy(value, "off");
        } else {
            snprintf(value, size, "%d seconds", screenTimeout);
        }}
    },
    {
        "auditlog.maxsize",
        [](const char *value) {
            int32_t size = atoi(value);
            if (size == 0) {
                size = 100000;
            }
            auditLogger.setMaxSize(size);
        ;},
        [](char *value, int size) {snprintf(value, size, "%d bytes", auditLogger.getMaxSize());}
    },
    {
        "auditlog.flush",
        [](const char *value) {auditLogger.setAlwaysFlush(strcmp(value, "off"));},
        [](char *value, int size) {strcpy(value, auditLogger.getAlwaysFlush()? "on" : "off");}
    },
    {
        "auditlog.filename",
        [](const char *value) {auditLogger.setFilename(value);},
        [](char *value, int size) {snprintf(value, size, "%s", auditLogger.getFilename().c_str());}
    }
};

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
    lastCmd = reinterpret_cast<char *>(ps_malloc(LastCommandBufferSize));
    lastCmd[0] = 0;
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
    } else if ((ch >= ' ') || (ch == 27)) {
        buffer[bufferUsed++] = ch;

        if (bufferUsed >= bufferSize - 1) {
            print("Cmd: Buffer too long\r\n");
            bufferUsed = 0;
        }
        if ((bufferUsed >= 3) && (buffer[bufferUsed - 3] == 27) && (buffer[bufferUsed - 2] == '[')) {
            if ((buffer[bufferUsed - 1] == 'A') && (lastCmd[0])) {
                strcpy(buffer, lastCmd);
                bufferUsed = strlen(buffer);
                printf("\r%s%s", getPrompt(), buffer);
                printf("\x1b[%dG", bufferUsed);
            }
        }
    } else if (ch == 28) {
        bufferUsed = 0;
        printf("\r%s", getPrompt());
    } else if (ch == 8) {
        if (bufferUsed > 0) {
            --bufferUsed;
        }
    }
    yield();
}

void Terminal::processCommand(char *cmd) {
    snprintf(lastCmd, LastCommandBufferSize, "%s", cmd);
    auditLogger.printf(LOG_TERMINAL, LOG_INFO, "Terminal (%c) command: %s\n", getType(), cmd);

    const char *command = strtok(cmd, " ");
    const char *param = strtok(nullptr, " ");

    if (strcmp(command, "mem") == 0) {
        printf("Heap: %d, PSHeap: %d\r\n", ESP.getFreeHeap(), ESP.getFreePsram());
    }
    else if (strcmp(command, "monitor") == 0) {
        if (param) {
            setMonitoring(strcmp(param, "off"));
        }
        printf("Monitoring %s\r\n", getMonitoring() ? "on" : "off");
    } else if (strcmp(command, "help") == 0) {
        print("dir - directory listing of SD card\r\n");
        print("cat <filename> - display contents of file from SD card\r\n");
        print("del <filename> - delete contents of file from SD card\r\n");
        print("quit - exit Telnet session\r\n");
        print("reboot - reboot\r\n");
        print("wifi - show Wifi details\r\n");
        print("wifi <ssid> <password> - change Wifi connection\r\n");
        print("log - show main logging details\r\n");
        print("log off - turn off main logging\r\n");
        print("log on [<level 0-5>] - turn on main logging, optionally set log level\r\n");
        print("mem - show free memory\r\n");
        print("set <setting> <value>\r\n");
        print("show [<setting>]\r\n");
        print("battery [enable|disable|demo]- show battery data\r\n");
        print("solar [enable|disable|demo]- show solar charger data\r\n");
        print("mains [enable|disable|demo]- show mains charger data\r\n");
        print("b2b [enable|disable|demo]- show b2b charger data\r\n");
    } else if (strcmp(command, "show") == 0) {
        const int num = sizeof(settings) / sizeof(settings[0]);
        if (param) {
            bool settingFound = false;
            for (int i = 0; i < num; i++) {
                if (strcmp(param, settings[i].name) == 0) {
                    char buffer[1000];
                    settingFound = true;
                    settings[i].show(buffer, sizeof(buffer));
                    printf("show %s: %s\r\n", param, buffer);
                    break;
                }
            }
            if (!settingFound) {
                print("Unknown setting\r\n");
            }
        } else {
            for (int i = 0; i < num; i++) {
                char buffer[1000];
                settings[i].show(buffer, sizeof(buffer));
                printf("show %s: %s\r\n", settings[i].name, buffer);
            }
        }
    } else if (strcmp(command, "set") == 0) {
        const int num = sizeof(settings) / sizeof(settings[0]);
        bool settingFound = false;
        if (param) {
            for (int i = 0; i < num; i++) {
                if (strcmp(param, settings[i].name) == 0) {
                    const char *param2 = strtok(nullptr, "\n");
                    settingFound = true;
                    if (param2) {
                        settings[i].set(param2);
                    }
                    char buffer[1000];
                    settings[i].show(buffer, sizeof(buffer));
                    printf("set %s: %s\r\n", param, buffer);
                    IniFile::save(SETTINGS_FILENAME);
                    break;
                }
            }
        }
        if (!settingFound) {
            print("Unknown setting\r\n");
        }
    } else if (strcmp(command, "dir") == 0) { //tail
        doDir(param);
    } else if (strcmp(command, "cat") == 0) {
        if (param) {
            doCat(param);
        }
    } else if (strcmp(command, "tail") == 0) {
        if (param) {
            int lines = 15;
            const char *param2 = strtok(nullptr, " ");
            if (param2) {
                lines = atoi(param2);
                if (lines == 0) {
                    lines = 15;
                }
            }
            doTail(param, lines);
        }
    } else if (strcmp(command, "del") == 0) {
        if (param) {
            doDelete(param);
        }
    } else if (strcmp(command, "reboot") == 0) {
        ESP.restart();
    } else if ((strcmp(command, "quit") == 0) && (getType() == 'T')) {
        doQuit();
    } else if (strcmp(command, "battery") == 0) {
        if (param) {
            if (strcmp(param, "enable") == 0) {
                ksEnergyBattery.enable(true);
                IniFile::save(SETTINGS_FILENAME);
            } else if (strcmp(param, "disable") == 0) {
                ksEnergyBattery.enable(false);
                IniFile::save(SETTINGS_FILENAME);
            } else if (strcmp(param, "demo") == 0) {
                const char *v = strtok(nullptr, " ");
                const char *i = strtok(nullptr, " ");
                const char *s = strtok(nullptr, " ");
                const char *soc = strtok(nullptr, " ");
                if (soc) {
                    ksEnergyBattery.setDemoMode(true);
                    ksEnergyBattery.setDemoData(atof(v) * 1000, atof(i) * 1000, strtoul(s, nullptr, 16), atoi(soc));
                } else if (v && (strcmp(v, "off") == 0)) {
                    ksEnergyBattery.setDemoMode(false);
                }
            }
        }
        printf("Battery is: %s %s %s\r\n", ksEnergyBattery.isEnabled() ? "Enabled" : "Disabled", 
            ksEnergyBattery.isConnected() ? "Connected" : "Disconnected",
            ksEnergyBattery.isDemoMode() ? "Demo Mode" : "");
        printf("Last Update: %s\r\n", ksEnergyBattery.getLastUpdateTimeText());
        printf("Voltage: %.2fV\r\n", ksEnergyBattery.getVoltage() / 1000.0);
        printf("Current: %.2fA\r\n", ksEnergyBattery.getCurrent() / 1000.0);
        printf("Temperature: %d\r\n", ksEnergyBattery.getTemperature() - 40);
        printf("Cycles: %d\r\n", ksEnergyBattery.getCycles());
        printf("State: %s\r\n", ksEnergyBattery.getStateText().c_str());
        printf("SOC: %d%% +/- %d %%\r\n", ksEnergyBattery.getSOC(), ksEnergyBattery.getSOCError());
        printf("SOH: %d\r\n", ksEnergyBattery.getSOH());
        printf("learnState: %d\r\n", ksEnergyBattery.getLearnState());
        printf("designCapacity: %.0f Ah\r\n", ksEnergyBattery.getDesignCapacity() / 1000.0);
        printf("remainingCapacity: %.1f Ah\r\n", ksEnergyBattery.getRemainingCapacity() / 1000.0);
        printf("fullCapacity: %.0f Ah\r\n", ksEnergyBattery.getFullCapacity() / 1000.0);

    } else if (strcmp(command, "solar") == 0) {
        if (param) {
            if (strcmp(param, "enable") == 0) {
                victronSmartSolar.enable(true);
                IniFile::save(SETTINGS_FILENAME);
            } else if (strcmp(param, "disable") == 0) {
                victronSmartSolar.enable(false);
                IniFile::save(SETTINGS_FILENAME);
            } else if (strcmp(param, "demo") == 0) {
                const char *v1 = strtok(nullptr, " ");
                const char *p1 = strtok(nullptr, " ");
                const char *v2 = strtok(nullptr, " ");
                const char *i2 = strtok(nullptr, " ");
                const char *t = strtok(nullptr, " ");
                const char *s = strtok(nullptr, " ");
                if (s) {
                    victronSmartSolar.setDemoMode(true);
                    victronSmartSolar.setDemoData(atoi(v1), atoi(v2), 0, atoi(i2), atoi(p1), 0, atoi(t), atoi(s));
                } else if (v1 && (strcmp(v1, "off") == 0)) {
                    victronSmartSolar.setDemoMode(false);
                } else {
					printf("solar demo <panel volts> <panel power> <out volts> <out current> <temp> <status>\n");
					printf("solar demo off\n");
                }
            }
        }
        printf("Solar is: %s %s %s\r\n", victronSmartSolar.isEnabled() ? "Enabled" : "Disabled", 
            victronSmartSolar.isConnected() ? "Connected" : "Disconnected",
            victronSmartSolar.isDemoMode() ? "Demo Mode" : "");
        printf("Last Update: %s\r\n", victronSmartSolar.getLastUpdateTimeText());
        printf("Solar Voltage: %.2fV\r\n", victronSmartSolar.getVoltage(0) / 100.0);
        printf("Solar Current: %.2fA\r\n", victronSmartSolar.getCurrent(0) / 100.0);
        printf("Solar Power: %.2fW\r\n", victronSmartSolar.getPower(0) / 100.0);
        printf("Charge Voltage: %.2fV\r\n", victronSmartSolar.getVoltage(1) / 100.0);
        printf("Charge Current: %.2fA\r\n", victronSmartSolar.getCurrent(1) / 100.0);
        printf("Charge Power: %.2fW\r\n", victronSmartSolar.getPower(1) / 100.0);
        printf("State: %d\r\n", victronSmartSolar.getState());
        printf("Temperature: %.2f\r\n", victronSmartSolar.getTemperature() / 100.0);

    } else if (strcmp(command, "b2b") == 0) {
        if (param) {
            if (strcmp(param, "enable") == 0) {
                victronB2B.enable(true);
                IniFile::save(SETTINGS_FILENAME);
            } else if (strcmp(param, "disable") == 0) {
                victronB2B.enable(false);
                IniFile::save(SETTINGS_FILENAME);
            } else if (strcmp(param, "demo") == 0) {
                const char *v0 = strtok(nullptr, " ");
                const char *v1 = strtok(nullptr, " ");
                const char *t = strtok(nullptr, " ");
                const char *s = strtok(nullptr, " ");
                if (s) {
                    victronMainsCharger.setDemoMode(true);
                    victronMainsCharger.setDemoData(atof(v0) * 100, atof(v1) * 100, 0, 0, 0, 0, atof(t) * 100, atoi(s));
                } else if (v1 && (strcmp(v1, "off") == 0)) {
                    victronMainsCharger.setDemoMode(false);
                } else {
					printf("b2b demo <in volts> <out volts> <temp> <status>\n");
					printf("b2b demo off\n");
				}
            }
        }
        printf("B2B charger is: %s %s\r\n", victronB2B.isEnabled() ? "Enabled" : "Disabled", 
            victronB2B.isConnected() ? "Connected" : "Disconnected");
        printf("Last Update: %s\r\n", victronB2B.getLastUpdateTimeText());
        printf("Starter Batt Voltage: %.2fV\r\n", victronB2B.getVoltage(0) / 100.0);
        printf("Starter Batt Current: %.2fA\r\n", victronB2B.getCurrent(0) / 100.0);
        printf("Starter Batt Power: %.2fW\r\n", victronB2B.getPower(0) / 100.0);
        printf("Charge Voltage: %.2fV\r\n", victronB2B.getVoltage(1) / 100.0);
        printf("Charge Current: %.2fA\r\n", victronB2B.getCurrent(1) / 100.0);
        printf("Charge Power: %.2fW\r\n", victronB2B.getPower(1) / 100.0);
        printf("State: %d\r\n", victronB2B.getState());
        printf("Temperature: %.2f\r\n", victronB2B.getTemperature() / 100.0);

    } else if (strcmp(command, "mains") == 0) {
        if (param) {
            if (strcmp(param, "enable") == 0) {
                victronMainsCharger.enable(true);
                IniFile::save(SETTINGS_FILENAME);
            } else if (strcmp(param, "disable") == 0) {
                victronMainsCharger.enable(false);
                IniFile::save(SETTINGS_FILENAME);
            } else if (strcmp(param, "demo") == 0) {
                const char *v1 = strtok(nullptr, " ");
                const char *i1 = strtok(nullptr, " ");
                const char *t = strtok(nullptr, " ");
                const char *s = strtok(nullptr, " ");
                if (s) {
                    victronMainsCharger.setDemoMode(true);
                    victronMainsCharger.setDemoData(atof(v1) * 100, 0, atof(i1) * 100, 0, 0, 0, atof(t) * 100, atoi(s));
                } else if (v1 && (strcmp(v1, "off") == 0)) {
                    victronMainsCharger.setDemoMode(false);
                } else {
					printf("mains demo <out volts> <out current> <temp> <status>\n");
					printf("mains demo off\n");
                }
            }
        }
        printf("Mains Charger is: %s %s %s\r\n", victronMainsCharger.isEnabled() ? "Enabled" : "Disabled", 
            victronMainsCharger.isConnected() ? "Connected" : "Disconnected",
			victronMainsCharger.isDemoMode() ? "Demo Mode" : "");
        printf("Last Update: %s\r\n", victronMainsCharger.getLastUpdateTimeText());

        printf("Charge Voltage: %.2f V\r\n", victronMainsCharger.getVoltage(1) / 100.0);
        printf("Charge Current: %.2f A\r\n", victronMainsCharger.getCurrent(1) / 100.0);
        printf("State: %d (%s)\r\n", victronMainsCharger.getState(), victronMainsCharger.getStateText());
        printf("Temperature: %.2f C\r\n", victronMainsCharger.getTemperature() / 100.0);
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

std::string Terminal::getNormalisedFilename(const char *filename) {
    std::string val;
    if (filename[0] != '/') {
        val.reserve(strlen(filename + 2));
        val = "/";
        val += filename;
    } else {
        val = filename;
    }
    return val;
}

void Terminal::doCat(const char *param) {
    std::string filename = getNormalisedFilename(param);

    File file = SD.open(filename.c_str());
    if (!file) {
        print("Failed to open file\r\n");
        return;
    }
    dumpFile(file, 0);
}

void Terminal::doTail(const char *param, int lines) {
    std::string filename = getNormalisedFilename(param);

    File file = SD.open(filename.c_str());
    if (!file) {
        print("Failed to open file\r\n");
        return;
    }
    uint32_t fileLength = file.size();
    uint32_t startPos = fileLength;

    do {
        int size = 1000;
        char buf[1000];
        if (startPos < 1000) {
            size = startPos;
        }
        startPos = startPos - size;
        file.seek(startPos);
        file.readBytes(buf, size);
        for (int i = size - 1; i >= 0; i--) {
            if (buf[i] == '\n') {
                if (startPos + i < fileLength - 1) {
                    if (--lines == 0) {
                        startPos += i + 1;
                        break;
                    }
                }
            }
        }
    } while ((lines > 0) && (startPos > 0));
    dumpFile(file, startPos);
}

void Terminal::dumpFile(File &file, uint32_t startPos) {
    file.seek(startPos);
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

void Terminal::doDelete(const char *param) {
    std::string filename = "/sd" + getNormalisedFilename(param);

    if (remove(filename.c_str()) == 0) {
        print("File deleted\r\n");
    } else {
        printf("Error: File not deleted - %d\r\n", errno);
    }
}
