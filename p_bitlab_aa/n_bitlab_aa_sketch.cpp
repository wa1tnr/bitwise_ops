// n_bitlab_aa_sketch.cpp

// Sat Jul 17 12:17:13 UTC 2021

#include <Arduino.h>

void init_serial(void) {
}

void init_gpio(void) {
    pinMode(LED_BUILTIN, 1); // OUTPUT
}

bool pin_state;

void cpl(int pin) {
    pin_state = digitalRead(LED_BUILTIN);
    pin_state = !pin_state;
    digitalWrite(LED_BUILTIN, pin_state);
}

void blink(void) {
    cpl(LED_BUILTIN);
    delay(500);
}

void setup(void) {
    // gpio serial init
    init_gpio();
}

void loop(void) {
    blink();
}

// compiles as-is. ;)

// END.
