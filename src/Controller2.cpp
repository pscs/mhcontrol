#include <Arduino.h>
//#include <SPI.h>
#include <lvgl.h>
#include <TFT_eSPI.h>      // Hardware-specific library
#include <WiFi.h>
#include <WiFiGeneric.h>
#include <ArduinoOTA.h>
#include <TinyGPS++.h>
#include <vector>
#include <Wire.h>
#include "DS3231.h"
#include <SD.h>

//#define GPS_USESOFTWARESERIAL
#ifdef GPS_USESOFTWARESERIAL
#include <SoftwareSerial.h>
#endif

#include "screens.h"
#include "timezone.h"
#include "Controller.h"
#include "location.h"
#include "locator.h"
#include "weather.h"
#include "ble.h"
#include "thingsboard.h"
#include "config.h"
#include "serial_terminal.h"
#include "logger.h"
#include "inifile.h"
#include "sound.h"
#include "telnet_terminal.h"
#include "accelerometer.h"

//#define _TASK_SLEEP_ON_IDLE_RUN
#define _TASK_STATUS_REQUEST
//#include "esp32_sleep.h"
#include <TaskScheduler.h>

#define LVGL_TICK_PERIOD 60

static const int RXPin = 39, TXPin = 33;
static const uint32_t GPSBaud = 9600;

// The TinyGPS++ object

const char *gpsDemoStream =
  "$GPRMC,045103.000,A,3014.1984,N,09749.2872,W,0.67,161.46,030913,,,A*7C\r\n"
  "$GPGGA,045104.000,3014.1985,N,09749.2873,W,1,09,1.2,211.6,M,-22.5,M,,0000*62\r\n"
  "$GPRMC,045200.000,A,3014.3820,N,09748.9514,W,36.88,65.02,030913,,,A*77\r\n"
  "$GPGGA,045201.000,3014.3864,N,09748.9411,W,1,10,1.2,200.8,M,-22.5,M,,0000*6C\r\n"
  "$GPRMC,045251.000,A,3014.4275,N,09749.0626,W,0.51,217.94,030913,,,A*7D\r\n"
  "$GPGGA,045252.000,3014.4273,N,09749.0628,W,1,09,1.3,206.9,M,-22.5,M,,0000*6F\r\n";

bool gpsDemoMode = false;

#ifdef GPS_USESOFTWARESERIAL
// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);
#endif

//Ticker tick; /* timer for interrupt handler */
TFT_eSPI tft = TFT_eSPI(); /* TFT instance */

static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[screenWidth * 10];

uint32_t wifiLastTryConnect;
uint32_t wifiNextTryConnect;
std::string WifiSSID;
std::string WifiPassword;
bool DHCPMode = true;
IPAddress StaticIP;
IPAddress StaticMask;
IPAddress StaticGateway;
IPAddress StaticDNS[2];

uint32_t screenTimeout = 30;

DS3231 Clock;
bool sdCardMounted = false;

Scheduler ts;

void displayWifiInfo();
Task tUpdateWifiStatus(500, -1, &displayWifiInfo, &ts, true);
void fnDimDisplay();
Task tDimDisplay(30, -1, &fnDimDisplay, &ts);
void fnWakeDisplay();
Task tWakeDisplay(30, -1, &fnWakeDisplay, &ts);
void fnFeedGPS();
Task tFeedGPS(100, -1, &fnFeedGPS, &ts, true);
void fnUpdateDisplay();
Task tUpdateDisplay(1000, -1, &fnUpdateDisplay, &ts, true);
void fnFastUpdateDisplay();
Task tFastUpdateDisplay(75, -1, &fnFastUpdateDisplay, &ts, true);
void fnUpdateTimezone();
Task tUpdateTimezone(300 * 1000, -1, &fnUpdateTimezone, &ts, false);
void fnUpdateLocator();
Task tUpdateLocator(300 * 1000, -1, &fnUpdateLocator, &ts, false);
void fnUpdateWeather();
Task tUpdateWeather(600 * 1000, -1, &fnUpdateWeather, &ts, false);
Task tBLEConnect(20 * 1000, -1, &BLEConnect, &ts, true);
Task tBLEKeepAlive(2 * 1000, -1, &BLEKeepAlive, &ts, true);
//Task tBLEKeepAlive(1 * 1000, -1, &BLEKeepAlive, &ts, true);
Task tUpdateThingsboard(10 * 1000, -1, &updateThingsBoard, &ts, true);
void fnFlushLogfiles();
Task tFlushLogFiles(600 * 1000, -1, &fnFlushLogfiles, &ts, true);
void fnCloseServerPinScreen();
Task tCloseServerPinScreen(0, -1, &fnCloseServerPinScreen, &ts, false);

