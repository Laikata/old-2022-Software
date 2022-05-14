#include "leds.h"

#include <FastLED.h>

void initLeds(){
    FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, 8);
}

void updateLeds(){

}