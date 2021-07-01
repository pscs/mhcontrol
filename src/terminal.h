#pragma once

#include <cstdint>

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

    void doDir(const char *param);
    void doCat(const char *param);

    virtual const char *getPrompt() const {
        return "";
    }
    virtual void doQuit() {
    }
protected:
    char *buffer = nullptr;
    uint16_t bufferSize = 0;
    uint16_t bufferUsed = 0;
    char *sendBuffer = nullptr;
};
