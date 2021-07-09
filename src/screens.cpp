#include "screens.h"

#include "options.h"
#include "Controller.h"
#include "location.h"
#include "DS3231.h"
#include "timezone.h"
#include "power_screen.h"
#include "weather_screen.h"
#include "config.h"
#include <TFT_eSPI.h>
#include "inifile.h"
#include "sound.h"
#include "logger.h"

ScreenClass Screen;

uint16_t ScreenClass::calibData[5];


extern lv_img_dsc_t imgSdCard;

void ScreenClass::Create(lv_obj_t *scr)
{
	tabs = lv_tabview_create(scr, LV_DIR_TOP, 20);
	lv_obj_set_size(tabs, lv_disp_get_hor_res(NULL), lv_disp_get_ver_res(NULL) - 20);
	lv_obj_align(tabs, LV_ALIGN_BOTTOM_MID, 0, 0);
	lv_obj_t *tabPower = lv_tabview_add_tab(tabs, "Power");
	lv_obj_t *tabWeather = lv_tabview_add_tab(tabs, "Weather");
	lv_obj_t *tabLocation = lv_tabview_add_tab(tabs, "Location");
	lv_obj_t *tabOptions = lv_tabview_add_tab(tabs, "Options");

	//lv_obj_t * tv = lv_tabview_create(scr);
	//lv_obj_set_size(tv, lv_disp_get_hor_res(NULL), lv_disp_get_ver_res(NULL));

	wifiStrength_label = lv_label_create(scr);
	lv_label_set_text(wifiStrength_label, "str");
	lv_obj_align(wifiStrength_label, LV_ALIGN_TOP_RIGHT, -16, 0);
	//lv_obj_set_auto_realign(wifiStrength_label, true); 

	wifiStrength_icon = lv_img_create(scr);
	// lv_img_set_src(wifiStrength_icon, &img_Wifi0);
	lv_obj_set_style_img_recolor(wifiStrength_icon, lv_theme_get_color_primary(scr), 0);
	lv_obj_align(wifiStrength_icon, LV_ALIGN_TOP_RIGHT, 0, 0);

	diskIcon = lv_img_create(scr);
	lv_img_set_src(diskIcon, &imgSdCard);
	lv_obj_align(diskIcon, LV_ALIGN_TOP_LEFT, 0, 0);

	time_label = lv_label_create(scr);
	lv_label_set_text(time_label, "time");
	lv_obj_align(time_label, LV_ALIGN_TOP_MID, 0, 0);

	powerScreen.Create(tabPower);
	Options.Create(tabOptions);
	Location.Create(tabLocation);
	weatherScreen.Create(tabWeather);
}

void ScreenClass::Update()
{
	powerScreen.Update();
	Options.Update();
	Location.Update();
	weatherScreen.Update();
	log_v("End Weather Update\n");

	char buf[20];

	time_t clockTime = Clock.Get();
	clockTime += TZ.getOffset();
	tm *clockTm = gmtime(&clockTime);
	strftime(buf, sizeof(buf), "%T", clockTm);
	lv_label_set_text(time_label, buf);

	log_v("End Screen Update\n");
}

void ScreenClass::updateSdCardIcon(bool mounted) {
	if (mounted) {
		lv_obj_set_style_img_opa(diskIcon, LV_OPA_COVER, LV_PART_MAIN);
	} else {
		lv_obj_set_style_img_opa(diskIcon, LV_OPA_TRANSP, LV_PART_MAIN);
	}
}


void ScreenClass::doCalibration(lv_event_t *) {
	sound.addNote(1000, 100);

	tft.fillScreen(TFT_BLACK);
	tft.setCursor(20, 0);
	tft.setTextFont(2);
	tft.setTextSize(1);
	tft.setTextColor(TFT_WHITE, TFT_BLACK);

	tft.println("Touch corners as indicated");

	tft.setTextFont(1);
	tft.println();

	uint16_t calData[5];

	tft.calibrateTouch(calData, TFT_MAGENTA, TFT_BLACK, 15);

	lv_obj_t *scr = lv_scr_act();
	lv_scr_load(scr);
	for (int i = 0; i < 5; i++) {
		logger.printf(LOG_GENERAL, LOG_INFO, "Screen cal data %d = %d\n", i, calData[i]);
	}

	memcpy(calibData, calData, sizeof(calibData));
	setCalibration();
	IniFile::save(SETTINGS_FILENAME);

	sound.addNote(1000, 50);
	sound.addNote(2000, 50);
	sound.addNote(4000, 50);
}

void ScreenClass::setCalibration() {
//  uint16_t calData[5] = { 419, 3299, 428, 3182, 1 };
	if ((calibData[0] == 0) && (calibData[1] == 0)) {
		setCalibrationFromString(DEFAULT_SCREEN_CALIBRATION);
	} else {
		tft.setTouch(calibData);
	}
}

void ScreenClass::setCalibrationFromString(const char *str_) {
	char *str = strdup(str_);
	const char *p1 = strtok(str, ",");
	const char *p2 = strtok(nullptr, ",");
	const char *p3 = strtok(nullptr, ",");
	const char *p4 = strtok(nullptr, ",");
	const char *p5 = strtok(nullptr, ",");
	if (p5) {
		calibData[0] = atoi(p1);
		calibData[1] = atoi(p2);
		calibData[2] = atoi(p3);
		calibData[3] = atoi(p4);
		calibData[4] = atoi(p5);
		tft.setTouch(calibData);
	}
	free(str);
}

std::string ScreenClass::getCalibrationString() {
	char buffer[50];
	snprintf(buffer, sizeof(buffer), "%d,%d,%d,%d,%d", calibData[0], calibData[1], calibData[2], calibData[3], calibData[4]);
	return buffer;
}
