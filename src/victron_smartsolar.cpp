#include "victron_smartsolar.h"

#include "config.h"

VictronSmartSolar victronSmartSolar(VICTRON_SOLAR_ADDRESS, VICTRON_SOLAR_PASSCODE);

VictronSmartSolar::VictronSmartSolar(const char *addr, uint32_t passcode): Victron(addr, passcode) {
}

const char *VictronSmartSolar::getName() const {
    return "SmartSolar";
}