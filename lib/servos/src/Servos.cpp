/*
// Si el comentario es "//*" significa anotacion personal que luego sera removida, no es un comentario.
*/
//HOLA



#include <Arduino.h>
#include <Servo.h>
#include <servos.h>

Servo servoRight;
Servo servoLeft;

float lastAngleRight = 0;
float lastAngleLeft = 0;

unsigned long actualTime;
unsigned long lastTimeLeft = 0;
unsigned long lastTimeRight = 0;

unsigned long interval = 10;

/*
PONER EN INITS

servoRight.attach(9); //*Poner pin servo der.
servoLeft.attach(9);  //*Poner pin servo izq.

servoRight.write(0);
servoLeft.write(0);

*/

void angleServoRight(float angle) {
    actualTime = millis();
    if(angle > lastAngleRight && (actualTime - lastTimeRight) > interval){

        lastTimeRight = actualTime;
        servoRight.write(lastAngleRight + 5);  //* El numero indica cuantos grados quieres que mueva
        lastAngleRight = lastAngleRight + 5;

    }else if(angle < lastAngleRight && (actualTime - lastTimeRight) > interval){

        lastTimeRight = actualTime;
        servoRight.write(lastAngleRight - 5);   //* El numero indica cuantos grados quieres que mueva
        lastAngleRight = lastAngleRight - 5;
        
    }
}

void angleServoLeft(float angle) {
    actualTime = millis();
    if(angle > lastAngleLeft && (actualTime - lastTimeLeft) > interval){

        lastTimeLeft = actualTime;
        servoLeft.write(lastAngleLeft + 5);  //* El numero indica cuantos grados quieres que mueva
        lastAngleLeft = lastAngleLeft + 5;

    }else if(angle < lastAngleLeft && (actualTime - lastTimeLeft) > interval){

        lastTimeLeft = actualTime;
        servoLeft.write(lastAngleLeft - 5);   //* El numero indica cuantos grados quieres que mueva
        lastAngleLeft = lastAngleLeft - 5;
        
    }
  
  
}