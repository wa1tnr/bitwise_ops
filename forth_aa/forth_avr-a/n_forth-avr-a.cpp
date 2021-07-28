// Wed Jul 28 16:41:52 UTC 2021
#include <Arduino.h>
void setup(void) {
    Serial.begin(115200);
    while(!Serial);
    delay(3000);
    Serial.println("here.");
}
void loop(void) {
    // while(-1);
    while(0);
    delay(5000);
    Serial.println("Fall-through.  ERROR.");
}
