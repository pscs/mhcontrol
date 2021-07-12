#include "ble.h"

#include "NimBLEDevice.h"

#include "victron_smartsolar.h"
#include "victron_B2B.h"
#include "victron_mainscharger.h"
#include "ksenergy_battery.h"
#include "logger.h"
#include "mybleserver.h"
#include "Arduino.h"

BLEManager bleManager;

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
public:
    MyAdvertisedDeviceCallbacks(BLEManager *p): pManager(p) {}
 /**
   * Called for each advertising BLE server.
   */
  void onResult(BLEAdvertisedDevice *advertisedDevice) override {
    //logger.print(LOG_BLE, LOG_VERBOSE, "BLE Advertised Device found: %s\n", advertisedDevice->toString().c_str());
    logger.printf(LOG_BLE, LOG_VERBOSE, "BLE advertised device found - address: %s - %u\n", advertisedDevice->getAddress().toString().c_str(), 
      advertisedDevice->getAddress().getType());
    pManager->setFound(advertisedDevice->getAddress());
  }
private:
    BLEManager *pManager;
};

void BLEScanDone(BLEScanResults) {
    logger.send(LOG_BLE, LOG_INFO, "BLE Scan Done\n");

    bleManager.scanInProgress = false;
}

void BLEManager::initialise()
{
    BLEDevice::init("mhcontrol");

    NimBLEDevice::deleteAllBonds();

    NimBLEDevice::setSecurityAuth(true, true, false);
    NimBLEDevice::setSecurityIOCap(BLE_HS_IO_KEYBOARD_DISPLAY);
    NimBLEDevice::setSecurityRespKey(BLE_SM_PAIR_KEY_DIST_ENC | BLE_SM_PAIR_KEY_DIST_ID  | BLE_SM_PAIR_KEY_DIST_SIGN | BLE_SM_PAIR_KEY_DIST_LINK);
//works for Victron servers
//   NimBLEDevice::setSecurityAuth(true, true, false);
//    NimBLEDevice::setSecurityIOCap(BLE_HS_IO_KEYBOARD_DISPLAY);
//    NimBLEDevice::setSecurityRespKey(BLE_SM_PAIR_KEY_DIST_ENC | BLE_SM_PAIR_KEY_DIST_ID  | BLE_SM_PAIR_KEY_DIST_SIGN | BLE_SM_PAIR_KEY_DIST_LINK);

    pScanner = BLEDevice::getScan();
    pScanner->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks(this));
    pScanner->setInterval(1349);
    pScanner->setWindow(449);

    addBLEClient(victronSmartSolar);
    addBLEClient(victronB2B);
    addBLEClient(victronMainsCharger);
    addBLEClient(ksEnergyBattery);

	server = new MyBLEServer();
	
    logger.send(LOG_BLE, LOG_INFO, "Start BLE Scanner\n");
    scanInProgress = true;
	scanStarted = millis();
    pScanner->start(5, BLEScanDone, false);
    logger.send(LOG_BLE, LOG_VERBOSE, "BLE Scanner started\n");
}

void BLEManager::addBLEClient(MyBLEClient &client) {
  clients.emplace_back(RegisteredBLEClient(client));
}

void BLEManager::setFound(BLEAddress addr) {
    for (auto &c: clients) {
      if (c.client.getAddress() == addr) {
        logger.printf(LOG_BLE, LOG_INFO, "Set found - %s\n", c.client.getName());
        c.found = true;
        break;
      }
    }
}

void BLEManager::connect() {
	logger.send(LOG_BLE, LOG_INFO, "Check for connections\n");
  if (scanInProgress) {
	  if (millis() - scanStarted > 6000) {
		  logger.send(LOG_BLE, LOG_INFO, "Force stop scanner after 6 seconds\n");
		  pScanner->stop();
	  }
  } else {

    bool anyNotConnected = false;
	bool anyNewConnections = false;
	
    for (auto &c: clients) {
      if (c.found) {
        //if (!c.client.isConnected()) {
          c.client.connectIfNecessary();
		  anyNewConnections = true;
        //}
        c.found = false;
	  }
    }

	if (!anyNewConnections) {
		for (auto &c: clients) {
			if (!c.client.isConnected()) {
				anyNotConnected = true;
				break;
			}
		}

		if (anyNotConnected) {
			logger.send(LOG_BLE, LOG_INFO, "BLE Scanner restarting\n");

			scanInProgress = true;
			scanStarted = millis();
			pScanner->start(5, BLEScanDone, true);
			logger.send(LOG_BLE, LOG_INFO, "BLE Scanner restarted\n");
		}
	}
  }
	logger.send(LOG_BLE, LOG_INFO, "Done Check for connections\n");

}

void BLEManager::keepAlive() {
	logger.send(LOG_BLE, LOG_INFO, "BLEMgr Keepalive\n");
  for (auto &c: clients) {
    if (c.client.isConnected()) {
      c.client.keepAlive();
    }
  }
  logger.send(LOG_BLE, LOG_INFO, "BLEMgr done keepalive\n");
}

bool BLEManager::isServerPinRequired() const {
	return server && server->isPinRequired() && !server->isPinScreenVisible();
}

void BLEManager::requestServerPin() {
	if (server) {
		server->requestPin();
	}
}

void BLEManager::closeServerPinScreen() {
	if (server) {
		server->closePinScreen();
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
