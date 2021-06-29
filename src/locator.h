#pragma once
#include <JsonHandler.h>
#include <JsonStreamingParser2.h>
#include <string>

class Locator: public JsonHandler {
public:
  void getAddressFromLocation(float lat, float lng);

  std::string getAddress() const;

  //void whitespace(char c) override;
  void startDocument() override;
  void value(ElementPath path, ElementValue value) override;
  void endArray(ElementPath path) override;
  void endObject(ElementPath path) override;
  void endDocument() override;
  void startArray(ElementPath path) override;
  void startObject(ElementPath path) override;
  void whitespace(char c) override;

private:
  std::string area1;
  std::string area3;
  std::string area4;
  std::string area5;
  std::string area6;
  std::string postcode;
  std::string street;
};

extern Locator locator;