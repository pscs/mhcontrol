#include "victron_mainscharger.h"

#include "config.h"

VictronMainsCharger victronMainsCharger(VICTRON_MAINS_ADDRESS, VICTRON_MAINS_PASSCODE);

VictronMainsCharger::VictronMainsCharger(const char *addr, uint32_t passcode): Victron(addr, passcode) {
}

const char *VictronMainsCharger::getName() const {
    return "Mainscharger";
}