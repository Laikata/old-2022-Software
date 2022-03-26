#include <Arduino.h>
#include <servos.h>



void setup(){
    servos_init(0, 3, 5);
    Serial.begin(9600);
}

void loop(){
    servos_angleRight(Serial.read());
}