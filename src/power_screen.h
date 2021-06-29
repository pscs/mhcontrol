#pragma once

#include <lvgl.h>

class PowerScreen {
public:
  void Create(lv_obj_t *scr);

  void Update();
};

extern PowerScreen powerScreen;