#include "ble.h"

#include "NimBLEDevice.h"

#include "victron_smartsolar.h"
#include "victron_B2B.h"
#include "victron_mainscharger.h"
#include "ksenergy_battery.h"
#include "logger.h"

BLEManager bleManager;

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
public:
    MyAdvertisedDeviceCallbacks(BLEManager *p): pManager(p) {}
 /**
   * Called for each advertising BLE server.
   */
  void onResult(BLEAdvertisedDevice *advertisedDevice) override {
    //logger.print(LOG_BLE, LOG_VERBOSE, "BLE Advertised Device found: %s\n", advertisedDevice->toString().c_str());
    logger.print(LOG_BLE, LOG_VERBOSE, "BLE advertised device found - address: %s - %u\n", advertisedDevice->getAddress().toString().c_str(), 
      advertisedDevice->getAddress().getType());
    pManager->setFound(advertisedDevice->getAddress());
  }
private:
    BLEManager *pManager;
};

void BLEScanDone(BLEScanResults) {
    logger.print(LOG_BLE, LOG_INFO, "BLE Scan Done\n");

    bleManager.scanInProgress = false;
}

void BLEManager::initialise()
{
    BLEDevice::init("");

    NimBLEDevice::deleteAllBonds();

    //BLEDevice::setEncryptionLevel(ESP_BLE_SEC_ENCRYPT_MITM);

    NimBLEDevice::setSecurityAuth(true, true, false);
    NimBLEDevice::setSecurityIOCap(BLE_HS_IO_KEYBOARD_DISPLAY);
    NimBLEDevice::setSecurityRespKey(BLE_SM_PAIR_KEY_DIST_ENC | BLE_SM_PAIR_KEY_DIST_ID  | BLE_SM_PAIR_KEY_DIST_SIGN | BLE_SM_PAIR_KEY_DIST_LINK);

    pScanner = BLEDevice::getScan();
    pScanner->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks(this));
    pScanner->setInterval(1349);
    pScanner->setWindow(449);

    addBLEClient(victronSmartSolar);
    addBLEClient(victronB2B);
    addBLEClient(victronMainsCharger);
    addBLEClient(ksEnergyBattery);

    logger.print(LOG_BLE, LOG_INFO, "Start BLE Scanner\n");
    scanInProgress = true;
    pScanner->start(5, BLEScanDone, false);
    logger.print(LOG_BLE, LOG_VERBOSE, "BLE Scanner started\n");
}

void BLEManager::addBLEClient(MyBLEClient &client) {
  clients.emplace_back(RegisteredBLEClient(client));
}

void BLEManager::setFound(BLEAddress addr) {
    for (auto &c: clients) {
      if (c.client.getAddress() == addr) {
        printf("Set found - %s\n", c.client.getName());
        c.found = true;
        break;
      }
    }
}

void BLEManager::connect() {
  if (!scanInProgress) {
    for (auto &c: clients) {
      if (c.found) {
        if (!c.client.isConnected()) {
          c.client.connectIfNecessary();
        }
        c.found = false;
      }
    }

    bool anyNotConnected = false;
    for (auto &c: clients) {
      if (!c.client.isConnected()) {
        anyNotConnected = true;
        break;
      }
    }

    if (anyNotConnected) {
      scanInProgress = true;
      pScanner->start(5, BLEScanDone, false);
      logger.print(LOG_BLE, LOG_INFO, "BLE Scanner started\n");
    }
  }

}

void BLEManager::keepAlive() {
  for (auto &c: clients) {
    if (c.client.isConnected()) {
      c.client.keepAlive();
    }
  }
}

void BLEConnect()
{
    bleManager.connect();
}

void BLEKeepAlive()
{
    bleManager.keepAlive();
}
