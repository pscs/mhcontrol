#pragma once

#include <cstdint>
#include <string>

namespace fs {
    class File;
}
using fs::File;

class Terminal {
public:
    Terminal();
    virtual ~Terminal();

    virtual void initialise();

    virtual void processData() = 0;
    void print(const char *str) {
        internalSend(str);
    };
    void printf(const char *fmt, ...);
    virtual void internalSend(const char *str) = 0;
    void processCharacter(char ch);

    virtual void processCommand(char *cmd);
    virtual char getType() const = 0;
    virtual void setMonitoring(bool m) const = 0;
    virtual bool getMonitoring() const = 0;

    void doDir(const char *param);
    void doCat(const char *param);
    void doTail(const char *param, int lines);
    void doDelete(const char *param);

    virtual const char *getPrompt() const {
        return "";
    }
    virtual void doQuit() {
    }
protected:
    static std::string getNormalisedFilename(const char *filename);
    void dumpFile(File &file, uint32_t startPos);

    char *buffer = nullptr;
    char *lastCmd = nullptr;
    uint16_t bufferSize = 0;
    uint16_t bufferUsed = 0;
    char *sendBuffer = nullptr;
};
