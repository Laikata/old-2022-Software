#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <leds.h>

Adafruit_NeoPixel g_Led(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

void leds_init(){
    g_Led.begin();
}

void leds_clear(){
    g_Led.clear();
}

void leds_set(){
    for(int i = 0; i < LED_COUNT; i++) {
        g_Led.setPixelColor(i, g_Led.Color(0, 150, 0));
        g_Led.show();
        delay(500);
    }
}