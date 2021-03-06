// Sat 14 Jul 05:09:18 UTC 2018
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

    Serial.print("  x = ");
    Serial.println(x);

    Serial.print("  n = ");
    Serial.println(n);

    a = x << n;

    Serial.println("");

    Serial.println("  a = x << n");
    // Serial.println("  <--- suppressed operation ");

    Serial.print("  a = ");
    Serial.println(a);


    while(true) {
    // do nothing
    }
    Serial.println("Line B."); // never reached
}

void loop(void) {
    Serial.println("Line C.");
}

