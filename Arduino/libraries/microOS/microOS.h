#ifndef MICROOS_H
#define MICROOS_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>
//#include <SPI.h>
//#include "eeprom_m.h"
#include "hal_interface.h"
#include "communicator_interface.h"
#include "thread.h"

/*Defines with respect to the automatic threads: onboard and communication */

#define MICROOS_SLOW_THREAD_ID				100
#define MICROOS_SLOW_THREAD_PERIOD			1000000	//1Hz
#define MICROOS_FAST_THREAD_ID				101
#define MICROOS_FAST_THREAD_PERIOD			10000	//100Hz

#define MICROOS_I2C_ENABLE	1
#define MICROOS_SPI_ENABLE	1<<1
#define MICROOS_ONB_DISABLE	1<<2
#define MICROOS_COM_DISABLE	1<<3

typedef enum system_info_t {
	NOINFO = 0, 		//nothing particular to report
	BOOTING,			//state at startup
	RUNNING,			//state of normal operation
	HWFAIL,				//error when initializing the hardware
	OVERLOAD, 			//latency of the threads is to large
	ERROR = 255
} system_info_t;

typedef enum system_request_t {
	NOREQUEST = 0,
	THREADINFO,
	NEXTTHREADINFO,
	HWINFO
} system_request_t;

/*
	All static members to ensure there is only one copy of the OS!
*/

class MicroOS
{
private:
	static system_info_t			_system_info;
	static uint8_t					_system_request;
	static float					_gpin_float[8];
	static int32_t					_gpin_int[4];
	static uint16_t					_gpin_changed;
	static float					_gpout_float[8];
	static int32_t					_gpout_int[4];
	static uint16_t					_gpout_changed;

	static uint8_t					_thread_count; 
	static uint8_t					_next_thread;	
	static Thread**					_threads;
	
	static HALInterface*			_hal;
	static CommunicatorInterface*	_communicator;
	
	static int findThread(uint8_t ID);
	
public:
	MicroOS();		
	
	static uint8_t init(HALInterface* hal, CommunicatorInterface* communicator, uint8_t config = 0);
	static void start();
	static void run();
	
	static HALInterface*	hal();
	
	static uint8_t addThread(priority_t priority, uint32_t period, int (*Fcn)(), bool start, uint8_t ID=0);
	static uint8_t addThread(Thread *thread);
	static Thread *thread(uint8_t ID);
	//deleteThread();
	
	static int microOSSlowLoop(void);
	static int microOSFastLoop(void);
	//static int stepperControl(void);

	static void sendSystemRequest(uint8_t system_request);
	static void handleSystemRequest(void);
	
	static float getGPinFloat(uint8_t index);
	static int32_t getGPinInt(uint8_t index);
	static bool getGPinFloatChanged(uint8_t index);
	static bool getGPinIntChanged(uint8_t index);
	static void setGPinFloat(uint8_t index, float value);
	static void setGPinInt(uint8_t index, int32_t value);
	static float* getGPoutFloat(void);
	static int32_t* getGPoutInt(void);
	static void setGPoutFloat(uint8_t index, float value);
	static void setGPoutInt(uint8_t index, int32_t value);
};

#endif //MICROOS_H
