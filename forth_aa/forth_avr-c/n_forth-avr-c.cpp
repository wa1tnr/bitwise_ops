#define DATESTAMP "Sat Jul 31 03:41:20 UTC 2021"

/* Includes Charley Shattuck's Tiny interpreter,
   similar to myforth's Standalone Interpreter
   Charley's example code is in the public domain */

#include <Arduino.h>

// verbatim:

/* Tiny interpreter,
   similar to myforth's Standalone Interpreter
   This example code is in the public domain */

/* Structure of a dictionary entry */
typedef struct {
    const char *name;
    void (*function)();
} entry;

/* NAMED creates a string in flash */
#define NAMED(x, y) const char x[]=y

/* Terminal Input Buffer for interpreter */
const byte maxtib = 64; // 16 may be more appropriate
char tib[maxtib];
/* buffer required for strings read from flash */
char namebuf[maxtib];
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

/* End of application words */
/* ******************************************** */
/* Now build the dictionary */

NAMED(_nop_nulled, "xjqcwk"); // unlikely string to encounter
void nop_nulled() {
    Serial.print(" you said squawkbox, my dude. ");
}

/* empty words don't cause an error */
NAMED(_nop_void, ""); // not sure what to do about this one
void nop_void() {
    Serial.print(" you said NO_SPACE yer Lordsh'p. ");
}

NAMED(_nop, "nop");
void nop() {
    Serial.print(" you said nop Cap'n. ");
}

NAMED(_nopp, "nopp");
void nopp() {
    Serial.print(" you said nopp Govn'r. ");
}

/* Forward declaration required here */
NAMED(_words, "words");
void words();

NAMED(_noppp, "noppp");
void noppp() {
    Serial.print(" you said noppp Your Grace. ");
}

int BASE;

NAMED(_bin, "bin");
void binary() {
    BASE = 2;
}

NAMED(_autobase, "autob");
void auto_base() {
    BASE = 0;
}

NAMED(_decimal, "decimal");
void decimal_base() {
    BASE = 10;
}

NAMED(_hex, "hex");
void hexadecimal() {
    BASE = 16;
}

NAMED(_octal, "octal");
void octal_base() {
    BASE = 8;
}

/* table of names and function addresses in flash */
const entry dictionary[] = {
    {_nop_nulled, nop_nulled},
    {_nop_void, nop_void},
    {_nop, nop},
    {_nopp, nopp},
    {_autobase, auto_base},
    {_bin, binary},
    {_decimal, decimal_base},
    {_hex, hexadecimal},
    {_octal, octal_base},
    {_words, words},
    {_noppp, noppp},
};

/* Number of words in the dictionary */
const int entries = sizeof dictionary / sizeof dictionary[0];

/* Display all words in dictionary */
void words() {
    for (int i = entries - 1; i >= 2; i--) {  // was >= 0
        strcpy(namebuf, dictionary[i].name);
        if (namebuf[0] != ' ') {
            if (namebuf[1] != '\0') {
                Serial.print(namebuf);
                Serial.print(' ');
                // Serial.print("  debug_words  ");
            }
        }
    }
}

/* Find a word in the dictionary, returning its position */
int locate() {
    for (int i = entries - 1; i >= 0; i--) {
        strcpy(namebuf, dictionary[i].name);
        if (!strcmp(tib, namebuf)) return i;
    }
    return 0;
}

/* Find a word in the dictionary, returning its position */
int locate_old() {
    return 0;
}


void triggered_binread(void) {
    BASE = 2;
    Serial.println("DEBUG: triggered_binread() seen.");
}

#define DDEBUG_LVL 0

/* Is the word in tib a number? */
int isNumber() {
    char *endptr;
    if (tib[0] == '0') {
        if (tib[1] == 'b') {
            triggered_binread(); // unused hook
        }
    }
    strtol(tib, &endptr, BASE);
    // if (0) {
    if (-1) {
        if (endptr == tib) return 0;
        if (*endptr != '\0') return 0;
    }
    if (DDEBUG_LVL == 2) {
        Serial.println("Fall-thru: isNumber() logic; it is a number.");
    }
    return 1;
}

#define TIB_LIMIT 0x7FFF // 32767 decimal

bool tib_outside_limits(void) {
    char* endptr;
    int result;
    // result = (int) strtol(tib, &endptr, 0);
    result = (int) strtol(tib, &endptr, BASE);
    if (result > 0x7FFF) {
        Serial.println("ERROR: OVER_RANGE");
        return -1;
    }
    if (result < 0) {
        Serial.println("ERROR: under zero"); // we don't handle negative numbers
        return -1;
    }
    return 0;
}

/* Convert number in tib */
int number(void) {
    char *endptr;
    bool over_limit;
    over_limit = tib_outside_limits();
    if (over_limit) {
        Serial.println("DEBUG: tib_outside_limits() result");
        return -1; // return a minus one and skip all base conversions
    }
    // above is part of a sieve and its placement matters
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
    Serial.println("ERROR in number()"); // don't remember ever seeing this triggered
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

    if (ch == EOL_CHAR) {
        Serial.print('\r');
    } // try to do CR without LF here
    if ((ch == EOL_CHAR) || (ch == ' ')) {
        return 0;
    }
    if ((ch == '\010') || (ch == '\177')) { // backspace or rubout
        if (ch == '\177') {
            Serial.print("\010"); // 0x08
            Serial.print(" mutt and jeff "); // 0x08
        }
        Serial.print(' ');
        Serial.print("\010"); // 0x08
        if (pos == 0) {
            return 1;
        }
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
    if (DDEBUG_LVL == 2) {
        Serial.println("  that was \'tib\'  for you.");
    }
}

/* Run a word via its name */
void runword(void) {
    int place = locate();
    if (DDEBUG_LVL == 2) {
        Serial.print("DEBUG: locate() was: ");
        Serial.print(place);
    }
    // Serial.println(); // fall thru, all DDBUG_LVL 's
    if (place != 0) {
        dictionary[place].function();
        ok();
        return;
    }
    // auto base kludge prior to all cases coded for
    if ((BASE != -99)  &&
        (BASE !=   2)  &&
        (BASE !=   8)  &&
        (BASE !=  10)  &&
        (BASE !=  16)) BASE =  0; // kludge

    if (BASE == -99) BASE =  0; // more kludge

    if (isNumber()) {
        push(number());
        ok();
        return;
    }
    Serial.println("?");
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
    BASE = -99;
}

void loop(void) {
    readword();
    runword();
    // runword_old();
}

// END.
