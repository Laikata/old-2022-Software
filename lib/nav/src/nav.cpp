#include "gps.h"
#include <Arduino.h>
#include <TinyGPSPlus.h>
#include <vector.h>
#include <SoftwareSerial.h>
#include <stdint.h>
#include <math.h> 

#define PI 3.14159265

float nav_angle(vec3_t pos, vec3_t target, float dir) {
    return angleBetweenPoints(pos, target) + dir;
}

float angleBetweenPoints(vec3_t p1, vec3_t p2) {
    return atan2(p2.y - p1.y, p2.x - p1.x) * 180 / PI;
}