#pragma once

#include <cstdint>
#include <string>

const uint8_t LOG_GENERAL = 0;
const uint8_t LOG_WEATHER = 1;
const uint8_t LOG_WIFI = 2;
const uint8_t LOG_BLE = 3;
const uint8_t LOG_THINGSBOARD = 4;
const uint8_t LOG_VICTRON = 5;
const uint8_t LOG_TERMINAL = 6;
const uint8_t LOG_GPS = 7;
const uint8_t LOG_ACCELEROMETER = 8;
const uint8_t LOG_RTC = 9;
const uint8_t LOG_BATTERY = 10;
const uint8_t NUM_LOG_SOURCES = 11;

const int8_t LOG_SECURITY = -1;
const int8_t LOG_FATAL = 0;
const int8_t LOG_ERROR = 1;
const int8_t LOG_WARNING = 2;
const int8_t LOG_INFO = 3;
const int8_t LOG_VERBOSE = 4;
const int8_t LOG_DEBUG = 5;

class Logger {
public:
    Logger(int8_t level) {
        setAllLevels(level);
    }
    Logger(int8_t level, const char *fn, bool alwaysFlush_, uint32_t maxSize_): serialOut(false), 
        filename(fn), alwaysFlush(alwaysFlush_), maxSize(maxSize_) {
            setAllLevels(level);
        }
    ~Logger();

    void enable(bool e) {
        enabled = e;
    }
    bool isEnabled() const {
        return enabled;
    }

    int8_t getLevel(uint8_t component) const {
        return (component < NUM_LOG_SOURCES) ? logLevel[component] : logLevel[0];
    };
    void setLevel(uint8_t component, int8_t level);
    void setAllLevels(int8_t level);
    const char *getLevelName(uint8_t component) const;
    const char *getComponentName(uint8_t component) const;

    void setLevelsFromString(const char *str);
    std::string getLevelString() const;

    void setSerialOut(bool b) {
        serialOut = b;
    }
    bool getSerialOut() const {
        return serialOut;
    };
    void setTelnetOut(bool b) {
        telnetOut = b;
    }
    bool getTelnetOut() const {
        return telnetOut;
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
    void printf(uint8_t component, int8_t level, const char *fmt, ...);
    void send(uint8_t component, int8_t level, const char *str);

private:
    bool enabled = true;
    bool serialOut = true;
    bool telnetOut = false;
    int8_t logLevel[NUM_LOG_SOURCES];
    std::string filename;
    FILE *file = nullptr;
    bool alwaysFlush = false;
    uint32_t maxSize = 10000;
};

extern Logger logger;
extern Logger auditLogger;
