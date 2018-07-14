/* Tiny interpreter,
   similar to myforth's Standalone Interpreter
   This example code is in the public domain */

// Sat 14 Jul 17:44:04 UTC 2018
// bitlab_cc.ino (and newer, when available)

// previous timestamps:
// Sun 17 Jun 03:03:26 UTC 2018
// 4737-a3a-0ca-

// No changes - runs well on ItsyBitsy M4 during brief testing.

/* Sat 12 May 04:30:48 UTC 2018 */
/* Sun Feb 26 04:53:32 UTC 2017 */

/* added: tick, EXECUTE by Chris_H */
/* correction to locate() by skiphs May 2018 */

/* Structure of a dictionary entry */
/* old:
  typedef struct {
  const char *name;
  void (*function)();
  } entry;
*/

/* new typedef:  (in support of new xt>adrs word) */
typedef void (*func)(void);         // signature of functions in dictionary

/* Structure of a dictionary entry */
typedef struct {
  const char*    name;              // Pointer the Word Name in flash
  const func     function;          // Pointer to function
} entry;

/* Data stack for parameter passing
   This "stack" is circular,
   like a Green Arrays F18A data stack,
   so overflow and underflow are not possible
   Number of items must be a power of 2 */
const int STKSIZE = 8;
const int STKMASK = 7;
int stack[STKSIZE];
int p = 0;

/* new declaration: tickstate */
int tickstate = 0; // used to differentiate keyboard input after a tick

/* new declaration: crlfstate */
int crlfstate = 0; // differentiate when ascii 13 is entered to the terminal


/* TOS is Top Of Stack */
#define TOS stack[p]
/* NAMED creates a string in flash */
#define NAMED(x, y) const char x[]=y

/* Terminal Input Buffer for interpreter */
const byte maxtib = 16;
char tib[maxtib];
/* buffer required for strings read from flash */
char namebuf[maxtib];
byte pos;

/* push n to top of data stack */
void push(int n) {
  p = (p + 1)& STKMASK;
  TOS = n;
}

/* return top of stack */
int pop() {
  int n = TOS;
  p = (p - 1)& STKMASK;
  return n;
}

/* new forward declaration: xtTOadrs */
NAMED(_xtTOadrs, "xt>adrs");
void xtTOadrs();

/* new word: tick */
NAMED(_tick, "\'");
void tick() {
  tickstate = -1; // true: tickstate flag has been raised
}

/* Global delay timer */
// int spd = 100; // 15;
int spd = 44;

/* top of stack becomes current spd */
NAMED(_speed, "speed");
void speed() {
  spd = pop();
}

/* discard top of stack */
NAMED(_drop, "drop");
void drop() {
  pop();
}

/* recover dropped stack item */
NAMED(_back, "back");
void back() {
  for (int i = 1; i < STKSIZE; i++) drop();
}

/* copy top of stack */
NAMED(_dup, "dup");
void dup() {
  push(TOS);
}

/* exchange top two stack items */
NAMED(_swap, "swap");
void swap() {
  int a;
  int b;
  a = pop();
  b = pop();
  push(a);
  push(b);
}

/* copy second on stack to top */
NAMED(_over, "over");
void over() {
  int a;
  int b;
  a = pop();
  b = pop();
  push(b);
  push(a);
  push(b);
}

/* add top two items */
NAMED(_add, "+");
void add() {
  int a = pop();
  TOS = a + TOS;
}

/* bitwise and top two items */
NAMED(_and, "and");
void and_() {
  int a = pop();
  TOS = a & TOS;
}

/* inclusive or top two items */
NAMED(_or, "or");
void or_() {
  int a = pop();
  TOS = a | TOS;
}

/* exclusive or top two items */
NAMED(_xor, "xor");
void xor_() {
  int a = pop();
  TOS = a ^ TOS;
}

