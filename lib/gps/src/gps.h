#pragma once
#include <vector.h>
#include <Arduino.h>
#include <TinyGPSPlus.h>
#include <stdint.h>
#include <comms.h>
#include <SoftwareSerial.h>

vec3_t gps_position();
int gps_satellites();
void gps_init();
