#include "gps.h"
#include <Arduino.h>
#include <TinyGPSPlus.h>
#include <vector.h>
#include <SoftwareSerial.h>

static const int RXPin = D5, TXPin = 3;
static const uint32_t GPSBaud = 9600;

// The TinyGPSPlus object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

vec3_t *getPosition(){
    vec3_t *pos = vec3_init(gps.location.lng(), gps.location.lat(), gps.altitude.meters());
    return pos;
}

int getSatellites(){
  return gps.satellites.value();
}

void gpsInit(){
  ss.begin(GPSBaud);
}