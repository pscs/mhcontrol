#include "screens.h"

#include "options.h"
#include "Controller.h"
#include "location.h"
#include "DS3231.h"
#include "timezone.h"
#include "power_screen.h"
#include "weather_screen.h"

ScreenClass Screen;

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
