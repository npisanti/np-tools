
#pragma once

#include "ofMain.h"
#include "ofxPDSP.h"
#include "HQSaturator.h"
#include "ModalTable.h"

namespace np { namespace synth {

class FMSub : public pdsp::Patchable {

public:

    // synth public API --------------------------------------

    FMSub(){ patch(); }
    FMSub( const FMSub& other ){ patch(); }

    pdsp::Patchable& in_trig();
    pdsp::Patchable& in_pitch();

    
    float meter_mod_env() const;
    float meter_pitch() const;

    ofParameterGroup    parameters;

    ofParameterGroup & label (std::string name );

    void oscMapping( pdsp::osc::Input & osc, std::string address, np::tuning::ModalTable * table );

    void patch();
    // ------- parameters ------------------------------------

    pdsp::PatchNode     voiceTrigger;
    pdsp::PatchNode     pitchNode;
    
    pdsp::FMOperator    carrierA;
    pdsp::FMOperator    carrierB;
    pdsp::FMOperator    modulator;

    pdsp::Amp           fmAmp;
    pdsp::Amp           voiceAmp;

    pdsp::AHR           ampEnv;
    pdsp::Parameter         envAttackControl;
    pdsp::Parameter         envReleaseControl;

    pdsp::AHR           modEnv;
    pdsp::Parameter         modEnvAttackControl;
    pdsp::Parameter         modEnvHoldControl;
    pdsp::Parameter         modEnvReleaseControl;            

    pdsp::Amp               driftAmt;
    pdsp::LFOPhazor         phazorFree;
    pdsp::TriggeredRandom   rnd;
    pdsp::OnePole           randomSlew;

    pdsp::Amp  fm_mod;

    pdsp::Parameter     ratio_ctrl;

    pdsp::Parameter     drift;
    
    pdsp::Parameter     detune_ctrl;
    pdsp::Parameter     osc_mix;
    
    pdsp::LinearCrossfader oscXFader;

    np2::effect::HQSaturator saturator;
};

}} // end namspaces 
