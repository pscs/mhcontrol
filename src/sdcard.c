#if defined(LV_LVGL_H_INCLUDE_SIMPLE)
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif

//This icon is from Flaticon.com

#ifndef LV_ATTRIBUTE_MEM_ALIGN
#define LV_ATTRIBUTE_MEM_ALIGN
#endif

#ifndef LV_ATTRIBUTE_IMG_SDCARD
#define LV_ATTRIBUTE_IMG_SDCARD
#endif

const LV_ATTRIBUTE_MEM_ALIGN LV_ATTRIBUTE_LARGE_CONST LV_ATTRIBUTE_IMG_SDCARD uint8_t sdcard_map[] = {
#if LV_COLOR_DEPTH == 1 || LV_COLOR_DEPTH == 8
  /*Pixel format: Blue: 2 bit, Green: 3 bit, Red: 3 bit, Alpha 8 bit */
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4f, 0x27, 0x4f, 0x90, 0x4f, 0x93, 0x4f, 0x93, 0x4f, 0x93, 0x4f, 0x93, 0x4f, 0x93, 0x4f, 0x93, 0x4f, 0x93, 0x4f, 0x93, 0x4f, 0x8c, 0x2f, 0x07, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4f, 0x7f, 0x53, 0xff, 0x73, 0xff, 0x53, 0xff, 0x73, 0xff, 0x73, 0xff, 0x73, 0xff, 0x73, 0xff, 0x53, 0xff, 0x73, 0xff, 0x4f, 0xff, 0x4f, 0x4c, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4f, 0x80, 0x73, 0xff, 0xfe, 0xff, 0x93, 0xff, 0xbb, 0xff, 0xb7, 0xff, 0x97, 0xff, 0xfa, 0xff, 0x73, 0xff, 0xfa, 0xff, 0x4f, 0xff, 0x4f, 0x53, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4f, 0x80, 0x73, 0xff, 0xfe, 0xff, 0x93, 0xff, 0xbb, 0xff, 0xb7, 0xff, 0x97, 0xff, 0xfa, 0xff, 0x73, 0xff, 0xfa, 0xff, 0x4f, 0xff, 0x4f, 0x53, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4f, 0x80, 0x73, 0xff, 0xfa, 0xff, 0x73, 0xff, 0xb7, 0xff, 0xb7, 0xff, 0x97, 0xff, 0xdb, 0xff, 0x73, 0xff, 0xdb, 0xff, 0x4f, 0xff, 0x4f, 0x53, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4f, 0x80, 0x53, 0xff, 0x4f, 0xff, 0x4f, 0xff, 0x4f, 0xff, 0x4f, 0xff, 0x4f, 0xff, 0x4f, 0xff, 0x53, 0xff, 0x4f, 0xff, 0x53, 0xff, 0x4f, 0x53, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x4f, 0x17, 0x4f, 0xb4, 0x53, 0xff, 0x53, 0xff, 0x53, 0xff, 0x53, 0xff, 0x53, 0xff, 0x53, 0xff, 0x53, 0xff, 0x53, 0xff, 0x53, 0xff, 0x53, 0xff, 0x4f, 0x53, 0x00, 0x00, 
  0x00, 0x00, 0x4f, 0x28, 0x4f, 0xdc, 0x53, 0xff, 0x53, 0xff, 0x53, 0xff, 0x53, 0xff, 0x53, 0xff, 0x53, 0xff, 0x53, 0xff, 0x53, 0xff, 0x53, 0xff, 0x53, 0xff, 0x53, 0xff, 0x4f, 0x53, 0x00, 0x00, 
  0x00, 0x00, 0x4f, 0x58, 0x53, 0xff, 0x53, 0xff, 0x53, 0xff, 0x53, 0xff, 0x53, 0xff, 0x53, 0xff, 0x53, 0xff, 0x53, 0xff, 0x53, 0xff, 0x53, 0xff, 0x53, 0xff, 0x53, 0xff, 0x4f, 0x53, 0x00, 0x00, 
  0x00, 0x00, 0x4f, 0x3c, 0x4f, 0xe0, 0x53, 0xff, 0x73, 0xff, 0x97, 0xff, 0x97, 0xff, 0x97, 0xff, 0x97, 0xff, 0x97, 0xff, 0x97, 0xff, 0x97, 0xff, 0x97, 0xff, 0x53, 0xff, 0x4f, 0x53, 0x00, 0x00, 
  0x00, 0x00, 0x4f, 0x08, 0x4f, 0xab, 0x53, 0xff, 0x73, 0xff, 0xbb, 0xff, 0xbb, 0xff, 0xbb, 0xff, 0xbb, 0xff, 0xbb, 0xff, 0xbb, 0xff, 0xbb, 0xff, 0xbb, 0xff, 0x73, 0xff, 0x4f, 0x53, 0x00, 0x00, 
  0x00, 0x00, 0x4f, 0x58, 0x53, 0xff, 0x53, 0xff, 0x73, 0xff, 0xbb, 0xff, 0xbb, 0xff, 0xbb, 0xff, 0xbb, 0xff, 0xbb, 0xff, 0xbb, 0xff, 0xbb, 0xff, 0xbb, 0xff, 0x73, 0xff, 0x4f, 0x53, 0x00, 0x00, 
  0x00, 0x00, 0x4f, 0x58, 0x53, 0xff, 0x53, 0xff, 0x73, 0xff, 0xbb, 0xff, 0xbb, 0xff, 0xbb, 0xff, 0xbb, 0xff, 0xbb, 0xff, 0xbb, 0xff, 0xbb, 0xff, 0xbb, 0xff, 0x73, 0xff, 0x4f, 0x53, 0x00, 0x00, 
  0x00, 0x00, 0x4f, 0x58, 0x53, 0xff, 0x53, 0xff, 0x73, 0xff, 0xbb, 0xff, 0xbb, 0xff, 0xbb, 0xff, 0xbb, 0xff, 0xbb, 0xff, 0xbb, 0xff, 0xbb, 0xff, 0xbb, 0xff, 0x73, 0xff, 0x4f, 0x53, 0x00, 0x00, 
  0x00, 0x00, 0x4f, 0x53, 0x53, 0xff, 0x53, 0xff, 0x53, 0xff, 0x73, 0xff, 0x73, 0xff, 0x73, 0xff, 0x73, 0xff, 0x73, 0xff, 0x73, 0xff, 0x73, 0xff, 0x73, 0xff, 0x4f, 0xff, 0x4f, 0x4c, 0x00, 0x00, 
  0x00, 0x00, 0x4f, 0x07, 0x4f, 0x90, 0x4f, 0xa0, 0x4f, 0xa0, 0x4f, 0xa0, 0x4f, 0xa0, 0x4f, 0xa0, 0x4f, 0xa0, 0x4f, 0xa0, 0x4f, 0xa0, 0x4f, 0xa0, 0x4f, 0xa0, 0x4f, 0x8f, 0x4f, 0x04, 0x00, 0x00, 
#endif
#if LV_COLOR_DEPTH == 16 && LV_COLOR_16_SWAP == 0
  /*Pixel format: Blue: 5 bit, Green: 6 bit, Red: 5 bit, Alpha 8 bit*/
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x19, 0x43, 0x27, 0x79, 0x43, 0x90, 0x79, 0x43, 0x93, 0x79, 0x43, 0x93, 0x79, 0x43, 0x93, 0x79, 0x43, 0x93, 0x79, 0x43, 0x93, 0x79, 0x43, 0x93, 0x79, 0x43, 0x93, 0x79, 0x43, 0x93, 0x59, 0x43, 0x8c, 0xbb, 0x2a, 0x07, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79, 0x43, 0x7f, 0x99, 0x4b, 0xff, 0x18, 0x64, 0xff, 0xd8, 0x53, 0xff, 0xf8, 0x5b, 0xff, 0xf8, 0x5b, 0xff, 0xd8, 0x53, 0xff, 0x18, 0x64, 0xff, 0xb9, 0x53, 0xff, 0x18, 0x64, 0xff, 0x99, 0x43, 0xff, 0x5a, 0x3b, 0x4c, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79, 0x43, 0x80, 0xf8, 0x5b, 0xff, 0x13, 0xf7, 0xff, 0x97, 0x7c, 0xff, 0xb5, 0xb5, 0xff, 0x75, 0xa5, 0xff, 0xb7, 0x84, 0xff, 0x94, 0xde, 0xff, 0x57, 0x74, 0xff, 0x94, 0xde, 0xff, 0x99, 0x4b, 0xff, 0x39, 0x3b, 0x53, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79, 0x43, 0x80, 0xf8, 0x5b, 0xff, 0x13, 0xf7, 0xff, 0x97, 0x7c, 0xff, 0xb5, 0xb5, 0xff, 0x75, 0xa5, 0xff, 0xb7, 0x84, 0xff, 0x94, 0xde, 0xff, 0x57, 0x74, 0xff, 0x94, 0xde, 0xff, 0x99, 0x4b, 0xff, 0x39, 0x3b, 0x53, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79, 0x43, 0x80, 0xd8, 0x5b, 0xff, 0x74, 0xd6, 0xff, 0x57, 0x74, 0xff, 0x56, 0xa5, 0xff, 0x36, 0x9d, 0xff, 0x97, 0x7c, 0xff, 0xf5, 0xc5, 0xff, 0x38, 0x6c, 0xff, 0xf5, 0xbd, 0xff, 0x99, 0x4b, 0xff, 0x39, 0x3b, 0x53, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79, 0x43, 0x80, 0x99, 0x4b, 0xff, 0x99, 0x43, 0xff, 0x99, 0x4b, 0xff, 0x99, 0x4b, 0xff, 0x99, 0x4b, 0xff, 0x99, 0x4b, 0xff, 0x99, 0x43, 0xff, 0x99, 0x4b, 0xff, 0x99, 0x43, 0xff, 0x99, 0x4b, 0xff, 0x39, 0x3b, 0x53, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x39, 0x3b, 0x17, 0x79, 0x43, 0xb4, 0x99, 0x4b, 0xff, 0x99, 0x4b, 0xff, 0x99, 0x4b, 0xff, 0x99, 0x4b, 0xff, 0x99, 0x4b, 0xff, 0x99, 0x4b, 0xff, 0x99, 0x4b, 0xff, 0x99, 0x4b, 0xff, 0x99, 0x4b, 0xff, 0x99, 0x4b, 0xff, 0x39, 0x3b, 0x53, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x39, 0x3b, 0x28, 0x79, 0x43, 0xdc, 0x99, 0x4b, 0xff, 0x99, 0x4b, 0xff, 0x99, 0x4b, 0xff, 0x99, 0x4b, 0xff, 0x99, 0x4b, 0xff, 0x99, 0x4b, 0xff, 0x99, 0x4b, 0xff, 0x99, 0x4b, 0xff, 0x99, 0x4b, 0xff, 0x99, 0x4b, 0xff, 0x99, 0x4b, 0xff, 0x39, 0x3b, 0x53, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x39, 0x43, 0x58, 0x99, 0x4b, 0xff, 0x99, 0x4b, 0xff, 0x99, 0x4b, 0xff, 0x99, 0x4b, 0xff, 0x99, 0x4b, 0xff, 0x99, 0x4b, 0xff, 0x99, 0x4b, 0xff, 0x99, 0x4b, 0xff, 0x99, 0x4b, 0xff, 0x99, 0x4b, 0xff, 0x99, 0x4b, 0xff, 0x99, 0x4b, 0xff, 0x39, 0x3b, 0x53, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x3a, 0x3b, 0x3c, 0x79, 0x43, 0xe0, 0x99, 0x4b, 0xff, 0xfa, 0x53, 0xff, 0xfd, 0x74, 0xff, 0xfd, 0x7c, 0xff, 0xfd, 0x7c, 0xff, 0xfd, 0x7c, 0xff, 0xfd, 0x7c, 0xff, 0xfd, 0x7c, 0xff, 0xfd, 0x7c, 0xff, 0xfd, 0x74, 0xff, 0xb9, 0x4b, 0xff, 0x39, 0x3b, 0x53, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x1c, 0x43, 0x08, 0x59, 0x43, 0xab, 0x99, 0x4b, 0xff, 0x7b, 0x64, 0xff, 0x5f, 0xa6, 0xff, 0x5f, 0xa6, 0xff, 0x5f, 0xa6, 0xff, 0x5f, 0xa6, 0xff, 0x5f, 0xa6, 0xff, 0x5f, 0xa6, 0xff, 0x5f, 0xa6, 0xff, 0x5f, 0xa6, 0xff, 0xfa, 0x53, 0xff, 0x39, 0x3b, 0x53, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x39, 0x43, 0x58, 0x99, 0x4b, 0xff, 0x99, 0x4b, 0xff, 0x7b, 0x64, 0xff, 0x5f, 0xa6, 0xff, 0x5f, 0xa6, 0xff, 0x5f, 0xa6, 0xff, 0x5f, 0xa6, 0xff, 0x5f, 0xa6, 0xff, 0x5f, 0xa6, 0xff, 0x5f, 0xa6, 0xff, 0x5f, 0xa6, 0xff, 0xfa, 0x53, 0xff, 0x39, 0x3b, 0x53, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x39, 0x43, 0x58, 0x99, 0x4b, 0xff, 0x99, 0x4b, 0xff, 0x7b, 0x64, 0xff, 0x5f, 0xa6, 0xff, 0x5f, 0xa6, 0xff, 0x5f, 0xa6, 0xff, 0x5f, 0xa6, 0xff, 0x5f, 0xa6, 0xff, 0x5f, 0xa6, 0xff, 0x5f, 0xa6, 0xff, 0x5f, 0xa6, 0xff, 0xfa, 0x53, 0xff, 0x39, 0x3b, 0x53, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x39, 0x43, 0x58, 0x99, 0x4b, 0xff, 0x99, 0x4b, 0xff, 0x5b, 0x64, 0xff, 0x5f, 0xa6, 0xff, 0x5f, 0xa6, 0xff, 0x5f, 0xa6, 0xff, 0x5f, 0xa6, 0xff, 0x5f, 0xa6, 0xff, 0x5f, 0xa6, 0xff, 0x5f, 0xa6, 0xff, 0x5f, 0xa6, 0xff, 0xfa, 0x53, 0xff, 0x39, 0x3b, 0x53, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x5a, 0x3b, 0x53, 0x99, 0x4b, 0xff, 0x99, 0x4b, 0xff, 0x99, 0x4b, 0xff, 0xfa, 0x53, 0xff, 0xfa, 0x5b, 0xff, 0xfa, 0x5b, 0xff, 0xfa, 0x5b, 0xff, 0xfa, 0x5b, 0xff, 0xfa, 0x5b, 0xff, 0xfa, 0x5b, 0xff, 0xfa, 0x53, 0xff, 0x99, 0x4b, 0xff, 0x5a, 0x3b, 0x4c, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x7b, 0x4b, 0x07, 0x59, 0x43, 0x90, 0x79, 0x43, 0xa0, 0x79, 0x43, 0xa0, 0x79, 0x43, 0xa0, 0x79, 0x43, 0xa0, 0x79, 0x43, 0xa0, 0x79, 0x43, 0xa0, 0x79, 0x43, 0xa0, 0x79, 0x43, 0xa0, 0x79, 0x43, 0xa0, 0x79, 0x43, 0xa0, 0x79, 0x43, 0x8f, 0x39, 0x33, 0x04, 0x00, 0x00, 0x00, 
#endif
#if LV_COLOR_DEPTH == 16 && LV_COLOR_16_SWAP != 0
  /*Pixel format:  Blue: 5 bit Green: 6 bit, Red: 5 bit, Alpha 8 bit  BUT the 2  color bytes are swapped*/
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x43, 0x19, 0x27, 0x43, 0x79, 0x90, 0x43, 0x79, 0x93, 0x43, 0x79, 0x93, 0x43, 0x79, 0x93, 0x43, 0x79, 0x93, 0x43, 0x79, 0x93, 0x43, 0x79, 0x93, 0x43, 0x79, 0x93, 0x43, 0x79, 0x93, 0x43, 0x59, 0x8c, 0x2a, 0xbb, 0x07, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x43, 0x79, 0x7f, 0x4b, 0x99, 0xff, 0x64, 0x18, 0xff, 0x53, 0xd8, 0xff, 0x5b, 0xf8, 0xff, 0x5b, 0xf8, 0xff, 0x53, 0xd8, 0xff, 0x64, 0x18, 0xff, 0x53, 0xb9, 0xff, 0x64, 0x18, 0xff, 0x43, 0x99, 0xff, 0x3b, 0x5a, 0x4c, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x43, 0x79, 0x80, 0x5b, 0xf8, 0xff, 0xf7, 0x13, 0xff, 0x7c, 0x97, 0xff, 0xb5, 0xb5, 0xff, 0xa5, 0x75, 0xff, 0x84, 0xb7, 0xff, 0xde, 0x94, 0xff, 0x74, 0x57, 0xff, 0xde, 0x94, 0xff, 0x4b, 0x99, 0xff, 0x3b, 0x39, 0x53, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x43, 0x79, 0x80, 0x5b, 0xf8, 0xff, 0xf7, 0x13, 0xff, 0x7c, 0x97, 0xff, 0xb5, 0xb5, 0xff, 0xa5, 0x75, 0xff, 0x84, 0xb7, 0xff, 0xde, 0x94, 0xff, 0x74, 0x57, 0xff, 0xde, 0x94, 0xff, 0x4b, 0x99, 0xff, 0x3b, 0x39, 0x53, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x43, 0x79, 0x80, 0x5b, 0xd8, 0xff, 0xd6, 0x74, 0xff, 0x74, 0x57, 0xff, 0xa5, 0x56, 0xff, 0x9d, 0x36, 0xff, 0x7c, 0x97, 0xff, 0xc5, 0xf5, 0xff, 0x6c, 0x38, 0xff, 0xbd, 0xf5, 0xff, 0x4b, 0x99, 0xff, 0x3b, 0x39, 0x53, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x43, 0x79, 0x80, 0x4b, 0x99, 0xff, 0x43, 0x99, 0xff, 0x4b, 0x99, 0xff, 0x4b, 0x99, 0xff, 0x4b, 0x99, 0xff, 0x4b, 0x99, 0xff, 0x43, 0x99, 0xff, 0x4b, 0x99, 0xff, 0x43, 0x99, 0xff, 0x4b, 0x99, 0xff, 0x3b, 0x39, 0x53, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3b, 0x39, 0x17, 0x43, 0x79, 0xb4, 0x4b, 0x99, 0xff, 0x4b, 0x99, 0xff, 0x4b, 0x99, 0xff, 0x4b, 0x99, 0xff, 0x4b, 0x99, 0xff, 0x4b, 0x99, 0xff, 0x4b, 0x99, 0xff, 0x4b, 0x99, 0xff, 0x4b, 0x99, 0xff, 0x4b, 0x99, 0xff, 0x3b, 0x39, 0x53, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x3b, 0x39, 0x28, 0x43, 0x79, 0xdc, 0x4b, 0x99, 0xff, 0x4b, 0x99, 0xff, 0x4b, 0x99, 0xff, 0x4b, 0x99, 0xff, 0x4b, 0x99, 0xff, 0x4b, 0x99, 0xff, 0x4b, 0x99, 0xff, 0x4b, 0x99, 0xff, 0x4b, 0x99, 0xff, 0x4b, 0x99, 0xff, 0x4b, 0x99, 0xff, 0x3b, 0x39, 0x53, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x43, 0x39, 0x58, 0x4b, 0x99, 0xff, 0x4b, 0x99, 0xff, 0x4b, 0x99, 0xff, 0x4b, 0x99, 0xff, 0x4b, 0x99, 0xff, 0x4b, 0x99, 0xff, 0x4b, 0x99, 0xff, 0x4b, 0x99, 0xff, 0x4b, 0x99, 0xff, 0x4b, 0x99, 0xff, 0x4b, 0x99, 0xff, 0x4b, 0x99, 0xff, 0x3b, 0x39, 0x53, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x3b, 0x3a, 0x3c, 0x43, 0x79, 0xe0, 0x4b, 0x99, 0xff, 0x53, 0xfa, 0xff, 0x74, 0xfd, 0xff, 0x7c, 0xfd, 0xff, 0x7c, 0xfd, 0xff, 0x7c, 0xfd, 0xff, 0x7c, 0xfd, 0xff, 0x7c, 0xfd, 0xff, 0x7c, 0xfd, 0xff, 0x74, 0xfd, 0xff, 0x4b, 0xb9, 0xff, 0x3b, 0x39, 0x53, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x43, 0x1c, 0x08, 0x43, 0x59, 0xab, 0x4b, 0x99, 0xff, 0x64, 0x7b, 0xff, 0xa6, 0x5f, 0xff, 0xa6, 0x5f, 0xff, 0xa6, 0x5f, 0xff, 0xa6, 0x5f, 0xff, 0xa6, 0x5f, 0xff, 0xa6, 0x5f, 0xff, 0xa6, 0x5f, 0xff, 0xa6, 0x5f, 0xff, 0x53, 0xfa, 0xff, 0x3b, 0x39, 0x53, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x43, 0x39, 0x58, 0x4b, 0x99, 0xff, 0x4b, 0x99, 0xff, 0x64, 0x7b, 0xff, 0xa6, 0x5f, 0xff, 0xa6, 0x5f, 0xff, 0xa6, 0x5f, 0xff, 0xa6, 0x5f, 0xff, 0xa6, 0x5f, 0xff, 0xa6, 0x5f, 0xff, 0xa6, 0x5f, 0xff, 0xa6, 0x5f, 0xff, 0x53, 0xfa, 0xff, 0x3b, 0x39, 0x53, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x43, 0x39, 0x58, 0x4b, 0x99, 0xff, 0x4b, 0x99, 0xff, 0x64, 0x7b, 0xff, 0xa6, 0x5f, 0xff, 0xa6, 0x5f, 0xff, 0xa6, 0x5f, 0xff, 0xa6, 0x5f, 0xff, 0xa6, 0x5f, 0xff, 0xa6, 0x5f, 0xff, 0xa6, 0x5f, 0xff, 0xa6, 0x5f, 0xff, 0x53, 0xfa, 0xff, 0x3b, 0x39, 0x53, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x43, 0x39, 0x58, 0x4b, 0x99, 0xff, 0x4b, 0x99, 0xff, 0x64, 0x5b, 0xff, 0xa6, 0x5f, 0xff, 0xa6, 0x5f, 0xff, 0xa6, 0x5f, 0xff, 0xa6, 0x5f, 0xff, 0xa6, 0x5f, 0xff, 0xa6, 0x5f, 0xff, 0xa6, 0x5f, 0xff, 0xa6, 0x5f, 0xff, 0x53, 0xfa, 0xff, 0x3b, 0x39, 0x53, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x3b, 0x5a, 0x53, 0x4b, 0x99, 0xff, 0x4b, 0x99, 0xff, 0x4b, 0x99, 0xff, 0x53, 0xfa, 0xff, 0x5b, 0xfa, 0xff, 0x5b, 0xfa, 0xff, 0x5b, 0xfa, 0xff, 0x5b, 0xfa, 0xff, 0x5b, 0xfa, 0xff, 0x5b, 0xfa, 0xff, 0x53, 0xfa, 0xff, 0x4b, 0x99, 0xff, 0x3b, 0x5a, 0x4c, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x4b, 0x7b, 0x07, 0x43, 0x59, 0x90, 0x43, 0x79, 0xa0, 0x43, 0x79, 0xa0, 0x43, 0x79, 0xa0, 0x43, 0x79, 0xa0, 0x43, 0x79, 0xa0, 0x43, 0x79, 0xa0, 0x43, 0x79, 0xa0, 0x43, 0x79, 0xa0, 0x43, 0x79, 0xa0, 0x43, 0x79, 0xa0, 0x43, 0x79, 0x8f, 0x33, 0x39, 0x04, 0x00, 0x00, 0x00, 
#endif
#if LV_COLOR_DEPTH == 32
  /*Pixel format:  Blue: 8 bit, Green: 8 bit, Red: 8 bit, Alpha: 8 bit*/
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xcb, 0x62, 0x41, 0x27, 0xca, 0x6c, 0x40, 0x90, 0xc9, 0x6c, 0x41, 0x93, 0xc9, 0x6c, 0x41, 0x93, 0xc9, 0x6c, 0x41, 0x93, 0xc9, 0x6c, 0x41, 0x93, 0xc9, 0x6c, 0x41, 0x93, 0xc9, 0x6c, 0x41, 0x93, 0xc9, 0x6c, 0x41, 0x93, 0xc9, 0x6c, 0x41, 0x93, 0xc8, 0x6a, 0x3e, 0x8c, 0xd5, 0x55, 0x2b, 0x07, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc8, 0x6b, 0x3f, 0x7f, 0xc5, 0x72, 0x48, 0xff, 0xbf, 0x82, 0x62, 0xff, 0xc4, 0x77, 0x50, 0xff, 0xc1, 0x7d, 0x5b, 0xff, 0xc1, 0x7d, 0x59, 0xff, 0xc3, 0x78, 0x53, 0xff, 0xc0, 0x81, 0x60, 0xff, 0xc5, 0x73, 0x4d, 0xff, 0xc0, 0x80, 0x60, 0xff, 0xc6, 0x70, 0x44, 0xff, 0xcd, 0x67, 0x38, 0x4c, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xca, 0x6b, 0x3f, 0x80, 0xc1, 0x7d, 0x5a, 0xff, 0x97, 0xdf, 0xef, 0xff, 0xb9, 0x8f, 0x75, 0xff, 0xa9, 0xb4, 0xae, 0xff, 0xac, 0xad, 0xa2, 0xff, 0xb6, 0x96, 0x82, 0xff, 0x9e, 0xcf, 0xd8, 0xff, 0xbc, 0x89, 0x6d, 0xff, 0x9e, 0xcf, 0xd6, 0xff, 0xc7, 0x70, 0x45, 0xff, 0xcb, 0x65, 0x3a, 0x53, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xca, 0x6b, 0x3f, 0x80, 0xc1, 0x7d, 0x5a, 0xff, 0x97, 0xdf, 0xef, 0xff, 0xb9, 0x8f, 0x75, 0xff, 0xa9, 0xb4, 0xae, 0xff, 0xac, 0xad, 0xa2, 0xff, 0xb6, 0x96, 0x82, 0xff, 0x9e, 0xcf, 0xd8, 0xff, 0xbc, 0x89, 0x6d, 0xff, 0x9e, 0xcf, 0xd6, 0xff, 0xc7, 0x70, 0x45, 0xff, 0xcb, 0x65, 0x3a, 0x53, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xca, 0x6b, 0x3f, 0x80, 0xc2, 0x7a, 0x56, 0xff, 0xa0, 0xcb, 0xd2, 0xff, 0xba, 0x8a, 0x6f, 0xff, 0xae, 0xa8, 0x9d, 0xff, 0xb1, 0xa3, 0x95, 0xff, 0xb7, 0x92, 0x7a, 0xff, 0xa6, 0xbd, 0xbe, 0xff, 0xbd, 0x83, 0x66, 0xff, 0xa5, 0xbd, 0xbc, 0xff, 0xc6, 0x70, 0x45, 0xff, 0xcb, 0x65, 0x3a, 0x53, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xca, 0x6b, 0x3f, 0x80, 0xc6, 0x71, 0x45, 0xff, 0xc7, 0x6f, 0x44, 0xff, 0xc6, 0x70, 0x45, 0xff, 0xc6, 0x70, 0x45, 0xff, 0xc6, 0x70, 0x45, 0xff, 0xc6, 0x70, 0x45, 0xff, 0xc6, 0x6f, 0x44, 0xff, 0xc6, 0x71, 0x46, 0xff, 0xc6, 0x6f, 0x44, 0xff, 0xc6, 0x71, 0x46, 0xff, 0xcb, 0x65, 0x3a, 0x53, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc8, 0x64, 0x37, 0x17, 0xc9, 0x6c, 0x41, 0xb4, 0xc6, 0x71, 0x46, 0xff, 0xc6, 0x71, 0x46, 0xff, 0xc6, 0x71, 0x46, 0xff, 0xc6, 0x71, 0x46, 0xff, 0xc6, 0x71, 0x46, 0xff, 0xc6, 0x71, 0x46, 0xff, 0xc6, 0x71, 0x46, 0xff, 0xc6, 0x71, 0x46, 0xff, 0xc6, 0x71, 0x46, 0xff, 0xc6, 0x71, 0x46, 0xff, 0xcb, 0x65, 0x3a, 0x53, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0xcc, 0x66, 0x39, 0x28, 0xc7, 0x6d, 0x42, 0xdc, 0xc6, 0x71, 0x46, 0xff, 0xc6, 0x71, 0x46, 0xff, 0xc6, 0x71, 0x46, 0xff, 0xc6, 0x71, 0x46, 0xff, 0xc6, 0x71, 0x46, 0xff, 0xc6, 0x71, 0x46, 0xff, 0xc6, 0x71, 0x46, 0xff, 0xc6, 0x71, 0x46, 0xff, 0xc6, 0x71, 0x46, 0xff, 0xc6, 0x71, 0x46, 0xff, 0xc6, 0x71, 0x46, 0xff, 0xcb, 0x65, 0x3a, 0x53, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0xcb, 0x65, 0x3d, 0x58, 0xc6, 0x71, 0x46, 0xff, 0xc6, 0x71, 0x46, 0xff, 0xc6, 0x71, 0x46, 0xff, 0xc6, 0x71, 0x46, 0xff, 0xc6, 0x71, 0x46, 0xff, 0xc6, 0x71, 0x46, 0xff, 0xc6, 0x71, 0x46, 0xff, 0xc6, 0x71, 0x46, 0xff, 0xc6, 0x71, 0x46, 0xff, 0xc6, 0x71, 0x46, 0xff, 0xc6, 0x71, 0x46, 0xff, 0xc6, 0x71, 0x46, 0xff, 0xcb, 0x65, 0x3a, 0x53, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0xcd, 0x64, 0x3b, 0x3c, 0xc6, 0x6d, 0x43, 0xe0, 0xc6, 0x71, 0x46, 0xff, 0xd2, 0x7d, 0x53, 0xff, 0xe6, 0x9c, 0x74, 0xff, 0xe6, 0x9c, 0x75, 0xff, 0xe6, 0x9c, 0x75, 0xff, 0xe6, 0x9c, 0x75, 0xff, 0xe6, 0x9c, 0x75, 0xff, 0xe6, 0x9c, 0x75, 0xff, 0xe6, 0x9c, 0x75, 0xff, 0xe5, 0x9b, 0x72, 0xff, 0xcb, 0x76, 0x4b, 0xff, 0xcb, 0x65, 0x3a, 0x53, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0xdf, 0x60, 0x40, 0x08, 0xc9, 0x6a, 0x3f, 0xab, 0xc6, 0x71, 0x46, 0xff, 0xdb, 0x8c, 0x62, 0xff, 0xff, 0xc9, 0xa4, 0xff, 0xff, 0xc9, 0xa4, 0xff, 0xff, 0xc9, 0xa4, 0xff, 0xff, 0xc9, 0xa4, 0xff, 0xff, 0xc9, 0xa4, 0xff, 0xff, 0xc9, 0xa4, 0xff, 0xff, 0xc9, 0xa4, 0xff, 0xff, 0xc9, 0xa4, 0xff, 0xd2, 0x7d, 0x54, 0xff, 0xcb, 0x65, 0x3a, 0x53, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0xcb, 0x65, 0x3d, 0x58, 0xc6, 0x71, 0x46, 0xff, 0xc6, 0x71, 0x46, 0xff, 0xdb, 0x8c, 0x62, 0xff, 0xff, 0xc9, 0xa4, 0xff, 0xff, 0xc9, 0xa4, 0xff, 0xff, 0xc9, 0xa4, 0xff, 0xff, 0xc9, 0xa4, 0xff, 0xff, 0xc9, 0xa4, 0xff, 0xff, 0xc9, 0xa4, 0xff, 0xff, 0xc9, 0xa4, 0xff, 0xff, 0xc9, 0xa4, 0xff, 0xd2, 0x7d, 0x54, 0xff, 0xcb, 0x65, 0x3a, 0x53, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0xcb, 0x65, 0x3d, 0x58, 0xc6, 0x71, 0x46, 0xff, 0xc6, 0x71, 0x46, 0xff, 0xdb, 0x8c, 0x62, 0xff, 0xff, 0xc9, 0xa4, 0xff, 0xff, 0xc9, 0xa4, 0xff, 0xff, 0xc9, 0xa4, 0xff, 0xff, 0xc9, 0xa4, 0xff, 0xff, 0xc9, 0xa4, 0xff, 0xff, 0xc9, 0xa4, 0xff, 0xff, 0xc9, 0xa4, 0xff, 0xff, 0xc9, 0xa4, 0xff, 0xd2, 0x7d, 0x54, 0xff, 0xcb, 0x65, 0x3a, 0x53, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0xcb, 0x65, 0x3d, 0x58, 0xc6, 0x71, 0x46, 0xff, 0xc6, 0x71, 0x46, 0xff, 0xd9, 0x89, 0x60, 0xff, 0xff, 0xc9, 0xa4, 0xff, 0xff, 0xc9, 0xa4, 0xff, 0xff, 0xc9, 0xa4, 0xff, 0xff, 0xc9, 0xa4, 0xff, 0xff, 0xc9, 0xa4, 0xff, 0xff, 0xc9, 0xa4, 0xff, 0xff, 0xc9, 0xa4, 0xff, 0xff, 0xc9, 0xa4, 0xff, 0xd0, 0x7b, 0x52, 0xff, 0xcb, 0x65, 0x3a, 0x53, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0xcd, 0x67, 0x3b, 0x53, 0xc6, 0x71, 0x46, 0xff, 0xc6, 0x71, 0x46, 0xff, 0xc9, 0x72, 0x48, 0xff, 0xd3, 0x7e, 0x53, 0xff, 0xd2, 0x7e, 0x55, 0xff, 0xd2, 0x7e, 0x55, 0xff, 0xd2, 0x7e, 0x55, 0xff, 0xd2, 0x7e, 0x55, 0xff, 0xd2, 0x7e, 0x55, 0xff, 0xd2, 0x7e, 0x55, 0xff, 0xd1, 0x7c, 0x52, 0xff, 0xc6, 0x70, 0x46, 0xff, 0xcd, 0x67, 0x38, 0x4c, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0xdb, 0x6d, 0x49, 0x07, 0xc8, 0x6a, 0x40, 0x90, 0xc9, 0x6c, 0x41, 0xa0, 0xc9, 0x6c, 0x41, 0xa0, 0xc9, 0x6c, 0x41, 0xa0, 0xc9, 0x6c, 0x41, 0xa0, 0xc9, 0x6c, 0x41, 0xa0, 0xc9, 0x6c, 0x41, 0xa0, 0xc9, 0x6c, 0x41, 0xa0, 0xc9, 0x6c, 0x41, 0xa0, 0xc9, 0x6c, 0x41, 0xa0, 0xc9, 0x6c, 0x41, 0xa0, 0xca, 0x6b, 0x40, 0x8f, 0xcc, 0x66, 0x33, 0x04, 0x00, 0x00, 0x00, 0x00, 
#endif
};

const lv_img_dsc_t imgSdCard = {
  .header.always_zero = 0,
  .header.w = 16,
  .header.h = 16,
  .data_size = 256 * LV_IMG_PX_SIZE_ALPHA_BYTE,
  .header.cf = LV_IMG_CF_TRUE_COLOR_ALPHA,
  .data = sdcard_map,
};

