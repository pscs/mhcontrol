#pragma once

#include <string>
#include <IPAddress.h>

class TFT_eSPI;

const int screenWidth = 320;
const int screenHeight = 240;
const int fontHeight = 14;


extern std::string WifiSSID;
extern std::string WifiPassword;
extern bool DHCPMode;
extern IPAddress StaticIP;
extern IPAddress StaticMask;
extern IPAddress StaticGateway;
extern IPAddress StaticDNS[2];

extern uint32_t screenTimeout;
extern TFT_eSPI tft;

extern bool sdCardMounted;

const uint8_t BacklightChannel = 0;
const uint8_t BacklightPin = 15;
const uint8_t SoundChannel = 8;
const uint8_t SoundPin = 21;
