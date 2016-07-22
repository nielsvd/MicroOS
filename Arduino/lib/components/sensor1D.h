#ifndef SENSOR1D_H
#define SENSOR1D_H

#include "component_interface.h"

class Sensor1D : public ComponentInterface
{
protected:
	int _raw_value;
	float _calibrated_value;
	float _scale;
	int _offset;

public:
	Sensor1D(uint8_t ID = 0);

	virtual int readRawValue();
	virtual float readCalibratedValue();
	int peekRawValue();
	float peekCalibratedValue();

	void setRawValue(int raw_value);
	void setCalibratedValue(float calibrated_value);

	void setScale(float scale);
	void setOffset(int offset);

	float getScale();
	int getOffset();

};

#endif //SENSOR1D_H
