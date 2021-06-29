#pragma once

#include <lvgl.h>

extern lv_obj_t * wifiStrength_label;
extern lv_obj_t * wifiStrength_icon;
extern lv_obj_t * freeMem_label;

//extern lv_obj_t *scrPreferences;

void locationUpdate();

class ScreenClass {
public:
  void Create(lv_obj_t *scr);
  void Update();

private:
  lv_obj_t * counter_label;
  lv_obj_t * freeMem_label;
};

extern ScreenClass Screen;
