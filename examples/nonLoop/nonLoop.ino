#include "MaxyFunction.h"

Maxy::Function function;

void setup() {
    Serial.begin(115200);
    
    function.add(1.0, 1000);
    function.add(0.5, 500);
    function.add(0.6, 2000);
    function.add(0.6, 1000);
    function.add(0.3, 100);

    delay(1000);
    function.begin();
}

void loop() {
    Serial.println("Time: " + String(millis() * 0.001) + ", Value: " + String(function.get()));
}
