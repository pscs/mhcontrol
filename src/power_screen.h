#pragma once

#include <lvgl.h>

#include "window.h"

class PowerScreen: public Window {
public:
	void Create(lv_obj_t *scr);

	void Update() override;

//	void showMainsInfo();
	void showSolarInfo();
	void showB2BInfo();

private:
	//lv_obj_t *tab;

	lv_obj_t *battery;
	lv_obj_t *currentMeter;
	lv_meter_scale_t *currentMeterScale;
	lv_meter_indicator_t *currentIndicator;
	lv_meter_indicator_t *maxCurrentIndicator;
	lv_meter_indicator_t *minCurrentIndicator;
	lv_obj_t *socMeter;
	lv_obj_t *solarBar;
	//lv_obj_t *powerSources;
	lv_obj_t *mainsImg;
	lv_obj_t *solarImg;
	lv_obj_t *b2bImg;
};

class MainsChargerInfoWindow: public Window {
public:
	MainsChargerInfoWindow(Window *parent);

	void Update() override;

private:
	lv_obj_t *connState;
	lv_obj_t *lastUpdate;
	lv_obj_t *voltage;
	lv_obj_t *current;
	lv_obj_t *temperature;
	lv_obj_t *status;
};

class SolarInfoWindow: public Window {
public:
	SolarInfoWindow(Window *parent);

	void Update() override;

private:
	lv_obj_t *connState;
	lv_obj_t *lastUpdate;
	lv_obj_t *panelVoltage;
	lv_obj_t *panelPower;
	lv_obj_t *chargeVoltage;
	lv_obj_t *chargeCurrent;
	lv_obj_t *temperature;
	lv_obj_t *status;
};

class B2BInfoWindow: public Window {
public:
	B2BInfoWindow(Window *parent);

	void Update() override;

private:
	lv_obj_t *connState;
	lv_obj_t *lastUpdate;
	lv_obj_t *inputVoltage;
	lv_obj_t *chargeVoltage;
	lv_obj_t *temperature;
	lv_obj_t *status;
};

extern PowerScreen powerScreen;