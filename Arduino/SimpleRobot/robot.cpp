#include "robot.h"

Robot::Robot(HBridgeInterface* hbridge, Sensor1D* sensor, uint8_t ID ,uint32_t mode):
	_ID(ID),
    _mode(mode),
    _event(0),
    _hbridge(hbridge),
    _sensor(sensor)
{
	//do something else
}

void Robot::init(){
	//initialize the robot - sort of starting procedure
}

void Robot::controllerHook(){
	//do something that is periodic
	_sensor->readRawValue();
	_hbridge->setBridgeVoltage(1000); //1000 mV = 1 V
}

void Robot::setID(uint8_t ID){
    _ID = ID;
}

void Robot::setMode(uint32_t mode){
	_mode = mode;
}

void Robot::setEvent(uint32_t event)
{
	_event = event;
}
uint8_t Robot::ID()
{
    return _ID;
}

uint32_t Robot::mode()
{
	return _mode;
}

uint32_t Robot::event()
{
	uint32_t event_copy = _event;
	_event = 0;
	return event_copy;
}

uint32_t Robot::peekEvent()
{
	return _event;
}

HBridgeInterface* Robot::getHBridge()
{
	return _hbridge;
}

Sensor1D* Robot::getSensor()
{
	return _sensor;
}


