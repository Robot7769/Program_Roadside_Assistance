#include <Arduino.h>
#include "RBControl_manager.hpp"
#include "nvs_flash.h"
#include "BluetoothSerial.h"
#include <Servo.h>

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

int s3 = 0;
int s2 = 0;
int s1 = 0;
int s0 = 0;

int8_t i[7];
uint8_t x;

int cross = 0;
int posm = 0;
int posm0 = 0;   //pozice levého motoru
int posm1 = 0;   //pozice Pravého motoru
int armx = 0;

rb::Manager* man = nullptr;

BluetoothSerial SerialBT;

Stream* serial = nullptr;

Servo servo3;
Servo servo2;
Servo servo1;
Servo servo0;

void setup()
{
  Serial.begin(115200);
    if (!SerialBT.begin("test:Falcon")) //Bluetooth device name
    {
        Serial.println("!!! Bluetooth initialization failed!");
        serial = &Serial;
    }
    else
    {
        serial = &SerialBT;
    }
  servo3.attach(32);
  servo2.attach(33);
  servo1.attach(25);
  servo0.attach(26);


}


//uint32_t t_enc = 0;

void read()
{
 if (SerialBT.available() > 0)
 {
     uint8_t test = SerialBT.read();
     if (test == 0x80)
         for (x = 0; x < 7; x++)
         {
			 while(SerialBT.available() < 1) {
				// DO NOTHING - WAITING FOR PACKET
				delay(1);
			 }

             i[x] = SerialBT.read();
             Serial.print(x);
             Serial.print(": ");
             Serial.print(i[x], DEC);
             Serial.print(" ");
         }
     else if  ( test == 0x81 )
     {
		 while(SerialBT.available() < 1) {
			// DO NOTHING - WAITING FOR PACKET
			delay(1);
		 }
         Serial.println(SerialBT.read(), DEC);
		 while(SerialBT.available() < 1) {
			// DO NOTHING - WAITING FOR PACKET
			delay(1);
		 }
		 Serial.println(SerialBT.read(), DEC);
     }
   Serial.println(" ");
   SerialBT.write(millis());
 }
}

void drive() {
  if (i[2] > 127)        //128
  {
    posm = (i[2])-127;   //128
  }
  else if (i[2] < 127)   //126
  {
    posm = (i[2])-127;   //126
  }
  else {
    posm = 0;
  }

  if (i[0] > 127)        //128
  {
    cross = (i[0])-127;   //128
  }
  else if (i[0] < 127)   //126
  {
    cross = (i[0])-127;   //126
  }
  else {
    cross = 0;
  }

  posm0 = posm0 + (posm - cross);
  posm1 = posm1 + (posm + cross);

  /*     odrive
  zapsat pozice "posm0" & "posm1"
  do pozice motorů "0" & "1"   */
}

void servo() {

}

void loop() {
  read();
  drive();
  servo();

}
