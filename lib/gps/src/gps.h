#pragma once
#include <vector.h>
#include <Arduino.h>
#include <TinyGPSPlus.h>
#include <stdint.h>
#include <comms.h>
#include <SoftwareSerial.h>

dvec3_t gps_position();
int gps_satellites();
void gps_init();
void gps_update();
