#ifndef THREADING_H
#define THREADING_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#define THREAD_NAME_SIZE				10
//#define AUTO_SCHEDULE_THREADING //do this only if you have a good reason, because thread update frequencies will be off

typedef enum priority_t{	
	LOWEST,
	BELOWNORMAL,
	NORMAL,
	ABOVENORMAL,
	HIGHEST
} priority_t;

class Thread
{
private:
	bool 			_active;
	uint8_t 		_ID;
	char 			_name[THREAD_NAME_SIZE];
	
	priority_t 		_priority;
	uint32_t 		_period;
	uint32_t	    _scheduling_time;
	uint32_t		_duration;
	uint32_t		_latency;
	uint32_t		_total_duration;
	uint32_t		_total_latency;
	uint32_t		_number_of_executions;
		
	int				(*_pFcn)();
	
public:
	Thread();
	Thread(priority_t priority, uint32_t period, int (*Fcn)(), char* name = "");

	//void operator=(const Thread t);

	bool start();
	bool idle();
	bool sleep();
	bool action();
	
	void setID(uint8_t ID);
	
	uint8_t getID();
	char* getName();
	priority_t getPriority();
	uint32_t getWakeupTime();
	uint32_t getDuration();
	uint32_t getLatency();
	uint32_t getTotalDuration();
	uint32_t getTotalLatency();
	uint32_t getNumberOfExecutions();
	uint32_t getAverageDuration();
	uint32_t getAverageLatency();
};

#endif //THREADING_H