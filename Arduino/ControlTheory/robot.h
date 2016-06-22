#ifndef ROBOT_H
#define ROBOT_H

//!  ROBOT Class 
/*!
  Class incorporating the robot. This class is used to define state machines, control algorithms, sensor readings,...
  It should be interfaced with the communicator to send data to the world.
*/

#include <inttypes.h>
#include "math.h"

#include "microOS.h"
#include "encoder_sensor.h"
#include "difference_sensor.h"
#include "sharp41S.h"
#include "l293d.h"

class Robot
{
private:
	uint8_t _ID;			//give the robot an ID so that you can recognize it
	
	// Give the robot some sensors
	Sensor1D* _encoder1;
	Sensor1D* _encoder2;
	Sensor1D* _speed1;
	Sensor1D* _speed2;
	Sensor1D* _distance1;
	Sensor1D* _distance2;
	
	// Give the robot some motors
	HBridgeInterface* _motor1;
	HBridgeInterface* _motor2;
	
	// Kalman variables
	float p11,p12,p13,p21,p22,p23,p31,p32,p33;
	float x1,x2,x3;
	
public:
    Robot(uint8_t ID = 0);
	
	////////
	/// FUNC
	void init();			//set up the robot
	void controllerHook();	//update function which can be executed continuously
	
	///////
	/// SET
	void setID(uint8_t ID);
	
	///////
	/// GET
    uint8_t ID();
};

#endif //ROBOT_H
