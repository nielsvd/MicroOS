#ifndef COMMUNICATOR_H
#define COMMUNICATOR_H

#include "microOS.h"
#include "communicator_interface.h"
#include "communicator_config.h"
#include "channel.h"
#include "protocol_mavlink.h"
#include "robot.h"

class Communicator : public CommunicatorInterface
{
private:
	Channel* _channels[NUM_COMM_PORTS];
	Robot* _robot;
	
	void inline action(mavlink_message_t &msg);
	
public:
	Communicator(Robot* robot);

	void init();

	void receive();
	void transmit(mavlink_message_t &msg);
	//void transmit(uint8_t msgID);
	
	void sendHeartbeat();
	void sendThreadInfo(uint8_t ID, char* name, uint8_t priority,
						uint32_t duration, uint32_t latency, 
						uint32_t total_duration, uint32_t total_latency, uint32_t number_of_executions);
	void sendGPIO();
};

#endif //COMMUNICATOR_H
