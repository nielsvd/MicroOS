#include "led.h"

LED::LED(uint8_t pin, uint8_t ID) :
	ComponentInterface(ID),
	_pin(pin),
	_on(true),
	_wake_up(0),
	_on_time(100),
	_off_time(0)
{
	//do nothing
}

bool LED::init() {
  pinMode(_pin, OUTPUT);
  return true;
}

void LED::update() {
  if(millis()>_wake_up) {
    if(_on) digitalWrite(_pin, HIGH);
    else digitalWrite(_pin, LOW);
  }
}

void LED::ledOn(){ _on = true; }
void LED::ledOff(){ _on = false; }

void LED::toggle() {
  if(_on) _on = false;
  else _on = true;
  this->update();
}

uint8_t LED::setSchedule(uint8_t duty_cycle, uint16_t period){
	if(period == 0){
		return 0; 	//invalid input
	}
	if(duty_cycle>100){
		_on_time = 100;//
		_off_time = 0;//
	}
	else{
		_on_time = (uint16_t)(period*duty_cycle)/100.0;
		_off_time = (uint16_t)(period-_on_time);
	}

	return 1;
}

bool LED::getOn(){ return _on; }
uint16_t LED::getOntime(){ return _on_time; }
