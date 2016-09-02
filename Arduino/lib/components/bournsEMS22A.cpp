/*
 * BOURNS EMS22A50-B28-LS6 encoder driver
 *  by Niels van Duijkeren
 *  2016-07-28
 *
 * WORKS FOR ONLY ONE SENSOR AT THE MOMENT
 */

#include "bournsEMS22A.h"
#include "Arduino.h"
#include <math.h>

#define BOURNS_PIN_DI 22
#define BOURNS_PIN_CLK 24
#define BOURNS_PIN_DO 26
#define BOURNS_PIN_CS 28

static volatile short int sensor_d;          /* Absolute angular position data */
static volatile short int sensor_s1;         /* End of offset compensation algorithm */
static volatile short int sensor_s2;         /* Cordic overflow indicating an error in cordic plant */
static volatile short int sensor_s3;         /* Linearity alarm */
static volatile short int sensor_s4;         /* Increase in magnitude */
static volatile short int sensor_s5;         /* Decrease in magnitude */
static volatile bool      sensor_data_valid; /* Data validity based on parity bit */

BournsEMS22A::BournsEMS22A(uint8_t ID) : Sensor1D(ID) {

}

void BournsEMS22A::setup_timer() {

  cli();//stop interrupts

  //set timer0 interrupt at 2 MHz
  TCCR3A = 0;// set entire TCCR3A register to 0
  TCCR3B = 0;// same for TCCR3B
  TCNT3  = 0;//initialize counter value to 0
  // set compare match register for 1kHz increments
  OCR3A = 1;// = (16*10^6) / (2*10^6*1) - 1 (must be <2^8)
  // turn on CTC mode
  TCCR3B |= (1 << WGM32);
  // Set CS32 bits for 256 prescaler
  TCCR3B |= (1 << CS32);
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

float BournsEMS22A::readCalibratedValue() {
  return sensor_d/1023.f*2*M_PI;
}

char BournsEMS22A::get_status1() {
  return sensor_s1;
}

char BournsEMS22A::get_status2() {
  return sensor_s2;
}

char BournsEMS22A::get_status3() {
  return sensor_s3;
}

char BournsEMS22A::get_status4() {
  return sensor_s4;
}

char BournsEMS22A::get_status5() {
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

short int swap_10_bits(short int data) {
  char i;
  short int output = 0;
  for(i=0;i<5;++i) {
    output |= (data & (1<<(9-i))) >> (9-i*2);
    output |= (data & (1<<i)    ) << (9-i*2);
  }
  return output;
}

bool interpret_data(short int data) {
  sensor_data_valid = ((__builtin_popcount(data)%2)==0);
  if (sensor_data_valid) {
    /* Interpret message */
    sensor_d = swap_10_bits(data & 0x3FF);
    sensor_s1 = (data & 0x400) >> 10;
    sensor_s2 = (data & 0x800) >> 11;
    sensor_s3 = (data & 0x1000) >> 12;
    sensor_s4 = (data & 0x2000) >> 13;
    sensor_s5 = (data & 0x4000) >> 14;
  }
  return sensor_data_valid;
}

typedef enum protocol_state_ {
  CS,
  WAIT_AFTER_CS,
  DATA,
  WAIT_AFTER_DATA,
  IDLE
} protocol_state;
static protocol_state state = CS;
static short int counter = 0;
static int idle_counter = 0;
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
      temp_data = 0;
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
            short int data_in = digitalRead(BOURNS_PIN_DO);
            /* Write input to temporary data. */
            temp_data |= (data_in << (counter/2-1));
          }
        } else {
          /* Set CLK high */
          digitalWrite(BOURNS_PIN_CLK,HIGH);
        }
      } else {
        state = WAIT_AFTER_DATA;
      }
      counter++;
      break;
    case WAIT_AFTER_DATA:
      /* Write data to encoder class */
      interpret_data(temp_data);
      state = IDLE;
      break;
    case IDLE:
      // Update data at 200 Hz (non-guaranteed success)
      if (idle_counter++ > 118) {
        idle_counter = 0;
        state = CS;
      }
      break;
  }
}
