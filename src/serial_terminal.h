#pragma once

#include <cstdint>
#include "terminal.h"

class SerialTerminal: public Terminal {
public:
    void processData() override;
    void internalSend(const char *str) override;
    char getType() const override;

    void setMonitoring(bool m) const override;
    bool getMonitoring() const override;;

private:
};

extern SerialTerminal serialTerminal;