#define DATESTAMP "Fri Jul 30 03:06:20 UTC 2021"

/* Includes Charley Shattuck's Tiny interpreter,
   similar to myforth's Standalone Interpreter
   Charley's example code is in the public domain */

#include <Arduino.h>

/* Terminal Input Buffer for interpreter */
const byte maxtib = 64; // 16 may be more appropriate
char tib[maxtib];

byte pos;
char ch;

int dW; // debug working

void debugShowNumber(void) {
    int n = dW;
    Serial.print("n is:  0b");
    Serial.print(n, BIN); Serial.print("  \\");
    Serial.print(n, OCT); Serial.print("  ");
    Serial.print(n, DEC); Serial.print("  0x");
    Serial.print(n, HEX); Serial.println();
}

/* push n to top of data stack */
void push(int n) {
    dW = n;
    debugShowNumber();
}

int BASE;

void triggered_binread(void) {
    BASE = 2;
}

/* Is the word in tib a number? */
int isNumber() {
  char *endptr;
  if (tib[0] == '0') {
    if (tib[1] == 'b') {
      triggered_binread();
    }
  }
  strtol(tib, &endptr, BASE);
  // if (0) {
  if (-1) {
    if (endptr == tib) return 0;
    if (*endptr != '\0') return 0;
  }
  Serial.println("Fall-thru: isNumber() logic; it is a number.");
  return 1;
}

/* Convert number in tib */
int number(void) {
    char *endptr;
    if (BASE ==  10) {
        Serial.print(" in BASE 10: ");
        return (int) strtol(tib, &endptr, 10);
    }
    if (BASE ==  8) {
        Serial.print(" in BASE  8: ");
        return (int) strtol(tib, &endptr,  8);
    }
    if (BASE == 16) {
        Serial.print(" in BASE 16: ");
        return (int) strtol(tib, &endptr, 16);
    }
    if (BASE ==  2) {
        Serial.print(" in BASE  2: ");
        return (int) strtol(tib, &endptr,  2);
    }
    if (BASE == 0) {
        Serial.print(" in BASE  0: ");
        return (int) strtol(tib, &endptr,  0);
    }
    Serial.println("ERROR in number()");
}

#define EOL_CHAR '\n'

void ok() {
  if (ch == EOL_CHAR) Serial.println("ok");
}

/* Incrementally read command line from serial port */
byte reading() {
    if (!Serial.available()) return 1;
    ch = Serial.read();
    Serial.print(ch); // keystroke echo.  OPTIONAL.

    if (ch == EOL_CHAR) { Serial.print('\r'); } // try to do CR without LF here
    if ((ch == EOL_CHAR) || (ch == ' ')) { return 0; }
    if ((ch == '\010') || (ch == '\177')) { // backspace or rubout
        if (ch == '\177') {
            Serial.print("\010"); // 0x08
            Serial.print(" mutt and jeff "); // 0x08
        }
        Serial.print(' ');
        Serial.print("\010"); // 0x08
        if (pos == 0) { return 1; }
        tib[pos--] = 0;
        tib[pos] = 0;
        return 1; // continue reading keystrokes for this word's name &c.
    } // backspace or rubout

    if (pos < maxtib) {
        tib[pos++] = ch;
        tib[pos] = 0;
    }
    // individual, non-special compositional keystrokes
    // fall through to here:
    return 1;
}

/* Block on reading the command line from serial port */
/* then echo each word */
void readword() {

    pos = 0;
    tib[0] = 0;

    while (reading());

    Serial.print(tib);

    Serial.print(" ");
    Serial.println("  that was \'tib\'  for you.");
}

void runword(void) {
    BASE =  0;
    if (isNumber()) {
        push(number());
    }
    BASE = 10;
    if (isNumber()) {
        push(number());
    }
    BASE = 16;
    if (isNumber()) {
        push(number());
    }
    BASE =  8;
    if (isNumber()) {
        push(number());
    }
    BASE =  2;
    if (isNumber()) {
        push(number());
    }
    ok();
    return;
}

bool pin_state;

int wasted;

void cpl(int pin) {
    wasted = pin;
    pin_state = digitalRead(LED_BUILTIN);
    pin_state = !pin_state;
    digitalWrite(LED_BUILTIN, pin_state);
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
    delayed();
}

void init_gpio(void) {
    pinMode(LED_BUILTIN, 1);
    digitalWrite(LED_BUILTIN, 0);
}

void init_serial(void) {
    Serial.begin(115200);
    while(!Serial) {
        blink();
    }
}

void setup(void) {
    init_gpio();
    init_serial();
    delay(30); // vestigal - may be removed
    Serial.println("here.");
    Serial.println(DATESTAMP);
    Serial.print("__LED_BUILTIN__ ");
    Serial.println(wasted); // temporary - delete me soon
}

void loop(void) {
    readword();
    runword();
}

// END.
