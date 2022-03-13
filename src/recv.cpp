#include <Arduino.h>
#include <SoftwareSerial.h>

SoftwareSerial ss(10,11);

int count = 0;

void setup() {
  Serial.begin(115200);
  ss.begin(9600);
  //ss.write("AT+P8");
  Serial.println("Hello");
}

void loop() {
  if (ss.available()) {
    Serial.write(ss.read());
  }
}