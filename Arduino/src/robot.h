#ifndef ROBOT_H
#define ROBOT_H

//!  ROBOT Class
/*!
  Class incorporating the robot. This class is used to define state machines, control algorithms, sensor readings,...
  It should be interfaced with the communicator to send data to the world.
*/

#include <inttypes.h>
#include "math.h"

#include "microOS.h"
#include "encoder_sensor.h"
#include "difference_sensor.h"
#include "sharp41S.h"
// #include "AF_motorshield.h"  // Adafruit motor shield v1 library
#include "led.h"
#include "bournsEMS22A.h"
#include "vishay157.h"

class Robot {
  private:
    uint8_t _ID;			//give the robot an ID so that you can recognize it

    // Give the robot some sensors and motors
    // Sensor1D* _encoder1;
    // Sensor1D* _encoder2;
    // Sensor1D* _speed1;
    // Sensor1D* _speed2;
    // Sensor1D* _distance1;
    // Sensor1D* _distance2;
    LED* _LED1;
    LED* _LED2;
    Sensor1D* _pendulum_potmeter;
    Sensor1D* _pendulum_encoder;

    // Kalman variables
    // float p11,p12,p13,p21,p22,p23,p31,p32,p33;
    // float x1,x2,x3;

    // motor instances
    // AF_DCMotor motor_FL, motor_FR, motor_RL, motor_RR;

    // dummys
    int16_t cnt;
    // float motor_FL_speed_fltr_mem[10] = {0.0f};
    // float motor_FR_speed_fltr_mem[10] = {0.0f};
    // float fir_mavg_coeff = 0.1f;
    // float motor_FL_speed_PID_int = 0.0f;
    // float motor_FR_speed_PID_int = 0.0f;
    // float motor_FL_speed_err_old = 0.0f;
    // float motor_FR_speed_err_old = 0.0f;

    // void lightMyFire(LED* led, );

  public:
    Robot(uint8_t ID = 0);

    ////////
    /// FUNC
    void init();			//set up the robot
    void controllerHook();	//update function which can be executed continuously

    ///////
    /// SET
    void setID(uint8_t ID);

    ///////
    /// GET
    uint8_t ID();

};

#endif //ROBOT_H
