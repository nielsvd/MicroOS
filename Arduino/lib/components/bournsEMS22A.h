#ifndef BOURNSEMS22A_H
#define BOURNSEMS22A_H

#include "sensor1D.h"

class BournsEMS22A : public Sensor1D {
  public:
    BournsEMS22A(uint8_t ID = 0);
    bool init();
    int readRawValue();
    float readCalibratedValue();
  private:
    char get_status1();
    char get_status2();
    char get_status3();
    char get_status4();
    char get_status5();
    bool get_data_validity();
    void setup_timer();
    void setup_pins();
};

#endif /* BOURNSEMS22A_H */
