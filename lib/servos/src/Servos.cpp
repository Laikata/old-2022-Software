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
<<<<<<< HEAD
int velocity;


void servos_init(int rightPin, int leftPin, int ServosVelocity) {
    velocity = ServosVelocity;

    servoRight.attach(rightPin); //*Poner pin servo der.
    servoLeft.attach(leftPin);  //*Poner pin servo izq.

    servoRight.write(0);
    servoLeft.write(0);
}

void servos_angleRight(float angle) {
=======

void angleServoRight(float angle) {
>>>>>>> 8e5d4e3952ccfc51d2ce925feaf6f5ad4d030b85
    actualTime = millis();
    if(angle > lastAngleRight && (actualTime - lastTimeRight) > interval){

        lastTimeRight = actualTime;
<<<<<<< HEAD
        servoRight.write(lastAngleRight + velocity);  
        lastAngleRight = lastAngleRight + velocity;
=======
        servoRight.write(lastAngleRight + 5);  //* El numero indica cuantos grados quieres que mueva
        lastAngleRight = lastAngleRight + 5;
>>>>>>> 8e5d4e3952ccfc51d2ce925feaf6f5ad4d030b85

    }else if(angle < lastAngleRight && (actualTime - lastTimeRight) > interval){

        lastTimeRight = actualTime;
<<<<<<< HEAD
        servoRight.write(lastAngleRight - velocity);   
        lastAngleRight = lastAngleRight - velocity;
=======
        servoRight.write(lastAngleRight - 5);   //* El numero indica cuantos grados quieres que mueva
        lastAngleRight = lastAngleRight - 5;
>>>>>>> 8e5d4e3952ccfc51d2ce925feaf6f5ad4d030b85
        
    }
}

<<<<<<< HEAD
void servos_angleLeft(float angle) {
=======
void angleServoLeft(float angle) {
>>>>>>> 8e5d4e3952ccfc51d2ce925feaf6f5ad4d030b85
    actualTime = millis();
    if(angle > lastAngleLeft && (actualTime - lastTimeLeft) > interval){

        lastTimeLeft = actualTime;
<<<<<<< HEAD
        servoLeft.write(lastAngleLeft + velocity);  
        lastAngleLeft = lastAngleLeft + velocity;
=======
        servoLeft.write(lastAngleLeft + 5);  //* El numero indica cuantos grados quieres que mueva
        lastAngleLeft = lastAngleLeft + 5;
>>>>>>> 8e5d4e3952ccfc51d2ce925feaf6f5ad4d030b85

    }else if(angle < lastAngleLeft && (actualTime - lastTimeLeft) > interval){

        lastTimeLeft = actualTime;
<<<<<<< HEAD
        servoLeft.write(lastAngleLeft - velocity);   
        lastAngleLeft = lastAngleLeft - velocity;
=======
        servoLeft.write(lastAngleLeft - 5);   //* El numero indica cuantos grados quieres que mueva
        lastAngleLeft = lastAngleLeft - 5;
>>>>>>> 8e5d4e3952ccfc51d2ce925feaf6f5ad4d030b85
        
    }
  
  
}