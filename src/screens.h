#pragma once

#include <lvgl.h>
#include <string>

void locationUpdate();

class ScreenClass {
public:
	void Create(lv_obj_t *scr);
	void Update();

	void updateSdCardIcon(bool mounted);

	static void doCalibration(lv_event_t *);
	static void setCalibration();
	static void setCalibrationFromString(const char *str);
	static std::string getCalibrationString();

	lv_obj_t *wifiStrength_label;
	lv_obj_t *wifiStrength_icon;
	lv_obj_t *diskIcon;

	lv_obj_t *tabs;
private:
	lv_obj_t *time_label;

	static uint16_t calibData[5];


};

extern ScreenClass Screen;
