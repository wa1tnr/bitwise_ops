// Sat 14 Jul 04:47:39 UTC 2018
// bitlab_aa.ino

// target MCU: Adafruit Feather M0 Express

// any SAMD21 (M0+) or SAMD51 (M4) MCU will work; probably
// AVR's as well.

void setup(void) {
    int x = 3;
    int n = 2;
    int a = 0; // accumulator stand-in

    Serial.begin(9600);
    while(!Serial) {
        // do nothing
    }
    Serial.println("\r\n Example A:\r\n");
}

void loop(void) {
}

