#ifndef COMMUNICATOR_INTERFACE_H
#define COMMUNICATOR_INTERFACE_H

#include <inttypes.h>

class CommunicatorInterface
{	
public:
	virtual void init();

	virtual void receive();

	virtual void sendHeartbeat();
	virtual void sendThreadInfo(uint8_t ID, char* name, uint8_t priority,
							 	uint32_t duration, uint32_t latency, 
							 	uint32_t total_duration, uint32_t total_latency, uint32_t number_of_executions);
	virtual void sendGPIO();
	//virtual void sendHwinfo()
};

#endif //COMMUNICATOR_INTERFACE_H