//------------------------------------------------------------------------------------------

int16_t brightness = 255;
int16_t highBrightness = 200;
int16_t lowBrightness = 0;

#if LV_USE_LOG != 0
/* Serial debugging */
void my_print( lv_log_level_t level, const char * file, uint32_t line, const char * fn_name, const char * dsc )
{
   Serial.printf( "%s(%s)@%d->%s\r\n", file, fn_name, line, dsc );
   Serial.flush();
}
#endif

void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
   uint32_t w = ( area->x2 - area->x1 + 1 );
   uint32_t h = ( area->y2 - area->y1 + 1 );

   tft.startWrite();
   tft.setAddrWindow( area->x1, area->y1, w, h );
   tft.pushColors( ( uint16_t * )&color_p->full, w * h, true );
   tft.endWrite();

   lv_disp_flush_ready( disp );
}

void my_touchpad_read(lv_indev_drv_t * indev_driver, lv_indev_data_t * data)
{
    uint16_t touchX, touchY;

    bool touched = tft.getTouch(&touchX, &touchY, 600);

    if(!touched)
    {
      return ;//false;
    }

    if(touchX>screenWidth || touchY > screenHeight)
    {
      Serial.println("Y or y outside of expected parameters..");
      Serial.print("y:");
      Serial.print(touchX);
      Serial.print(" x:");
      Serial.print(touchY);
    }
    else
    {

      data->state = touched ? LV_INDEV_STATE_PR : LV_INDEV_STATE_REL; 
  
      /*Save the state and save the pressed coordinate*/
      //if(data->state == LV_INDEV_STATE_PR) touchpad_get_xy(&last_x, &last_y);
     
      /*Set the coordinates (if released use the last pressed coordinates)*/
      data->point.x = touchX;
      data->point.y = touchY;

	  logger.printf(LOG_GENERAL, LOG_DEBUG, "Touch - %d, %d\n", touchX, touchY);
  
      if (screenTimeout >= 30) {
        tDimDisplay.enableDelayed(screenTimeout * 1000);
      }
      if (brightness < highBrightness) {
        tWakeDisplay.enable();
      }
    }

    return ;//false; /*Return `false` because we are not buffering and no more data to read*/
}

void WiFiEvent(WiFiEvent_t event)
{
    logger.printf(LOG_WIFI, LOG_VERBOSE, "[WiFi-event] event: %d\n", event);

    switch (event) {
        case ARDUINO_EVENT_WIFI_READY: 
            Serial.println("WiFi interface ready");
            break;
        case ARDUINO_EVENT_WIFI_SCAN_DONE:
            Serial.println("Completed scan for access points");
            break;
        case ARDUINO_EVENT_WIFI_STA_START:
            Serial.println("WiFi client started");
            break;
        case ARDUINO_EVENT_WIFI_STA_STOP:
            Serial.println("WiFi clients stopped");
            break;
        case ARDUINO_EVENT_WIFI_STA_CONNECTED:
            logger.send(LOG_WIFI, LOG_VERBOSE, "Connected to access point\n");
            tUpdateLocator.enableDelayed(1000);
            tUpdateTimezone.enableDelayed(2000);
            tUpdateWeather.enableDelayed(3000);
            break;
        case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
            logger.send(LOG_WIFI, LOG_VERBOSE, "Disconnected from WiFi access point\n");
            tUpdateLocator.disable();
            tUpdateTimezone.disable();
            tUpdateWeather.disable();
            break;
        case ARDUINO_EVENT_WIFI_STA_AUTHMODE_CHANGE:
            Serial.println("Authentication mode of access point has changed");
            break;
        case ARDUINO_EVENT_WIFI_STA_GOT_IP:
            Serial.print("Obtained IP address: ");
            Serial.println(WiFi.localIP());
            break;
        case ARDUINO_EVENT_WIFI_STA_LOST_IP:
            Serial.println("Lost IP address and IP address is reset to 0");
            break;
        case ARDUINO_EVENT_WPS_ER_SUCCESS:
            Serial.println("WiFi Protected Setup (WPS): succeeded in enrollee mode");
            break;
        case ARDUINO_EVENT_WPS_ER_FAILED:
            Serial.println("WiFi Protected Setup (WPS): failed in enrollee mode");
            break;
        case ARDUINO_EVENT_WPS_ER_TIMEOUT:
            Serial.println("WiFi Protected Setup (WPS): timeout in enrollee mode");
            break;
        case ARDUINO_EVENT_WPS_ER_PIN:
            Serial.println("WiFi Protected Setup (WPS): pin code in enrollee mode");
            break;
        case ARDUINO_EVENT_WIFI_AP_START:
            Serial.println("WiFi access point started");
            break;
        case ARDUINO_EVENT_WIFI_AP_STOP:
            Serial.println("WiFi access point  stopped");
            break;
        case ARDUINO_EVENT_WIFI_AP_STACONNECTED:
            Serial.println("Client connected");
            break;
        case ARDUINO_EVENT_WIFI_AP_STADISCONNECTED:
            Serial.println("Client disconnected");
            break;
        case ARDUINO_EVENT_WIFI_AP_STAIPASSIGNED:
            Serial.println("Assigned IP address to client");
            break;
        case ARDUINO_EVENT_WIFI_AP_PROBEREQRECVED:
            Serial.println("Received probe request");
            break;
        case ARDUINO_EVENT_WIFI_AP_GOT_IP6:
            Serial.println("AP IPv6 is preferred");
            break;
        case ARDUINO_EVENT_WIFI_STA_GOT_IP6:
            Serial.println("STA IPv6 is preferred");
            break;
        case ARDUINO_EVENT_ETH_GOT_IP6:
            Serial.println("Ethernet IPv6 is preferred");
            break;
        case ARDUINO_EVENT_ETH_START:
            Serial.println("Ethernet started");
            break;
        case ARDUINO_EVENT_ETH_STOP:
            Serial.println("Ethernet stopped");
            break;
        case ARDUINO_EVENT_ETH_CONNECTED:
            Serial.println("Ethernet connected");
            break;
        case ARDUINO_EVENT_ETH_DISCONNECTED:
            Serial.println("Ethernet disconnected");
            break;
        case ARDUINO_EVENT_ETH_GOT_IP:
            Serial.println("Obtained IP address");
            break;
        default: break;
    }}

