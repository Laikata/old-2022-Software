#include "gps.h"


static const uint32_t GPSBaud = 9600;

// The TinyGPSPlus object
TinyGPSPlus gps;

SoftwareSerial gpsSerial (D6, D7);

void displayInfo();

vec3_t gps_position(){
    while(gpsSerial.available()>0){
        if (gps.encode(gpsSerial.read() ) )
            displayInfo();
    }
    vec3_t pos = {gps.location.lng(), gps.location.lat(), gps.altitude.meters()};
    Serial.printf("GPS_DIRECTO: (%g, %g, %g)\n", gps.location.lng(), gps.location.lat(), gps.altitude.meters());
    return pos;
}

int gps_satellites(){
    return gps.satellites.value();
}

void gps_init(){
    gpsSerial.begin(GPSBaud);
}

void displayInfo()
{
  comms_debug("Location: "); 
  if (gps.location.isValid())
  {
    comms_debug(gps.location.lat(), 6);
    comms_debug(F(","));
    comms_debug(gps.location.lng(), 6);
  }
  else
  {
    comms_debug(F("INVALID"));
  }

  comms_debug(F("  Date/Time: "));
  if (gps.date.isValid())
  {
    comms_debug("%c", gps.date.month());
    comms_debug(F("/"));
    comms_debug(gps.date.day());
    comms_debug(F("/"));
    comms_debug(gps.date.year());
  }
  else
  {
    comms_debug(F("INVALID"));
  }

  comms_debug(F(" "));
  if (gps.time.isValid())
  {
    if (gps.time.hour() < 10) comms_debug(F("0"));
    comms_debug(gps.time.hour());
    comms_debug(F(":"));
    if (gps.time.minute() < 10) comms_debug(F("0"));
    comms_debug(gps.time.minute());
    comms_debug(F(":"));
    if (gps.time.second() < 10) comms_debug(F("0"));
    comms_debug(gps.time.second());
    comms_debug(F("."));
    if (gps.time.centisecond() < 10) comms_debug(F("0"));
    comms_debug(gps.time.centisecond());
  }
  else
  {
    comms_debug(F("INVALID"));
  }
}