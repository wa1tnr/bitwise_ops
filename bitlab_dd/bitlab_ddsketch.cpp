// Sat 14 Jul 19:05:26 UTC 2018
// bitlab_ddsketch.cpp

// was:
// bitlab_cc.ino

// target MCU: Adafruit Feather M0 Express

#include <Arduino.h>
#include "bitlab_ddsketch.h"

void setup(void) {
    Serial.begin(9600);
    while(!Serial) {
        // do nothing
    }

    Serial.println("Test message - signon.");

    while(true) {
    // do nothing
    }
}

void loop(void) {
    Serial.println("Line C.");
}

