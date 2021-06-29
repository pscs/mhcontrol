#include <cinttypes>

#include <lvgl.h>
#include <WiFi.h>

#include "screens.h"

static const uint8_t img_WifiNone_data[] = {
  0x00, 0x00, 0x00, 0x00,   /*Color of index 0*/
  0x00, 0x00, 0x00, 0xff,
  
  0,0,0,0, 0x00, 0x00, 0x00, 0x00,
  0,0,0,0, 0x00, 0x00, 0x00, 0x00,
  0,0,0,0, 0x00, 0x00, 0x00, 0x00,
  0,0,0,0, 0xc0, 0x00, 0xc0, 0x00};

static const lv_img_dsc_t img_WifiNone {
  .header = {.cf = LV_IMG_CF_INDEXED_1BIT, .always_zero = 0, .reserved = 0, .w = 16, .h = 16},
  .data_size = 16 * 16 / 8,
  .data = img_WifiNone_data
};

static const uint8_t img_Wifi0_data[] = {
  0x00, 0x00, 0x00, 0x00,   /*Color of index 0*/
  0x00, 0x00, 0x00, 0xff,
  
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b11000000, 0b00000000,
  0b11000000, 0b00000000,
  0b11000000, 0b00000000,
  0b11000000, 0b00000000
};

static const lv_img_dsc_t img_Wifi0 {
  .header = {.cf = LV_IMG_CF_INDEXED_1BIT, .always_zero = 0, .reserved = 0, .w = 16, .h = 16},
  .data_size = 16 * 16 / 8,
  .data = img_Wifi0_data
};

static const uint8_t img_Wifi1_data[] = {
  0x00, 0x00, 0x00, 0x00,   /*Color of index 0*/
  0x00, 0x00, 0x00, 0xff,
  
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00001100, 0b00000000,
  0b00001100, 0b00000000,
  0b00001100, 0b00000000,
  0b00001100, 0b00000000,
  0b11001100, 0b00000000,
  0b11001100, 0b00000000,
  0b11001100, 0b00000000,
  0b11001100, 0b00000000
};

static const lv_img_dsc_t img_Wifi1 {
  .header = {.cf = LV_IMG_CF_INDEXED_1BIT, .always_zero = 0, .reserved = 0, .w = 16, .h = 16},
  .data_size = 16 * 16 / 8,
  .data = img_Wifi1_data
};


static const uint8_t img_Wifi2_data[] = {
  0x00, 0x00, 0x00, 0x00,   /*Color of index 0*/
  0x00, 0x00, 0x00, 0xff,

  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b11000000,
  0b00000000, 0b11000000,
  0b00000000, 0b11000000,
  0b00000000, 0b11000000,
  0b00001100, 0b11000000,
  0b00001100, 0b11000000,
  0b00001100, 0b11000000,
  0b00001100, 0b11000000,
  0b11001100, 0b11000000,
  0b11001100, 0b11000000,
  0b11001100, 0b11000000,
  0b11001100, 0b11000000
};

static const lv_img_dsc_t img_Wifi2 {
  .header = {.cf = LV_IMG_CF_INDEXED_1BIT, .always_zero = 0, .reserved = 0, .w = 16, .h = 16},
  .data_size = 16 * 16 / 8,
  .data = img_Wifi2_data
};


static const uint8_t img_Wifi3_data[] = {
  0x00, 0x00, 0x00, 0x00,   /*Color of index 0*/
  0x00, 0x00, 0x00, 0xff,
  

  0b00000000, 0b00001100,
  0b00000000, 0b00001100,
  0b00000000, 0b00001100,
  0b00000000, 0b00001100,
  0b00000000, 0b11001100,
  0b00000000, 0b11001100,
  0b00000000, 0b11001100,
  0b00000000, 0b11001100,
  0b00001100, 0b11001100,
  0b00001100, 0b11001100,
  0b00001100, 0b11001100,
  0b00001100, 0b11001100,
  0b11001100, 0b11001100,
  0b11001100, 0b11001100,
  0b11001100, 0b11001100,
  0b11001100, 0b11001100
};

static const lv_img_dsc_t img_Wifi3 {
  .header = {.cf = LV_IMG_CF_INDEXED_1BIT, .always_zero = 0, .reserved = 0, .w = 16, .h = 16},
  .data_size = 16 * 16 / 8,
  .data = img_Wifi3_data
};

void displayWifiInfo()
{
  char buf[20];
  snprintf(buf, sizeof(buf), "%s", WiFi.SSID().c_str());
  lv_label_set_text(wifiStrength_label, buf);
  int rssi = WiFi.RSSI();
  if (rssi == 0) {
    lv_img_set_src(wifiStrength_icon, &img_WifiNone);
  } else {
    int quality = (rssi + 100) * 2;
    if (quality >= 90) {
      lv_img_set_src(wifiStrength_icon, &img_Wifi3);
    } else if (quality >= 50) {
      lv_img_set_src(wifiStrength_icon, &img_Wifi2);
    } else if (quality >= 0) {
      lv_img_set_src(wifiStrength_icon, &img_Wifi1);
    } else {
      lv_img_set_src(wifiStrength_icon, &img_Wifi0);
    }
  }
}
