#define DATESTAMP "Wed Jul 28 18:22:26 UTC 2021"

/* Includes Charley Shattuck's Tiny interpreter,
   similar to myforth's Standalone Interpreter
   Charley's example code is in the public domain */

#include <Arduino.h>

void init_gpio(void) {
    pinMode(LED_BUILTIN, 1);
    digitalWrite(LED_BUILTIN, 0);
}

bool pin_state;

int wasted;

void cpl(int pin) {
    int test_del = pin;
    wasted = test_del;
    // pin_state = digitalRead(LED_BUILTIN);
    // pin_state = !pin_state;
    // digitalWrite(LED_BUILTIN, pin_state);
}

void delayed(void) {
    if (pin_state != 0) {
        delay(10);
        return;
    }
    delay(3000); // harmless - waiting for serial
}

void blink(void) {
    cpl(LED_BUILTIN);
    delayed(); // delay(500);
}

void init_serial(void) {
    Serial.begin(115200);
    while(!Serial) { blink(); }
}

void setup(void) {
    init_gpio();
    init_serial();
    delay(3000);
    Serial.println("here.");
    Serial.println(DATESTAMP);
}
void loop(void) {
    Serial.print("__LED_BUILTIN__ ");
    Serial.println(wasted); // temporary - delete me soon
    Serial.println("Entering loop.  LAST message, EVER, seen.");
    while(-1);
    // while(0);
    delay(5000);
    Serial.println("Fall-through.  ERROR.");
}

// END.
