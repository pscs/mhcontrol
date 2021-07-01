#include "logger.h"

#include <stdio.h>
#include <stdarg.h>
#include <HardwareSerial.h>

Logger logger(LOG_DEBUG);
Logger auditLogger(LOG_DEBUG, "/sd/audit.log", true, 10000);

Logger::~Logger() {
    if (file) {
        fclose(file);
    }
}

void Logger::print(uint16_t component, int8_t level, const char *fmt, ...) {
    if ((enabled) && (level <= logLevel)) {
        va_list args;
        va_start(args, fmt);
        char buffer[500];
        vsnprintf(buffer, sizeof(buffer), fmt, args);
        send(component, level, buffer);
        va_end(args);
    }
}
/*
void Logger::println(uint16_t component, const char *fmt, ...) {
    if (enabled) {
        char buffer[100];
        va_list args;
        va_start(args, fmt);
        vsnprintf(buffer, sizeof(buffer), fmt, args);
        send(component, buffer);
        va_end(args);
        send(component, "\n");
    }
}*/

void Logger::send(uint16_t component, int8_t level, const char *str) {
    if (enabled & (level <= logLevel)) {
        if (serialOut) {
            fputs(str, stdout);
        }
        if ((!file) && (!filename.empty())) {
            file = fopen(filename.c_str(), "at");
            if (!file) {
                printf("Unable to open %s for logging - %d\n", filename.c_str(), errno);
            }
        }
        if (file) {
            fputs(str, file);
            if (alwaysFlush) {
                flush();
            }
        }
    }
}

void Logger::flush() {
    if (file) {
        long pos = ftell(file);

        fflush(file);

        if ((pos > maxSize) && (maxSize)) {
            fclose(file);
            file = nullptr;
            printf("archive old log file %d > %d\n", pos, maxSize);
            char newname[filename.size() + 5];
            snprintf(newname, filename.size() + 5, "%s.old", filename.c_str());
            remove(newname);
            rename(filename.c_str(), newname);
        }

    }
}

void Logger::setFilename(const char *fn) {
    if (file) {
        fclose(file);
        file = nullptr;
    }
    filename = fn;
}