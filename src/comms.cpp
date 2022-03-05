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