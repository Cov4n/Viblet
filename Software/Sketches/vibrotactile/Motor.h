#ifndef Motor_h
#define Motor_h

#include "Arduino.h"

class Motor {
  
  private:
    int motorPin;
    int motorSteps = 10;
    int motorSpeed = 255;
    
  public:
    Motor(int pin);
    int getMotorSteps();
    void setMotorSteps(int mSteps);
    int getMotorSpeed();
    void setMotorSpeed(int mSpeed);
    void startVibration();
    void startVibration(int mSpeed);
    void stopVibration();

};

#endif
