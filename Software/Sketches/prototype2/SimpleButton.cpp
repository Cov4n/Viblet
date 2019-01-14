#include "Arduino.h"
#include "SimpleButton.h"

SimpleButton::SimpleButton(int bPin) {
  buttonPin = bPin;
  pinMode(bPin, INPUT);
}

boolean SimpleButton::buttonPressed() {
  boolean pressed = false;
  buttonState = digitalRead(buttonPin);
  if (buttonState == 1 && buttonStateBefore == 0) {
    pressed = true;
  }
  buttonStateBefore = buttonState;
  return pressed;  
}

int SimpleButton::check(long bTimer) {
  buttonTimer = bTimer;
  if (buttonPressed() == true) {
    if (buttonTimer > buttonTimerStart + clickInterval) {
      buttonTimerStart = buttonTimer;
      clickState = 1;
    } else if (buttonTimer <= buttonTimerStart + clickInterval) {
      clickState = 2; 
    } 
  }
  if (buttonTimer > buttonTimerStart + clickInterval) {
    int cState = clickState;
    clickState = 0;
    return cState;
  } else {
    return 0;
  }
}

