#pragma once

#include <lvgl.h>

class WeatherScreenNow {
public:
  void Create(lv_obj_t *scr);
  void Update();

private:
  lv_obj_t *tab;
  lv_obj_t *temp;
  lv_obj_t *feelsLike;

  lv_obj_t *windMeter;
  lv_meter_indicator_t *wind;
  lv_meter_indicator_t *windGusts;
  lv_obj_t *windInfo;

  lv_obj_t *icon;
};

class WeatherScreen {
public:
  void Create(lv_obj_t *scr);

  void Update();

private:
  WeatherScreenNow weatherScreenNow;
  lv_obj_t *tabHourly;
  lv_obj_t *tabDaily;
};

extern WeatherScreen weatherScreen;