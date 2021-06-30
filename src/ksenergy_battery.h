#pragma once

#include <BLEDevice.h>

#include "mybleclient.h"

class KSEnergyBattery: public MyBLEClient {
public:
    KSEnergyBattery(const char *address);
    virtual ~KSEnergyBattery();

    bool connectIfNecessary() override;

    int16_t getVoltage() const {
        return voltage;
    }
    int16_t getCurrent() const {
        return current;
    }
    int16_t getTemperature() const {
        return temperature;
    }
    uint8_t getState() const {
        return state;
    }
    uint8_t getSOC() const {
        return SOC;
    }

    void keepAlive() override;
    const char *getName() const override;
    void unregisterAll() override;

    void onNotify(uint8_t index, uint8_t* pData, size_t length) override;

private:
    BLERemoteCharacteristic *pCharacteristic = nullptr;
    int16_t voltage = 0;
    int16_t current = 0;
    int16_t temperature = 0;
    uint8_t state = 0;
    uint8_t SOC = 0;
};

extern KSEnergyBattery ksEnergyBattery;