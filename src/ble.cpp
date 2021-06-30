#include "ble.h"

//#include <BLEDevice.h>
#include "NimBLEDevice.h"

#include "victron_smartsolar.h"
#include "victron_B2B.h"
#include "victron_mainscharger.h"
#include "ksenergy_battery.h"
#include "logger.h"
//#include "config.h"

BLEManager bleManager;

/*class MySecurityCallbacks: public BLESecurityCallbacks {
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
  void onAuthenticationComplete(ble_gap_conn_desc *desc) override {
    logger.print(LOG_BLE, LOG_VERBOSE, "onAuthenticationComplete\n");
    logger.print(LOG_BLE, LOG_INFO, "pair status = %s\n", auth_cmpl.success ? "success" : "fail");
  }
  bool onConfirmPIN(uint32_t pin) override {
    return true;
  }

  uint32_t passcode = 0;
};*/

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

    NimBLEDevice::deleteAllBonds();

    //BLEDevice::setEncryptionLevel(ESP_BLE_SEC_ENCRYPT_MITM);

    NimBLEDevice::setSecurityAuth(true, true, false);
    NimBLEDevice::setSecurityIOCap(BLE_HS_IO_KEYBOARD_DISPLAY);
    NimBLEDevice::setSecurityRespKey(BLE_SM_PAIR_KEY_DIST_ENC | BLE_SM_PAIR_KEY_DIST_ID  | BLE_SM_PAIR_KEY_DIST_SIGN | BLE_SM_PAIR_KEY_DIST_LINK);

 /*   pSecurityCallbacks = new MySecurityCallbacks();
    BLEDevice::setSecurityCallbacks(pSecurityCallbacks);

    BLESecurity *pSecurity = new BLESecurity();
    //pSecurity->setStaticPin(0);
    pSecurity->setAuthenticationMode(ESP_LE_AUTH_REQ_SC_ONLY | ESP_LE_AUTH_BOND);
    pSecurity->setCapability(ESP_IO_CAP_KBDISP);
    pSecurity->setRespEncryptionKey(ESP_BLE_ENC_KEY_MASK | ESP_BLE_ID_KEY_MASK | ESP_BLE_LINK_KEY_MASK);
*/

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
  //pSecurityCallbacks->passcode = passcode;
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
    printf("Connect1\n");
    for (auto &c: clients) {
      if (c.found) {
    printf("Try Connect1\n");
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
