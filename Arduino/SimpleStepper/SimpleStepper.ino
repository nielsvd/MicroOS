#include <Wire.h>
#include "robot.h"
#include "robot_hal.h"
#include "fast_stepper.h"
#include "communicator.h"
#include "microOS.h"
#include "math.h"

#define USB_SERIAL

#define ROBOT_ID	178

static RobotHAL* hal = new RobotHAL();
static FastStepper* stepper = new FastStepper(1);
static Robot* robot = new Robot(ROBOT_ID);
static Communicator* communicator = new Communicator(robot);
static long stepcounter = 0;

int updateRobot(void)
{
	robot->controllerHook();
	stepper->setSpeed(2000*(sin(2*M_PI*millis()*0.001) + 1));
	stepper->update(0.01f);
	MicroOS::setGPoutInt(1,stepper->getTimerCompare(250000));
	if(stepper->speed() >= 0){
		//set direction pin HIGH
	} else {
		//set direction pin LOW
	}
	updateTimer1Period();
	
	//show wave forms on micro OS
	MicroOS::setGPoutFloat(0,stepper->position());
	MicroOS::setGPoutFloat(1,stepper->speed());
	MicroOS::setGPoutFloat(2,stepper->acceleration());
	MicroOS::setGPoutInt(0,stepcounter);
	return 0;
}

int slowHook(void)
{
	MicroOS::sendSystemRequest(NEXTTHREADINFO);
	return 0;
}

void setupTimer1()
{
	cli();	//disable interrupts
	
	TCCR1A = 0;// set entire TCCR1A register to 0
	TCCR1B = 0;// same for TCCR1B
	TCNT1  = 0;//initialize counter value to 0
	// set compare match register for 1hz increments
	OCR1A = 0xFFFF; //do nothing at this point
	// turn on CTC mode
	TCCR1B |= (1 << WGM12);
	// Set CS10 and CS12 bits for 64 prescaler -> 250kHz timer
	TCCR1B |= (1 << CS11) | (1 << CS10);  
	// enable timer compare interrupt
	TIMSK1 |= (1 << OCIE1A);
	
	sei();//allow interrupts
}

void updateTimer1Period()
{
	cli();	//disable interrupts
	
	OCR1A = stepper->getTimerCompare(250000);
	TCNT1  = OCR1A >> 2;//initialize counter value to half
	
	sei();//allow interrupts
}

ISR(TIMER1_COMPA_vect)
{
	//toggle the right stepper pin
	PORTB |= 1<<5;		//set bit 5 high (LED)
	PORTB &= ~(1<<5);	//set bit 5 low (LED)
	stepcounter++;
}

void setup()
{
	MicroOS::init(hal, communicator);

	MicroOS::addThread(ABOVENORMAL, 10000, &updateRobot, false);
	MicroOS::addThread(LOWEST, 2000000, &slowHook, false);
	
	robot->init();
	setupTimer1();
	MicroOS::start(REGULAR);
}

void loop()
{
	MicroOS::run(RESCHEDULED);
}
