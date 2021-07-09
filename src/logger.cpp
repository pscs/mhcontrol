#include "logger.h"

#include <stdio.h>
#include <stdarg.h>
#include <HardwareSerial.h>
#include "telnet_terminal.h"

Logger logger(LOG_DEBUG);
Logger auditLogger(LOG_DEBUG, "/sd/audit.log", true, 10000);

Logger::~Logger() {
    if (file) {
        fclose(file);
    }
}

void Logger::printf(LogSource component, int8_t level, const char *fmt, ...) {
    if ((enabled) && (level <= getLevel(component))) {
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

void Logger::send(LogSource component, int8_t level, const char *str) {
    if ((enabled) && (level <= getLevel(component))) {
        if (serialOut) {
            ::printf("%s(%d) %s", getComponentName(component), level, str);
        }
        if ((telnetOut) && (telnetTerminal.isConnected())) {
            char *x = strdup(str);
            int len = strlen(x);
            int start = 0;
            for (int i = 0; i < len; i++) {
                if (x[i] == '\n') {
                    x[i] = 0;
                    telnetTerminal.printf("-- %s(%d) %s\r\n", getComponentName(component), level, x + start);
                    start = i + 1;
                }
            }
            if (start < len) {
                telnetTerminal.printf("-- %s(%d) %s\r\n", getComponentName(component), level, x + start);
            }
            free(x);
        }
        if ((!file) && (!filename.empty())) {
            file = fopen(filename.c_str(), "at");
            if (!file) {
                ::printf("Unable to open %s for logging - %d\n", filename.c_str(), errno);
            }
        }
        if (file) {
            fprintf(file, "%s(%d) %s", getComponentName(component), level, str);
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
            ::printf("archive old log file %d > %d\n", pos, maxSize);
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
    if ((fn) && (fn[0] == '/')) {
        filename = fn;
    } else {
        filename.clear();
    }
}

void Logger::setLevel(LogSource component, int8_t level) {
    if (component < NUM_LOG_SOURCES) {
        logLevel[component] = level;
    }
}

void Logger::setAllLevels(int8_t level) {
    for (int i = 0; i < NUM_LOG_SOURCES; i++) {
        logLevel[i] = level;
    }
}

void Logger::setLevelsFromString(const char *str) {
    char *str2 = strdup(str);
    char *p = strtok(str2, " ");
    int index = 0;
    while (p) {
        if (index < NUM_LOG_SOURCES) {
            logLevel[index] = atoi(p);
        }
        index++;
        p = strtok(nullptr, " ");
    }
    if (index == 0) {
        logLevel[index++] = 0;
    }
    while (index < NUM_LOG_SOURCES) {
        logLevel[index++] = logLevel[0];
    }
    free(str2);
}

std::string Logger::getLevelString() const {
    std::string result;

    for (int i = 0; i < NUM_LOG_SOURCES; i++) {
        char buf[10];
        snprintf(buf, sizeof(buf), "%d ", logLevel[i]);
        result += buf;
    }

    return result;
}

const char *Logger::getComponentName(LogSource component) const {
    switch(component) {
    case LOG_GENERAL:
        return "";
    case LOG_WEATHER:
        return "wthr";
    case LOG_WIFI:
        return "wifi";
    case LOG_BLE:
        return "ble";
    case LOG_THINGSBOARD:
        return "tb";
    case LOG_VICTRON:
        return "victron";
    case LOG_TERMINAL:
        return "term";
    case LOG_GPS:
        return "gps";
    case LOG_ACCELEROMETER:
        return "accel";
    case LOG_RTC:
        return "rtc";
    case LOG_BATTERY:
        return "batt";
	case LOG_OPTIONS:
		return "options";
	case LOG_SOUND:
		return "sound";
    default:
        return "unknwn";            
    }
}

const char *Logger::getLevelName(LogSource component) const {
    switch(getLevel(component)) {
    case LOG_SECURITY:
        return "security";
    case LOG_FATAL:
        return "fatal";
    case LOG_ERROR:
        return "error";
    case LOG_WARNING:
        return "warning";
    case LOG_INFO:
        return "info";
    case LOG_VERBOSE:
        return "verbose";
    case LOG_DEBUG:
        return "debug";
    default:
        return "unknown";
    }
}
