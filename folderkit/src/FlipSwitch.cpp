
#include "FlipSwitch.h"


np::FlipSwitch::FlipSwitch(){
    gate = false;
}

void np::FlipSwitch::init( bool value ){
    gate = value;
}

float np::FlipSwitch::formula(const float &x) noexcept {
    if( pdsp::checkTrigger(x) ){
        gate = !gate;
        if( gate ){
            return 1.0f;
        }else{
            return PDSP_TRIGGER_OFF;
        }
    }
    return x;
}

void np::FlipSwitch::formulaAudioRate(float* &output, const float* &input, const int &bufferSize) noexcept {

    ofx_Aeq_Zero( output, bufferSize );
    
    for( int n=0; n<bufferSize; ++n ){
        if( pdsp::checkTrigger( input[n] ) ){
            gate = !gate;
            if( gate ){
                output[n] = 1.0f;
            }else{
                output[n] = PDSP_TRIGGER_OFF;
            }
        }
    }
}
