#include <microOS.h>
#include <Wire.h>

static HALInterface* hal = new HALInterface(13, 100);		//make a dummy HAL - led only
static CommunicatorInterface* comm = new CommunicatorInterface();	//make a dummy communicator - make the OS happy

void setup()
{
	MicroOS::init(hal,comm);
	MicroOS::start();
}

void loop()
{
	MicroOS::run();
}
