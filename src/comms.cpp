// WARNING: This file WON'T compile with current settings as it redefines setup() and loop()
// Take a look at commit 723288a9bfd032b74af433fb4bc791f2669dbb0b (https://github.com/Laikata/Software/commit/723288a9bfd032b74af433fb4bc791f2669dbb0b)
// If you want this file to compile remove it from src_filter in platformio.ini

#include <Arduino.h>
#include <SoftwareSerial.h>

SoftwareSerial ss(10,11);

int count = 0;

void setup() {
  ss.begin(9600);
  ss.write("AT+P8");
}

void loop() {
  count += 1;
  ss.write(count);
  delay(1000);
}