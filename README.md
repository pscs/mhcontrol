# mhcontrol

Software for ESP32 WROVER (8MB flash, 8MB PSRAM) for a little control panel for our motorhome using an AZ-Touch unit, with a GPS, RTC and accelerometer.

Developed using PlatformIO with the Arduino framework

Communicates with Victron SmartSolar charge controller, B2B charger and mains charger, and KS Energy Lithium battery for power display

Shows current location (via MapQuest), weather (weather OpenWeatherMap), time via GPS & RTC, with Timezone info from OpenTimeZoneDB

Uploads telemetry data to ThingsBoard server for remote monitoring
