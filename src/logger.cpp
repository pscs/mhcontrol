#include "logger.h"

#include <stdio.h>
#include <stdarg.h>
#include <HardwareSerial.h>

Logger logger;

void Logger::print(uint16_t component, uint8_t level, const char *fmt, ...) {
    if ((enabled) && (level <= logLevel)) {
        va_list args;
        va_start(args, fmt);
        vprintf(fmt, args);
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

void Logger::send(uint16_t component, const char *str) {
    if (enabled) {
        fputs(str, stdout);
    }
}
