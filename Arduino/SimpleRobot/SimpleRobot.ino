#include <Wire.h>
#include <pololuMD08A.h>
#include "robot.h"
#include "robot_hal.h"
#include "communicator.h"
#include "microOS.h"
#include "math.h"

#define USB_SERIAL

#define ROBOT_ID	178

static RobotHAL* hal = new RobotHAL();
static Robot* robot = new Robot(hal->getMotor(), hal->getSensor(), ROBOT_ID);
static Communicator* communicator = new Communicator(robot);

int updateRobot(void)
{
	robot->controllerHook();
	MicroOS::setGPoutInt(3,10*sin(M_PI*2.0f*millis()*0.001f));
	MicroOS::setGPoutFloat(1,sin(2*M_PI*2.0f*millis()*0.001f));
	MicroOS::setGPoutFloat(2,(2.0f+MicroOS::getGPinFloat(0))*sin(2*M_PI*2.0f*millis()*0.001f));
	return 0;
}

int slowHook(void)
{
	MicroOS::sendSystemRequest(NEXTTHREADINFO);
	return 0;
}

void setup()
{
	delay(1000);
	MicroOS::init(hal, communicator);

	MicroOS::addThread(ABOVENORMAL, 5000, &updateRobot, false);
	MicroOS::addThread(LOWEST, 2000000, &slowHook, false);
	
	robot->init();
	MicroOS::start(REGULAR);
}

void loop()
{
	MicroOS::run(RESCHEDULED);
}
