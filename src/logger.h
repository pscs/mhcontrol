#pragma once

#include <cstdint>
#include <string>

#define LOG_WEATHER 1
#define LOG_WIFI 2
#define LOG_BLE 3
#define LOG_THINGSBOARD 4
#define LOG_VICTRON 5
#define LOG_TERMINAL 6

#define LOG_SECURITY -1
#define LOG_FATAL 0
#define LOG_ERROR 1
#define LOG_WARNING 2
#define LOG_INFO 3
#define LOG_VERBOSE 4
#define LOG_DEBUG 5

class Logger {
public:
    Logger(uint8_t level): logLevel(level) {}
    Logger(uint8_t level, const char *fn, bool alwaysFlush_, uint32_t maxSize_): serialOut(false), logLevel(level), 
        filename(fn), alwaysFlush(alwaysFlush_), maxSize(maxSize_) {}
    ~Logger();

    void enable(bool e) {
        enabled = e;
    }
    bool isEnabled() const {
        return enabled;
    }
    int8_t getLevel() const {
        return logLevel;
    }
    void setLevel(int8_t level) {
        logLevel = level;
    }

    void setFilename(const char *fn);
    std::string getFilename() const {
        return filename;
    }
    void setMaxSize(uint32_t size) {
        maxSize = size;
    }
    uint32_t getMaxSize() const {
        return maxSize;
    }

    void setAlwaysFlush(bool b) {
        alwaysFlush = b;
        if (b) {
            flush();
        }
    }
    bool getAlwaysFlush() const {
        return alwaysFlush;
    }

    void flush(); /*{
        if (file) {
            fflush(file);
        }
    }*/

    //void println(uint16_t component, const char *fmt, ...);
    void print(uint16_t component, int8_t level, const char *fmt, ...);
    void send(uint16_t component, int8_t level, const char *str);
private:
    bool enabled = true;
    bool serialOut = true;
    int8_t logLevel = LOG_DEBUG;
    std::string filename;
    FILE *file = nullptr;
    bool alwaysFlush = false;
    uint32_t maxSize = 10000;
};

extern Logger logger;
extern Logger auditLogger;
