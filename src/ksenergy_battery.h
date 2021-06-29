#pragma once

#include <BLEDevice.h>

#include "mybleclient.h"

class KSEnergyBattery: public MyBLEClient {
public:
    KSEnergyBattery(const char *address);
    virtual ~KSEnergyBattery();

    bool connectIfNecessary() override;

    int16_t getVoltage(int index) const;
    int16_t getCurrent(int index) const;
    int16_t getTemperature() const {
        return temperature;
    }
    uint8_t getState() const {
        return state;
    }

    void keepAlive() override;
    const char *getName() const override;
    void unregisterAll() override;

    void onNotify(uint8_t index, uint8_t* pData, size_t length) override;

private:
    BLERemoteCharacteristic *pCharacteristic = nullptr;
    int16_t voltage;
    int16_t current;
    int16_t temperature = 0;
    uint8_t state = 0;
};

extern KSEnergyBattery ksEnergyBattery;