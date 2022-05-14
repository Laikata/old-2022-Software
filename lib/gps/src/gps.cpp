#include "gps.h"
#include <Arduino.h>
#include <TinyGPSPlus.h>
#include <vector.h>
#include <stdint.h>
#include <SoftwareSerial.h>

static const uint32_t GPSBaud = 9600;

// The TinyGPSPlus object
TinyGPSPlus gps;

SoftwareSerial gpsSerial (D6, D7);

vec3_t gps_position(){
    vec3_t pos = {gps.location.lng(), gps.location.lat(), gps.altitude.meters()};
    return pos;
}

int gps_satellites(){
    return gps.satellites.value();
}

void gps_init(){
    gpsSerial.begin(GPSBaud);
}
