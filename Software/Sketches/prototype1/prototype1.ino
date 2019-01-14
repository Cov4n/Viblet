#include <Arduino.h>
#include <SPI.h>
#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_SPI.h"
#include "Adafruit_BluefruitLE_UART.h"

#include "BluefruitConfig.h"
#include "Motor.h"

#if SOFTWARE_SERIAL_AVAILABLE
  #include <SoftwareSerial.h>
#endif


#define FACTORYRESET_ENABLE         1
#define MINIMUM_FIRMWARE_VERSION    "0.6.6"
#define MODE_LED_BEHAVIOUR          "MODE"

Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_CS, BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);

String input = "0";

Motor motor1 = Motor(11);
Motor motor2 = Motor(12);
Motor motor3 = Motor(13);


void error(const __FlashStringHelper*err) {
  //Serial.println(err);
  while (1);
}

void setup(void)
{
  //while (!Serial);  // required for Flora & Micro
  delay(500);

  Serial.begin(115200);
  //Serial.println(F("Adafruit Bluefruit Command <-> Data Mode Example"));
  //Serial.println(F("------------------------------------------------"));

  /* Initialise the module */
  //Serial.print(F("Initialising the Bluefruit LE module: "));

  if ( !ble.begin(VERBOSE_MODE) )
  {
    //error(F("Couldn't find Bluefruit, make sure it's in CoMmanD mode & check wiring?"));
  }
  //Serial.println( F("OK!") );

  if ( FACTORYRESET_ENABLE )
  {
    /* Perform a factory reset to make sure everything is in a known state */
    //Serial.println(F("Performing a factory reset: "));
    if ( ! ble.factoryReset() ){
      //error(F("Couldn't factory reset"));
    }
  }

  /* Disable command echo from Bluefruit */
  ble.echo(false);

  //Serial.println(F("Please use Adafruit Bluefruit LE app to connect in UART mode"));
  //Serial.println(F("Then Enter characters to send to Bluefruit"));
  //Serial.println();

  ble.verbose(false);  // debug info is a little annoying after this point!

  /* Wait for connection */
  while (! ble.isConnected()) {
      delay(500);
  }

  //Serial.println(F("******************************"));

  // LED Activity command is only supported from 0.6.6
  if ( ble.isVersionAtLeast(MINIMUM_FIRMWARE_VERSION) )
  {
    // Change Mode LED Activity
    //Serial.println(F("Change LED activity to " MODE_LED_BEHAVIOUR));
    ble.sendCommandCheckOK("AT+HWModeLED=" MODE_LED_BEHAVIOUR);
  }

  // Set module to DATA mode
  //Serial.println( F("Switching to DATA mode!") );
  ble.setMode(BLUEFRUIT_MODE_DATA);

  //Serial.println(F("******************************"));

}

