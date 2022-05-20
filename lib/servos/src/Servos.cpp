/* 
This is the servo controller library. The following functions are capable to controling the angle of the servos with a certain velocity.
It can also attach or detach the servos, in order to consume less energy.
*/

//
#include <Arduino.h>
#include <Servo.h>
#include <servos.h>

Servo servoRight;
Servo servoLeft;

int lastAngleRight = 0;
int lastAngleLeft = 0;

unsigned long actualTime = 0;
unsigned long lastTimeLeft = 0;
unsigned long lastTimeRight = 0;

unsigned long interval = 1;

int velocity = 0;

#define RIGHT_PIN D0
#define LEFT_PIN D3


Servos::Servos(int ServosVelocity) {
    velocity = ServosVelocity;
}

void Servos::angleRight(int angle) {
    Serial.printf("AngleRightBeforeMAP: %i\n", angle);
    actualTime = millis();
    angle = constrain(map(angle, 0, 100, 30, 160),30,160);
    Serial.printf("AngleRightAfterMAP: %i\n", angle);
    if((actualTime - lastTimeRight) > interval){

        lastTimeRight = actualTime;
        servoRight.write(constrain(angle,lastAngleRight-velocity,lastAngleRight+velocity));  
        lastAngleRight = angle;

    }
}

void Servos::angleLeft(int angle) {
    Serial.printf("AngleRightBeforeMAP: %i\n", angle);
    actualTime = millis();
    angle = constrain(map(angle, 0, 100, 160, 25), 25, 160);
    Serial.printf("AngleRightAfterMAP: %i\n", angle);
    if((actualTime - lastTimeLeft) > interval){

        lastTimeLeft = actualTime;
        servoLeft.write(constrain(angle,lastAngleLeft-velocity,lastAngleLeft+velocity));  
        lastAngleLeft = angle;

    }
    
}

void Servos::attach(){
    servoRight.attach(RIGHT_PIN); 
    servoLeft.attach(LEFT_PIN);
    this->attached = true;
}

void Servos::detach(){
    servoRight.detach();
    servoLeft.detach();
    this->attached = false;
}

bool Servos::isAttached(){
    return this->attached;
}

//  :)