#pragma once

#include <lvgl.h>
#include <string>

void locationUpdate();

class ScreenClass {
public:
	void Create(lv_obj_t *scr);
	void Update();
	void fastUpdate();

	void updateSdCardIcon(bool mounted);

	void closeKeyboard();

	static void doCalibration();
	static void setCalibration();
	static void setCalibrationFromString(const char *str);
	static std::string getCalibrationString();

	static void textFieldEvent(lv_event_t *e);

	lv_obj_t *wifiStrength_label;
	lv_obj_t *wifiStrength_icon;
	lv_obj_t *diskIcon;

	lv_obj_t *tabs;
	lv_obj_t *kb;

private:
	lv_obj_t *time_label;
	uint16_t kbParentHeight = 0;
	lv_obj_t *kbParent = nullptr;

	static uint16_t calibData[5];


};

extern ScreenClass Screen;
