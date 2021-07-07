#include "DS3231.h"

#include <Wire.h>

#include "logger.h"

DS3231::DS3231(uint8_t addr) : m_addr(addr)
{
}

time_t DS3231::Get() const
{
  if (m_CheckpointTime != 0) {
    uint32_t diff = millis() - m_CheckpointMillis;
    if (diff < 600 * 1000) {
      // printf("return fake time %u %u %u\n", m_CheckpointTime, diff / 1000, m_CheckpointTime + diff / 1000);
      return m_CheckpointTime + (diff / 1000);
    }
  }
  
  Wire.beginTransmission(m_addr);
  Wire.write(0);
  Wire.endTransmission();

  m_CheckpointMillis = millis();
  Wire.requestFrom(m_addr, 7);

  //while (!Wire.available()) {};

  tm tmInfo;


  for (int i = 0; i < 7; i++) {
    uint8_t x = Wire.read();
    logger.printf(LOG_RTC, LOG_DEBUG, "ClockData %d - %d %02x\n", i, x, x);
    uint8_t xx = Bcd2Dec(x);
    switch(i) {
      case 0:
        tmInfo.tm_sec = xx;
        break;
      case 1:
        tmInfo.tm_min = xx;
        break;
      case 2:
        tmInfo.tm_hour = xx;
        break;
      case 3:
        tmInfo.tm_wday = xx - 1;
        break;
      case 4:
        tmInfo.tm_mday = xx;
        break;
      case 5:
        tmInfo.tm_mon = xx - 1;
        break;
      case 6:
        tmInfo.tm_year = xx + 100;
        break;
    }
  }

  m_CheckpointTime = mktime(&tmInfo);
  return m_CheckpointTime;
}

void DS3231::Set(time_t t)
{
  tm *tmInfo = gmtime(&t);

  logger.printf(LOG_RTC, LOG_INFO, "RTC Set - %d %d %d %d %d %d %d\n", tmInfo->tm_sec, tmInfo->tm_min, tmInfo->tm_hour, tmInfo->tm_wday + 1, tmInfo->tm_mday, tmInfo->tm_mon + 1, tmInfo->tm_year - 100);

  Wire.beginTransmission(m_addr);
  Wire.write(0);
  logger.printf(LOG_RTC, LOG_DEBUG, "Sec = %d -> %d %02x\n", tmInfo->tm_sec, Dec2Bcd(tmInfo->tm_sec), Dec2Bcd(tmInfo->tm_sec));
  Wire.write(Dec2Bcd(tmInfo->tm_sec));
  Wire.write(Dec2Bcd(tmInfo->tm_min));
  Wire.write(Dec2Bcd(tmInfo->tm_hour));
  Wire.write(Dec2Bcd(tmInfo->tm_wday + 1));
  Wire.write(Dec2Bcd(tmInfo->tm_mday));
  Wire.write(Dec2Bcd(tmInfo->tm_mon + 1));
  Wire.write(Dec2Bcd(tmInfo->tm_year - 100));

  Wire.endTransmission();
}

uint8_t DS3231::Bcd2Dec(uint8_t a)
{
  return (a / 16 * 10) + (a % 16);
}

uint8_t DS3231::Dec2Bcd(uint8_t a)
{
  return (a / 10 * 16) + (a % 10);
}
