#include "weather_screen.h"

#include "weather.h"

#include "01d.cpp"
#include "icon02d.cpp"

WeatherScreen weatherScreen;

static lv_style_t largeFontStyle;
static lv_style_t redFontStyle;
static lv_style_t blueFontStyle;
static lv_style_t smallFontStyle;

LV_IMG_DECLARE(icon01d);
LV_IMG_DECLARE(icon02d);

const char * const dirs[] {"N", "NNE", "NE", "ENE", "E", "ESE", "SE", "SSE", "S", "SSW", "SW", "WSW", "W", "WNW", "NW", "NNW"};

void WeatherScreen::Create(lv_obj_t *scr) {
  lv_obj_t *tabs = lv_tabview_create(scr, LV_DIR_LEFT, 40);

  lv_style_init(&largeFontStyle);
  //lv_style_set_text_color(&largeFontStyle, lv_palette_main(LV_PALETTE_RED));
  lv_style_set_text_font(&largeFontStyle, &lv_font_montserrat_26);

  lv_style_init(&smallFontStyle);
  //lv_style_set_text_color(&largeFontStyle, lv_palette_main(LV_PALETTE_RED));
  lv_style_set_text_font(&smallFontStyle, &lv_font_montserrat_8);

  lv_style_init(&redFontStyle);
  lv_style_set_text_color(&redFontStyle, lv_palette_main(LV_PALETTE_RED));

  lv_style_init(&blueFontStyle);
  lv_style_set_text_color(&blueFontStyle, lv_palette_main(LV_PALETTE_BLUE));

  weatherScreenNow.Create(lv_tabview_add_tab(tabs, "Now"));

  tabHourly = lv_tabview_add_tab(tabs, "Hr"); 
  tabDaily = lv_tabview_add_tab(tabs, "Day"); 
}

void WeatherScreen::Update()
{
    log_v("weatherUpdate\n");
    if (weather.isLoaded()) {
        weatherScreenNow.Update();
    }
}

void WeatherScreenNow::Create(lv_obj_t *scr) {
  tab = scr;

  temp = lv_label_create(tab);
  lv_obj_set_align(temp, LV_ALIGN_TOP_LEFT);
  lv_obj_add_style(temp, &largeFontStyle, LV_PART_MAIN);
  lv_label_set_text_static(temp, "");

  feelsLike = lv_label_create(tab);
  lv_obj_set_align(feelsLike, LV_ALIGN_TOP_RIGHT);
  lv_obj_add_style(feelsLike, &largeFontStyle, LV_PART_MAIN);
  lv_label_set_text_static(feelsLike, "");

  windMeter = lv_meter_create(tab);
  lv_obj_remove_style(windMeter, NULL, LV_PART_INDICATOR);
  lv_obj_set_align(windMeter, LV_ALIGN_BOTTOM_LEFT);
  lv_obj_set_size(windMeter, 100, 100);
  lv_obj_add_style(windMeter, &smallFontStyle, LV_PART_TICKS);
  lv_meter_scale_t *scale = lv_meter_add_scale(windMeter);
  lv_meter_set_scale_ticks(windMeter, scale, 11, 1, 5, lv_palette_main(LV_PALETTE_GREY));
  lv_meter_set_scale_major_ticks(windMeter, scale, 5, 1, 10, lv_color_hex3(0xeee), 10);
  lv_meter_set_scale_range(windMeter, scale, 0, 100, 270, 90);

  wind = lv_meter_add_arc(windMeter, scale, 5, lv_palette_main(LV_PALETTE_BLUE), 0);
  windGusts = lv_meter_add_arc(windMeter, scale, 5, lv_palette_main(LV_PALETTE_RED), -5);

  windInfo = lv_label_create(tab);
  lv_obj_set_align(windInfo, LV_ALIGN_BOTTOM_RIGHT);
  lv_obj_add_style(windInfo, &largeFontStyle, LV_PART_MAIN);
  lv_label_set_text_static(windInfo, "");

  icon = lv_img_create(tab);
  lv_obj_set_align(icon, LV_ALIGN_RIGHT_MID);

}

void WeatherScreenNow::Update() {

  lv_obj_remove_style(temp, &blueFontStyle, LV_PART_MAIN);
  lv_obj_remove_style(temp, &redFontStyle, LV_PART_MAIN);
  if (weather.now.temp >= (2200 + 27315)) {
      lv_obj_add_style(temp, &redFontStyle, LV_PART_MAIN);
  } else if (weather.now.temp <= (800 + 27315)) {
      lv_obj_add_style(temp, &blueFontStyle, LV_PART_MAIN);
  }
  lv_label_set_text_fmt(temp, "%s\u00b0C", String(((weather.now.temp - 27315.0) / 100), 1).c_str());

  
  lv_obj_remove_style(feelsLike, &blueFontStyle, LV_PART_MAIN);
  lv_obj_remove_style(feelsLike, &redFontStyle, LV_PART_MAIN);
  if (weather.now.feelsLike >= (2200 + 27315)) {
      lv_obj_add_style(feelsLike, &redFontStyle, LV_PART_MAIN);
  } else if (weather.now.feelsLike <= (800 + 27315)) {
      lv_obj_add_style(feelsLike, &blueFontStyle, LV_PART_MAIN);
  }
  lv_label_set_text_fmt(feelsLike, "(%s\u00b0C)", String(((weather.now.feelsLike - 27315.0) / 100), 1).c_str());

  float w = (weather.now.windSpeed / 100.0) * 2.23694;
  lv_meter_set_indicator_end_value(windMeter, wind, w);

  lv_meter_set_indicator_end_value(windMeter, windGusts, (weather.now.windGust / 100.0) * 2.23694);

  char buf[20];

  int iDir = static_cast<int>((weather.now.windDeg + 11.25) / 22.5) % 16;
  const char *dir = dirs[iDir];

  snprintf(buf, sizeof(buf), "%d mph %s", (int)w, dir);

  lv_label_set_text(windInfo, buf);

  //lv_label_set_text_fmt(icon, "%d", weather.now.icon);

  lv_img_set_src(icon, &icon02d);

}
