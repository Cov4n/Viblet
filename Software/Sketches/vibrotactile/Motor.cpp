#include "Arduino.h"
#include "Motor.h"

Motor::Motor(int mPin) {
  motorPin = mPin;
  pinMode(motorPin, OUTPUT);
}

int Motor::getMotorSteps() {
  return motorSteps;
}

void Motor::setMotorSteps(int mSteps) {
  motorSteps = mSteps;
}

int Motor::getMotorSpeed() {
  return motorSpeed;
}

void Motor::setMotorSpeed(int mSpeed) {
  motorSpeed = mSpeed;
}

void Motor::startVibration() {
  analogWrite(motorPin, 255);
}

void Motor::startVibration(int mSpeed) {
  analogWrite(motorPin,mSpeed);
}

void Motor::stopVibration() {
  analogWrite(motorPin, 0);
}