void setup() {
  // Use serial port
  Serial.begin(115200);
  
  //pinMode(15, OUTPUT);
  //digitalWrite(15, LOW);

  brightness = highBrightness;
  brightness = 128;
  ledcSetup(BacklightChannel, 5000, 8);
  ledcAttachPin(BacklightPin, BacklightChannel);
  ledcWrite(BacklightChannel, 255 - brightness);

  
  printf("Flash chip size: %d\n", ESP.getFlashChipSize());
  printf("Total heap: %d\n", ESP.getHeapSize());
  printf("Free heap: %d\n", ESP.getFreeHeap());
  printf("Total PSRAM: %d\n", ESP.getPsramSize());
  printf("Free PSRAM: %d\n", ESP.getFreePsram());

#ifdef GPS_USESOFTWARESERIAL
  ss.begin(GPSBaud);
#else
  Serial1.begin(GPSBaud, SERIAL_8N1, RXPin, TXPin);
#endif

  if (gpsDemoMode) {
    while (*gpsDemoStream)
     gps.encode(*gpsDemoStream++);
  }

  serialTerminal.initialise();
  bleManager.initialise();

  Wire.begin(26, 25, 100000);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  WiFi.onEvent(WiFiEvent);


  // Initialise the TFT screen

  lv_init();

  #if LV_USE_LOG != 0
   lv_log_register_print_cb( my_print ); /* register print function for debugging */
#endif

  tft.begin();
  tft.setRotation(3);

  ScreenClass::setCalibration();
  accelerometer.initialise(0x53);


  lv_disp_draw_buf_init(&draw_buf, buf, NULL, screenWidth * 10);

    /*Initialize the display*/
  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);
  disp_drv.hor_res = screenWidth;
  disp_drv.ver_res = screenHeight;
  disp_drv.flush_cb = my_disp_flush;
  disp_drv.draw_buf = &draw_buf;
  static lv_disp_t *disp = lv_disp_drv_register(&disp_drv);

  /*Initialize the input device driver*/
  static lv_indev_drv_t indev_drv;
  lv_indev_drv_init(&indev_drv);             /*Descriptor of a input device driver*/
  indev_drv.type = LV_INDEV_TYPE_POINTER;    /*Touch pad is a pointer-like device*/
  indev_drv.read_cb = my_touchpad_read;      /*Set your driver function*/
  lv_indev_drv_register(&indev_drv);         /*Finally register the driver*/

  //Set the theme..
