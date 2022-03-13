#include <Arduino.h>
#include <SoftwareSerial.h>
#include <comms.h>

void setup() {
  comms_init();
}

void loop() {
  unsigned char* test_message = (unsigned char*) "This is a test message: all characters here should be transmitted succesfully";
  send(test_message, sizeof(test_message));
  delay(1000);
}