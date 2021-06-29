#include "locator.h"

#include <HTTPClient.h>
#include "config.h"

Locator locator;

void Locator::getAddressFromLocation(float lat, float lng)
{
    HTTPClient http;

    char buf[200];
    snprintf(buf, sizeof(buf), "http://open.mapquestapi.com/geocoding/v1/reverse?key=%s&location=%s,%s&thumbMaps=false",
      MAPQUESTAPIKEY, String(lat, 5).c_str(), String(lng, 5).c_str());
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
      printf("Address: %s~%s~%s~%s~%s~%s~%s\n", street.c_str(), area6.c_str(), area5.c_str(), area4.c_str(), area3.c_str(), area1.c_str(), postcode.c_str());
    } else {
      printf("http response %d\n", httpResponseCode);
    }
    http.end();

}

std::string Locator::getAddress() const
{
  std::string address;
  if (!street.empty()) {
      address += street;
      address += ", ";
  }
  if (!area6.empty()) {
      address += area6;
      address += ", ";
  }
  if (!area5.empty()) {
      address += area5;
      address += ", ";
  }
  if (!area4.empty()) {
      address += area4;
      address += ", ";
  }
  if (!area3.empty()) {
      address += area3;
      address += ", ";
  }
  if (!area1.empty()) {
      address += area1;
      address += ", ";
  }
  if (!postcode.empty()) {
      address += postcode;
      address += ", ";
  }
  if (address.size() > 2) {
      address.pop_back();
      address.pop_back();
  }

  return address;
}

void Locator::startDocument() {
}

void Locator::value(ElementPath path, ElementValue value) {
  std::string strPath {path.getKey()};

  printf("Loc: value %s\n", strPath.c_str());

  if (strPath == "adminArea1") {
    area1 = value.getString();
  } else if (strPath == "adminArea3") {
    area3 = value.getString();
  } else if (strPath == "adminArea4") {
    area4 = value.getString();
  } else if (strPath == "adminArea5") {
    area5 = value.getString();
  } else if (strPath == "adminArea6") {
    area6 = value.getString();
  } else if (strPath == "street") {
    street = value.getString();
  } else if (strPath == "postalCode") {
    postcode = value.getString();
  }
}

void Locator::endArray(ElementPath path) {
}

void Locator::endObject(ElementPath path) {
}

void Locator::endDocument() {
}

void Locator::startArray(ElementPath path) {
}

void Locator::startObject(ElementPath path) {
}

void Locator::whitespace(char c) {
}