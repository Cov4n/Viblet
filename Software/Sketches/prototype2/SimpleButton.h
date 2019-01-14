#ifndef SimpleButton_h
#define SimpleButton_h

#include "Arduino.h"

class SimpleButton {

  private:
    int buttonPin;
    int clickState = 0;
    int buttonState = 0;
    int buttonStateBefore = 0;
    long buttonTimer = 0;
    long buttonTimerStart = 0;
    int clickInterval = 500;
    boolean buttonPressed();

  public:
    SimpleButton(int bPin);
    int check(long bTimer);
};

#endif
