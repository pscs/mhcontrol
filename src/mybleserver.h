#pragma once
#include "NimBLEDevice.h"
#include <lvgl.h>

class MyBLEServerCallbacks;

class MyBLEServer {
public:
	MyBLEServer();
	~MyBLEServer();

	void setConnected(bool c) {
		connected = c;
	}
	void setPIN(uint32_t pin);
	uint32_t getPIN() const {
		return pin;
	}
	void requirePin() {
		pinRequired = true;
	}
	void clearPinRequired() {
		pinRequired = false;
	}
	bool isPinRequired() const {
		return pinRequired;
	}
	bool isPinScreenVisible() const {
		return pinScr;
	}

	void requestPin();
	void closePinScreen();

private:
	BLEServer *server = nullptr;
	MyBLEServerCallbacks *serverCB = nullptr;
	BLEService *service;
	BLECharacteristic *characteristic;
	bool connected = false;
	uint32_t pin = 0;
	bool pinRequired = false;
	lv_obj_t *pinScr = nullptr;
};