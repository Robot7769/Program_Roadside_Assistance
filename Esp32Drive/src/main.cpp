#include <Arduino.h>
#include <HardwareSerial.h>
#include <ODriveArduino.h>

HardwareSerial odriveSerial(1);
ODriveArduino odrive(odriveSerial);

void setup() {
  Serial.begin(115200);
  odriveSerial.begin(115200, SERIAL_8N1, 13, 15);

  Serial.println("Starting");
  odrive.run_state(0, ODriveArduino::AXIS_STATE_FULL_CALIBRATION_SEQUENCE, true);
  odrive.run_state(1, ODriveArduino::AXIS_STATE_FULL_CALIBRATION_SEQUENCE, true);
  odrive.run_state(0, ODriveArduino::AXIS_STATE_CLOSED_LOOP_CONTROL, true);
  odrive.run_state(1, ODriveArduino::AXIS_STATE_CLOSED_LOOP_CONTROL, true);
}

void loop() {
  Serial.println("Going up");
  for (int i = 0; i != 10000000; i += 100) {
    odrive.SetPosition(0, i);
    odrive.SetPosition(1, i);
    delay(1);
  }
  delay(1000);
  Serial.println("Going down");
  for (int i = 100000000; i != 0; i -= 100) {
    odrive.SetPosition(0, i);
    odrive.SetPosition(1, i);
    delay(1);
  }
}
