#include "mybleserver.h"

#include "logger.h"
#include "Controller.h"
#include <Arduino.h>
#include "sound.h"

#define SERVICE_UUID1           "6E400001-B5A3-F393-E0A9-E50E24DCCA9E" 
#define SERVICE_UUID "ABCD"
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID "1234"

class MyBLEServerCallbacks: public BLEServerCallbacks {
public:
	MyBLEServerCallbacks(MyBLEServer *s): server(s) {};

    void onConnect(BLEServer* pServer) override {
		printf("BLEServer Connected\n");
      server->setConnected(true);
    };

    void onDisconnect(BLEServer* pServer) override {
		printf("BLEServer Disconnected\n");
		//NimBLEDevice::startAdvertising();
		//printf("BLEServer start advertising\n");
		server->setConnected(false);
    }

	uint32_t onPassKeyRequest() override {
		logger.send(LOG_BLE, LOG_DEBUG, "BLEServer onPassKeyRequest\n");
		server->setPIN(0);
		uint32_t start = millis();
		server->requirePin();
		//NimBLE is running in core 0, and Arduino is running in core 1, so we have multithreading here
		while (server->isPinRequired()) {
			delay(100);
			if (millis() - start > 30000) {
				logger.send(LOG_BLE, LOG_DEBUG, "Passkey Timeout\n");
				break;
			}
		}
		logger.printf(LOG_BLE, LOG_DEBUG, "Got Passkey - %d\n", server->getPIN());
		return server->getPIN();
	}

	void onAuthenticationComplete(ble_gap_conn_desc *desc) override {
		printf("BLEServer onAuthenticationComplete %d %d %d %d\n", desc->sec_state.encrypted, desc->sec_state.authenticated, desc->sec_state.bonded, desc->sec_state.key_size);
	}

	bool onConfirmPIN(uint32_t pin) override {
		printf("BLEServer onConfirmPin %d\n", pin);
		return true;
	}

private:
	MyBLEServer *server;
};

class MyCharacteristicCallbacks: public BLECharacteristicCallbacks {
public:
	void onRead(BLECharacteristic *characteristic) override {
		printf("BLEServer onRead\n");
	}

	void onWrite(BLECharacteristic *characteristic) override {
		printf("BLEServer onWrite\n");
	}

	void onNotify(BLECharacteristic *characteristic) override {
		printf("BLEServer onNotify\n");
	}

	void onStatus(BLECharacteristic *characteristic, Status s, int code) override {
		printf("BLEServer onStatus %d %d\n", (int)s, code);
	}

	void onSubscribe(BLECharacteristic *characteristic, ble_gap_conn_desc *desc, uint16_t subValue) override {
		printf("BLEServer onSubscribe\n");
	}
};

MyBLEServer::MyBLEServer() {
	server = BLEDevice::createServer();
	serverCB = new MyBLEServerCallbacks(this);
	server->setCallbacks(serverCB);
	service = server->createService(SERVICE_UUID);
	characteristic = service->createCharacteristic(
                                         CHARACTERISTIC_UUID,
                                         NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::READ_ENC | NIMBLE_PROPERTY::READ_AUTHEN |
										 NIMBLE_PROPERTY::WRITE
                                        // BLECharacteristic::PROPERTY_READ |
                                       //  BLECharacteristic::PROPERTY_WRITE
                                       );
	characteristic->setCallbacks(new MyCharacteristicCallbacks());
	characteristic->setValue("Hello World says Neil");
	service->start();
	// BLEAdvertising *pAdvertising = pServer->getAdvertising();  // this still is working for backward compatibility
	BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
	pAdvertising->addServiceUUID(SERVICE_UUID);
	pAdvertising->setScanResponse(true);
	pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
	pAdvertising->setMinPreferred(0x12);
	BLEDevice::startAdvertising();
	//printf("Characteristic defined! Now you can read it in your phone!");
	return;

#if 0
	//NimBLEDevice::setSecurityPasskey(123456);
	server = BLEDevice::createServer();
	serverCB = new MyBLEServerCallbacks(this);
	server->setCallbacks(serverCB);
	service = server->createService(SERVICE_UUID);

	characteristic = service->createCharacteristic(
                      CHARACTERISTIC_UUID,
                     // NIMBLE_PROPERTY::NOTIFY | 
					  NIMBLE_PROPERTY::READ | //NIMBLE_PROPERTY::READ_ENC | NIMBLE_PROPERTY::READ_AUTHEN | 
					  NIMBLE_PROPERTY::WRITE //| NIMBLE_PROPERTY::WRITE_ENC | NIMBLE_PROPERTY::WRITE_AUTHEN
                    );

	//characteristic->setCallbacks(new MyCharacteristicCallbacks());

	//BLEAdvertisementData responseData;
  	//responseData.setName("mhcontrol");
  	//responseData.setCompleteServices(NimBLEUUID(SERVICE_UUID));

	service->start();

	characteristic->setValue("Hello World says Neil");

	//characteristic->setValue("hello");

	BLEAdvertising *pAdvertising = NimBLEDevice::getAdvertising(); //server->getAdvertising();
	//pAdvertising->setScanResponseData(responseData);
  	pAdvertising->addServiceUUID(SERVICE_UUID);
	pAdvertising->setScanResponse(true);
  	pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
  	pAdvertising->setMinPreferred(0x12);

	pAdvertising->start();
#endif
}

