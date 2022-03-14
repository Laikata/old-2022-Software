#include <Arduino.h>
#include <SoftwareSerial.h>
#include <comms.h>

unsigned char test_message[13]= {'s', 'h', 'o', 'r', 't', ' ', 'm', 'e', 's', 's', 'a', 'g', 'e'};

void setup() {
  comms_init();
}


void loop() {
  send(test_message, sizeof(test_message));
  delay(1000);
}