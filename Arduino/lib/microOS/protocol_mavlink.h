#ifndef PROTOCOL_MAVLINK_H
#define PROTOCOL_MAVLINK_H

#include "protocol_interface.h"
#include "mavlink.h"

class ProtocolMavlink : public ProtocolInterface
{
private:
	mavlink_message_t	_msg;
	mavlink_status_t	_status;

public:
	ProtocolMavlink();
	
	bool decode(uint8_t b);
	uint16_t encode(void* msg, uint8_t *buffer);
	
	uint32_t getPacketsReceived();
	uint32_t getPacketsDropped();
	
	void* getMessage();
};

#endif //PROTOCOL_MAVLINK_H
