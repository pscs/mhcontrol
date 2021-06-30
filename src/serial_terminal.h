#pragma once

#include <cstdint>
#include "terminal.h"

class SerialTerminal: public Terminal {
public:
    void processData() override;
    void internalSend(const char *str) override;
    char getType() const override;

private:
};

extern SerialTerminal serialTerminal;