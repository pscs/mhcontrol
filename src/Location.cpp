#include "location.h"

#include <lvgl.h>
#include <stdio.h>

#include "DS3231.h"
#include "Controller.h"
#include "locator.h"

TinyGPSPlus gps;
LocationScreen Location;

void LocationScreen::Create(lv_obj_t *scr)
{
  static const lv_coord_t column_dsc[] = {50, LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST}; 
  static const lv_coord_t row_dsc[] = {fontHeight, fontHeight, fontHeight, fontHeight, fontHeight, 
    fontHeight, fontHeight, LV_GRID_TEMPLATE_LAST};
  lv_obj_set_grid_dsc_array(scr, column_dsc, row_dsc);
  lv_obj_set_layout(scr, LV_LAYOUT_GRID);

  int row = -1;

  lv_obj_t *label = lv_label_create(scr);
  lv_obj_set_grid_cell(label, LV_GRID_ALIGN_CENTER, 0, 1, LV_GRID_ALIGN_CENTER, ++row, 1);
  lv_label_set_text_static(label, "Sats");
  sats = lv_label_create(scr);
  lv_obj_set_grid_cell(sats, LV_GRID_ALIGN_CENTER, 1, 1, LV_GRID_ALIGN_CENTER, row, 1);
  lv_label_set_text(sats, "**");

  label = lv_label_create(scr);
  lv_obj_set_grid_cell(label, LV_GRID_ALIGN_CENTER, 0, 1, LV_GRID_ALIGN_CENTER, ++row, 1);
  lv_label_set_text_static(label, "HDOP");
  hdop = lv_label_create(scr);
  lv_obj_set_grid_cell(hdop, LV_GRID_ALIGN_CENTER, 1, 1, LV_GRID_ALIGN_CENTER, row, 1);
  lv_label_set_text(hdop, "**");

  label = lv_label_create(scr);
  lv_obj_set_grid_cell(label, LV_GRID_ALIGN_CENTER, 0, 1, LV_GRID_ALIGN_CENTER, ++row, 1);
  lv_label_set_text_static(label, "Coords");
  coords = lv_label_create(scr);
  lv_obj_set_grid_cell(coords, LV_GRID_ALIGN_CENTER, 1, 1, LV_GRID_ALIGN_CENTER, row, 1);
  lv_label_set_text(coords, "**");

  label = lv_label_create(scr);
  lv_obj_set_grid_cell(label, LV_GRID_ALIGN_CENTER, 0, 1, LV_GRID_ALIGN_CENTER, ++row, 1);
  lv_label_set_text_static(label, "Addr");
  addr = lv_label_create(scr);
  lv_obj_set_grid_cell(addr, LV_GRID_ALIGN_CENTER, 1, 1, LV_GRID_ALIGN_CENTER, row, 1);
  lv_label_set_text(addr, "**");

  label = lv_label_create(scr);
  lv_obj_set_grid_cell(label, LV_GRID_ALIGN_CENTER, 0, 1, LV_GRID_ALIGN_CENTER, ++row, 1);
  lv_label_set_text_static(label, "Time");
  gpsclock = lv_label_create(scr);
  lv_obj_set_grid_cell(gpsclock, LV_GRID_ALIGN_CENTER, 1, 1, LV_GRID_ALIGN_CENTER, row, 1);
  lv_label_set_text(gpsclock, "**");
  
  label = lv_label_create(scr);
  lv_obj_set_grid_cell(label, LV_GRID_ALIGN_CENTER, 0, 1, LV_GRID_ALIGN_CENTER, ++row, 1);
  lv_label_set_text_static(label, "Chars");
  chars = lv_label_create(scr);
  lv_obj_set_grid_cell(chars, LV_GRID_ALIGN_CENTER, 1, 1, LV_GRID_ALIGN_CENTER, row, 1);
  lv_label_set_text(chars, "**");
}

void LocationScreen::Update()
{
  log_v("locationUpdate\n");
  if (gps.satellites.isValid()) {
    lv_label_set_text_fmt(sats, "%d", gps.satellites.value()); 
  } else {
    lv_label_set_text(sats, "***"); 
  }
  if (gps.hdop.isValid()) {
    lv_label_set_text(hdop, String(gps.hdop.hdop(), 3).c_str()); 
  } else {
    lv_label_set_text(hdop, "***"); 
  }
  if (gps.location.isValid()) {
    float lat = gps.location.lat();
    float lng = gps.location.lng();
    char latD = 'N';
    char lngD = 'E';
    if (lat < 0) {
      latD = 'S';
      lat = -lat;
    };
    if (lng < 0) {
      lngD = 'W';
      lng = -lng;
    }
    lv_label_set_text_fmt(coords, "%s\u00b0%c,%s\u00b0%c", 
      String(lat, 5).c_str(), latD, 
      String(lng, 5).c_str(), lngD); 
  } else {
    lv_label_set_text(coords, "***,***"); 
  }
  if (gps.date.isValid()) {
    lv_label_set_text_fmt(gpsclock, "%d/%d/%d %d:%02d:%02d %d", gps.date.day(), gps.date.month(), gps.date.year(), 
      gps.time.hour(), gps.time.minute(), gps.time.second(), gps.date.age()); 

    if (gps.date.age() < 1500) {
      tm gpsTm {
        .tm_sec = gps.time.second(),
        .tm_min = gps.time.minute(),
        .tm_hour = gps.time.hour(),
        .tm_mday = gps.date.day(),
        .tm_mon = gps.date.month() - 1,
        .tm_year = gps.date.year() - 1900
      };
      printf("GPSTime %d %d %d %d %d %d\n", gpsTm.tm_year, gpsTm.tm_mon, gpsTm.tm_mday, gpsTm.tm_hour, gpsTm.tm_min, gpsTm.tm_sec);
      time_t gpsTime = mktime(&gpsTm);// + gps.date.age();

      time_t clockTime = Clock.Get();
      printf("RTC = %lu GPS = %lu\n", (long)clockTime, (long)gpsTime);
      if (abs(gpsTime - clockTime) > 10000) {
        Clock.Set(gpsTime);
      }
    }
  }

  lv_label_set_text(addr, locator.getAddress().c_str());
  lv_label_set_text_fmt(chars, "%lu", gps.charsProcessed());
}
