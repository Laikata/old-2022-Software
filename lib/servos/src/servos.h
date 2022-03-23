#pragma once
#include <Arduino.h>
#include <Servo.h>
#include <servos.h>

void servos_angleRight(float angle);
void servos_angleLeft(float angle);

void servos_init(int rightPin, int leftPin, int ServosVelocity);