//  lv_theme_t * th = lv_theme_material_init(LV_THEME_DEFAULT_COLOR_PRIMARY, LV_THEME_DEFAULT_COLOR_SECONDARY, LV_THEME_DEFAULT_FLAG, LV_THEME_DEFAULT_FONT_SMALL , LV_THEME_DEFAULT_FONT_NORMAL, LV_THEME_DEFAULT_FONT_SUBTITLE, LV_THEME_DEFAULT_FONT_TITLE);     
//  lv_theme_t *th = lv_theme_night_init(210, NULL);
//  lv_theme_set_act(th);

  lv_obj_t * scr = lv_obj_create(NULL);
  lv_scr_load(scr);
  
  lv_theme_t * th = lv_theme_default_init(disp,  /*Use the DPI, size, etc from this display*/ 
                                        lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_CYAN),   /*Primary and secondary palette*/
                                        false,    /*Light or dark mode*/ 
                                        &lv_font_montserrat_14); /*Small, normal, large fonts*/
                                        
  lv_disp_set_theme(disp, th); /*Assign the theme to the display*/

  Screen.Create(scr);

  WifiSSID = WIFI_SSID;
  WifiPassword = WIFI_PASSWORD;

	{
		uint32_t start = millis();
		do {
			if(SD.begin(32, SPI, 100000)) {
				sdCardMounted = true;

				IniFile::load(SETTINGS_FILENAME);
			}
			else {
				delay(10);
			}
		}
		while ((!sdCardMounted) && (millis() - start < 2000));
	}
	
	if (!sdCardMounted) {
		logger.send(LOG_GENERAL, LOG_ERROR, "Failed to mount SD card\n");
	}
	Screen.updateSdCardIcon(sdCardMounted);

	  //sound configuration
	ledcSetup(SoundChannel, 10000, 12);
  	ledcAttachPin(SoundPin, SoundChannel);

	sound.initialise(SoundChannel, 20);

	sound.addNote(4000, 100);
	sound.addNote(2000, 100);

	//WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    ArduinoOTA
    .onStart([]() {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH)
        type = "sketch";
      else // U_SPIFFS
        type = "filesystem";

      // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
      Serial.println("Start updating " + type);
    })
    .onEnd([]() {
      Serial.println("\nEnd");
    })
    .onProgress([](unsigned int progress, unsigned int total) {
      Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    })
    .onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
      else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
      else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
      else if (error == OTA_END_ERROR) Serial.println("End Failed");
    });

  ArduinoOTA.begin();

  telnetTerminal.initialise();

//  createScreens(scr);

    /*Create a drop down list*/
#if 0
  lv_obj_t * dropdownMenu = lv_dropdown_create(lv_scr_act());
  lv_obj_align(dropdownMenu, LV_ALIGN_TOP_LEFT, 0, 0);
  lv_dropdown_set_options(dropdownMenu, "Preferences\n"
                                    "Time\n"
                                    "Network\n"
                                    "Level\n"
                                    "Location\n"
                                    "Restart");

  /*Set a fixed text to display on the button of the drop-down list*/
  lv_dropdown_set_text(dropdownMenu, "Options");
  
  /*In a menu we don't need to show the last clicked item*/
  lv_dropdown_set_selected_highlight(dropdownMenu, false);

  lv_obj_add_event_cb(dropdownMenu, menu_cb, LV_EVENT_VALUE_CHANGED, NULL);
#endif
  wifiNextTryConnect = 1000;
  wifiLastTryConnect = millis();

  if (screenTimeout >= 30) {
    tDimDisplay.enableDelayed(screenTimeout * 1000);
  }
}

void loop() {
  static uint32_t lastDim = 0;
  static uint32_t lastLocationUpdate = 0;

	if (bleManager.isServerPinRequired()) {
		tCloseServerPinScreen.enableDelayed(20000);
		tWakeDisplay.enable();
		bleManager.requestServerPin();
	}

  lv_timer_handler(); /* let the GUI do its work */
  ArduinoOTA.handle();

  ts.execute();

  serialTerminal.processData();
  telnetTerminal.processData();

  uint32_t now = millis();

  if (now - wifiLastTryConnect >= wifiNextTryConnect) {
    if (WiFi.status() != WL_CONNECTED) {
      WiFi.disconnect();
	  if (!DHCPMode) {
		  WiFi.config(StaticIP, StaticGateway, StaticMask, StaticDNS[0], StaticDNS[1]);
	  } else {
		  WiFi.config(static_cast<uint32_t>(0), static_cast<uint32_t>(0), static_cast<uint32_t>(0));
	  }
      if (!WiFi.begin(WifiSSID.c_str(), WifiPassword.c_str())) {
        logger.send(LOG_WIFI, LOG_WARNING, "WiFi not connected!\n");
      } else {
        logger.printf(LOG_WIFI, LOG_INFO, "WiFi connected to %s\n", WiFi.SSID());
      }
    }
    wifiLastTryConnect = now;
    wifiNextTryConnect = 10000;
  }
/*
  if (now - lastLocationUpdate > 1000) {
    lastLocationUpdate = now;

    {
    uint8_t gpsbuf[8] = {0xb5, 0x62, 0x06, 0x3b, 0x00, 0x00, 0x00, 0x00};

    uint8_t ck_a = 0, ck_b = 0;
    for (int i = 2; i < 6; i++) {
      ck_a += gpsbuf[i];
      ck_b += ck_a;
    }
    gpsbuf[6] = ck_a;
    gpsbuf[7] = ck_b;
    
    //Serial1.write(gpsbuf, sizeof(gpsbuf));
    
  }*/

//  }

  // put your main code here, to run repeatedly:
}

