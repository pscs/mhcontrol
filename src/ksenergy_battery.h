#pragma once

//#include <BLEDevice.h>

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
    uint16_t getState() const {
        return state;
    }
    std::string getStateText() const;
    uint16_t getCycles() const {
        return cycles;
    }
    uint8_t getSOC() const {
        return SOC;
    }
    uint8_t getSOCError() const {
        return SOCError;
    }
    uint16_t getSOH() const {
        return SOH;
    }
    uint8_t getLearnState() const {
        return learnState;
    }
    uint32_t getDesignCapacity() const {
        return designCapacity;
    }
    uint32_t getRemainingCapacity() const {
        return remainingCapacity;
    }
    uint32_t getFullCapacity() const {
        return fullCapacity;
    }

    void keepAlive() override;
    const char *getName() const override;
    void unregisterAll() override;

    void onNotify(uint8_t index, uint8_t* pData, size_t length) override;

    void processData();
    static uint8_t charToHex(char ch);

    void setDemoMode(bool d) {
        demoMode = d;
    }
    bool isDemoMode() const {
        return demoMode;
    }
    void setDemoData(int16_t v, int32_t i, uint16_t s, uint8_t soc);

private:
    BLERemoteCharacteristic *pCharacteristic = nullptr;

    int16_t voltage = 0;
    int32_t current = 0;
    int16_t temperature = 0;
    uint16_t cycles = 0;
    uint16_t state = 0;
    uint8_t SOC = 0;
    uint8_t SOCError = 0;
    uint8_t learnState = 0;
    uint32_t designCapacity = 0;
    uint32_t remainingCapacity = 0;
    uint32_t fullCapacity = 0;
    uint16_t SOH = 0;
    bool demoMode = false;

};

extern KSEnergyBattery ksEnergyBattery;