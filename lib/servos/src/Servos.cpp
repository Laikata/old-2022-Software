/* 
This is the servo controller library. The functions allow us to controll the angle of the servos with a certain velocity.
They also allow us to attach or detach the servos, in order to consume less energy.
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

unsigned long interval = 10;

int velocity = 0;

int _rightPin;
int _leftPin;


Servos::Servos(int rightPin, int leftPin, int ServosVelocity) {
    velocity = ServosVelocity;
    _rightPin = rightPin; 
    _leftPin = leftPin;
}

void Servos::angleRight(int angle) {
    actualTime = millis();
    int _angle = map(angle, 0, 100, 30, 160);
    if(_angle > lastAngleRight && (actualTime - lastTimeRight) > interval){

        lastTimeRight = actualTime;
        servoRight.write(lastAngleRight + velocity);  
        lastAngleRight = lastAngleRight + velocity;

    }
    else if(_angle < lastAngleRight && (actualTime - lastTimeRight) > interval){

        lastTimeRight = actualTime;
        servoRight.write(lastAngleRight - velocity);   
        lastAngleRight = lastAngleRight - velocity;
        
    }
}

void Servos::angleLeft(int angle) {
    actualTime = millis();
    int _angle = map(angle, 0, 100, 160, 25);
    if(_angle > lastAngleLeft && (actualTime - lastTimeLeft) > interval){

        lastTimeLeft = actualTime;
        servoLeft.write(lastAngleLeft + velocity);  
        lastAngleLeft = lastAngleLeft + velocity;

    }
    else if(_angle < lastAngleLeft && (actualTime - lastTimeLeft) > interval){

        lastTimeLeft = actualTime;
        servoLeft.write(lastAngleLeft - velocity);   
        lastAngleLeft = lastAngleLeft - velocity;
        
    }
}

void Servos::attach(){
    servoRight.attach(_rightPin); 
    servoLeft.attach(_leftPin);
}

void Servos::detach(){
    servoRight.detach();
    servoLeft.detach();
}

//  :)