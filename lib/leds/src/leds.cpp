#include "leds.h"

#include <FastLED.h>

static CRGB leds[8];

void initLeds(){
    FastLED.addLeds<WS2812B, D5, RGB>(leds, 8);
}

void updateLeds(){
    float value = analogRead(A0);
    value = value*4.7f/1023;

    static unsigned long tNextLed = millis();
    static unsigned long tNextOff;

    if (millis() > tNextLed) {
        tNextLed = millis() + 2000;
        if (value >= 3.8) {
            leds[0] = CRGB::Green;
        }
        else if (value <= 3.6) {
            leds[0] = CRGB::Red;
        }
        else {
            leds[0] = CRGB::Yellow;
        }
        FastLED.show();
        tNextOff = millis() + 25;
    }

    if (millis() > tNextOff) {
        leds[0] = CRGB::Black;
        FastLED.show();
    }
}
