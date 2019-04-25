#include <Arduino.h>
#include <Servo.h>
int s1=89;   //100 = rovina -/+ orintace od nazvu serva
int s2=88;
int s3=125;  //125 = zavrený kleště \ 180 = otevrěný kleště    90 = optimal pos
int s0=85;


Servo servo0;
Servo servo1;
Servo servo2;
Servo servo3;


void setup() {
    Serial.begin(115200);


    //Servo 0
    servo0.attach(27);
    //servo0.write(90);
    //Servo 1
    servo1.attach(32);
  //  servo1.write(90);
    //Servo 2
    servo2.attach(33);
    //servo2.write(90);
    //Servo 3
    servo3.attach(25);
  //  servo3.write(90);

  servo0.write(s0);
  servo1.write(s1);
  servo2.write(s2);
  servo3.write(s3);
  delay(3000);

  servo3.write(180);
  delay(500);
  servo0.write(120);
  delay(500);
  servo1.write(130);
  delay(500);
  servo2.write(160);
  delay(500);
  servo3.write(s3);
  delay(3000);

  servo0.write(s0);
  delay(500);
  servo1.write(s1);
  delay(500);
  servo2.write(s2);
  delay(3000);
  servo0.write(120);
  delay(500);
  servo1.write(130);
  delay(500);
  servo2.write(160);
  delay(500);
  servo3.write(180);
  delay(500);
  servo0.write(s0);
  delay(500);
  servo1.write(s1);
  delay(500);
  servo2.write(s2);
  delay(500);
  servo3.write(s3);



}


void loop() {
   // servo0.write(s0);
   // servo1.write(s1);
   // servo2.write(s2);
   // servo3.write(s3);


   // // s0 = analogRead(35);
   // // s1 = analogRead(32);
   // // s2 = analogRead(33);
   // // s3 = analogRead(25);
   //
   //  Serial.println(s0);
   //  Serial.println("         ");
   //  Serial.println(s1);
   //  Serial.println("         ");
   //  Serial.println(s2);
   //  Serial.println("         ");
   //  Serial.println(s3);
   //  Serial.println("         ");
   //  delay(500);
}
