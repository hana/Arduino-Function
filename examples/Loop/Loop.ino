#include "MaxyFunction.h"

Maxy::Function function;

void setup() {
    Serial.begin(115200);
    
    function.add(0.3, 1000);
    function.add(1.0, 2000);

    delay(1000);

    function.begin(true);   // if the loop is on, the initial value is set to the last value.
}

void loop() {
    Serial.println("Time: " + String(millis() * 0.001) + ", Value: " + String(function.get()));
}
