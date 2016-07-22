#include "communicator.h"

Communicator::Communicator(Robot* robot) :
	_robot(robot)
{
	_channels[0] = new Channel(&COMM_PORTA, new ProtocolMavlink);
}

void Communicator::init()
{
	COMM_PORTA.begin(COMM_PORTA_BAUD);
	_channels[0]->start();
}

void Communicator::receive()
{
	for(uint8_t k=0; k<NUM_COMM_PORTS ; k++){
		while(_channels[k]->receive()){
			action(*reinterpret_cast<mavlink_message_t*>(_channels[k]->getMessage()));
		}
	}
}

void Communicator::transmit(mavlink_message_t &msg)
{
	_channels[0]->send(&msg);
}

/*void Communicator::transmit(uint8_t msgID)
{
	// try to send the requested message
	// if the message is not found, we cancel the send request
    switch(msgID){
	    case MAVLINK_MSG_ID_HEARTBEAT:
		    sendHeartbeat();
		    break;
		case MAVLINK_MSG_ID_GPIO:
			sendGPIO();
			break;
	}
	
	return true;
}*/

void Communicator::sendHeartbeat()
{
	mavlink_message_t msg;
	mavlink_msg_heartbeat_pack( _robot->ID(), _robot->ID(), &msg, 0, millis(), 0, 0, 0);
	
	transmit(msg);
}

void Communicator::sendThreadInfo(uint8_t ID, char* name, uint8_t priority, uint32_t duration, uint32_t latency, uint32_t total_duration, uint32_t total_latency, uint32_t number_of_executions)
{
	mavlink_message_t msg;
	mavlink_msg_thread_info_pack(_robot->ID(), _robot->ID(), &msg, millis(), ID, name, priority, duration, latency, total_duration, total_latency, number_of_executions);
	
	transmit(msg);
}

void Communicator::sendGPIO()
{
	mavlink_message_t msg;
	mavlink_msg_gpio_pack(_robot->ID(), _robot->ID(), &msg,  millis(), 0, MicroOS::getGPoutFloat(), MicroOS::getGPoutInt());
	
	transmit(msg);
}

inline void Communicator::action(mavlink_message_t &msg)
{
	switch(msg.msgid){
		case MAVLINK_MSG_ID_THREAD_INFO:{
			MicroOS::sendSystemRequest(THREADINFO);
			break; }
			
		case MAVLINK_MSG_ID_GPIO:{
			mavlink_gpio_t gpio;
			mavlink_msg_gpio_decode(&msg,&gpio);
		
			for(uint8_t k=0;k<4;k++){
				MicroOS::setGPinInt(k,gpio.gpio_int[k]);
				MicroOS::setGPinFloat(k,gpio.gpio_float[k]);
				MicroOS::setGPinFloat(k+4,gpio.gpio_float[k+4]);
			}
			break;}
			
		/*case INTERESTING_MSG_ID:{
			//do something with the message
			break; }
			
		case LESS_INTERESTING_MSG_ID:{
			//do something with the message
			break; }*/
	}
}
