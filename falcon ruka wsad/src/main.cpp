#include <Arduino.h>
//#include "ODriveArduino.h"
#include "RBControl_manager.hpp"
#include <Servo.h>

// static const int kMotorOffsetFloat = 2;
// static const int kMotorStrideFloat = 28;
// static const int kMotorOffsetInt32 = 0;
// static const int kMotorStrideInt32 = 4;
// static const int kMotorOffsetBool = 0;
// static const int kMotorStrideBool = 4;
// static const int kMotorOffsetUint16 = 0;
// static const int kMotorStrideUint16 = 2;

// #define MOTOR 1
int motorr = 0;
int motorl = 0;

int s0 = 90;
int s1 = 90;
int s2 = 90;

rb::Manager* man = nullptr;

Servo servo0;
Servo servo1;
Servo servo2;
Servo servo3;


void setup() {
    Serial.begin(115200);

    //Servo 0
    servo0.attach(27);
    servo0.write(90);
    //Servo 1
    servo1.attach(25);
    servo1.write(s1);
    //Servo 2
    servo2.attach(33);
    servo2.write(s2);
    //Servo 3
    servo3.attach(32);
    servo3.write(90);
}


void loop()
{
    servo0.write(s0);
    servo1.write(s1);
    servo2.write(s2);

    if(Serial.available()) {
        char o = Serial.read();
        switch(o) {
            case 'w':
                  motorr = motorr + 1;
                  motorl = motorl + 1;
                break;
            case 's':
                  motorr = motorr - 1;
                  motorl = motorl - 1;
                break;
            case 'a':
                  motorr = motorr + 1;
                  motorl = motorl - 1;
                break;
            case 'd':
                  motorr = motorr - 1;
                  motorl = motorl + 1;
                break;
            case 'u':
                servo3.write(40);

                break;
            case 'i':
                servo3.write(170);

                break;
            case 'j':
                if (s2<=181)
                {
                  s2 = s2 + 2;
                }
                break;
            case 'n':
                if (s2>0)
                {
                  s2 = s2 - 2;
                }
                break;
            case 'k':
                if (s1<=180)
                {
                  s1 = s1 + 2;
                }
                break;
            case 'm':
                if (s1>0)
                {
                  s1 = s1 - 2;
                }
                break;
            case 'c':
                if (s0<=180)
                {
                  s0 = s0 + 2;
                }
                 break;
            case 'v':
                if (s0>0)
                {
                  s0 = s0 - 2;
                }
                  break;
            //  case '0':
             // man->setMotors().power(MOTOR, -10)
             //                 .set();
            //
            //     break;
            // case ',':
            // man->setMotors().power(MOTOR, 10)
            //                 .set();

            //    break;
            default:
                Serial.write(o);
                Serial.write(motorr);
                Serial.write(motorl);
                Serial.write(s1);
                Serial.write(s2);
                break;
        }
    }
       Serial.println(motorl);
       Serial.println('/n');
       Serial.println(motorr);
       Serial.println('/n');
       Serial.println(s0);
       Serial.println('/n');
       Serial.println(s1);
       Serial.println('/n');
       Serial.println(s2);
       Serial.println('/n');
       Serial.println(s3);
       Serial.println('/n');
       delay(200);
}
// // Print with stream operator
// template<class T> inline Print& operator <<(Print &obj,     T arg) { obj.print(arg);    return obj; }
// template<>        inline Print& operator <<(Print &obj, float arg) { obj.print(arg, 4); return obj; }
//
// ODriveArduino::ODriveArduino(Stream& serial)
//     : serial_(serial) {}
//
// void ODriveArduino::SetPosition(int motor_number, float position) {
//     SetPosition(motor_number, position, 0.0f, 0.0f);
// }
//
// void ODriveArduino::SetPosition(int motor_number, float position, float velocity_feedforward) {
//     SetPosition(motor_number, position, velocity_feedforward, 0.0f);
// }
//
// void ODriveArduino::SetPosition(int motor_number, float position, float velocity_feedforward, float current_feedforward) {
//     serial_ << "$p " << motor_number  << " " << position << " " << velocity_feedforward << " " << current_feedforward << "!";
// }
//
// void ODriveArduino::SetVelocity(int motor_number, float velocity) {
//     SetVelocity(motor_number, velocity, 0.0f);
// }
//
// void ODriveArduino::SetVelocity(int motor_number, float velocity, float current_feedforward) {
//     serial_ << "$v " << motor_number  << " " << velocity << " " << current_feedforward << "!";
// }
//
// float ODriveArduino::getBusVoltage() {
//     serial_ << "$g 0 0!";
//     return readFloat();
// }
//
// float ODriveArduino::GetParameter(int motor_number, ParamNamesFloat parameter) {
//     int idx = kMotorOffsetFloat + kMotorStrideFloat * motor_number + parameter;
//     serial_ << "$g 0 " << idx << "!";
//     return readString().toFloat();
// }
//
// int32_t ODriveArduino::GetParameter(int motor_number, ParamNamesInt32 parameter) {
//     int idx = kMotorOffsetInt32 + kMotorStrideInt32 * motor_number + parameter;
//     serial_ << "$g 1 " << idx << "!";
//     return readString().toInt();
// }
//
// bool ODriveArduino::GetParameter(int motor_number, ParamNamesBool parameter) {
//     int idx = kMotorOffsetBool + kMotorStrideBool * motor_number + parameter;
//     serial_ << "$g 2 " << idx << "!";
//     return readString().toInt();
// }
//
// uint16_t ODriveArduino::GetParameter(int motor_number, ParamNamesUint16 parameter) {
//     int idx = kMotorOffsetUint16 + kMotorStrideUint16 * motor_number + parameter;
//     serial_ << "$g 3 " << idx << "!";
//     return readString().toInt();
// }
//
//
// void ODriveArduino::SetParameter(int motor_number, ParamNamesFloat parameter, float value) {
//     int idx = kMotorOffsetFloat + kMotorStrideFloat * motor_number + parameter;
//     serial_ << "$s 0 " << idx << " " << value << "!";
// }
//
// void ODriveArduino::SetParameter(int motor_number, ParamNamesInt32 parameter, int32_t value) {
//     int idx = kMotorOffsetInt32 + kMotorStrideInt32 * motor_number + parameter;
//     serial_ << "$s 1 " << idx << " " << value << "!";
// }
//
// void ODriveArduino::SetParameter(int motor_number, ParamNamesBool parameter, bool value) {
//     int idx = kMotorOffsetBool + kMotorStrideBool * motor_number + parameter;
//     serial_ << "$s 2 " << idx << " " << value << "!";
// }
//
// void ODriveArduino::SetParameter(int motor_number, ParamNamesUint16 parameter, uint16_t value) {
//     int idx = kMotorOffsetUint16 + kMotorStrideUint16 * motor_number + parameter;
//     serial_ << "$s 3 " << idx << " " << value << "!";
// }
//
//
// float ODriveArduino::readFloat() {
//     return readString().toFloat();
// }
//
// int32_t ODriveArduino::readInt() {
//     return readString().toInt();
// }
//
// String ODriveArduino::readString() {
//     String str = "";
//     static const unsigned long timeout = 1000;
//     unsigned long timeout_start = millis();
//     for (;;) {
//         while (!serial_.available()) {
//             if (millis() - timeout_start >= timeout) {
//                 return str;
//             }
//         }
//         char c = serial_.read();
//         if (c == '\n')
//             break;
//         str += c;
//     }
//     return str;
// }
