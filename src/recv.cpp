#include <Arduino.h>
#include <comms.h>


void setup() {
  comms_init();
}

char* result;

void loop() {
  if (incoming_pkg()) {
    Serial.println("Package incoming!");
    int packet_info = recv(&result);
    if (packet_info != 0) {
      Serial.print("Checksums don't match! ");
    }else{
      Serial.write(result);
    }
    
  }
}