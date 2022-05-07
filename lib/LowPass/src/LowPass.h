#pragma once

class LowPass {

    private: 
        float prevValue = 0;
        float prevProcessedValue = 0;

    public:
        float low_pass(float value) ;
};
