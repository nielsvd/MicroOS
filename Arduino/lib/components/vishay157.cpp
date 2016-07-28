#include "vishay157.h"

Vishay157::Vishay157(uint8_t pin, uint8_t ID):
	AnalogSensor(pin,ID)
{
	//do nothing special
}

float Vishay157::readCalibratedValue()
{
	//Scale output between 0-1:
  //Uout = R1/Rtot * Uin

	_calibrated_value = this->readRawValue()/1023.4f;
}
