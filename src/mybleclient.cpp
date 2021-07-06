#include "mybleclient.h"

#include "logger.h"
#include "timezone.h"

char MyBLEClient::timeBuf[20];

struct NotifyInfo {
    NotifyInfo(): pClient(nullptr), index(0) {}
    NotifyInfo(MyBLEClient *p, uint8_t index_): pClient(p), index(index_) {}
    MyBLEClient *pClient;
    uint8_t index;
};

static std::map<const BLERemoteCharacteristic *, NotifyInfo> characteristics;

MyBLEClient::MyBLEClient(const char *addr, bool hasPasscode_, uint32_t passcode_): address(addr, 1), 
            hasPasscode(hasPasscode_), passcode(passcode_) {
    pClient = BLEDevice::createClient();
    pClientCallback = new MyBLEClientCallback(this);
    pClient->setClientCallbacks(pClientCallback, false);
}

MyBLEClient::~MyBLEClient() {
    unregisterAll();

    if (isConnected()) {
        pClient->disconnect();
        pClient->setClientCallbacks(nullptr);
        BLEDevice::deleteClient(pClient);
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


const char *MyBLEClient::getLastUpdateTimeText() const {
    time_t t = lastUpdateTime;
    t += TZ.getOffset();
    tm *tm = gmtime(&t);
    strftime(timeBuf, sizeof(timeBuf), "%F %T", tm);

    return timeBuf;
}


MyBLEClientCallback::MyBLEClientCallback(MyBLEClient *p): pClient(p) {
}

uint32_t MyBLEClientCallback::onPassKeyRequest() {
    logger.printf(LOG_BLE, LOG_INFO, "Client Passkey - %06d\n", passcode);
    return passcode;
}

void MyBLEClientCallback::onAuthenticationComplete(ble_gap_conn_desc *desc) {
    logger.printf(LOG_BLE, LOG_INFO, "Client Authentication complete - %d %d %d\n", desc->sec_state.authenticated, 
        desc->sec_state.encrypted, desc->sec_state.bonded);
}

bool MyBLEClientCallback::onConfirmPIN(uint32_t pin) {
    logger.printf(LOG_BLE, LOG_INFO, "Client Confirm PIN - %06d\n", pin);
    return true;
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
    //printf("BLE notify\n");
    if ((it != characteristics.end()) && (it->second.pClient)) {
        it->second.pClient->onNotify(it->second.index, pData, length);
    }
}
