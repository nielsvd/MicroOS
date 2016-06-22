#include <Wire.h>
#include <encoder_sensor.h>
#include "robot.h"
#include "communicator.h"
#include "microOS.h"

#define USB_SERIAL

static Robot* robot = new Robot();
static Communicator* communicator = new Communicator(robot);

int updateRobot(void)
{
	robot->controllerHook();
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
	MicroOS::init(communicator);

	MicroOS::addThread(ABOVENORMAL, 10000, &updateRobot, false);
	MicroOS::addThread(LOWEST, 2000000, &slowHook, false);
	
	robot->init();
	MicroOS::start(SEQUENTIAL);
}

void loop()
{
	MicroOS::run(RESCHEDULED);
}
