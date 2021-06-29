#pragma once
#include <JsonHandler.h>
#include <JsonStreamingParser2.h>
#include <string>

struct WeatherInfo {
 uint16_t code;
 uint8_t icon;
 uint16_t pressure;
 uint8_t humidity;
 uint16_t dewPoint;
 uint8_t clouds;
 uint16_t uvi;
 uint16_t visibility;
 uint16_t windSpeed;
 uint16_t windGust;
 uint16_t windDeg;
 uint8_t rain;
 uint8_t snow;

  bool load(const ElementPath &path, int baseDepth, const ElementValue &value);

} __attribute__((packed));
static_assert(sizeof(WeatherInfo) == 21, "Struct not packed");

struct WeatherInfoNow: public WeatherInfo {
  uint16_t temp;
  uint16_t feelsLike;

  bool load(const ElementPath &path, int baseDepth, const ElementValue &value);

} __attribute__((packed));
static_assert(sizeof(WeatherInfoNow) == 25, "Struct not packed");

struct WeatherInfoHourly: public WeatherInfo {
  uint32_t time;
  uint8_t pop;
  uint16_t temp;
  uint16_t feelsLike;

  bool load(const ElementPath &path, int baseDepth, const ElementValue &value);

} __attribute__((packed));
static_assert(sizeof(WeatherInfoHourly) == 30, "Struct not packed");

struct WeatherInfoDaily: public WeatherInfo {
  uint32_t time;
  uint8_t pop;
  uint16_t tempMorn;
  uint16_t tempDay;
  uint16_t tempEve;
  uint16_t tempNight;
  uint16_t tempMin;
  uint16_t tempMax;
  uint16_t feelsLikeMorn;
  uint16_t feelsLikeDay;
  uint16_t feelsLikeEve;
  uint16_t feelsLikeNight;
  uint32_t sunrise;
  uint32_t sunset;
  uint32_t moonrise;
  uint32_t moonset;
  uint8_t moonPhase;

  bool load(const ElementPath &path, int baseDepth, const ElementValue &value);
} __attribute__((packed));
static_assert(sizeof(WeatherInfoDaily) == 63, "Struct not packed");

class Weather: public JsonHandler {
public:
  void getWeatherFromLocation(float lat, float lng);

  bool isLoaded() const {
    return loaded;
  }
  bool loaded = false;
  WeatherInfoNow now;
  WeatherInfoHourly hourly[24];
  WeatherInfoDaily daily[7];

  void startDocument() override;
  void value(ElementPath path, ElementValue value) override;
  void endArray(ElementPath path) override;
  void endObject(ElementPath path) override;
  void endDocument() override;
  void startArray(ElementPath path) override;
  void startObject(ElementPath path) override;
  void whitespace(char c) override;

private:
//  bool loadWeatherBase(WeatherInfo &info, const ElementPath &path, int baseDepth, const ElementValue &value);
//  bool loadWeather(WeatherInfoNow &info, const ElementPath &path, int baseDepth, const ElementValue &value);
};

extern Weather weather;

