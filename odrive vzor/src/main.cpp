#include <esp_log.h>
#include <string.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_system.h>

#include <Arduino.h>
#include <HardwareSerial.h>
#include <ODriveArduino.h>


#include "rbprotocol.h"
#include "rbwebserver.h"

#include "RBControl_manager.hpp"
#include "RBControl_battery.hpp"
#include "RBControl_wifi.hpp"

// CHANGE THESE so you can find the robot in the Android app
#define OWNER "Mirek"
#define NAME "MirkovaPříšera"

// CHANGE THESE to your WiFi's settings
#define WIFI_NAME "Technika"
#define WIFI_PASSWORD "materidouska"

HardwareSerial odriveSerial(1);
ODriveArduino odrive(odriveSerial);
bool motorUpdated;

int scale_motors(float val) {
    return 0.9 * val;
}

void motors_handle_joysticks(rbjson::Object *pkt) {
    const rbjson::Array *data = pkt->getArray("data");

    // Drive
    {
        const rbjson::Object *joy = data->getObject(0);
        int x = joy->getInt("x");
        int y = joy->getInt("y");

        printf("Motor: %d, %d\n", x, y);

        if(x != 0)
            x = scale_motors(x);
        if(y != 0)
            y = scale_motors(y);

        int r = ((y - (x/2)));
        int l = ((y + (x/2)));
        // if(r < 0 && l < 0) {
        //     int tmp = r; r = l; l = tmp;
        // }

        printf("Res: %d, %d\n", l, r);


        odrive.SetVelocity(0, l);
        odrive.SetVelocity(1, r);
        motorUpdated = true;
    }
}

void setup() {
    delay(1000);
    // Initialize the robot manager
    rb::Manager man;

    // Initilize oDrive
    printf("Connecting to oDrive\n");
    odriveSerial.begin(115200, SERIAL_8N1, 13, 15);
    for (int axis : { 0, 1 }) {
        odrive.run_state(axis, ODriveArduino::AXIS_STATE_IDLE, true);
    }
    printf("Motors disabled\n");
    for (int axis : { 0, 1 }) {
        odrive.run_state(axis, ODriveArduino::AXIS_STATE_FULL_CALIBRATION_SEQUENCE, true);
    }
    printf("Calibrated\n");
    for (int axis : { 0, 1 }) {
        odrive.run_state(axis, ODriveArduino::AXIS_STATE_CLOSED_LOOP_CONTROL, false);
    }
    printf("Odrive ready\n");

    // Set the battery measuring coefficient.
    // Measure voltage at battery connector and
    // coef = voltageMeasureAtBatteriesInMilliVolts / raw
    auto& batt = man.battery();
    batt.setCoef(9.0);

    // Connect to the WiFi network
    // If the button 1 is not pressed: connect to WIFI_NAME
    // else create an AP.
    // if(man.expander().digitalRead(rb::SW1) == 0) {
    //     man.leds().yellow();
    //     printf("Connecting to WiFi...\n");
    //     rb::WiFi::connect(WIFI_NAME, WIFI_PASSWORD);
    //     printf("Connected\n");
    // } else {
    //     man.leds().green();
    rb::WiFi::startAp("Flus" OWNER "-" NAME, "flusflus");
    printf("WiFi started\n");
    // }

    rb_web_start(80);   // Start web server with control page (see data/index.html)

    // Initialize the communication protocol
    rb::Protocol prot(OWNER, NAME, "Compiled at " __DATE__ " " __TIME__, [&](const std::string& command, rbjson::Object *pkt) {
        if(command == "joy") {
            motors_handle_joysticks(pkt);
        }
    });

    prot.start();

    printf("%s's mickoflus '%s' started!\n", OWNER, NAME);

    vTaskDelay(1000 / portTICK_PERIOD_MS);
    printf("\n\nBATTERY CALIBRATION INFO: %d (raw) * %.2f (coef) = %dmv\n\n\n", batt.raw(), batt.coef(), batt.voltageMv());

    int i = 0;
    while(true) {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        if(prot.is_possessed()) {
            // Send text to the android application
            prot.send_log("Tick #%d, battery at %d%%, %dmv\n", i++, batt.pct(), batt.voltageMv());
        }
        if (!motorUpdated) {
            for (int axis : { 0, 1 }) {
                odrive.SetVelocity(axis,0);
            }
        }
    }
}

void loop() {

}