MyBLEServer::~MyBLEServer() {
	if (server) {
		server->setCallbacks(nullptr);
	}
}

void MyBLEServer::setPIN(uint32_t pin_) {
	pin = pin_;
}

void MyBLEServer::closePinScreen() {
	if (pinScr) {
		lv_obj_del_async(pinScr);
		pinScr = nullptr;
		clearPinRequired();
	}
}
void MyBLEServer::requestPin() {
	pinScr = lv_win_create(lv_scr_act(), 40);
	lv_win_add_title(pinScr, "Bluetooth PIN");
	sound.addNote(500, 50);
	sound.addNote(1000, 50);

	lv_obj_t *cont = lv_win_get_content(pinScr);
	lv_obj_t *pin = lv_textarea_create(cont);
	lv_obj_set_size(pin, screenWidth - 10, 35);
	lv_textarea_set_placeholder_text(pin, "Passcode");
    lv_obj_add_state(pin, LV_STATE_FOCUSED);
	lv_obj_set_align(pin, LV_ALIGN_TOP_MID);
	lv_textarea_set_max_length(pin, 6);

	lv_obj_add_event_cb(pin, [](lv_event_t *e) {
		MyBLEServer *s = reinterpret_cast<MyBLEServer *>(lv_event_get_user_data(e));
		lv_obj_t *ta = lv_event_get_target(e);
		printf("Entered PIN - %s\n", lv_label_get_text(ta));
		s->setPIN(atoi(lv_textarea_get_text(ta)));
		s->closePinScreen();
	}, LV_EVENT_READY, this);

    static const char * btnm_map[] = {"1", "2", "3", "\n",
                               "4", "5", "6", "\n",
                               "7", "8", "9", "\n",
                               LV_SYMBOL_BACKSPACE, "0", LV_SYMBOL_NEW_LINE, ""};
    lv_obj_t * btnm = lv_btnmatrix_create(cont);
    lv_obj_set_size(btnm, 200, screenHeight - 110);
    lv_obj_align(btnm, LV_ALIGN_BOTTOM_MID, 0, -5);
    lv_obj_add_event_cb(btnm, [](lv_event_t *e) {
		lv_obj_t * obj = lv_event_get_target(e);
		lv_obj_t * ta = reinterpret_cast<lv_obj_t *>(lv_event_get_user_data(e));
		const char * txt = lv_btnmatrix_get_btn_text(obj, lv_btnmatrix_get_selected_btn(obj));

		if(strcmp(txt, LV_SYMBOL_BACKSPACE) == 0) lv_textarea_del_char(ta);
		else if(strcmp(txt, LV_SYMBOL_NEW_LINE) == 0) lv_event_send(ta, LV_EVENT_READY, NULL);
		else lv_textarea_add_text(ta, txt);
	}, LV_EVENT_VALUE_CHANGED, pin);
    lv_obj_clear_flag(btnm, LV_OBJ_FLAG_CLICK_FOCUSABLE); /*To keep the text area focused on button clicks*/
    lv_btnmatrix_set_map(btnm, btnm_map);
}
