#include <Arduino.h>
#include <SPI.h>
#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_SPI.h"
#include "Adafruit_BluefruitLE_UART.h"
#include "BluefruitConfig.h"

#include <Adafruit_NeoPixel.h>
#include "Motor.h"
#include "SimpleButton.h"

#if SOFTWARE_SERIAL_AVAILABLE
  #include <SoftwareSerial.h>
#endif

#define FACTORYRESET_ENABLE         1
#define MINIMUM_FIRMWARE_VERSION    "0.6.6"
#define MODE_LED_BEHAVIOUR          "MODE"

int pinButtonEmergency = 6;
int pinButtonAcknowledge = 9;
int pinLed = 10;
int pinMotor1 = 11;
int pinMotor2 = 12;
int pinMotor3 = 13;

Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_CS, BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);

Motor motor1 = Motor(pinMotor1);
Motor motor2 = Motor(pinMotor2);
Motor motor3 = Motor(pinMotor3);

SimpleButton buttonAcknowledge = SimpleButton(pinButtonAcknowledge);
SimpleButton buttonEmergency = SimpleButton(pinButtonEmergency);

Adafruit_NeoPixel led = Adafruit_NeoPixel(1, pinLed, NEO_GRB + NEO_KHZ800);
boolean ledStatus = false;
long ledTimerEnd = 0;

String bleInput = "0";
long motorTimerStart = 0;
long motorTimer = 0;

void error(const __FlashStringHelper*err) {
  while (1);
}

void setup() {

  led.begin();

  Serial.begin(115200);

  /* Initialise the module */
  if ( !ble.begin(VERBOSE_MODE) )
  {
    error(F("Couldn't find Bluefruit, make sure it's in CoMmanD mode & check wiring?"));
  }
  if ( FACTORYRESET_ENABLE )
  {
    /* Perform a factory reset to make sure everything is in a known state */
    if ( ! ble.factoryReset() ){
      error(F("Couldn't factory reset"));
    }
  }
  stopAllVibration();
  /* Disable command echo from Bluefruit */
  ble.echo(false);
  ble.verbose(false);
  /* Wait for connection */
  while (! ble.isConnected()) {
      delay(500);
  }

  // LED Activity command is only supported from 0.6.6
  if ( ble.isVersionAtLeast(MINIMUM_FIRMWARE_VERSION) )
  {
    // Change Mode LED Activity
    ble.sendCommandCheckOK("AT+HWModeLED=" MODE_LED_BEHAVIOUR);
  }
  // Set module to DATA mode
  ble.setMode(BLUEFRUIT_MODE_DATA);

  motorTimerStart = millis();
}

void loop() {
  
  receiveData();
  if (!bleInput.equals("0")) {
    vibrationPattern(bleInput.substring(0,2), millis());
  }
  
  int clickButtonAcknowledge = buttonAcknowledge.check(millis());  
  if (clickButtonAcknowledge == 1) { //Einfacher Klick
    bleInput="00";
    if (ledStatus == true) {
      led.setPixelColor(0, led.Color(0, 204, 0));
      led.show();
      ledTimerEnd = millis() + 1500;
    } else {
      led.setPixelColor(0, led.Color(0, 0, 0));
      led.show();
    }
  } else if (clickButtonAcknowledge == 2) { //Doppelklick
    bleInput="00";
    if (ledStatus == true) {
      led.setPixelColor(0, led.Color(255, 215, 0));
      led.show();
      ledTimerEnd = millis() + 1500;
    } else {
      led.setPixelColor(0, led.Color(0, 0, 0));
      led.show();
    }

  }

  int clickButtonEmergency = buttonEmergency.check(millis());
  if (clickButtonEmergency > 0) { // Einfacher Klick
    bleInput="00";
    if (ledStatus == true) {
      led.setPixelColor(0, led.Color(255, 0, 0));
      led.show();
      ledTimerEnd = millis() + 1500;
    } else {
      led.setPixelColor(0, led.Color(0, 0, 0));
      led.show();
    }
  }

  if (millis() > ledTimerEnd) {
    led.setPixelColor(0, led.Color(0, 0, 0));
    led.show();
  }

}

