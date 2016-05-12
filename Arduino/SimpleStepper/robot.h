#ifndef ROBOT_H
#define ROBOT_H

#include <inttypes.h>

//!  ROBOT Class 
/*!
  Class incorporating the robot. This class is used to define state machines, control algorithms, sensor readings,...
  It should be interfaced with the communicator to send data to the world.
*/

class Robot
{
private:
	uint8_t _ID;			//give the robot an ID so that you can recognize it
	uint32_t _mode;			//make the robot operate in a specific mode
	uint32_t _event;		//send and receive events which happened    
	
public:
    Robot(uint8_t ID, uint32_t mode = 0);
	
	////////
	/// FUNC
	void init();			//set up the robot
	void controllerHook();	//update function which can be executed continuously
	
	///////
	/// SET
	void setID(uint8_t ID);
	void setMode(uint32_t mode);
	void setEvent(uint32_t event);
	
	///////
	/// GET
    uint8_t ID();
    uint32_t mode();
    uint32_t event();
    uint32_t peekEvent();
};

#endif //ROBOT_H
