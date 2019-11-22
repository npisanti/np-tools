
#pragma once

#include "ofxPDSP.h"

namespace np{

class FlipSwitch : public pdsp::Formula {

public:
    FlipSwitch();

    void init( bool value );
    
private:
    bool gate;
        
    float formula(const float &x) noexcept override;
    
    void formulaAudioRate(float* &output, const float* &input, const int &bufferSize) noexcept override;
    
};

}