/* invert all bits in top of stack */
NAMED(_invert, "invert");
void invert() {
  TOS = ~(TOS);
}

/* negate top of stack */
NAMED(_negate, "negate");
void negate() {
  TOS = -(TOS);
}

/* destructively display top of stack, decimal */
NAMED(_dot, ".");
void dot() {
  Serial.print(pop());
  Serial.print(" ");
}

/* destructively display top of stack, hex */
NAMED(_dotHEX, ".h");
void dotHEX() {
  Serial.print(0xffff & pop(), HEX);
  Serial.print(" ");
}

/* display whole stack, hex */
NAMED(_dotShex, ".sh");
void dotShex() {
  for (int i = 0; i < STKSIZE; i++) dotHEX();
}

/* display whole stack, decimal */
NAMED(_dotS, ".s");
void dotS() {
  for (int i = 0; i < STKSIZE; i++) dot();
}

/* new word: clearstack */
NAMED(_clearstack, "clearstack"); // ( n n n n ... -- ) 
void clearstack() {

//  (8 is the STKSIZE in the reference program)

  for (int i = 0; i < STKSIZE; i++) push(0);
  p = 0; // not especially required
}

/* delay TOS # of milliseconds */
NAMED(_delay, "delay");
void del() {
  delay(pop());
}

void defspeed() {
    if ((spd < 2) || (spd > 22333) ) { spd = 100; }
}

/* Toggle pin at TOS and delay(spd), repeat... */
NAMED(_wiggle, "wiggle");
void wiggle() {
  int a = pop();
  pinMode(a, OUTPUT);
  for (int i = 0; i < 20; i++) {
    digitalWrite(a, HIGH);
    defspeed();
    delay(spd); // delay(100);
    digitalWrite(a, LOW);
    delay(spd); // delay(100);
  }
}

/* TOS is pin number, set it HIGH */
NAMED(_high, "high");
void high() {
  digitalWrite(pop(), HIGH);
}

/* set TOS pin LOW */
NAMED(_low, "low");
void low() {
  digitalWrite(pop(), LOW);
}

/* read TOS pin */
NAMED(_in, "in");
void in() {
  TOS = digitalRead(TOS);
}

/* make TOS pin an input */
NAMED(_input, "input");
void input() {
  pinMode(pop(), INPUT);
}

/* make TOS pin an output */
NAMED(_output, "output");
void output() {
  pinMode(pop(), OUTPUT);
}

/* make TOS pin an input with weak pullup */
NAMED(_input_pullup, "input_pullup");
void input_pullup() {
  pinMode(pop(), INPUT_PULLUP);
}

/* dump 16 bytes of RAM in hex with ascii on the side */
void dumpRAM() {
  char buffer[5] = "";
  char *ram;
  int p = pop();
  ram = (char*)p;
  sprintf(buffer, "%4x", p);
  Serial.print(buffer);
  Serial.print("   ");
  for (int i = 0; i < 16; i++) {
    char c = *ram++;
    sprintf(buffer, " %2x", (c & 0xff));
    Serial.print(buffer);
  }
  ram = (char*)p;
  Serial.print("   ");
  for (int i = 0; i < 16; i++) {
    buffer[0] = *ram++;
    if (buffer[0] > 0x7f || buffer[0] < ' ') buffer[0] = '.';
    buffer[1] = '\0';
    Serial.print(buffer);
  }
  push(p + 16);
}

/* dump 256 bytes of RAM */
NAMED(_dumpr, "dump");
void rdumps() {
  for (int i = 0; i < 16; i++) {
    Serial.println();
    dumpRAM();
  }
}

/* End of Forth interpreter words */
/* ******************************************** */
/* Beginning of application words */




/* End of application words */
/* ******************************************** */
/* Now build the dictionary */

/* everybody loves a nop */
NAMED(_nopp, "nop");
void nopp() { } // pad the dictionary

/* empty words don't cause an error */
NAMED(_nop, " ");
void nop() { }

