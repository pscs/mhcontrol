#include "accelerometer.h"

Accelerometer accelerometer;

Accelerometer::Accelerometer() {
}

Accelerometer::~Accelerometer() {
	if (acc) {
		sleep();
		delete acc;
	}
	if (tMinMax) {
		delete tMinMax;
	}
}

bool Accelerometer::initialise(uint8_t addr) {
	printf("Initialise accelerometer\n");
	acc = new ADXL345_WE(addr);

	printf("created object - %d\n", (int)acc);

	if (!acc->init()) {
		printf("Accelerometer not initialised\n");
		delete acc;
		return false;
	}
	printf("accelerometer initialised\n");

	acc->setDataRate(ADXL345_DATA_RATE_6_25);
	acc->setRange(ADXL345_RANGE_4G);
	printf("accel configured\n");

	tMinMax = new Task(2, -1, &Accelerometer::readMinMax, &ts, false);
	sleep();

	return true;
}

void Accelerometer::sleep() {
	if (acc) {
		acc->setSleep(true);
		tMinMax->disable();
	}
}

void Accelerometer::wake(adxl345_dataRate rate) {
	if (acc) {
		acc->setSleep(false);
		acc->setDataRate(rate);
	}
}

xyzFloat Accelerometer::getRawValues() {
	if (isConnected()) {
		return acc->getRawValues();
	}
	return xyzFloat{0,0,0};
}

void Accelerometer::startMinMax() {
	accelerometer.wake(ADXL345_DATA_RATE_400);
	minValues = {0, 0, 0};
	maxValues = minValues;
	tMinMax->enable();
}

void Accelerometer::endMinMax() {
	sleep();
}

xyzFloat Accelerometer::getMinValues() {
	return minValues;
}

xyzFloat Accelerometer::getMaxValues() {
	return maxValues;
}

void Accelerometer::readMinMax() {
	accelerometer.logMinMax();
}

void Accelerometer::logMinMax() {
	if (acc) {
		xyzFloat v = acc->getRawValues();
		if (v.x < minValues.x) {
			minValues.x = v.x;
		} else if (v.x > maxValues.x) {
			maxValues.x = v.x;
		}
		if (v.y < minValues.y) {
			minValues.y = v.y;
		} else if (v.y > maxValues.y) {
			maxValues.y = v.y;
		}
		if (v.z < minValues.z) {
			minValues.z = v.z;
		} else if (v.z > maxValues.z) {
			maxValues.z = v.z;
		}
	}
}
