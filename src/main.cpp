#include <Arduino.h>
#include <SoftwareSerial.h>
#include <comms.h>

void setup() {
  comms_init();
}

unsigned char test_message[13]= {'s', 'h', 'o', 'r', 't', ' ', 'm', 'e', 's', 's', 'a', 'g', 'e'};

void loop() {
  send(test_message, sizeof(test_message));
  delay(1000);
}