/* new word: throw */
NAMED(_throw, "throw");
void throw_();

/* new forward declaration: execadrs */
NAMED(_execadrs, "execadrs");
void execadrs(); // execute at address

/* new forward declaration: execXT */
NAMED(_execXT, "execxt"); // execute from an xt (execution token) lookup
void execXT();

/* new forward declaration: xtTOword */
NAMED(_xtTOword, "xt>word");
void xtTOword();

/* new forward declaration: EXECUTE */
NAMED(_EXECUTE, "EXECUTE"); // ( xt -- ) action: execute
void EXECUTE(); // ( xt -- ) action: execute

/* new forward declaration: xxt */
NAMED(_xxt, "xxt"); // alias for EXECUTE - execute execution token
void xxt();

/* Forward declaration required here */
NAMED(_words, "words");
void words();

/* new forward declaration: _entries */
NAMED(_entries_, "entries");
void _entries();

/* table of names and function addresses in flash */
const entry dictionary[] = {
  {_nop, nop},
  {_throw, throw_},      // new word
  {_xtTOword, xtTOword}, // new word
  {_xtTOadrs, xtTOadrs}, // new word
  {_execadrs, execadrs}, // new word
  {_execXT, execXT},     // new word
  {_EXECUTE, EXECUTE},   // new word
  {_xxt, xxt},           // new word
  {_words, words},
  {_tick, tick},         // new word
  {_entries_, _entries}, // new word
  {_drop, drop},
  {_dup, dup},
  {_back, back},
  {_swap, swap},
  {_over, over},
  {_add, add},
  {_and, and_},
  {_or, or_},
  {_xor, xor_},
  {_invert, invert},
  {_negate, negate},
  {_clearstack, clearstack},
  {_dotS, dotS},
  {_dotShex, dotShex},
  {_dot, dot},
  {_dotHEX, dotHEX},
  {_delay, del},
  {_high, high},
  {_low, low},
  {_in, in},
  {_input, input},
  {_output, output},
  {_input_pullup, input_pullup},
  {_wiggle, wiggle},
  {_dumpr, rdumps},
  {_speed, speed},
  {_nopp, nopp} // new word to pad dictionary
};

/* Number of words in the dictionary */
const int entries = sizeof dictionary / sizeof dictionary[0];

/* new word: xt>adrs */
void xtTOadrs() { // ( xt -- addrs )
  func function;
  // must be reflected at top of source
  // where the struct is
  int plc = pop();
  unsigned int adxrs; // REEXAMINE if adxrs cannot be adrs -- why uniqueness
  function = (func) pgm_read_word(&dictionary[plc].function);
  push((unsigned int) function);
  int a = pop();
  push(a - 1);
}

/* new word for xt execution */
void execadrs() { // ( adrs -- ) action: execute at adrs
  int a = pop();  // an address of a word's execution token
  push(a + 1);    // alignment (why?)
  func function = ((func) pop());
  function();
  // fix bottom of stack so that
  // adrs is reusable, with 'back EXECUTE':
  back(); push(1); negate(); add(); drop();
  // starts to look like forth, doesn't it.
}

/* new word for xt execution */
// execute from an xt (execution token) lookup
void execXT() { // ( xt -- ) action: execute    XT exec - the EXECUTE word
  xtTOadrs();  // ( xt -- addrs )
  execadrs();  // ( adrs -- ) action: execute at adrs
}

/* new alias for new word 'execXT' */
void EXECUTE() { // ( xt -- ) action: execute
  execXT();
}

/* short alias for EXECUTE:  xxt 'execute execution token' */
void xxt() {
  execXT();
}

/* Display all words in dictionary */
void words() {
  for (int i = entries - 1; i >= 0; i--) {
    strcpy(namebuf, dictionary[i].name);
    Serial.print(namebuf);
    Serial.print(" ");
  }
}

