/** @file
 *	@brief MAVLink comm protocol testsuite generated from microos_messages.xml
 *	@see http://qgroundcontrol.org/mavlink/
 */
#ifndef MICROOS_MESSAGES_TESTSUITE_H
#define MICROOS_MESSAGES_TESTSUITE_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef MAVLINK_TEST_ALL
#define MAVLINK_TEST_ALL
static void mavlink_test_microos_messages(uint8_t, uint8_t, mavlink_message_t *last_msg);
static void mavlink_test_microos_messages(uint8_t, uint8_t, mavlink_message_t *last_msg);

static void mavlink_test_all(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
	mavlink_test_microos_messages(system_id, component_id, last_msg);
	mavlink_test_microos_messages(system_id, component_id, last_msg);
}
#endif

#include "../microos_messages/testsuite.h"


static void mavlink_test_heartbeat(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
	mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
	mavlink_heartbeat_t packet_in = {
		963497464,
	}963497672,
	}963497880,
	}17859,
	}175,
	}2,
	};
	mavlink_heartbeat_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        	packet1.mode = packet_in.mode;
        	packet1.time = packet_in.time;
        	packet1.event = packet_in.event;
        	packet1.VBat = packet_in.VBat;
        	packet1.type = packet_in.type;
        	packet1.mavlink_version = packet_in.mavlink_version;
        
        

        memset(&packet2, 0, sizeof(packet2));
	mavlink_msg_heartbeat_encode(system_id, component_id, &msg, &packet1);
	mavlink_msg_heartbeat_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
	mavlink_msg_heartbeat_pack(system_id, component_id, &msg , packet1.type , packet1.mode , packet1.time , packet1.VBat , packet1.event );
	mavlink_msg_heartbeat_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
	mavlink_msg_heartbeat_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.type , packet1.mode , packet1.time , packet1.VBat , packet1.event );
	mavlink_msg_heartbeat_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
        	comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
	mavlink_msg_heartbeat_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
	mavlink_msg_heartbeat_send(MAVLINK_COMM_1 , packet1.type , packet1.mode , packet1.time , packet1.VBat , packet1.event );
	mavlink_msg_heartbeat_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
}

static void mavlink_test_robot_id(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
	mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
	mavlink_robot_id_t packet_in = {
		5,
	};
	mavlink_robot_id_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        	packet1.ID = packet_in.ID;
        
        

        memset(&packet2, 0, sizeof(packet2));
	mavlink_msg_robot_id_encode(system_id, component_id, &msg, &packet1);
	mavlink_msg_robot_id_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
	mavlink_msg_robot_id_pack(system_id, component_id, &msg , packet1.ID );
	mavlink_msg_robot_id_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
	mavlink_msg_robot_id_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.ID );
	mavlink_msg_robot_id_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
        	comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
	mavlink_msg_robot_id_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
	mavlink_msg_robot_id_send(MAVLINK_COMM_1 , packet1.ID );
	mavlink_msg_robot_id_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
}

static void mavlink_test_thread_info(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
	mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
	mavlink_thread_info_t packet_in = {
		963497464,
	}963497672,
	}963497880,
	}963498088,
	}18067,
	}18171,
	}65,
	}"VWXYZABCD",
	}34,
	};
	mavlink_thread_info_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        	packet1.time = packet_in.time;
        	packet1.total_duration = packet_in.total_duration;
        	packet1.total_latency = packet_in.total_latency;
        	packet1.number_of_executions = packet_in.number_of_executions;
        	packet1.duration = packet_in.duration;
        	packet1.latency = packet_in.latency;
        	packet1.ID = packet_in.ID;
        	packet1.priority = packet_in.priority;
        
        	mav_array_memcpy(packet1.name, packet_in.name, sizeof(char)*10);
        

        memset(&packet2, 0, sizeof(packet2));
	mavlink_msg_thread_info_encode(system_id, component_id, &msg, &packet1);
	mavlink_msg_thread_info_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
	mavlink_msg_thread_info_pack(system_id, component_id, &msg , packet1.time , packet1.ID , packet1.name , packet1.priority , packet1.duration , packet1.latency , packet1.total_duration , packet1.total_latency , packet1.number_of_executions );
	mavlink_msg_thread_info_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
	mavlink_msg_thread_info_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.time , packet1.ID , packet1.name , packet1.priority , packet1.duration , packet1.latency , packet1.total_duration , packet1.total_latency , packet1.number_of_executions );
	mavlink_msg_thread_info_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
        	comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
	mavlink_msg_thread_info_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
	mavlink_msg_thread_info_send(MAVLINK_COMM_1 , packet1.time , packet1.ID , packet1.name , packet1.priority , packet1.duration , packet1.latency , packet1.total_duration , packet1.total_latency , packet1.number_of_executions );
	mavlink_msg_thread_info_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
}

static void mavlink_test_partition(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
	mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
	mavlink_partition_t packet_in = {
		17235,
	}17339,
	}{ 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48 },
	};
	mavlink_partition_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        	packet1.ID = packet_in.ID;
        	packet1.index = packet_in.index;
        
        	mav_array_memcpy(packet1.value, packet_in.value, sizeof(uint8_t)*32);
        

        memset(&packet2, 0, sizeof(packet2));
	mavlink_msg_partition_encode(system_id, component_id, &msg, &packet1);
	mavlink_msg_partition_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
	mavlink_msg_partition_pack(system_id, component_id, &msg , packet1.ID , packet1.index , packet1.value );
	mavlink_msg_partition_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
	mavlink_msg_partition_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.ID , packet1.index , packet1.value );
	mavlink_msg_partition_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
        	comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
	mavlink_msg_partition_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
	mavlink_msg_partition_send(MAVLINK_COMM_1 , packet1.ID , packet1.index , packet1.value );
	mavlink_msg_partition_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
}

static void mavlink_test_gpio(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
	mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
	mavlink_gpio_t packet_in = {
		963497464,
	}{ 45.0, 46.0, 47.0, 48.0, 49.0, 50.0, 51.0, 52.0 },
	}{ 963499336, 963499337, 963499338, 963499339 },
	}161,
	};
	mavlink_gpio_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        	packet1.time = packet_in.time;
        	packet1.ID = packet_in.ID;
        
        	mav_array_memcpy(packet1.gpio_float, packet_in.gpio_float, sizeof(float)*8);
        	mav_array_memcpy(packet1.gpio_int, packet_in.gpio_int, sizeof(int32_t)*4);
        

        memset(&packet2, 0, sizeof(packet2));
	mavlink_msg_gpio_encode(system_id, component_id, &msg, &packet1);
	mavlink_msg_gpio_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
	mavlink_msg_gpio_pack(system_id, component_id, &msg , packet1.time , packet1.ID , packet1.gpio_float , packet1.gpio_int );
	mavlink_msg_gpio_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
	mavlink_msg_gpio_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.time , packet1.ID , packet1.gpio_float , packet1.gpio_int );
	mavlink_msg_gpio_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
        	comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
	mavlink_msg_gpio_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
	mavlink_msg_gpio_send(MAVLINK_COMM_1 , packet1.time , packet1.ID , packet1.gpio_float , packet1.gpio_int );
	mavlink_msg_gpio_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
}

static void mavlink_test_microos_messages(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
	mavlink_test_heartbeat(system_id, component_id, last_msg);
	mavlink_test_robot_id(system_id, component_id, last_msg);
	mavlink_test_thread_info(system_id, component_id, last_msg);
	mavlink_test_partition(system_id, component_id, last_msg);
	mavlink_test_gpio(system_id, component_id, last_msg);
}

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // MICROOS_MESSAGES_TESTSUITE_H
