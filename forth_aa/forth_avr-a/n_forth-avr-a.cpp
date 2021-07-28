#define DATESTAMP "Wed Jul 28 18:08:10 UTC 2021"

/* Includes Charley Shattuck's Tiny interpreter,
   similar to myforth's Standalone Interpreter
   Charley's example code is in the public domain */

#include <Arduino.h>

void setup_gpio(void) { }

void blink(void) { }

void setup(void) {
    setup_gpio();
    Serial.begin(115200);
    while(!Serial) { blink(); }
    delay(3000);
    Serial.println("here.");
    Serial.println(DATESTAMP);
}
void loop(void) {
    Serial.println("Entering loop.  LAST message, EVER, seen.");
    while(-1);
    // while(0);
    delay(5000);
    Serial.println("Fall-through.  ERROR.");
}

// END.
