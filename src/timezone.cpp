#include <Arduino.h>
#include <HTTPClient.h>

#include "timezone.h"
#include "config.h"

Timezone TZ;

void Timezone::getTimezoneFromLocation(float lat, float lng) {
  
    HTTPClient http;

    char buf[200];
    snprintf(buf, sizeof(buf), "http://api.timezonedb.com/v2.1/get-time-zone?key=%s&format=json&by=position&lat=%s&lng=%s",
      TIMEZONEAPIKEY, String(lat, 5).c_str(), String(lng, 5).c_str());
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
      printf("Timezone: %s (%s) %d\n", name.c_str(), abbr.c_str(), offset);
    } else {
      printf("http response %d\n", httpResponseCode);
    }
    http.end();
}

int Timezone::getOffset() const
{
	return offset;
}

std::string Timezone::getName() const
{
  return name;
}

void Timezone::startDocument() {
}

void Timezone::value(ElementPath path, ElementValue value) {
  std::string strPath {path.getKey()};
  printf("TZ: value %s\n", strPath.c_str());
  if (strPath == "zoneName") {
    name = value.getString();
  } else if (strPath == "abbreviation") {
    abbr = value.getString();
  } else if (strPath == "gmtOffset") {
    offset = value.getInt();
  }
}

void Timezone::endArray(ElementPath path) {
}

void Timezone::endObject(ElementPath path) {
}

void Timezone::endDocument() {
}

void Timezone::startArray(ElementPath path) {
}

void Timezone::startObject(ElementPath path) {
}

void Timezone::whitespace(char c) {
}
