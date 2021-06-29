#include "victron_B2B.h"

#include "config.h"

VictronB2B victronB2B(VICTRON_B2B_ADDRESS, VICTRON_B2B_PASSCODE);

VictronB2B::VictronB2B(const char *addr, uint32_t passcode): Victron(addr, passcode) {
}

const char *VictronB2B::getName() const {
    return "B2B";
}