#pragma once

//#include <BLEDevice.h>

#include "mybleclient.h"

//class Victron;

/*private:
    Victron *pVictron;
    uint8_t *pBuffer;
    uint8_t len;
};*/

class Victron: public MyBLEClient {
public:
    Victron(const char *address, uint32_t passcode);
    virtual ~Victron();

    bool connectIfNecessary() override;

    int16_t getVoltage(int index) const;
    int16_t getCurrent(int index) const;
    uint32_t getPower(int index) const;
    int16_t getTemperature() const {
        return temperature;
    }
    uint8_t getState() const {
        return state;
    }

    void keepAlive() override;
    virtual BLEUUID getServiceUUID() const;

    void unregisterAll() override;

    void onNotify(uint8_t index, uint8_t* pData, size_t length) override;

private:
    bool getCBOR(uint8_t startPos, uint8_t &valLength, int32_t &value) const;

    BLERemoteCharacteristic *pChar1 = nullptr;
    BLERemoteCharacteristic *pChar2 = nullptr;
    BLERemoteCharacteristic *pChar3 = nullptr;
    int16_t voltage[2];
    int16_t current[2];
    uint32_t power[2];
    int16_t temperature = 0;
    uint8_t state = 0;
};
