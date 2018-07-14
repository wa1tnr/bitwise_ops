// Sat 14 Jul 21:44:53 UTC 2018
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

/* Terminal Input Buffer for interpreter */
const byte maxtib = 16;
char tib[maxtib];

byte pos;
char ch;

void kludge_report_a(void) { // scaffolding - will go away
  if (ch == '\r') {
      Serial.print("\r\n"); // echo
  } else {
      delay(80); Serial.print(ch); // temporary; slow program to human speed
  }
}

void xkludge_report_a(void) { // renamed - to place outside program
    Serial.print(ch); Serial.print(" . ");
}

/* Incrementally read command line from serial port */
byte reading(void) {
  if (!Serial.available()) return 1;
  ch = Serial.read();
  kludge_report_a(); // scaffolding - will go away
}

/* Block on reading the command line from serial port */
/* then echo each word */
void readword() {
    pos = 0;
    tib[0] = 0;
    while (reading());
}

void setup(void) {
    Serial.begin(9600);
    while (!Serial);
    Serial.println ("Forth-like interpreter:");

    Serial.println("Test message - signon.");
    Serial.println("\r\n // bitlab_dd.ino ");
}

void loop(void) {
    Serial.println("Line C - type something:");
    readword();
}

