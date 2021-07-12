#pragma once

#include <ADXL345_WE.h>
#include "tasks.h"

class Accelerometer {
public:
	Accelerometer();
	~Accelerometer();

	bool initialise(uint8_t addr);
	void sleep();
	void wake(adxl345_dataRate rate = ADXL345_DATA_RATE_6_25);

	bool isConnected() {
		return acc;
	}

	xyzFloat getRawValues();

	static void readMinMax();
	void logMinMax();

	void startMinMax();
	void endMinMax();
	xyzFloat getMinValues();
	xyzFloat getMaxValues();

private:
	Task *tMinMax;
	float calibData[6];
	xyzFloat minValues;
	xyzFloat maxValues;
	ADXL345_WE *acc = nullptr;
};

extern Accelerometer accelerometer;