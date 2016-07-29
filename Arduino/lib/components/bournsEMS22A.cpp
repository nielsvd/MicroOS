/*
 * BOURNS EMS22A50-B28-LS6 encoder driver
 *  by Niels van Duijkeren
 *  2016-07-28
 *
 * WORKS FOR ONLY ONE SENSOR AT THE MOMENT
 */

#include "bournsEMS22A.h"
#include "Arduino.h"

#define BOURNS_PIN_DI 22
#define BOURNS_PIN_CLK 24
#define BOURNS_PIN_DO 26
#define BOURNS_PIN_CS 28

static volatile unsigned short int sensor_d;          /* Absolute angular position data */
static volatile unsigned char      sensor_s1;         /* End of offset compensation algorithm */
static volatile unsigned char      sensor_s2;         /* Cordic overflow indicating an error in cordic plant */
static volatile unsigned char      sensor_s3;         /* Linearity alarm */
static volatile unsigned char      sensor_s4;         /* Increase in magnitude */
static volatile unsigned char      sensor_s5;         /* Decrease in magnitude */
static volatile bool               sensor_data_valid; /* Data validity based on parity bit */

BournsEMS22A::BournsEMS22A(uint8_t ID) : Sensor1D(ID) {

}

void BournsEMS22A::setup_timer() {

  cli();//stop interrupts

  //set timer0 interrupt at 2 MHz
  TCCR3A = 0;// set entire TCCR3A register to 0
  TCCR3B = 0;// same for TCCR3B
  TCNT3  = 0;//initialize counter value to 0
  // set compare match register for 1kHz increments
  OCR3A = 15;// = (16*10^6) / (2*10^6*1) - 1 (must be <2^8)
  // turn on CTC mode
  TCCR3B |= (1 << WGM32);
  // Set CCS30 and CS32 bits for 1024 prescaler
  TCCR3B |= (1 << CS30) | (1 << CS32);
  // enable timer compare interrupt
  TIMSK3 |= (1 << OCIE3A);

  sei();//allow interrupts
}

void BournsEMS22A::setup_pins() {
  pinMode(BOURNS_PIN_DI,  OUTPUT);
  pinMode(BOURNS_PIN_CLK, OUTPUT);
  pinMode(BOURNS_PIN_DO,   INPUT);
  pinMode(BOURNS_PIN_CS,  OUTPUT);
}

bool BournsEMS22A::init() {
  setup_timer();
  setup_pins();
  return true;
}

int BournsEMS22A::readRawValue() {
  return sensor_d;
}

int BournsEMS22A::get_status1() {
  return sensor_s1;
}

int BournsEMS22A::get_status2() {
  return sensor_s2;
}

int BournsEMS22A::get_status3() {
  return sensor_s3;
}

int BournsEMS22A::get_status4() {
  return sensor_s4;
}

int BournsEMS22A::get_status5() {
  return sensor_s5;
}

bool BournsEMS22A::get_data_validity() {
  return sensor_data_valid;
}

/*
 *
 * TIMER INTERRUPT BASED COMMUNICATION PROTOCOL
 *
 */

void interpret_data(short int data) {
  // unsigned short int sensor_d_mirror;
  int i;
  sensor_data_valid = ((__builtin_popcount(data)%2)==0);
  if (sensor_data_valid) {
    /* Interpret message */
    sensor_d = data & 0x3FF;
    sensor_s1 = (data & 0x400) >> 10;
    sensor_s2 = (data & 0x800) >> 11;
    sensor_s3 = (data & 0x1000) >> 12;
    sensor_s4 = (data & 0x2000) >> 13;
    sensor_s5 = (data & 0x4000) >> 14;
  }
}

typedef enum protocol_state_ {
  CS,
  WAIT_AFTER_CS,
  DATA,
  WAIT_AFTER_DATA,
} protocol_state;
static protocol_state state = CS;
static short int counter = 0;
static short int temp_data = 0;
// timer compare interrupt service routine
ISR(TIMER3_COMPA_vect) {
  switch(state) {
    case CS:
      /* Set CS high */
      digitalWrite(BOURNS_PIN_CS,HIGH);
      state = WAIT_AFTER_CS;
      break;
    case WAIT_AFTER_CS:
      /* Set CS low */
      digitalWrite(BOURNS_PIN_CS,LOW);
      state = DATA;
      counter = 0;
      break;
    case DATA:
      if(counter<34) {
        /* For even values of counter: set CLK low and read data,
         *  for odd values of counter: CLK high.
         */
        if(counter%2==0) {
          /* Set CLK low */
          digitalWrite(BOURNS_PIN_CLK,LOW);
          if(counter>0) {
            /* Read data */
            short int data_in = digitalRead(BOURNS_PIN_DO) == HIGH ? 1 : 0;
            /* Write input to temporary data. */
            temp_data |= (data_in << counter/2);
          }
        } else {
          /* Set CLK high */
          digitalWrite(BOURNS_PIN_CLK,HIGH);
        }
      } else {
        state = WAIT_AFTER_DATA;
      }
      break;
    case WAIT_AFTER_DATA:
      /* Write data to encoder class */
      interpret_data(temp_data);
      state = CS;
      break;
  }
  counter++;
}
