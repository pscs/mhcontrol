#include "inifile.h"

#include "Controller.h"
#include "telnet_terminal.h"
#include "logger.h"

#include <string.h>

void IniFile::save(const char *filename) {
    FILE *file = fopen(filename, "wt");

    if (file) {
        fprintf(file, "wifi.ssid=%s\n", WifiSSID.c_str());
        fprintf(file, "wifi.password=%s\n", WifiPassword.c_str());
        fprintf(file, "telnet.password=%s\n", telnetTerminal.getPassword().c_str());
        fprintf(file, "logging.enable=%d\n", logger.isEnabled() ? 1 : 0);
        fprintf(file, "logging.level=%d\n", logger.getLevel());
        fprintf(file, "auditlog.filename=%s\n", auditLogger.getFilename().c_str());
        fprintf(file, "auditlog.maxsize=%d\n", auditLogger.getMaxSize());
        fprintf(file, "auditlog.flush=%d\n", auditLogger.getAlwaysFlush()? 1 : 0);
        fclose(file);
    }
}

void IniFile::load(const char *filename) {
    FILE *file = fopen(filename, "rt");

    if (file) {
        char line[500];
        while (fgets(line, sizeof(line), file)) {
            const char *field = strtok(line, " =");
            const char *data = strtok(nullptr, "\n");

            if (strcmp(field, "wifi.ssid") == 0) {
                WifiSSID = data;
            } else if (strcmp(field, "wifi.password") == 0) {
                WifiPassword = data;
            } else if (strcmp(field, "telnet.password") == 0) {
                telnetTerminal.setPassword(data);
            } else if (strcmp(field, "logging.enable") == 0) {
                logger.enable(atoi(data));
            } else if (strcmp(field, "logging.level") == 0) {
                logger.setLevel(atoi(data));
            } else if (strcmp(field, "auditlog.filename") == 0) {
                auditLogger.setFilename(data);                
            } else if (strcmp(field, "auditlog.maxsize") == 0) {
                auditLogger.setMaxSize(atoi(data));      
            } else if (strcmp(field, "auditlog.flush") == 0) {
                auditLogger.setAlwaysFlush(atoi(data));                  }
        }

        fclose(file);
    }
}