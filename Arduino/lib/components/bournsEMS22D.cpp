#include "bournsEMS22D.h"
#include "Arduino.h"
#include <math.h>

#define BOURNS_PIN_PULSE 21
#define BOURNS_PIN_DIRECTION 30
#define BOURNS_PIN_INDEX 32

static volatile int pos;

BournsEMS22D::BournsEMS22D(uint8_t ID) : Sensor1D(ID) {
  init_=false;
}

BournsEMS22D::~BournsEMS22D() {
  if(init_) detachInterrupt(BOURNS_PIN_PULSE);
}

void BournsEMS22D::pulse_callback() {
  pos += digitalRead(BOURNS_PIN_DIRECTION) == HIGH ? -1 : 1;
}

void BournsEMS22D::setup_pins() {
  pinMode(BOURNS_PIN_PULSE, INPUT);
  pinMode(BOURNS_PIN_DIRECTION, INPUT);
  pinMode(BOURNS_PIN_INDEX, INPUT);
}

void BournsEMS22D::setup_interrupt() {
  attachInterrupt(digitalPinToInterrupt(BOURNS_PIN_PULSE), BournsEMS22D::pulse_callback, CHANGE);
}

bool BournsEMS22D::init() {
  if(!init_) {
    setup_pins();
    setup_interrupt();
    init_=true;
  }
  pos=0;
}

int BournsEMS22D::readRawValue() {
  return pos;
}

float BournsEMS22D::readCalibratedValue() {
  return pos/1024.f*2*M_PI;
}
