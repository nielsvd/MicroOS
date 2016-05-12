#include "simple_protocol.h"

SimpleProtocol::SimpleProtocol():
	_packets_received(0),
	_decoding_counter(0)
{
	_msg.msg = _message_buffer;
}


bool SimpleProtocol::decode(uint8_t b)
{
	if(_decoding_counter == 0){ //start decoding a new message
		_msg.length = b;
	} else if(_decoding_counter == 1) { //fill out the msgid
		_msg.msgid = b;
	} else { //fill out the message buffer
		_msg.msg[_decoding_counter - 2] = b; 
	}
	
	_decoding_counter++;
	if(_decoding_counter == _msg.length){
		_decoding_counter = 0;
		_packets_received++;
		return true;
	} else {
		return false;
	}
}

uint16_t SimpleProtocol::encode(void* msg, uint8_t* buffer)
{
	simple_message_t* simple_message = static_cast<simple_message_t*>(msg);
	memcpy(buffer, (uint8_t*)simple_message, simple_message->length);
}

uint32_t SimpleProtocol::getPacketsReceived()
{
	return _packets_received;
}

void* SimpleProtocol::getMessage()
{
	return &_msg;
}
