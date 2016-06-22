#include "robot_hal.h"

RobotHAL::RobotHAL() :
	HALBase(13,78),
	_sensor(new AnalogSensor(SENSOR_PIN, SENSOR_ID)),
	_motor(new PololuMD08A(HBRIDGE_H1_PIN, HBRIDGE_PWM_PIN, 5000u, HBRIDGE_ID))
{		
	_sensor->setScale(SENSOR_SCALE);
}

Sensor1D* RobotHAL::getSensor()
{
	return _sensor;
}

HBridgeInterface* RobotHAL::getMotor()
{
	return _motor;
}
