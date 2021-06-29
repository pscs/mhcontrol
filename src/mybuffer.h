#pragma once

#include <Arduino.h>

class MyBuffer {
public:
    MyBuffer(size_t size_);
    ~MyBuffer();

    void *getBuffer() {
        return buffer;
    };
    char *getString() {
        return reinterpret_cast<char *>(buffer);
    }
    size_t getSize() {
        return size;
    }

private:
    void *buffer;
    size_t size;
};
