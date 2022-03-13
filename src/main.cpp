#include <Arduino.h>
#include <SoftwareSerial.h>
#include <comms.h>

void setup() {
  comms_init();
}

unsigned char* test_message = (unsigned char*) "This is a test message: all characters here should be transmitted succesfully";

void loop() {
  send(test_message, sizeof(test_message));
  delay(1000);
}