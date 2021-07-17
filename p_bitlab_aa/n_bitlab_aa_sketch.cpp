// n_bitlab_aa_sketch.cpp

// Sat Jul 17 15:01:18 UTC 2021

#include <Arduino.h>

#define QUICK_LED_BLINKS       5 // how many initial LED blinks
#define QUICK_INTER_BLINK    600 // was 200 ms now 600 ms OFF time
#define QUICK_BLINK_WIDTH     10 // 10 ms ON time
#define HU_XPCT_GUARD_BEGIN 4000 // begin: guard time to prep human expectation
#define HU_XPCT_GUARD_END   8000 // end:   guard time to prep human expectation

void quick_led_check(void) {
    delay(HU_XPCT_GUARD_BEGIN);
    for (int index = (QUICK_LED_BLINKS); index > 0; index--) {
        digitalWrite(LED_BUILTIN, 1); delay(QUICK_BLINK_WIDTH);
        digitalWrite(LED_BUILTIN, 0); delay(QUICK_INTER_BLINK);
    }
    delay(HU_XPCT_GUARD_END);
}

void init_gpio(void) {
    pinMode(LED_BUILTIN, 1); // OUTPUT
    digitalWrite(LED_BUILTIN, 0);
}

bool pin_state;

void cpl(int pin) {
    pin_state = digitalRead(LED_BUILTIN);
    pin_state = !pin_state;
    digitalWrite(LED_BUILTIN, pin_state);
}

void delayed(void) {
    if (pin_state != 0) {
        delay(10);
        return;
    }
    delay(3000);
}

void blink(void) {
    cpl(LED_BUILTIN);
    delayed(); // delay(500);
}

void init_serial(void) {
    Serial.begin(9600);
    while(!Serial) {
        blink();
    }
    delay(900);
    Serial.println(" init_serial();");
    Serial.println(" .. has met with success.  Target board ready.");
}

void setup(void) {
    // gpio serial init
    init_gpio();
    quick_led_check();
    init_serial();
}

void rhythm_b(void) {
    Serial.print("*");
    cpl(LED_BUILTIN);
    delay(100);
    cpl(LED_BUILTIN);
    delay(1000);
}

void rhythm_c(void) {
    Serial.print(".");
    cpl(LED_BUILTIN); delay(5);
    cpl(LED_BUILTIN); delay(400);
    Serial.print(".");
    cpl(LED_BUILTIN); delay(20);
    cpl(LED_BUILTIN); delay(4400);

}

int counted_loops = 12;

void loop(void) {
    if (counted_loops > 0) {
        counted_loops--;
        rhythm_b();
    }
    if (counted_loops == 0) {
        rhythm_c();
    }
}

// END.
