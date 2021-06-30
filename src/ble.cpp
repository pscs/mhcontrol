#include "ble.h"

#include <BLEDevice.h>

#include "victron_smartsolar.h"
#include "victron_B2B.h"
#include "victron_mainscharger.h"
#include "ksenergy_battery.h"
#include "logger.h"
//#include "config.h"

BLEManager bleManager;

class MySecurityCallbacks: public BLESecurityCallbacks {
  public:
  uint32_t onPassKeyRequest() override {
    logger.print(LOG_BLE, LOG_VERBOSE, "*********************onPasskeyrequest\n");
    return passcode; //BLEPASSCODE;  
  }
  void onPassKeyNotify(uint32_t pk) override {
  }
  bool onSecurityRequest() override {
    return true;
  }
  void onAuthenticationComplete(esp_ble_auth_cmpl_t auth_cmpl) override {
    logger.print(LOG_BLE, LOG_VERBOSE, "onAuthenticationComplete\n");
    logger.print(LOG_BLE, LOG_INFO, "pair status = %s\n", auth_cmpl.success ? "success" : "fail");
  }
  bool onConfirmPIN(uint32_t pin) override {
    return true;
  }

  uint32_t passcode = 0;
};

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
public:
    MyAdvertisedDeviceCallbacks(BLEManager *p): pManager(p) {}
 /**
   * Called for each advertising BLE server.
   */
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    logger.print(LOG_BLE, LOG_VERBOSE, "BLE Advertised Device found: %s\n", advertisedDevice.toString().c_str());
    pManager->setFound(advertisedDevice.getAddress());
/*       if (advertisedDevice.getAddress() == victronSmartSolar.getAddress()) {
        printf("Found MPPT\r\n");
        pManager->victronSolarFound = true;
//        pMPPTDevice = new BLEAdvertisedDevice(advertisedDevice);
//        advertisedDevice.getScan()->stop();
       }
       */
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

    BLEDevice::setEncryptionLevel(ESP_BLE_SEC_ENCRYPT_MITM);

    pSecurityCallbacks = new MySecurityCallbacks();
    BLEDevice::setSecurityCallbacks(pSecurityCallbacks);

    BLESecurity *pSecurity = new BLESecurity();
    //pSecurity->setStaticPin(0);
    pSecurity->setAuthenticationMode(ESP_LE_AUTH_REQ_SC_ONLY | ESP_LE_AUTH_BOND);
    pSecurity->setCapability(ESP_IO_CAP_KBDISP);
    pSecurity->setRespEncryptionKey(ESP_BLE_ENC_KEY_MASK | ESP_BLE_ID_KEY_MASK | ESP_BLE_LINK_KEY_MASK);


    pScanner = BLEDevice::getScan();
    pScanner->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks(this));
    pScanner->setInterval(1349);
    pScanner->setWindow(449);
    //pScanner->setActiveScan(true);

    addBLEClient(victronSmartSolar);
    addBLEClient(victronB2B);
    addBLEClient(victronMainsCharger);
    addBLEClient(ksEnergyBattery);

    //victronSolarFound = false;
    logger.print(LOG_BLE, LOG_INFO, "Start BLE Scanner\n");
    scanInProgress = true;
    pScanner->start(5, BLEScanDone, false);
    logger.print(LOG_BLE, LOG_VERBOSE, "BLE Scanner started\n");
}

void BLEManager::addBLEClient(MyBLEClient &client) {
  clients.emplace_back(RegisteredBLEClient(client));
}

void BLEManager::setPasscode(uint32_t passcode) {
  pSecurityCallbacks->passcode = passcode;
}


void BLEManager::setFound(BLEAddress addr) {
    for (auto &c: clients) {
      if (c.client.getAddress() == addr) {
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
