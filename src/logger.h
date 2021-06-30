#pragma once

#include <cstdint>

#define LOG_WEATHER 1
#define LOG_WIFI 2
#define LOG_BLE 3
#define LOG_THINGSBOARD 4
#define LOG_VICTRON 5

#define LOG_FATAL 0
#define LOG_ERROR 1
#define LOG_WARNING 2
#define LOG_INFO 3
#define LOG_VERBOSE 4
#define LOG_DEBUG 5

class Logger {
public:
    void enable(bool e) {
        enabled = e;
    };
    bool isEnabled() const {
        return enabled;
    }
    uint8_t getLevel() const {
        return logLevel;
    }
    void setLevel(uint8_t level) {
        logLevel = level;
    }

    //void println(uint16_t component, const char *fmt, ...);
    void print(uint16_t component, uint8_t level, const char *fmt, ...);
    void send(uint16_t component, const char *str);
private:
    bool enabled = true;
    uint8_t logLevel = LOG_DEBUG;
};

extern Logger logger;