/* new word: entries */
/* How many words are in the dictionary? */
void _entries() {
  int a;
  a = entries;
  push(a);
}

/* new word: throw */
void throw_() {
  Serial.print("\r\n        THROW:  -1 \r\n");
}

/* new word: xtTOword */
/* Find an xt of a word in the dictionary, returning its name */
void xtTOword() { // ( xt -- ) print: the words's name from the dictionary
  int i = pop();
  int j = entries - 1;
  if ( i < 0 or i > j ) { // bounds checking please
    throw_();
    return;
  }
  strcpy(namebuf, dictionary[i].name);
  Serial.print(namebuf);
  Serial.print(" ");
}

/* Find a word in the dictionary, returning its position */
int locate() {
  for (int i = entries - 1; i >= 0; i--) {
    strcpy(namebuf, dictionary[i].name);
    if (!strcmp(tib, namebuf)) return i;
  }
  return 0;
}

/* Is the word in tib a number? */
int isNumber() {
  char *endptr;
  strtol(tib, &endptr, 0);
  if (endptr == tib) return 0;
  if (*endptr != '\0') return 0;
  return 1;
}

/* Convert number in tib */
int number() {
  char *endptr;
  return (int) strtol(tib, &endptr, 0);
}

char ch;

/* newly modified word: ok */
void ok() {
  if (crlfstate == -1) {
    Serial.print(" ok\r\n");
    crlfstate = 0;
  }
  // ORIG:  // if (ch == '\r') Serial.println("ok");
}

/* new word: printing */
void printing() {
  if (int(ch) == 13) {
    crlfstate = -1; // raise crlfstate TRUE
    Serial.print(" ");
  } else {
    if (int(ch) != 32) { // a space delimiter
      Serial.print(ch);
    } else { // it's a space
      Serial.print(' ');
    }
  }
}

/* newly modified function: reading */
/* Incrementally read command line from serial port */
/* new modifications support backspacing and other increased functionality */
byte reading() {
  if (!Serial.available()) return 1;
  ch = Serial.read();
  printing(); // new function new call
              // alternate: Serial.print(ch); // char-by-char input, echo
  if (ch == '\n') {
    Serial.print("\r\n"); // echo
    return 1;
  }
  if (ch == '\r') return 0;
  if (ch == ' ') return 0;
  if (ch == '\010') { // backspace
    if (pos == 0) throw_();
    tib[pos--] = 0;
    tib[pos] = 0;
    Serial.print(" ");
    Serial.print("\010");
    return 1;
  }
  if (pos < maxtib) {
    tib[pos++] = ch;
    tib[pos] = 0;
  }
  return 1;
}

/* newly modified function: readword */
/* Block on reading the command line from serial port */
/* then echo each word */
void readword() {
  pos = 0;
  tib[0] = 0;
  while (reading());
  // Serial.print(tib);
  //  Serial.print(" ");
}

/* newly modified function: runword */
/* Run a word via its name */
/* new modification to support xt and tick */

/* has a new stack effect:

   old: ( ? -- ? )
   new: ( ? -- ? n ) // push(place);

   TODO:

   stack effects management

   'place' is pushed to the stack, but
    only on some execution paths.  That's incorrect.

*/

void runword() {
  int place = locate();
  if (tickstate == -1) {
    tickstate = 0;
    push(place); // aha - our xt // NEW STACK EFFECT
    return;
  }



  /* new error-check: discipline 'place' */
  if ((place != 0) & (place < (entries - 1))) {
    dictionary[place].function();
    ok();
    return;
  }
  if (isNumber()) {
    push(number());
    ok();
    return;
  }
  Serial.println("?");
}

/* Arduino main setup and loop */

void setup() {
  Serial.begin(38400);
  while (!Serial);
  Serial.println ("Forth-like interpreter:");
  words();
  Serial.println();
}

void loop() {
  readword();
  runword();
}

