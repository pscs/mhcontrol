#include "inifile.h"

#include "Controller.h"

#include <string.h>

void IniFile::save(const char *filename) {
    FILE *file = fopen(filename, "wt");

    if (file) {
        fprintf(file, "wifi.ssid=%s\n", WifiSSID.c_str());
        fprintf(file, "wifi.password=%s\n", WifiPassword.c_str());
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
            }
        }

        fclose(file);
    }
}