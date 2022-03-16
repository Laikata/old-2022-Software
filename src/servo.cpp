// WARNING: This file WON'T compile with current settings as it redefines setup() and loop()
// Take a look at commit 723288a9bfd032b74af433fb4bc791f2669dbb0b (https://github.com/Laikata/Software/commit/723288a9bfd032b74af433fb4bc791f2669dbb0b)
// If you want this file to compile remove it from src_filter in platformio.ini

#include <Arduino.h>
#include <SoftwareSerial.h>
#include <Servo.h>
#include <servos.h>
#include <TinyGPSPlus.h>

#define GPSBaud 115200

SoftwareSerial ss;
TinyGPSPlus gps;

void setup()
{
  Serial.begin(115200);
  ss.begin(GPSBaud);

  Serial.println(F("DeviceExample.ino"));
  Serial.println(F("A simple demonstration of TinyGPSPlus with an attached GPS module"));
  Serial.print(F("Testing TinyGPSPlus library v. ")); Serial.println(TinyGPSPlus::libraryVersion());
  Serial.println(F("by Mikal Hart"));
  Serial.println();
}

void loop()
{
  // This sketch displays information every time a new sentence is correctly encoded.
  while (ss.available() > 0) {
    if (gps.encode(ss.read())) {
      // This function does not exist?
      //displayInfo();
    }
  }
    

  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
    while(true);
  }
}