#pragma once

class LowPassFilter {

    private: 
        float prevValue = 0;
        float prevProcessedValue = 0;

    public:
        float low_pass(float value) ;
};