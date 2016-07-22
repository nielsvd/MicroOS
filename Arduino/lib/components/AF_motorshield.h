// Adafruit Motor shield library - stripped down
// copyright Adafruit Industries LLC, 2009
// this code is public domain, enjoy!

#ifndef _AFMotor_h_
#define _AFMotor_h_

#include <inttypes.h>
#include <avr/io.h>

#define MOTOR12_1KHZ 0x03 //_BV(CS22)
#define MOTOR34_1KHZ 0x03 //_BV(CS01) | _BV(CS00)

// define PWM frequencies to use
#define FREQ_MOTOR1 MOTOR12_1KHZ
#define FREQ_MOTOR2 MOTOR12_1KHZ
#define FREQ_MOTOR3 MOTOR34_1KHZ
#define FREQ_MOTOR4 MOTOR34_1KHZ

// bit positions in the 74HCT595 shift register output
#define MOTOR1_A 2
#define MOTOR1_B 3
#define MOTOR2_A 1
#define MOTOR2_B 4
#define MOTOR4_A 0
#define MOTOR4_B 6
#define MOTOR3_A 5
#define MOTOR3_B 7

// constants that the user passes in to the motor calls
#define FORWARD 1
#define BACKWARD 2
#define BRAKE 3   // TODO: check this
#define RELEASE 4

// Arduino pin names for interface to 74HCT595 latch
#define MOTORLATCH 12
#define MOTORCLK 4
#define MOTORENABLE 7
#define MOTORDATA 8

class AFMotorController {
  public:
    AFMotorController(void);
    void enable(void);
    friend class AF_DCMotor;
    void latch_tx(void);
    uint8_t TimerInitalized;
};

class AF_DCMotor {
  public:
    AF_DCMotor(uint8_t motornum);
    void run(uint8_t);
    void setSpeed(uint8_t);

  private:
    uint8_t motornum;
};

uint8_t getlatchstate(void);

#endif
