#include "inifile.h"

#include "Controller.h"
#include "telnet_terminal.h"
#include "logger.h"
#include "ksenergy_battery.h"
#include "victron_B2B.h"
#include "victron_mainscharger.h"
#include "victron_smartsolar.h"
#include "screens.h"

#include <string.h>

void IniFile::save(const char *filename) {
    FILE *file = fopen(filename, "wt");

    if (file) {
        fprintf(file, "wifi.ssid=%s\n", WifiSSID.c_str());
        fprintf(file, "wifi.password=%s\n", WifiPassword.c_str());
        fprintf(file, "telnet.password=%s\n", telnetTerminal.getPassword().c_str());
        fprintf(file, "logging.enable=%d\n", logger.isEnabled() ? 1 : 0);
        fprintf(file, "logging.levels=%s\n", logger.getLevelString().c_str());
        fprintf(file, "logging.filename=%s\n", logger.getFilename());
        fprintf(file, "logging.maxsize=%d\n", logger.getMaxSize());
        fprintf(file, "logging.serial=%d\n", logger.getSerialOut() ? 1 : 0);
        fprintf(file, "auditlog.filename=%s\n", auditLogger.getFilename().c_str());
        fprintf(file, "auditlog.maxsize=%d\n", auditLogger.getMaxSize());
        fprintf(file, "auditlog.flush=%d\n", auditLogger.getAlwaysFlush()? 1 : 0);
        fprintf(file, "solar.enabled=%d\n", victronSmartSolar.isEnabled() ? 1 : 0);
        fprintf(file, "b2b.enabled=%d\n", victronB2B.isEnabled() ? 1 : 0);
        fprintf(file, "mains.enabled=%d\n", victronMainsCharger.isEnabled() ? 1 : 0);
        fprintf(file, "battery.enabled=%d\n", ksEnergyBattery.isEnabled() ? 1 : 0);
		fprintf(file, "screen.calibration=%s\n", ScreenClass::getCalibrationString().c_str());
        fprintf(file, "screen.timeout=%d\n", screenTimeout);
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
                logger.setLevelsFromString(data);
            } else if (strcmp(field, "logging.filename") == 0) {
                logger.setFilename(data);
            } else if (strcmp(field, "logging.maxsize") == 0) {
                logger.setMaxSize(atoi(data)); 
            } else if (strcmp(field, "logging.serial") == 0) {
                logger.setSerialOut(atoi(data)); 
            } else if (strcmp(field, "auditlog.filename") == 0) {
                auditLogger.setFilename(data);                
            } else if (strcmp(field, "auditlog.maxsize") == 0) {
                auditLogger.setMaxSize(atoi(data));      
            } else if (strcmp(field, "auditlog.flush") == 0) {
                auditLogger.setAlwaysFlush(atoi(data));      
           } else if (strcmp(field, "solar.enabled") == 0) {
                victronSmartSolar.enable(atoi(data));      
            } else if (strcmp(field, "b2b.enabled") == 0) {
                victronB2B.enable(atoi(data));
            } else if (strcmp(field, "mains.enabled") == 0) {
                victronMainsCharger.enable(atoi(data));
            } else if (strcmp(field, "battery.enabled") == 0) {
                ksEnergyBattery.enable(atoi(data));
            } else if (strcmp(field, "screen.calibration") == 0) {
				ScreenClass::setCalibrationFromString(data);
            } else if (strcmp(field, "screen.timeout") == 0) {
                screenTimeout = atoi(data);      
 			}
        }

        fclose(file);
    }
}