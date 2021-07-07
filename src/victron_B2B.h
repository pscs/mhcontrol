#pragma once

#include "victron.h"

class VictronB2B: public Victron {
public:
    VictronB2B(const char *address, uint32_t passcode);

    const char *getName() const override;
    
protected:
    void askForInitialValues() override;
};

extern VictronB2B victronB2B;
