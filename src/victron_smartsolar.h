#pragma once

#include "victron.h"

class VictronSmartSolar: public Victron {
public:
    VictronSmartSolar(const char *address, uint32_t passcode);

    const char *getName() const override;
    
protected:
    void askForInitialValues() override;
};

extern VictronSmartSolar victronSmartSolar;
