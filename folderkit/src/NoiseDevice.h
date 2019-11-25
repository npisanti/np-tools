
#pragma once

#include "ofParameterGroup.h"
#include "pdspCore.h"
#include "envelopes/AHR.h"
#include "modules/filters/VAFilter.h"
#include "random/TriggeredRandom.h"
#include "ofx/Parameter.h"
#include "ofx/ParameterAmp.h"
#include "utility/DBtoLin.h"
#include "modules/sources/BitNoise.h"
#include "ModalTable.h"

namespace np2 { namespace synth {
    
class NoiseDevice : public pdsp::Patchable {

public:
    NoiseDevice(){ patch(); };
    NoiseDevice(const NoiseDevice & other){ patch(); };
    
    ofParameterGroup parameters;

    float meter() const;
    
    pdsp::Patchable & in_trig();
    pdsp::Patchable & in_mod();
    pdsp::Patchable & in_hold();
    pdsp::Patchable & in_release();

    pdsp::Patchable & ch( size_t index );

    ofParameterGroup & label( string name );

    void oscMapping( pdsp::osc::Input & osc, std::string address, np::tuning::ModalTable * table );
    
    

private:
    void patch();
    
    pdsp::BitNoise          noise;

    pdsp::AHR               env;
    pdsp::Amp               amp0;
    pdsp::Amp               amp1;
    pdsp::PatchNode         trigger;
    
    pdsp::VAFilter             filter;
    pdsp::Parameter            filterTypeControl;
    pdsp::Parameter            filterResoControl;
    
    pdsp::Parameter            noiseDecimateControl;
    
    pdsp::Amp                  modFilterAmt;
    
    pdsp::Parameter            lowcutControl;


    pdsp::Parameter            bitsControl;
    
    pdsp::LowCut               lowcut;
    

    pdsp::Amp           gain0;
    pdsp::Amp           gain1;
    pdsp::DBtoLin       dBtoLin;
    pdsp::Parameter     gainControl;  

};

}} // end namespaces
