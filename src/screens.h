#pragma once

#include <lvgl.h>

void locationUpdate();

class ScreenClass {
public:
	void Create(lv_obj_t *scr);
	void Update();

	void updateSdCardIcon(bool mounted);

	lv_obj_t *wifiStrength_label;
	lv_obj_t *wifiStrength_icon;
	lv_obj_t *diskIcon;

	lv_obj_t *tabs;
private:
	lv_obj_t *time_label;

};

extern ScreenClass Screen;
