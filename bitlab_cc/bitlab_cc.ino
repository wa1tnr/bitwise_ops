// Sat 14 Jul 17:44:04 UTC 2018
// bitlab_cc.ino

// target MCU: Adafruit Feather M0 Express

// any SAMD21 (M0+) or SAMD51 (M4) MCU will work; probably
// AVR's as well.

// previous timestamp:
// Sat 14 Jul 06:47:15 UTC 2018

void example_A_bins(void) {
    int x = 3;
    int n = 2;
    int a = 0; // accumulator stand-in

    Serial.println("\r\n Example A:\r\n");

    Serial.print("  x = ");
    Serial.print(x);

    Serial.println("         0000 0011");

    Serial.print("  n = ");
    Serial.println(n);

    a = x << n;

    Serial.println("");

    Serial.println("  a = x << n");

    Serial.print("  a = ");
    Serial.print(a);

    Serial.println("        0000 1100");
}

void example_B_bins(void) {
    int x = 3; // seed value, our 'interesting number' to be modified
    int n = 5; // powers of two -- how many places to the left we bit-shift 'x'
    int a = 0; // accumulator stand-in

    Serial.println("\r\n");
    Serial.println("                              7    6    5    4      3    2    1    0");
    Serial.println("                            128   64   32   16      8    4    2    1\r\n");
    Serial.println(" Example B: \r\n");

    Serial.print("  x = ");
    Serial.print(x);

    Serial.println("         0000 0011");

    Serial.print("  n = ");
    Serial.println(n);

    a = x << n;

    Serial.println("");

    Serial.println("  a = x << n");

    Serial.print("  a = ");
    Serial.print(a);

    Serial.println("        0110 0000");
}

void setup(void) {
    Serial.begin(9600);
    while(!Serial) {
        // do nothing
    }

    Serial.println("\r\n // bitlab_cc.ino ");
    example_A_bins();
    delay(7000); // nice long delay
    example_B_bins();

    Serial.print("\r\n\r\n END. "); // let them know there is no more.
    while(true) {
    // do nothing
    }
    Serial.println("Line B."); // never reached
}

void loop(void) {
    Serial.println("Line C.");
}