void loop(void)
{

  int inCount = 0;
  
  // Read received data
  while ( ble.available() )
  {
    if (inCount == 0) {
      input = ""; 
    }
    inCount++;
    int in = ble.read();
    input += (char) in;
  }
  
  if (input!= "") {
    input.trim();
    //Serial.println(input);

    if (input.equals("0")) {
      motor1.stopVibration();
      motor2.stopVibration();
      motor3.stopVibration();
    } else if (input.equals("1")) {
      motor1.startVibration();
      motor2.startVibration();
      motor3.startVibration();
    } else if (input.equals("11")) {
       motor1.startVibration();
       delay(400);
       motor1.stopVibration();
       delay(200);
       motor2.startVibration();
       delay(400);
       motor2.stopVibration();
       delay(200);
       motor3.startVibration();
       delay(400);
       motor3.stopVibration();
       delay(200);  
    } else if (input.equals("12")) {
       motor3.startVibration();
       delay(400);
       motor3.stopVibration();
       delay(200);
       motor2.startVibration();
       delay(400);
       motor2.stopVibration();
       delay(200);
       motor1.startVibration();
       delay(400);
       motor1.stopVibration();
       delay(200);     
    } else if (input.equals("13")) {
      motor2.startVibration();
      delay(400);
      motor2.stopVibration();
      delay(200);  
    } else if (input.equals("21")) {
      motor2.startVibration();
      delay(400);
      motor2.stopVibration();
      delay(200);
      motor1.startVibration();
      motor3.startVibration();
      delay(800);
      motor1.stopVibration();
      motor3.stopVibration();
      delay(400);
    } else if (input.equals("22")) {
      motor1.startVibration();
      motor3.startVibration();
      delay(400);
      motor1.stopVibration();
      motor3.stopVibration();
      delay(200);
      motor2.startVibration();
      delay(800);
      motor2.stopVibration();
      delay(400);
    } else if (input.equals("23")) {
      motor2.startVibration();
      delay(600);
      motor2.stopVibration();
      delay(200);
      motor2.startVibration();
      delay(600);
      motor2.stopVibration();
      delay(200);
    } else if (input.equals("31")) {
      motor1.startVibration();
      motor2.startVibration();
      motor3.startVibration();
      delay(400);
      motor1.stopVibration();
      motor2.stopVibration();
      motor3.stopVibration();
      delay(800);
    } else if (input.equals("32")) {
      motor1.startVibration();
      motor2.startVibration();
      motor3.startVibration();
      delay(400);
      motor1.stopVibration();
      motor2.stopVibration();
      motor3.stopVibration();
      delay(100);
      motor1.startVibration();
      motor2.startVibration();
      motor3.startVibration();
      delay(400);
      motor1.stopVibration();
      motor2.stopVibration();
      motor3.stopVibration();
      delay(800);
    } else if (input.equals("33")) {
      motor1.startVibration();
      motor2.startVibration();
      motor3.startVibration();
      delay(400);
      motor1.stopVibration();
      motor2.stopVibration();
      motor3.stopVibration();
      delay(100);
      motor1.startVibration();
      motor2.startVibration();
      motor3.startVibration();
      delay(400);
      motor1.stopVibration();
      motor2.stopVibration();
      motor3.stopVibration();
      delay(100);
      motor1.startVibration();
      motor2.startVibration();
      motor3.startVibration();
      delay(400);
      motor1.stopVibration();
      motor2.stopVibration();
      motor3.stopVibration();
      delay(800);
    } else if (input.equals("41")) {
      motor1.startVibration();
      motor2.startVibration();
      motor3.startVibration();
      delay(100);
      motor1.stopVibration();
      motor2.stopVibration();
      motor3.stopVibration();
      delay(100);
    } else if (input.equals("42")) {
      motor1.startVibration();
      motor2.startVibration();
      motor3.startVibration();
      delay(200);
      motor1.stopVibration();
      motor2.stopVibration();
      motor3.stopVibration();
      delay(200);
    } else if (input.equals("43")) {
      motor1.startVibration();
      motor2.startVibration();
      motor3.startVibration();
      delay(400);
      motor1.stopVibration();
      motor2.stopVibration();
      motor3.stopVibration();
      delay(400);
    } else if (input.equals("52")) {
      motor1.startVibration();
      motor2.startVibration(50);
      motor3.startVibration(50);
      delay(800);
      motor1.stopVibration();
      delay(200);
    } else if (input.equals("51")) {
      motor1.startVibration(50);
      motor2.startVibration();
      motor3.startVibration(50);
      delay(800);
      motor2.stopVibration();
      delay(200);
    } else if (input.equals("53")) {
      motor1.startVibration(50);
      motor2.startVibration(50);
      motor3.startVibration();
      delay(800);
      motor3.stopVibration();
      delay(200);
    } else if (input.equals("61")) {
      int fade = 50;
      while(fade < 250) {
        motor1.startVibration(fade);
        motor2.startVibration(fade);
        motor3.startVibration(fade);
        fade = fade + 10;
        delay(50);
      }
      motor1.stopVibration();
      motor2.stopVibration();
      motor3.stopVibration();
      delay(600);
    } else if (input.equals("62")) {
      int fade = 250;
      while(fade > 50) {
        motor1.startVibration(fade);
        motor2.startVibration(fade);
        motor3.startVibration(fade);
        fade = fade - 10;
        delay(50);
      }
      motor1.stopVibration();
      motor2.stopVibration();
      motor3.stopVibration();
      delay(600);
    } else if (input.equals("63")) {
      int fade = 50;
      while(fade < 250) {
        motor1.startVibration(fade);
        motor2.startVibration(fade);
        motor3.startVibration(fade);
        fade = fade + 10;
        delay(50);
      }
      fade = 250;
      while(fade > 50) {
        motor1.startVibration(fade);
        motor2.startVibration(fade);
        motor3.startVibration(fade);
        fade = fade - 10;
        delay(50);
      }
      motor1.stopVibration();
      motor2.stopVibration();
      motor3.stopVibration();
      delay(400);
    } else if (input.equals("71")) {
      motor1.startVibration();
      motor2.startVibration();
      motor3.startVibration();
      delay(200);
      motor1.stopVibration();
      motor2.stopVibration();
      motor3.stopVibration();
      delay(200);
      motor1.startVibration();
      motor2.startVibration();
      motor3.startVibration();
      delay(400);
      motor1.stopVibration();
      motor2.stopVibration();
      motor3.stopVibration();
      delay(200);
    } else if (input.equals("72")) {
      motor1.startVibration();
      motor2.startVibration();
      motor3.startVibration();
      delay(200);
      motor1.stopVibration();
      motor2.stopVibration();
      motor3.stopVibration();
      delay(200);
      motor1.startVibration();
      motor2.startVibration();
      motor3.startVibration();
      delay(200);
      motor1.stopVibration();
      motor2.stopVibration();
      motor3.stopVibration();
      delay(200);
      motor1.startVibration();
      motor2.startVibration();
      motor3.startVibration();
      delay(400);
      motor1.stopVibration();
      motor2.stopVibration();
      motor3.stopVibration();
      delay(200);
    } else if (input.equals("73")) {
      motor1.startVibration();
      motor2.startVibration();
      motor3.startVibration();
      delay(400);
      motor1.stopVibration();
      motor2.stopVibration();
      motor3.stopVibration();
      delay(200);
      motor1.startVibration();
      motor2.startVibration();
      motor3.startVibration();
      delay(400);
      motor1.stopVibration();
      motor2.stopVibration();
      motor3.stopVibration();
      delay(200);
      motor1.startVibration();
      motor2.startVibration();
      motor3.startVibration();
      delay(200);
      motor1.stopVibration();
      motor2.stopVibration();
      motor3.stopVibration();
      delay(200);
    } else if (input.equals("81")) {
      motor1.startVibration();
      delay(400);
      motor1.stopVibration();
      delay(200);
      motor2.startVibration();
      delay(400);
      motor2.stopVibration();
      delay(200);
    } else if (input.equals("82")) {
      motor2.startVibration();
      delay(400);
      motor2.stopVibration();
      delay(200);
      motor3.startVibration();
      delay(400);
      motor3.stopVibration();
      delay(200);
    } else if (input.equals("83")) {
      motor1.startVibration();
      delay(400);
      motor1.stopVibration();
      delay(200);
      motor2.startVibration();
      delay(400);
      motor2.stopVibration();
      delay(200);
      motor3.startVibration();
      delay(400);
      motor3.stopVibration();
      delay(200);
    }
  }
  
}
