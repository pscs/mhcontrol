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

lv_style_t *style_textbox;
lv_style_t *style_textbox_invalid;
lv_style_t *style_textbox_disabled;

void ScreenClass::Create(lv_obj_t *scr)
{
	style_textbox = new lv_style_t;
	lv_style_init(style_textbox);
	//lv_style_set_bg_color(style_textbox, lv_palette_main(LV_PALETTE_LIGHT_GREEN));
	//lv_style_set_border_width(style_textbox, 2);
	//lv_style_set_border_color(style_textbox, lv_color_black());

	style_textbox_invalid = new lv_style_t;
	*style_textbox_invalid = *style_textbox;
	lv_style_set_text_color(style_textbox_invalid, lv_palette_main(LV_PALETTE_RED));

	style_textbox_disabled = new lv_style_t;
	*style_textbox_disabled = *style_textbox;
	lv_style_set_bg_color(style_textbox_invalid, lv_palette_main(LV_PALETTE_GREY));

	lv_theme_t *th_act = lv_disp_get_theme(NULL);
	static lv_theme_t th_new;
	th_new = *th_act;
	lv_theme_set_parent(&th_new, th_act);
	lv_theme_set_apply_cb(&th_new, [](lv_theme_t *, lv_obj_t *obj) {
		if (lv_obj_check_type(obj, &lv_label_class)) {
			lv_obj_add_style(obj, style_textbox, 0);
		}
		if (lv_obj_check_type(obj, &lv_textarea_class)) {
			lv_obj_add_style(obj, style_textbox, 0);
			lv_obj_add_style(obj, style_textbox_invalid, LV_STATE_USER_1);
			lv_obj_add_style(obj, style_textbox_disabled, LV_STATE_DISABLED);
		}
	});

	lv_disp_set_theme(NULL, &th_new);

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

	kb = lv_keyboard_create(lv_layer_top());
	lv_obj_add_flag(kb, LV_OBJ_FLAG_IGNORE_LAYOUT | LV_OBJ_FLAG_HIDDEN);
	lv_obj_align(kb, LV_ALIGN_BOTTOM_MID, 0, 0);
	lv_obj_set_size(kb, screenWidth, screenHeight / 2);
	lv_obj_add_event_cb(kb, [](lv_event_t *e) {
		Screen.closeKeyboard();
	}, LV_EVENT_CANCEL, NULL);

	powerScreen.Create(tabPower);
	options.Create(tabOptions);
	Location.Create(tabLocation);
	weatherScreen.Create(tabWeather);
}

void ScreenClass::fastUpdate() {
	powerScreen.fastUpdate();
	options.fastUpdate();
}

void ScreenClass::Update()
{
	powerScreen.update();
	options.update();
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


void ScreenClass::doCalibration() {
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

void ScreenClass::textFieldEvent(lv_event_t *e) {
	lv_event_code_t code = lv_event_get_code(e);
	if ((code == LV_EVENT_CLICKED) || (code == LV_EVENT_FOCUSED)) {
		lv_obj_t *t = lv_event_get_target(e);
		if (!Screen.kbParent) {
			Screen.kbParent = lv_obj_get_parent(lv_obj_get_parent(t));
			Screen.kbParentHeight = lv_obj_get_height(Screen.kbParent);
			lv_area_t coords;
			lv_obj_get_coords(Screen.kbParent, &coords);
			logger.printf(LOG_GENERAL, LOG_DEBUG, "onkbd resize %d - %d - %d\n", screenHeight, coords.y1, (screenHeight / 2) - coords.y1);
			lv_obj_set_height(Screen.kbParent, (screenHeight / 2) - coords.y1);

			lv_obj_get_coords(t, &coords);
			lv_obj_scroll_to_view(t, LV_ANIM_ON);
		}
		lv_keyboard_mode_t mode = LV_KEYBOARD_MODE_TEXT_LOWER;
		Window *p = reinterpret_cast<Window *>(lv_event_get_user_data(e));
		if (p) {
			mode = p->kbMode(e);
		}

		lv_keyboard_set_mode(Screen.kb, mode);
		lv_obj_clear_flag(Screen.kb, LV_OBJ_FLAG_HIDDEN);
		lv_keyboard_set_textarea(Screen.kb, t);
	} else if (code == LV_EVENT_VALUE_CHANGED) {
		Window *p = reinterpret_cast<Window *>(lv_event_get_user_data(e));
		if (p) {

			if (p->kbValidate(e)) {
				lv_obj_clear_state(lv_event_get_target(e), LV_STATE_USER_1);
			} else {
				lv_obj_add_state(lv_event_get_target(e), LV_STATE_USER_1);
			}
			p->kbPostValidate(e);
		}

	} else if (code == LV_EVENT_READY) {
		printf("text field ready event\n");
		Screen.closeKeyboard();
		void *p = lv_event_get_user_data(e);
		printf("param = %lu\n", p);
		if (p) {
			printf("send kbdone\n");
			reinterpret_cast<Window *>(p)->kbDone(e);
		}
	}
}

void ScreenClass::closeKeyboard() {
	lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
	if (kbParent && kbParentHeight) {
		lv_obj_set_height(kbParent, kbParentHeight);
	}
	kbParent = nullptr;
	kbParentHeight = 0;
}
