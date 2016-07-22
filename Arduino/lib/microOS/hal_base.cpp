#include "hal_base.h"

HALBase::HALBase(const uint8_t onboard_led_pin, const uint8_t version) :
	_version(version), _onboard_led_pin(onboard_led_pin), _onboard_led_state(false)
{
	pinMode(_onboard_led_pin, OUTPUT);
	digitalWrite(_onboard_led_pin, _onboard_led_state);
}

bool HALBase::init()
{
	onboardLedToggle();
	return true;
}

const uint8_t HALBase::version()
{
	return _version;
}

const uint8_t HALBase::onboardLedPin()
{
	return _onboard_led_pin;
}

void HALBase::onboardLedToggle()
{
	_onboard_led_state = !_onboard_led_state;
	digitalWrite(_onboard_led_pin, _onboard_led_state);
}	

uint8_t HALBase::setAnalogReadResolution(const uint8_t resolution)
{
	#ifdef __arm__
		analogReadResolution(resolution);
		return resolution;
	#else
		return 10;
	#endif
}
