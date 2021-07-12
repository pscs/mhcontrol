#pragma once

#include <lvgl.h>
#include "window.h"

class OptionsScreen: public Window {
public:
  void Create(lv_obj_t *scr);

  void update() override;

private:
  lv_obj_t *timeField;
  lv_obj_t *timezoneField;
  lv_obj_t *uptimeField;
  lv_obj_t *freeMemField;
};

class NetworkWindow: public Window {
public:
	NetworkWindow(Window *parent);

	void update() override;

private:
	lv_obj_t *ssid;
	lv_obj_t *rssi;
	lv_obj_t *ipaddress;
	lv_obj_t *mask;
	lv_obj_t *gateway;
	lv_obj_t *dns;
};

class NetworkSettingsWindow: public Window {
public:
	NetworkSettingsWindow(Window *parent);

	void kbDone(lv_event_t *e) override;
	lv_keyboard_mode_t kbMode(lv_event_t *e) override;
	bool kbValidate(lv_event_t *e) override;
	void kbPostValidate(lv_event_t *e) override;
	void updateStates();
	void saveSettings();

private:
	lv_obj_t *ssid;
	lv_obj_t *password;
	lv_obj_t *DHCPSwitch;
	lv_obj_t *ipaddress;
	lv_obj_t *mask;
	lv_obj_t *gateway;
	lv_obj_t *dns1;
	lv_obj_t *dns2;
	lv_obj_t *saveBtn = nullptr;
};

extern OptionsScreen options;
