#ifndef _TASKSCHEDULERSLEEPMETHODS_H_
#define _TASKSCHEDULERSLEEPMETHODS_H_

void SleepMethod(unsigned long aDuration)
{
  WiFi.setSleep(true);
  //printf("Sleep %d\n", aDuration);
  setCpuFrequencyMhz(40);
  delayMicroseconds(1000 - aDuration);
  //printf("End sleep %d\n", aDuration);
  setCpuFrequencyMhz(240);
  WiFi.setSleep(false);
}

#endif
