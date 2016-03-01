#ifndef SIMPLE_PROTOCOL_H
#define SIMPLE_PROTOCOL_H

#include "protocol_interface.h"

typedef struct simple_message_t {
	uint8_t length;
	uint8_t msgid;
	uint8_t* msg;
} simple_message_t;

class SimpleProtocol : public ProtocolInterface
{
private:
	uint32_t _packets_received;
	uint8_t _decoding_counter;
	uint8_t _message_buffer[128];
	simple_message_t _msg;

public:
	SimpleProtocol();

	bool decode(uint8_t b);
    uint16_t encode(void* msg, uint8_t* buffer);
    
    uint32_t getPacketsReceived();
    uint32_t getPacketsDropped(){ return 0; };
    void* getMessage();
};

#endif //SIMPLE_PROTOCOL_H
