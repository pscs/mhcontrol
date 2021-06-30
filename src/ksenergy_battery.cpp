#include "ksenergy_battery.h"

#include <Arduino.h>

#include "config.h"

static const BLEUUID generalServiceUUID("fff0");
static const BLEUUID charUUID1("fff6");

const int NotifyBufferSize = 100;


KSEnergyBattery ksEnergyBattery(KSENERGY_BATTERY_ADDRESS);

KSEnergyBattery::KSEnergyBattery(const char *address): MyBLEClient(address, false, 0)
{}

KSEnergyBattery::~KSEnergyBattery() {
    unregisterAll();
}

const char *KSEnergyBattery::getName() const {
    return "Battery";
}

bool KSEnergyBattery::connectIfNecessary() {
 
   if (!pNotifyBuffer) {
        pNotifyBuffer = reinterpret_cast<uint8_t *>(ps_malloc(NotifyBufferSize));
    }
    printf("Battery - try connect\n");
    if (isConnected()) {
        printf("Battery - already connected\n");
        return true;
    } else if (!isEnabled()) {
        printf("Battery - disabled\n");
        return false;
    }

    printf("Battery - start connect\n");

    if (pClient->connect(address, false)) {
        printf("%s - Connected to Server\r\n", getName());
    } else {
        printf("%s - Failed to connect to server\r\n", getName());
        return false;
    }

    BLERemoteService* pRemoteService = pClient->getService(generalServiceUUID);
    if (pRemoteService == nullptr) {
        printf("%s Failed to find our service UUID\r\n", getName());
        pClient->disconnect();
        return false;
    } else {
        printf(" - %s Found our service\r\n", getName());

        pCharacteristic = pRemoteService->getCharacteristic(charUUID1);
        if (pCharacteristic == nullptr) {
            pClient->disconnect();
            return false;
        } else {
            if(pCharacteristic->canNotify()) {
                registerCharacteristics(pCharacteristic, this, 1);
                //characteristics[pChar1] = NotifyInfo(this, 1);
                pCharacteristic->subscribe(true, &notifyCallback);
                //pCharacteristic->registerForNotify(&notifyCallback);
            }
        }
    }

    connected = true;
    return true;
}

void KSEnergyBattery::unregisterAll() {

    pCharacteristic->unsubscribe();
    unregisterCharacteristics(pCharacteristic, this);
    pCharacteristic = nullptr;
}

void KSEnergyBattery::keepAlive() {
    if (isConnected() && isEnabled()) {
        const char *cmd = ":015150000EFE";
        pCharacteristic->writeValue(cmd, 13);
    }
}

void KSEnergyBattery::onNotify(uint8_t index, uint8_t *pData, size_t length) {
    printf("Battery Notify: ");
    for (int i = 0; i < length; i++) {
        printf("%c", pData[i]);
    }
    printf("\n");
}
