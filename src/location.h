#pragma once

#include <TinyGPS++.h>
#include <lvgl.h>

class LocationScreen {
public:
  void Create(lv_obj_t *scr);

  void Update();

private:
  lv_obj_t *sats;
  lv_obj_t *hdop;
  lv_obj_t *coords;
  lv_obj_t *addr;
  lv_obj_t *gpsclock;
  lv_obj_t *chars;
};

extern LocationScreen Location;

extern TinyGPSPlus gps;