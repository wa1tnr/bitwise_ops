// Sat 14 Jul 19:49:03 UTC 2018
// bitlab_ddsketch.cpp

/* Charley Shattuck's

   Tiny interpreter,
   similar to myforth's Standalone Interpreter
   This example code is in the public domain */

// was:
// bitlab_cc.ino

// previous timestamp:
// Sat 14 Jul 19:05:26 UTC 2018

// target MCU: Adafruit Feather M0 Express

#include <Arduino.h>
#include "bitlab_ddsketch.h"

void setup(void) {
    Serial.begin(9600);
    while (!Serial);
    Serial.println ("Forth-like interpreter:");

    Serial.println("Test message - signon.");
    Serial.println("\r\n // bitlab_dd.ino ");
    Serial.print("\r\n\r\n END. "); // let them know there is no more.

    while(true) {
    // do nothing
    }
    Serial.println("Line B."); // never reached
}

void loop(void) {
    Serial.println("Line C.");
}

