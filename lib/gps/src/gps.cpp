#include "gps.h"


static const uint32_t GPSBaud = 9600;

// The TinyGPSPlus object
extern TinyGPSPlus gps;

SoftwareSerial gpsSerial (D6, D7);

void displayInfo();

vec3_t gps_position(){
    vec3_t pos = {gps.location.lng(), gps.location.lat(), gps.altitude.meters()};
    #ifdef DEBUG
    Serial.printf("GPS_DIRECTO: (%g, %g, %g)\n", gps.location.lng(), gps.location.lat(), gps.altitude.meters());
    #endif
    return pos;
}

void gps_update(){
  while(gpsSerial.available()>0){
      if (gps.encode(gpsSerial.read() ) ){
          //displayInfo();
      }
  }
}

int gps_satellites(){
    return gps.satellites.value();
}

void gps_init(){
  gpsSerial.begin(GPSBaud);
  //TODO: Disable unneeded data
}

void displayInfo()
{
  comms_debug("Location: "); 
  if (gps.location.isValid())
  {
    comms_debug("%lf", gps.location.lat());
    comms_debug(",");
    comms_debug("%lf", gps.location.lng());
  }
  else
  {
    comms_debug("INVALID");
  }

  comms_debug("  Date/Time: ");
  if (gps.date.isValid())
  {
    comms_debug("%c", gps.date.month());
    comms_debug("/");
    comms_debug("%c", gps.date.day());
    comms_debug("/");
    comms_debug("%c", gps.date.year());
  }
  else
  {
    comms_debug("INVALID");
  }

  comms_debug(" ");
  if (gps.time.isValid())
  {
    if (gps.time.hour() < 10) comms_debug("0");
    comms_debug("%c", gps.time.hour());
    comms_debug(":");
    if (gps.time.minute() < 10) comms_debug("0");
    comms_debug("%c", gps.time.minute());
    comms_debug(":");
    if (gps.time.second() < 10) comms_debug("0");
    comms_debug("%c", gps.time.second());
    comms_debug(".");
    if (gps.time.centisecond() < 10) comms_debug("0");
    comms_debug("%c", gps.time.centisecond());
  }
  else
  {
    comms_debug("INVALID");
  }
}