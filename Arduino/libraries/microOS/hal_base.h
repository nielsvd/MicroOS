#ifndef HAL_INTERFACE_H
#define HAL_INTERFACE_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class HALBase
{
private:
	const uint8_t 	_version;	
	const uint8_t 	_onboard_led_pin;
	bool 			_onboard_led_state;
	
public:	
	HALBase(const uint8_t onboard_led_pin = 13, const uint8_t version = 0);
	
	virtual bool init();
	
	const uint8_t version();
	const uint8_t onboardLedPin();
	void onboardLedToggle();
	
	uint8_t setAnalogReadResolution(const uint8_t resolution);
};

#endif //HAL_INTERFACE_H
