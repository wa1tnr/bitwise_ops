#define DATESTAMP "Wed Jul 28 20:15:35 UTC 2021"

/* Includes Charley Shattuck's Tiny interpreter,
   similar to myforth's Standalone Interpreter
   Charley's example code is in the public domain */

#include <Arduino.h>

/* Terminal Input Buffer for interpreter */
const byte maxtib = 16;
char tib[maxtib];

byte pos;
char ch;

void init_gpio(void) {
    pinMode(LED_BUILTIN, 1);
    digitalWrite(LED_BUILTIN, 0);
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
    delayed(); // delay(500);
}

/* Incrementally read command line from serial port */
byte reading() {
  if (!Serial.available()) return 1;
  ch = Serial.read();
  Serial.print(ch); // scaffolding. Not the desired output.  temporary.

  if ((ch == '\n')|| (ch == ' ')) return 0;

  if (pos < maxtib) {
    tib[pos++] = ch;
    tib[pos] = 0;
  }
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
    Serial.print("__LED_BUILTIN__ ");
    Serial.println(wasted); // temporary - delete me soon
}

void loop(void) {
    readword();
    // Serial.println("___ end of loop - readword() returned.");
/*
    Serial.println("Entering loop.  LAST message, EVER, seen.");
    while(-1);
    delay(5000);
    Serial.println("Fall-through.  ERROR.");
*/
}

// END.
