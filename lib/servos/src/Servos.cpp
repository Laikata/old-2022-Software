
//#include <Arduino.h>
#include <Servo.h>
#include <servos.h>

Servo servoRight;
Servo servoLeft;

float lastAngleRight = 0;
float lastAngleLeft = 0;

unsigned long actualTime = 0;
unsigned long lastTimeLeft = 0;
unsigned long lastTimeRight = 0;

unsigned long interval = 10;



int velocity = 0;


void servos_init(int rightPin, int leftPin, int ServosVelocity) {
    velocity = ServosVelocity;

    servoRight.attach(rightPin); //*Poner pin servo der.
    servoLeft.attach(leftPin);  //*Poner pin servo izq.

    servoRight.write(0);
    servoLeft.write(0);
}

void servos_angleRight(float angle) {
    actualTime = millis();
    if(angle > lastAngleRight && (actualTime - lastTimeRight) > interval){

        lastTimeRight = actualTime;
        servoRight.write(lastAngleRight + velocity);  
        lastAngleRight = lastAngleRight + velocity;

    }
    else if(angle < lastAngleRight && (actualTime - lastTimeRight) > interval){

        lastTimeRight = actualTime;
        servoRight.write(lastAngleRight - velocity);   
        lastAngleRight = lastAngleRight - velocity;
        
    }
}

void servos_angleLeft(float angle) {
    actualTime = millis();
    if(angle > lastAngleLeft && (actualTime - lastTimeLeft) > interval){

        lastTimeLeft = actualTime;
        servoLeft.write(lastAngleLeft + velocity);  
        lastAngleLeft = lastAngleLeft + velocity;

    }
    else if(angle < lastAngleLeft && (actualTime - lastTimeLeft) > interval){

        lastTimeLeft = actualTime;
        servoLeft.write(lastAngleLeft - velocity);   
        lastAngleLeft = lastAngleLeft - velocity;
        
    }
}