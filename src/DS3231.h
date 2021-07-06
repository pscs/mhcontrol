#pragma once

#include <cinttypes>
#include <time.h>

class DS3231
{
public:
  DS3231(uint8_t addr = 0x68);

  time_t Get() const;
  void Set(time_t t);

  const char *getTimeText(time_t t);
  
private:
  static uint8_t Bcd2Dec(uint8_t a);
  static uint8_t Dec2Bcd(uint8_t a);

  int m_addr;

  mutable uint32_t m_CheckpointMillis = 0;
  mutable uint32_t m_CheckpointTime = 0;
};

extern DS3231 Clock;
