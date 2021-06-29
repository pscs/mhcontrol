#pragma once

#include <lvgl.h>

class OptionsScreen {
public:
  void Create(lv_obj_t *scr);

  void Update();

private:
  lv_obj_t *timeField;
  lv_obj_t *timezoneField;
};

extern OptionsScreen Options;
