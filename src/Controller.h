#pragma once

#include <string>

class TFT_eSPI;

const int screenWidth = 320;
const int screenHeight = 240;
const int fontHeight = 14;


extern std::string WifiSSID;
extern std::string WifiPassword;

extern uint32_t screenTimeout;
extern TFT_eSPI tft;

extern bool sdCardMounted;
