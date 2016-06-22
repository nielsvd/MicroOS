#ifndef ROBOT_HAL_H
#define ROBOT_HAL_H

#include <hal_base.h>
#include <pololuMD08A.h>
#include <analog_sensor.h>

#define SENSOR_ID		10
#define SENSOR_PIN		12
#define SENSOR_SCALE	1.0f
#define HBRIDGE_ID		11
#define HBRIDGE_H1_PIN	2
#define HBRIDGE_PWM_PIN	3

class RobotHAL : public HALBase
{
private:
	Sensor1D* 			_sensor;
	HBridgeInterface*	_motor;

public:
	RobotHAL();
	
	Sensor1D* getSensor();
	HBridgeInterface* getMotor();
};

#endif //ROBOT_HAL_H
