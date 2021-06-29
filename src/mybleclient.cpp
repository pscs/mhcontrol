#include "mybleclient.h"


struct NotifyInfo {
    NotifyInfo(): pClient(nullptr), index(0) {}
    NotifyInfo(MyBLEClient *p, uint8_t index_): pClient(p), index(index_) {}
    MyBLEClient *pClient;
    uint8_t index;
};

static std::map<const BLERemoteCharacteristic *, NotifyInfo> characteristics;

MyBLEClient::MyBLEClient(const char *addr, bool hasPasscode_, uint32_t passcode_): address(addr), 
            hasPasscode(hasPasscode_), passcode(passcode_) {
    pClient = BLEDevice::createClient();
    pClientCallback = new MyBLEClientCallback(this);
    pClient->setClientCallbacks(pClientCallback);
}

MyBLEClient::~MyBLEClient() {
    unregisterAll();

    if (isConnected()) {
        pClient->disconnect();
        pClient->setClientCallbacks(nullptr);
        delete pClient;
        delete pClientCallback;
    }
    free(pNotifyBuffer);
}

void MyBLEClient::onConnect() {
}

void MyBLEClient::onDisconnect() {
    connected = false;

    unregisterAll();

    lenNotifyBuffer = 0;
};




MyBLEClientCallback::MyBLEClientCallback(MyBLEClient *p): pClient(p) {
}

void registerCharacteristics(BLERemoteCharacteristic *pBLERemoteCharacteristics, MyBLEClient *client, int index)
{
    characteristics[pBLERemoteCharacteristics] = NotifyInfo(client, index);
}

void unregisterCharacteristics(BLERemoteCharacteristic *pBLERemoteCharacteristics, MyBLEClient *client) {
    characteristics.erase(pBLERemoteCharacteristics);
}

void notifyCallback(BLERemoteCharacteristic *pBLERemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify) {
    auto it = characteristics.find(pBLERemoteCharacteristic);
    if ((it != characteristics.end()) && (it->second.pClient)) {
        it->second.pClient->onNotify(it->second.index, pData, length);
    }
}