void fnDimDisplay() {
  brightness -= 5;
  if (brightness <= lowBrightness) {
    tDimDisplay.disable();
    brightness = lowBrightness;
  }
  if (brightness == 0) {
    ledcWrite(BacklightChannel, 256);
  } else {
    ledcWrite(BacklightChannel, 255 - brightness);
  }
}

void fnWakeDisplay() {
  brightness += 16;
  if (brightness >= highBrightness) {
    tWakeDisplay.disable();
    brightness = highBrightness;
  }
  ledcWrite(BacklightChannel, 255 - brightness);
}

void fnFeedGPS() {
  static bool gps_nmea = false;
  static std::string gps_str;
  static std::vector<uint8_t> gps_buf;

#ifdef GPS_USESOFTWARESERIAL
  while (ss.available()) {
    char ch = ss.read();
#else
  while (Serial1.available()) {
    char ch = Serial1.read();
#endif
    if (!gpsDemoMode) {
      gps.encode(ch);
    }
    if (gps_str.empty() && gps_buf.empty()) {
      gps_nmea = (ch != 0xb5);
      if (gps_nmea) {
        //printf("New NMEA %c\n", ch);
      } else {
        //printf("New UBX %c\n", ch);
      }
    }
    if (gps_nmea) {
      gps_str += ch;
      if (ch == '\n') {
        //printf("gps: %s\n", gps_str.c_str());
        gps_str.clear();
      }
    } else {
      gps_buf.emplace_back(ch);
      if (gps_buf.size() > 6) {
        size_t len = gps_buf[5] * 256 + gps_buf[4] + 8;
        if (gps_buf.size() == len) {
          printf("gpsu: ");
         for (int i = 0; i < len; i++) {
          printf("%02x ", gps_buf[i]); 
         }
         printf("\n");
         gps_buf.clear();
        }
      }
    }
  }
  
}

void fnUpdateTimezone() {
  logger.send(LOG_RTC, LOG_VERBOSE, "try update timezone\n");
  if ((WiFi.status() == WL_CONNECTED) && (gps.location.isValid())) {
	  
    TZ.getTimezoneFromLocation(gps.location.lat(), gps.location.lng());
  } else {
    logger.send(LOG_RTC, LOG_VERBOSE, "skip no data/wifi\n");
    tUpdateTimezone.restartDelayed(10000);
  }
}

void fnUpdateLocator() {
  logger.send(LOG_GPS, LOG_INFO, "try update locator\n");
  if ((WiFi.status() == WL_CONNECTED) && (gps.location.isValid())) {
	  
    locator.getAddressFromLocation(gps.location.lat(), gps.location.lng());
  } else {
    logger.send(LOG_GPS, LOG_INFO, "skip no data/wifi\n");
    tUpdateLocator.restartDelayed(10000);
  }
}

void fnUpdateWeather() {
  logger.send(LOG_WEATHER, LOG_VERBOSE, "try update weather\n");
  if ((WiFi.status() == WL_CONNECTED) && (gps.location.isValid())) {
    weather.getWeatherFromLocation(gps.location.lat(), gps.location.lng());
  } else {
    logger.send(LOG_WEATHER, LOG_VERBOSE, "skip no data/wifi\n");
    tUpdateWeather.restartDelayed(10000);
  }
}

void fnUpdateDisplay() {
  Screen.Update();
}

void fnFastUpdateDisplay() {
  Screen.fastUpdate();
}

void fnFlushLogfiles() {
  logger.flush();
}

void fnCloseServerPinScreen() {
	bleManager.closeServerPinScreen();
}