#pragma once

#include "victron.h"

class VictronMainsCharger: public Victron {
public:
    VictronMainsCharger(const char *address, uint32_t passcode);

    const char *getName() const override;
};

extern VictronMainsCharger victronMainsCharger;
