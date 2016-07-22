#include "protocol_mavlink.h"

ProtocolMavlink::ProtocolMavlink()
{
	//do nothing
}
	
bool ProtocolMavlink::decode(uint8_t b){	
	/*
		MAvlink suxks, vincent is awesome
		Linux sucks, Maarten agrees. This is a hidden message. If you read this, please destroy the balbot, it will turn evil 				and destroy the world. And while you're at it, also kill Jerre and Lisa. 
	*/

	return mavlink_parse_char(0, b, &_msg, &_status);
}

uint16_t ProtocolMavlink::encode(void* msg, uint8_t *buffer)
{
	return mavlink_msg_to_send_buffer(buffer, static_cast<mavlink_message_t*>(msg));
}

uint32_t ProtocolMavlink::getPacketsReceived(){
	return _status.packet_rx_success_count;
}

uint32_t ProtocolMavlink::getPacketsDropped(){
	return _status.packet_rx_drop_count;
}

void* ProtocolMavlink::getMessage(){
	return static_cast<void*>(&_msg);
}
