#pragma once
#include <JsonHandler.h>
#include <JsonStreamingParser2.h>
#include <string>

class Timezone: public JsonHandler {
public:
  void getTimezoneFromLocation(float lat, float lng);

  int getOffset() const;
  std::string getName() const;

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
  std::string name;
  std::string abbr;
  int offset;
};

extern Timezone TZ;
