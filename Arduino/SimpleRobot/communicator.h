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
	uint8_t _transmission_msg;
	uint8_t _log_msg;
	const uint8_t _update_messages[NUM_UPDATE_MESSAGES];

	Robot* _robot;
	
	int inline action(mavlink_message_t msg);

public:
	Communicator(Robot* robot);

	int start();
	int update();
	
	int receive();
	int transmit(uint8_t msgID);
	
	void sendThreadInfo(uint8_t ID, char* name, uint8_t priority,
						uint32_t duration, uint32_t latency, 
						uint32_t total_duration, uint32_t total_latency, uint32_t number_of_executions);
};

#endif //COMMUNICATOR_H
