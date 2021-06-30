#pragma once

//#include <BLEDevice.h>
#include "NimBLEDevice.h"

class MyBLEClientCallback;

class MyBLEClient {
public:
    MyBLEClient(const char *address, bool hasPasscode, uint32_t passcode);
    ~MyBLEClient();

    virtual void onConnect();
    virtual void onDisconnect();
    virtual void onNotify(uint8_t index, uint8_t* pData, size_t length) = 0;
    virtual void unregisterAll() {};

    virtual bool connectIfNecessary() = 0;

    bool isConnected() const {
        return connected;
    }
    bool isEnabled() const {
        return enabled;
    }

    BLEAddress getAddress() const {
        return address;
    }
    virtual const char *getName() const = 0;

    void enable(bool e);
    virtual void keepAlive() = 0;

protected:
    BLEClient *pClient = nullptr;
    MyBLEClientCallback *pClientCallback = nullptr;

    uint8_t *pNotifyBuffer = nullptr;
    uint8_t lenNotifyBuffer;
    BLEAddress address;
    bool hasPasscode;
    uint32_t passcode;
    bool enabled = true;
    bool connected = false;

};

class MyBLEClientCallback : public BLEClientCallbacks {
public:
    MyBLEClientCallback(MyBLEClient *pClient_);
    void setPasscode(uint32_t p) {
        passcode = p;
    }
    void onConnect(BLEClient* pclient) override; 
    void onDisconnect(BLEClient* pclient) override;
    uint32_t onPassKeyRequest() override;
    void onAuthenticationComplete(ble_gap_conn_desc *desc) override;
    bool onConfirmPIN(uint32_t pin) override;
private:
    MyBLEClient *pClient;
    uint32_t passcode;
};


inline void MyBLEClientCallback::onConnect(BLEClient* pclient) {
    pClient->onConnect();
}

inline void MyBLEClientCallback::onDisconnect(BLEClient* pclient) {
    pClient->onDisconnect();
}

/*struct NotifyInfo {
    NotifyInfo(): pVictron(nullptr), index(0) {}
    NotifyInfo(Victron *p, uint8_t index_): pVictron(p), index(index_) {}
    Victron *pVictron;
    uint8_t index;
};

static std::map<const BLERemoteCharacteristic *, NotifyInfo> characteristics;
*/

void registerCharacteristics(BLERemoteCharacteristic *pBLERemoteCharacteristics, MyBLEClient *client, int index);
void unregisterCharacteristics(BLERemoteCharacteristic *pBLERemoteCharacteristics, MyBLEClient *client);
void notifyCallback(BLERemoteCharacteristic *pBLERemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify);
