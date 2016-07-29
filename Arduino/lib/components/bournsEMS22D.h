#ifndef BOURNSEMS22D_H
#define BOURNSEMS22D_H

#include "sensor1D.h"

class BournsEMS22D : public Sensor1D {
  public:
    BournsEMS22D(uint8_t ID = 0);
    virtual ~BournsEMS22D();
    bool init();
    int readRawValue();
    float readCalibratedValue();
  private:
    static void pulse_callback();
    void setup_pins();
    void setup_interrupt();
    bool init_;
};

#endif /* BOURNSEMS22D_H */
