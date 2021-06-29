#include "weather.h"

#include <HTTPClient.h>
#include "config.h"

Weather weather;

void Weather::getWeatherFromLocation(float lat, float lng)
{
    HTTPClient http;

    char buf[200];
    snprintf(buf, sizeof(buf), "http://api.openweathermap.org/data/2.5/onecall?appid=%s&lat=%s&lon=%s&exclude=minutely",
      OPENWEATHERMAPAPIKEY, String(lat, 5).c_str(), String(lng, 5).c_str());
    printf("http request %s\n", buf);
    http.setReuse(false);
    http.begin(buf);
    int httpResponseCode = http.GET();
    if (httpResponseCode == 200) {

      JsonStreamingParser parser;
      parser.setHandler(this);
      WiFiClient *client = http.getStreamPtr();

      //printf("parse HTTP response\n");
     
      bool inBody = false;
      char lastChar = 0;
      int size;
      while(client->connected() || client->available()) {
        while((size = client->available()) > 0) {
          char c = client->read();
           //printf("p: %c (%02x)\n", c, c);
          if (c == '{') { 
            //printf("in body\n");
            inBody = true;
          }

          if (inBody) {
            parser.parse(c);
          } else {
            lastChar = c;
          }
          yield();
        }
      }
      printf("Weather: %d\n",now.temp);
      loaded = true;
    } else {
      printf("http response %d\n", httpResponseCode);
    }
    http.end();
}

void Weather::startDocument() {
}

void Weather::value(ElementPath path, ElementValue value) {
  char buf[200];

  if (path.getCount() > 1) {
      //printf("W: - %s - %s\n", path.getKey(0), path.getKey(1));
      if (strcmp(path.getKey(0), "current") == 0) {
        now.load(path, 1, value);
      } else if (strcmp(path.getKey(0), "hourly") == 0) {
        if ((path.getIndex(1) < 24) && (path.getIndex(1) >= 0)) {
            hourly[path.getIndex(1)].load(path, 2, value);
        }
      } else if (strcmp(path.getKey(0), "daily") == 0) {
        if ((path.getIndex(1) < 7) && (path.getIndex(1) >= 0)) {
            daily[path.getIndex(1)].load(path, 2, value);
        }
      }
  }
 }


void Weather::endArray(ElementPath path) {
}

void Weather::endObject(ElementPath path) {
}

void Weather::endDocument() {
}

void Weather::startArray(ElementPath path) {
}

void Weather::startObject(ElementPath path) {
}

void Weather::whitespace(char c) {
}

bool WeatherInfo::load(const ElementPath &path, int baseDepth, const ElementValue &value) {
  std::string strPath {path.getKey()};

  if (path.getCount() == baseDepth + 1) {
    if (strPath == "pressure") {
        pressure = value.getInt();
    } else if (strPath == "humidity") {
        humidity = value.getInt();
    } else if (strPath == "dew_point") {
        dewPoint = value.getFloat() * 100;
    } else if (strPath == "uvi") {
        uvi = value.getFloat() * 100;
    } else if (strPath == "clouds") {
        clouds = value.getInt();
    } else if (strPath == "visibility") {
        visibility = value.getInt();
    } else if (strPath == "wind_speed") {
        windSpeed = value.getFloat() * 100;
    } else if (strPath == "wind_gust") {
        windGust = value.getFloat() * 100;
    } else if (strPath == "wind_deg") {
        windDeg = value.getInt();
    } else {
        return false;
    }
  } else if (path.getCount() == baseDepth + 3) {
      if (strPath == "id") {
          code = value.getInt();
      } else if (strPath == "icon") {
          std::string strIcon = value.getString();
          strIcon.pop_back();
          icon = atoi(strIcon.c_str());
      } else {
          return false;
      }
  } else {
      return false;
  }
  return true;
}

bool WeatherInfoNow::load(const ElementPath &path, int baseDepth, const ElementValue &value) {
  if (WeatherInfo::load(path, baseDepth, value)) {
      return true;
  }
  
  std::string strPath {path.getKey()};

  if (path.getCount() == baseDepth + 1) {
    if (strPath == "temp") {
        temp = value.getFloat() * 100;
    } else if (strPath == "feels_like") {
        feelsLike = value.getFloat() * 100;
    } else {
        return false;
    }
  } else {
      return false;
  }
  return true;
}

bool WeatherInfoHourly::load(const ElementPath &path, int baseDepth, const ElementValue &value) {
  if (WeatherInfo::load(path, baseDepth, value)) {
      return true;
  }
  
  std::string strPath {path.getKey()};

  if (path.getCount() == baseDepth + 1) {
    if (strPath == "temp") {
        temp = value.getFloat() * 100;
    } else if (strPath == "feels_like") {
        feelsLike = value.getFloat() * 100;
    } else if (strPath == "time") {
        time = value.getFloat();
    } else if (strPath == "pop") {
        pop = value.getFloat() * 100;
    } else {
        return false;
    }
  } else {
      return false;
  }
  return true;
}

bool WeatherInfoDaily::load(const ElementPath &path, int baseDepth, const ElementValue &value) {
  if (WeatherInfo::load(path, baseDepth, value)) {
      return true;
  }
  
  std::string strPath {path.getKey()};

  if (path.getCount() == baseDepth + 1) {
    if (strPath == "time") {
        time = value.getFloat();
    } else if (strPath == "sunrise") {
        sunrise = value.getFloat() * 100;
    } else if (strPath == "sunset") {
        sunset = value.getFloat() * 100;
    } else if (strPath == "moonrise") {
        moonrise = value.getFloat() * 100;
    } else if (strPath == "moonset") {
        moonset = value.getFloat() * 100;
    } else if (strPath == "pop") {
        pop = value.getFloat() * 100;
    } else if (strPath == "moon_phase") {
        moonPhase = value.getFloat() * 100;
    } else {
        return false;
    }
  } else if (path.getCount() == baseDepth + 2) {
    if (strcmp(path.getKey(2), "temp") == 0) {
      if (strPath == "day") {
        tempDay = value.getFloat();
      } else if (strPath == "min") {
        tempMin = value.getFloat() * 100;
      } else if (strPath == "max") {
        tempMax = value.getFloat() * 100;
      } else if (strPath == "night") {
        tempNight = value.getFloat() * 100;
      } else if (strPath == "eve") {
        tempEve = value.getFloat() * 100;
      } else if (strPath == "morn") {
        tempMorn = value.getFloat() * 100;
      } else {
        return false;  
      }
   } else if (strcmp(path.getKey(2), "feels_like") == 0) {
      if (strPath == "day") {
        feelsLikeDay = value.getFloat();
      } else if (strPath == "night") {
        feelsLikeNight = value.getFloat() * 100;
      } else if (strPath == "eve") {
        feelsLikeEve = value.getFloat() * 100;
      } else if (strPath == "morn") {
        feelsLikeMorn = value.getFloat() * 100;
      } else {
        return false;
      }
   } else {
     return false;
   }
  } else {
      return false;
  }
  return true;
}