void receiveData() {
  int inCount = 0;
  while (ble.available()) {
    if (inCount == 0) {
      bleInput="";
    }
    inCount++;
    int in = ble.read();
    bleInput += (char) in;
  }
  bleInput.trim();
  if (bleInput.substring(2) == "1") {
    ledStatus = true;
  } else {
    ledStatus = false;
  }
}

void vibrationPattern(String pattern, long motorTimer) {
  if (pattern.equals("00")) {
    stopAllVibration();
  } else if (pattern.equals("11")) {
    if (motorTimer <= motorTimerStart + 400) {
      motor1.startVibration();
    } else if (motorTimer > motorTimerStart + 400 && motorTimer <= motorTimerStart + 600) {
      motor1.stopVibration();
    } else if (motorTimer > motorTimerStart + 600 && motorTimer <= motorTimerStart + 1000) {
      motor2.startVibration();
    } else if (motorTimer > motorTimerStart + 1000 && motorTimer <= motorTimerStart + 1200) {
      motor2.stopVibration();
    } else if (motorTimer > motorTimerStart + 1200 && motorTimer <= motorTimerStart + 1600) {
      motor3.startVibration();
    } else if (motorTimer > motorTimerStart + 1600 && motorTimer <= motorTimerStart + 1800) {
      motor3.stopVibration();     
    } else if (motorTimer > motorTimerStart + 1800){
      motorTimerStart=motorTimer;
    }
  } else if (pattern.equals("12")) {
    if (motorTimer <= motorTimerStart + 400) {
      motor3.startVibration();
    } else if (motorTimer > motorTimerStart + 400 && motorTimer <= motorTimerStart + 600) {
      motor3.stopVibration();
    } else if (motorTimer > motorTimerStart + 600 && motorTimer <= motorTimerStart + 1000) {
      motor2.startVibration();
    } else if (motorTimer > motorTimerStart + 1000 && motorTimer <= motorTimerStart + 1200) {
      motor2.stopVibration();
    } else if (motorTimer > motorTimerStart + 1200 && motorTimer <= motorTimerStart + 1600) {
      motor1.startVibration();
    } else if (motorTimer > motorTimerStart + 1600 && motorTimer <= motorTimerStart + 1800) {
      motor1.stopVibration();     
    } else if (motorTimer > motorTimerStart + 1800){
      motorTimerStart=motorTimer;
    }
  } else if (pattern.equals("13")) {
    if (motorTimer <= motorTimerStart + 400) {
      motor2.startVibration();
    } else if (motorTimer > motorTimerStart + 400 && motorTimer <= motorTimerStart + 600) {
      motor2.stopVibration();
    } else if (motorTimer > motorTimerStart + 600){
      motorTimerStart=motorTimer;
    }
  } else if (pattern.equals("21")) {
    if (motorTimer <= motorTimerStart + 400) {
      motor2.startVibration();
    } else if (motorTimer > motorTimerStart + 400 && motorTimer <= motorTimerStart + 600) {
      motor2.stopVibration();
    } else if (motorTimer > motorTimerStart + 600 && motorTimer <= motorTimerStart + 1400) {
      motor1.startVibration();
      motor3.startVibration();
    } else if (motorTimer > motorTimerStart + 1400 && motorTimer <= motorTimerStart + 1800) {
      motor1.stopVibration();
      motor3.stopVibration();    
    } else if (motorTimer > motorTimerStart + 1800){
      motorTimerStart=motorTimer;
    }
  } else if (pattern.equals("22")) {
    if (motorTimer <= motorTimerStart + 400) {
      motor1.startVibration();
      motor3.startVibration();
    } else if (motorTimer > motorTimerStart + 400 && motorTimer <= motorTimerStart + 600) {
      motor1.stopVibration();
      motor3.stopVibration();
    } else if (motorTimer > motorTimerStart + 600 && motorTimer <= motorTimerStart + 1400) {
      motor2.startVibration();
    } else if (motorTimer > motorTimerStart + 1400 && motorTimer <= motorTimerStart + 1800) {
      motor2.stopVibration();    
    } else if (motorTimer > motorTimerStart + 1800){
      motorTimerStart=motorTimer;
    }
  } else if (pattern.equals("23")) {
    if (motorTimer <= motorTimerStart + 600) {
      motor2.startVibration();
    } else if (motorTimer > motorTimerStart + 600 && motorTimer <= motorTimerStart + 800) {
      motor2.stopVibration();
    } else if (motorTimer > motorTimerStart + 800){
      motorTimerStart=motorTimer;
    }       
  } else if (pattern.equals("31")) {
    if (motorTimer <= motorTimerStart + 400) {
      startAllVibration();
    } else if (motorTimer > motorTimerStart + 400 && motorTimer <= motorTimerStart + 1200) {
      stopAllVibration();
    } else if (motorTimer > motorTimerStart + 1200){
      motorTimerStart=motorTimer;
    }
  } else if (pattern.equals("32")) {
    if (motorTimer <= motorTimerStart + 400) {
      startAllVibration();
    } else if (motorTimer > motorTimerStart + 400 && motorTimer <= motorTimerStart + 500) {
      stopAllVibration();
    } else if (motorTimer > motorTimerStart + 500 && motorTimer <= motorTimerStart + 900) {
      startAllVibration();
    } else if (motorTimer > motorTimerStart + 900 && motorTimer <= motorTimerStart + 1700) {
      stopAllVibration();
    } else if (motorTimer > motorTimerStart + 1700){
      motorTimerStart=motorTimer;
    }
  } else if (pattern.equals("33")) {
    if (motorTimer <= motorTimerStart + 400) {
      startAllVibration();
    } else if (motorTimer > motorTimerStart + 400 && motorTimer <= motorTimerStart + 500) {
      stopAllVibration();
    } else if (motorTimer > motorTimerStart + 500 && motorTimer <= motorTimerStart + 900) {
      startAllVibration();
    } else if (motorTimer > motorTimerStart + 900 && motorTimer <= motorTimerStart + 1000) {
      stopAllVibration();
    } else if (motorTimer > motorTimerStart + 1000 && motorTimer <= motorTimerStart + 1400) {
      startAllVibration();
    } else if (motorTimer > motorTimerStart + 1400 && motorTimer <= motorTimerStart + 2200) {
      stopAllVibration();
    } else if (motorTimer > motorTimerStart + 2200){
      motorTimerStart=motorTimer;
    }
  } else if (pattern.equals("41")) {
    if (motorTimer <= motorTimerStart + 150) {
      startAllVibration();
    } else if (motorTimer > motorTimerStart + 150 && motorTimer <= motorTimerStart + 250) {
      stopAllVibration();
    } else if (motorTimer > motorTimerStart + 250) {
      motorTimerStart = motorTimer;
    }
  } else if (pattern.equals("42")) {
    if (motorTimer <= motorTimerStart + 200) {
      startAllVibration();
    } else if (motorTimer > motorTimerStart + 200 && motorTimer <= motorTimerStart + 400) {
      stopAllVibration();
    } else if (motorTimer > motorTimerStart + 400) {
      motorTimerStart = motorTimer;
    }
  } else if (pattern.equals("43")) {
    if (motorTimer <= motorTimerStart + 400) {
      startAllVibration();
    } else if (motorTimer > motorTimerStart + 400 && motorTimer <= motorTimerStart + 800) {
      stopAllVibration();
    } else if (motorTimer > motorTimerStart + 800) {
      motorTimerStart = motorTimer;
    }
  } else if (pattern.equals("51")) {
    if (motorTimer <= motorTimerStart + 800) {
      motor1.startVibration(50);
      motor2.startVibration();
      motor3.startVibration(50);
    } else if (motorTimer > motorTimerStart + 800 && motorTimer <= motorTimerStart + 1000) {
      motor2.stopVibration();
    } else if (motorTimer > motorTimerStart + 1000) {
      motorTimerStart = motorTimer;
    }
  } else if (pattern.equals("52")) {
    if (motorTimer <= motorTimerStart + 800) {
      motor1.startVibration();
      motor2.startVibration(50);
      motor3.startVibration(50);
    } else if (motorTimer > motorTimerStart + 800 && motorTimer <= motorTimerStart + 1000) {
      motor1.stopVibration();
    } else if (motorTimer > motorTimerStart + 1000) {
      motorTimerStart = motorTimer;
    }
  } else if (pattern.equals("53")) {
    if (motorTimer <= motorTimerStart + 800) {
      motor1.startVibration(50);
      motor2.startVibration(50);
      motor3.startVibration();
    } else if (motorTimer > motorTimerStart + 800 && motorTimer <= motorTimerStart + 1000) {
      motor3.stopVibration();
    } else if (motorTimer > motorTimerStart + 1000) {
      motorTimerStart = motorTimer;
    }
  } else if (pattern.equals("61")) {
    if (motorTimer <= motorTimerStart + 1000) {
      int fade = 10 * ((motorTimer - motorTimerStart) / 50);
      motor1.startVibration(50 + fade);
      motor2.startVibration(50 + fade);
      motor3.startVibration(50 + fade);  
    } else if (motorTimer > motorTimerStart + 1000 && motorTimer <= motorTimerStart + 1600) {
      stopAllVibration();
    } else if (motorTimer > motorTimerStart + 1600) {
      motorTimerStart = motorTimer;
    }
  } else if (pattern.equals("62")) {
    if (motorTimer <= motorTimerStart + 1000) {
      int fade = 10 * ((motorTimer - motorTimerStart) / 50);
      motor1.startVibration(250 - fade);
      motor2.startVibration(250 - fade);
      motor3.startVibration(250 - fade);  
    } else if (motorTimer > motorTimerStart + 1000 && motorTimer <= motorTimerStart + 1600) {
      stopAllVibration();
    } else if (motorTimer > motorTimerStart + 1600) {
      motorTimerStart = motorTimer;
    }
  } else if (pattern.equals("63")) {
    if (motorTimer <= motorTimerStart + 1000) {
      int fade = 10 * ((motorTimer - motorTimerStart) / 50);
      motor1.startVibration(50 + fade);
      motor2.startVibration(50 + fade);
      motor3.startVibration(50 + fade);  
    } else if (motorTimer > motorTimerStart + 1000 && motorTimer <= motorTimerStart + 2000) {
      int fade = 10 * ((motorTimer - (motorTimerStart + 1000)) / 50);
      motor1.startVibration(250 - fade);
      motor2.startVibration(250 - fade);
      motor3.startVibration(250 - fade); 
    } else if (motorTimer > motorTimerStart + 2000 && motorTimer <= motorTimerStart + 2600) {
      stopAllVibration();     
    } else if (motorTimer > motorTimerStart + 2600) {
      motorTimerStart = motorTimer;
    }  
  } else if (pattern.equals("71")) {
    if (motorTimer <= motorTimerStart + 200) {
      startAllVibration();
    } else if (motorTimer > motorTimerStart + 200 && motorTimer <= motorTimerStart + 400) {
      stopAllVibration();
    } else if (motorTimer > motorTimerStart + 400 && motorTimer <= motorTimerStart + 800) {
      startAllVibration();
    } else if (motorTimer > motorTimerStart + 800 && motorTimer <= motorTimerStart + 1000) {
      stopAllVibration();    
    } else if (motorTimer > motorTimerStart + 1000) {
      motorTimerStart = motorTimer;
    }  
  } else if (pattern.equals("72")) {
    if (motorTimer <= motorTimerStart + 200) {
      startAllVibration();
    } else if (motorTimer > motorTimerStart + 200 && motorTimer <= motorTimerStart + 400) {
      stopAllVibration();
    } else if (motorTimer > motorTimerStart + 400 && motorTimer <= motorTimerStart + 600) {
      startAllVibration();
    } else if (motorTimer > motorTimerStart + 600 && motorTimer <= motorTimerStart + 800) {
      stopAllVibration();
    } else if (motorTimer > motorTimerStart + 800 && motorTimer <= motorTimerStart + 1200) {
      startAllVibration();
    } else if (motorTimer > motorTimerStart + 1200 && motorTimer <= motorTimerStart + 1400) {
      stopAllVibration();      
    } else if (motorTimer > motorTimerStart + 1400) {
      motorTimerStart = motorTimer;
    }   
  } else if (pattern.equals("73")) {
    if (motorTimer <= motorTimerStart + 400) {
      startAllVibration();
    } else if (motorTimer > motorTimerStart + 400 && motorTimer <= motorTimerStart + 600) {
      stopAllVibration();
    } else if (motorTimer > motorTimerStart + 600 && motorTimer <= motorTimerStart + 1000) {
      startAllVibration();
    } else if (motorTimer > motorTimerStart + 1000 && motorTimer <= motorTimerStart + 1200) {
      stopAllVibration();
    } else if (motorTimer > motorTimerStart + 1200 && motorTimer <= motorTimerStart + 1400) {
      startAllVibration();
    } else if (motorTimer > motorTimerStart + 1400 && motorTimer <= motorTimerStart + 1600) {
      stopAllVibration();      
    } else if (motorTimer > motorTimerStart + 1600) {
      motorTimerStart = motorTimer;
    }  
  } else if (pattern.equals("81")) {
    if (motorTimer <= motorTimerStart + 400) {
      motor1.startVibration();
    } else if (motorTimer > motorTimerStart + 400 && motorTimer <= motorTimerStart + 600) {
      motor1.stopVibration();
    } else if (motorTimer > motorTimerStart + 600 && motorTimer <= motorTimerStart + 1000) {
      motor2.startVibration();
    } else if (motorTimer > motorTimerStart + 1000 && motorTimer <= motorTimerStart + 1200) {
      motor2.stopVibration();
    } else if (motorTimer > motorTimerStart + 1600) {
      motorTimerStart = motorTimer;
    }  
  } else if (pattern.equals("82")) {
    if (motorTimer <= motorTimerStart + 400) {
      motor2.startVibration();
    } else if (motorTimer > motorTimerStart + 400 && motorTimer <= motorTimerStart + 600) {
      motor2.stopVibration();
    } else if (motorTimer > motorTimerStart + 600 && motorTimer <= motorTimerStart + 1000) {
      motor3.startVibration();
    } else if (motorTimer > motorTimerStart + 1000 && motorTimer <= motorTimerStart + 1200) {
      motor3.stopVibration();
    } else if (motorTimer > motorTimerStart + 1600) {
      motorTimerStart = motorTimer;
    }  
  } else if (pattern.equals("83")) {
    if (motorTimer <= motorTimerStart + 400) {
      motor1.startVibration();
    } else if (motorTimer > motorTimerStart + 400 && motorTimer <= motorTimerStart + 600) {
      motor1.stopVibration();
    } else if (motorTimer > motorTimerStart + 600 && motorTimer <= motorTimerStart + 1000) {
      motor2.startVibration();
    } else if (motorTimer > motorTimerStart + 1000 && motorTimer <= motorTimerStart + 1200) {
      motor2.stopVibration();
    } else if (motorTimer > motorTimerStart + 1200 && motorTimer <= motorTimerStart + 1600) {
      motor3.startVibration();
    } else if (motorTimer > motorTimerStart + 1600 && motorTimer <= motorTimerStart + 1800) {
      motor3.stopVibration();  
    } else if (motorTimer > motorTimerStart + 1800) {
      motorTimerStart = motorTimer;
    }                           
  }
}

void startAllVibration() {
  motor1.startVibration();
  motor2.startVibration();
  motor3.startVibration();
}

void stopAllVibration() {
  motor1.stopVibration();
  motor2.stopVibration();
  motor3.stopVibration();
}



