#include <Arduino.h>
#include <TinyGPSPlus.h>
#include <stdint.h>
#include <math.h> 
#include "LowPass.h"


float LowPassFilter::low_pass(float value)
{
    float processedValue = 0.6*prevProcessedValue + 0.1*prevValue + 0.3*value;
    prevValue = value;
    prevProcessedValue = processedValue;

    return processedValue;
}
