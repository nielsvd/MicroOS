#ifndef BOURNSEMS22A_H
#define BOURNSEMS22A_H

#include "sensor1D.h"

class BournsEMS22A : public Sensor1D {
  public:
    BournsEMS22A(uint8_t ID = 0);
    bool init();
    int readRawValue();
    int get_status1();
    int get_status2();
    int get_status3();
    int get_status4();
    int get_status5();
    bool get_data_validity();
  private:
    void setup_timer();
    void setup_pins();
};

#endif /* BOURNSEMS22A_H */
