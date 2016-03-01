#include "hal_interface.h"

HALInterface::HALInterface(const uint8_t onboard_led_pin, const uint8_t version) :
	_version(version), _onboard_led_pin(onboard_led_pin), _onboard_led_state(false)
{
	pinMode(_onboard_led_pin, OUTPUT);
	digitalWrite(_onboard_led_pin, _onboard_led_state);
}

bool HALInterface::init()
{
	return true;
}

void HALInterface::microOSUpdateHook()
{
	//do nothing
}

const uint8_t HALInterface::version()
{
	return _version;
}

const uint8_t HALInterface::onboardLedPin()
{
	return _onboard_led_pin;
}

void HALInterface::onboardLedToggle()
{
	_onboard_led_state = !_onboard_led_state;
	digitalWrite(_onboard_led_pin, _onboard_led_state);
}	

uint8_t HALInterface::setAnalogReadResolution(const uint8_t resolution)
{
	#ifdef __arm__
		analogReadResolution(resolution);
		return resolution;
	#else
		return 10;
	#endif
}
