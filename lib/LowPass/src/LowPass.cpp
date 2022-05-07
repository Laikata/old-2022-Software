#include <Arduino.h>
#include <TinyGPSPlus.h>
#include <stdint.h>
#include <math.h> 
#include "LowPass.h"


float LowPass::low_pass(float value)
{
    float processedValue = 0.8*prevProcessedValue + 0.1*prevValue + 0.1*value;
    prevValue = value;
    prevProcessedValue = processedValue;

    return processedValue;
}
