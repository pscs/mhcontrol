#include "screens.h"

#include "options.h"
#include "Controller.h"
#include "location.h"
#include "DS3231.h"
#include "timezone.h"
#include "power_screen.h"
#include "weather_screen.h"

lv_obj_t * wifiStrength_label;
lv_obj_t * wifiStrength_icon;

lv_obj_t * time_label;

ScreenClass Screen;

void ScreenClass::Create(lv_obj_t *scr)
{
  lv_obj_t *tabs = lv_tabview_create(scr, LV_DIR_TOP, 20);
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

  counter_label = lv_label_create(scr );
  lv_label_set_text(counter_label, "ctr");
  lv_obj_align(counter_label, LV_ALIGN_TOP_LEFT, 0, 0); 

  freeMem_label = lv_label_create(tabOptions);
  lv_label_set_text(freeMem_label, "mem");
  lv_obj_align(freeMem_label, LV_ALIGN_BOTTOM_LEFT, 0, 0); 

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
  snprintf(buf, sizeof(buf), "%u", millis() / 100);
  lv_label_set_text(counter_label, buf);

  snprintf(buf, sizeof(buf), "%u", ESP.getFreeHeap());
  lv_label_set_text(freeMem_label, buf);

  time_t clockTime = Clock.Get();
  clockTime += TZ.getOffset();
  tm *clockTm = gmtime(&clockTime);
  strftime(buf, sizeof(buf), "%T", clockTm);
  lv_label_set_text(time_label, buf);
  log_v("End Screen Update\n");
}
