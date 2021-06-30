#pragma once

#include <lvgl.h>

class PowerScreen {
public:
  void Create(lv_obj_t *scr);

  void Update();

private:
  lv_obj_t *solarIn;
  lv_obj_t *solarOut;
  lv_obj_t *b2b;
  lv_obj_t *mains;
  lv_obj_t *battery;
};

extern PowerScreen powerScreen;