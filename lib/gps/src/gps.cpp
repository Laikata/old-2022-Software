#include "gps.h"
#include <Arduino.h>
#include <TinyGPSPlus.h>
#include <vector.h>
#include <stdint.h>

static const uint32_t GPSBaud = 9600;

// The TinyGPSPlus object
TinyGPSPlus gps;

vec3_t gps_position(){
    vec3_t pos = {gps.location.lng(), gps.location.lat(), gps.altitude.meters()};
    return pos;
}

int gps_satellites(){
    return gps.satellites.value();
}

void gps_init(){
    Serial.begin(GPSBaud);
}
