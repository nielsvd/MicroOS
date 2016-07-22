#include "robot.h"

#define ENC1_PINA	20
#define ENC1_PINB	22
#define ENC2_PINA	21
#define ENC2_PINB	23
#define DIST1_PIN	0
#define DIST2_PIN	0
#define MOTOR_FL  4
#define MOTOR_FR  3
#define MOTOR_RL  1
#define MOTOR_RR  2
#define LED1_PIN 34
#define LED2_PIN 40


Robot::Robot(uint8_t ID):
  _ID(ID),
  // _encoder1(new EncoderSensor(ENC1_PINA,ENC1_PINB)),
  // _encoder2(new EncoderSensor(ENC2_PINA,ENC2_PINB)),
  // _speed1(new DifferenceSensor(_encoder1)),
  // _speed2(new DifferenceSensor(_encoder2)),
  // _distance1(new Sharp41S(DIST1_PIN)),
  // _distance2(new Sharp41S(DIST2_PIN)),
  // p11(0.0f),p12(0.0f),p13(0.0f),p21(0.0f),p22(0.0f),p23(0.0f),p31(0.0f),p32(0.0f),p33(0.0f),
  // x1(0.0f),x2(0.0f),x3(0.0f),
  // motor_FL(MOTOR_FL), motor_FR(MOTOR_FR), motor_RL(MOTOR_RL), motor_RR(MOTOR_RR),
  _LED1(new LED(LED1_PIN)), _LED2(new LED(LED2_PIN)),
  cnt(0)
  {
    // do something else
  }

void Robot::init(){
  //initialize the robot - sort of starting procedure

  _LED1->init();
  _LED2->init();

}

void Robot::controllerHook() {

  // _LED1->ledOn();
  // _LED1->update();

  cnt += 1;
  if(cnt > 100) {
    cnt = 0;
    _LED1->toggle();
  }

}

void Robot::setID(uint8_t ID) { _ID = ID; }

uint8_t Robot::ID() { return _ID; }
