#include <Arduino.h>
#include "RBControl_manager.hpp"
#include "nvs_flash.h"
#include "BluetoothSerial.h"
#include <Servo.h>
#include <HardwareSerial.h>
#include <ODriveArduino.h>


#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#define MAX_DEGREE 180
#define MIN_DEGREE 0


#define MOTOR  rb::MotorId::M1

int s3 = 125;
int s2 = 89;
int s1 = 88;
int s0 = 85;

int8_t i[7];
uint8_t x;

int btn = 0;
int btn_state = 0;

int cross = 0;
int posm = 0;
int posm0 = 0;   //pozice levého motoru
int posm1 = 0;   //pozice Pravého motoru
int armx = 0;

rb::Manager& rbc()
{
  static rb::Manager m(false);
  return m;
}

BluetoothSerial SerialBT;

Stream* serial = nullptr;

HardwareSerial odriveSerial(1);
ODriveArduino odrive(odriveSerial);

Servo servo3;
Servo servo2;
Servo servo1;
Servo servo0;

void setup()
{
  Serial.begin(115200);
    if (!SerialBT.begin("Falcon")) //Bluetooth device name   test:Falcon
    {
        Serial.println("!!! Bluetooth initialization failed!");
        serial = &Serial;
    }
    else
    {
        serial = &SerialBT;
    }
  odriveSerial.begin(115200, SERIAL_8N1, 13, 15);   //serial. linka pro odrive

  servo3.attach(27);
  servo2.attach(25);
  servo1.attach(26);
  servo0.attach(33);

  Serial.println("Starting");
  odrive.run_state(0, ODriveArduino::AXIS_STATE_FULL_CALIBRATION_SEQUENCE, true);   //kalibrace motoru
  odrive.run_state(1, ODriveArduino::AXIS_STATE_FULL_CALIBRATION_SEQUENCE, true);
  odrive.run_state(0, ODriveArduino::AXIS_STATE_CLOSED_LOOP_CONTROL, true);     //zamčení morotu nepohnou se bez udání pozice
  odrive.run_state(1, ODriveArduino::AXIS_STATE_CLOSED_LOOP_CONTROL, true);


}


void read()
{
    if (SerialBT.available() > 0)
    {
     uint8_t test = SerialBT.read();
     if (test == 0x80)
         for (x = 0; x < 7; x++)
         {
			 while(SerialBT.available() < 1)
       {
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
		 while(SerialBT.available() < 1)
     {
			// DO NOTHING - WAITING FOR PACKET
			delay(1);
		 }
         btn = SerialBT.read();
         Serial.println(btn, DEC);
		 while(SerialBT.available() < 1)
     {
			// DO NOTHING - WAITING FOR PACKET
			delay(1);
		 }
     btn_state = SerialBT.read();
		 Serial.println(btn_state, DEC);
     }

     Serial.println(" ");
     SerialBT.write(millis());
   }
 }

void drive()
{
  if (i[2] > 127)        //128
  {
    posm = (i[2])-127;   //128
  }
  else if (i[2] < 127)   //126
  {
    posm = (i[2])-127;   //126
  }
  else
  {
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
  odrive.SetPosition(0, posm0);
  odrive.SetPosition(1, posm1);
}

void servo()
{
  switch (btn)
  {
    case 0:
      if (btn_state > 0)
      {
        if (MIN_DEGREE <= s0 && s0 <= MAX_DEGREE)
        {
          s0++;
        }
      }
      break;
    case 3:
      if (btn_state > 0)
      {
        if (MIN_DEGREE <= s0 && s0 <= MAX_DEGREE)
        {
          s0--;
        }
      }
      break;
    case 2:
      if (btn_state > 0)
      {
        if (MIN_DEGREE <= s1 && s1 <= MAX_DEGREE)
        {
          s1++;
        }
      }
      break;
    case 1:
      if (btn_state > 0)
      {
        if (MIN_DEGREE <= s1 && s1 <= MAX_DEGREE)
        {
          s1--;
        }
      }
      break;
    case 4:
      if (btn_state > 0)
      {
        if (MIN_DEGREE <= s2 && s2 <= MAX_DEGREE)
        {
          s2++;
        }
      }
      break;
    case 5:
      if (btn_state > 0)
      {
        if (MIN_DEGREE <= s2 && s2 <= MAX_DEGREE)
        {
          s2--;
        }
      }
      break;
    case 6:
      if (btn_state > 0)
      {
        if (MIN_DEGREE <= s3 && s3 <= MAX_DEGREE)
        {
          s3++;
        }
      }
      break;
    case 7:
      if (btn_state > 0)
      {
        if (MIN_DEGREE <= s3 && s3 <= MAX_DEGREE)
        {
          s3--;
        }
      }
      break;
      //musí se doplnit podmika kdy a porč a kam k otočnému motoru
/*    rbc().setMotors().power(LEFT_MOTOR, 50)
                       .power(RIGHT_MOTOR, 50)
                       .set();

      rbc().setMotors().power(LEFT_MOTOR, 0)
                       .power(RIGHT_MOTOR, 0)
                       .set();

      rbc().motor(LEFT_MOTOR)->drive(256 * (c - '0'), 64, nullptr);
      */

  }

  servo0.write(s0);
  servo1.write(s1);
  servo2.write(s2);
  servo3.write(s3);
}

void loop()
{
  read();
  drive();
  servo();

  Serial.print(rbc().motor(MOTOR)->encoder()->value());
  Serial.print(' ');
}
