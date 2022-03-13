#include <Arduino.h>
#include <SoftwareSerial.h>
#include <comms.h>

void setup() {
  comms_init();
}

unsigned char test_message[14]= "short message";

void loop() {
  send(test_message, sizeof(test_message));
  delay(1000);
}