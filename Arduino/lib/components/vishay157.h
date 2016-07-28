#ifndef VISHAY157_H
#define VISHAY157_H

#include "analog_sensor.h"

class Vishay157: public AnalogSensor
{
public:
	Vishay157(uint8_t pin, uint8_t ID = 0);

	float readCalibratedValue();
};

#endif //VISHAY157_H
