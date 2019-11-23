
// FMSub.cpp
// Nicola Pisanti, MIT License, 2018

#include "FMSub.h"

void np::synth::FMSub::patch(){
    
    addModuleInput("trig", voiceTrigger);
    addModuleInput("pitch", pitchNode);
    addModuleInput("modulator", modulator.in_pitch() );
    addModuleInput("hold", ampEnv.in_hold() );
    addModuleOutput("signal", saturator);

    pitchNode >> carrierA.in_pitch();
    pitchNode >> carrierB.in_pitch();
    
    detune_ctrl * -1.0f >> carrierA.in_pitch();
    detune_ctrl         >> carrierB.in_pitch();

    ratio_ctrl >> modulator.in_ratio();

    // SIGNAL PATH
    modulator >> fmAmp >> carrierA.in_fm() >> oscXFader.in_A();
                 fmAmp >> carrierB.in_fm() >> oscXFader.in_B();
                 
    osc_mix >> oscXFader.in_fade();
    
    //oscXFader >> voiceAmp >> gain;
    oscXFader >> voiceAmp >> saturator;
    
    // MODULATIONS AND CONTROL
    modEnv >> fm_mod >> fmAmp.in_mod();

    
    voiceTrigger >> ampEnv >> voiceAmp.in_mod();
    
    envAttackControl  >> ampEnv.in_attack();
    envReleaseControl >> ampEnv.in_release();
    
    voiceTrigger >> modEnv;
    modEnvAttackControl  >> modEnv.in_attack();
    modEnvHoldControl    >> modEnv.in_hold();
    modEnvReleaseControl >> modEnv.in_release();

    0.2f    >> phazorFree;
    0.05f  >> randomSlew.in_freq();
                                         drift >> driftAmt.in_mod();        
    phazorFree.out_trig() >> rnd >> randomSlew >> driftAmt;
                                                  driftAmt >> carrierA.in_pitch();
                                                  driftAmt >> carrierB.in_pitch();
                                                  driftAmt >> modulator.in_pitch();

 
    // CONTROLS ---------------------------------------------------------------
    parameters.setName( "fm sub" );
    
    parameters.add( osc_mix.set("carrier mix", 0.0f, 0.0f, 1.0f) );
    parameters.add( detune_ctrl.set("detune", 0.0f, 0.0f, 0.5f) );
    
    parameters.add( ratio_ctrl.set( "ratio coarse", 1, 1, 13) );
    parameters.add( ratio_ctrl.set( "ratio fine", 0.0f, -1.0f, 0.0f) );

    parameters.add( drift.set("drift", 0.0f, 0.0f, 1.0f) );    
    
    parameters.add( envAttackControl.set("attack",  3, 0, 200 ) );
    parameters.add( envReleaseControl.set("release", 20, 5, 600 ) );

    parameters.add( modEnvAttackControl.set("mod attack",  0, 0, 200 ) );
    parameters.add( modEnvHoldControl.set( "mod hold",  40, 0, 300 ) );
    parameters.add( modEnvReleaseControl.set("mod release", 50, 5, 600 ) );    
    parameters.add( saturator.parameters );
    
}

void np::synth::FMSub::oscMapping( pdsp::osc::Input & osc, std::string address, np::tuning::ModalTable * table ){
    
    osc.out_trig( address, 0 ) >> voiceTrigger;
    osc.out_trig( address, 0 ) >> ampEnv.in_hold();
    osc.parser( address , 0) = [&]( float value ) noexcept {
        if( value == 0.0f ) {
            return pdsp::osc::Ignore;
        }
        return value * pdsp::Clockable::getOneBarTimeMs() * (1.0f/16.0f);
    };
    
    osc.out_value( address, 1 ) >> pitchNode;
    osc.out_value( address, 1 ) >> pitchNode;
    osc.parser( address, 1 ) = [&, table]( float value ) noexcept {
        int i = value;
        float p = table->pitches[i%table->degrees];
        int o = i / table->degrees;
        p += o*12.0f;
        return p;  
    };       
    
    osc.out_value( address, 2 ).enableSmoothing(50.0f);
    osc.out_value( address, 2 ) * (1.0f/40.0f) >> carrierA.in_fb();
    osc.out_value( address, 2 ) * (1.0f/40.0f) >> carrierB.in_fb();
    
    osc.out_value( address, 3 ) * (1.0f/8.0f) >> fm_mod.in_mod();
    
}

float np::synth::FMSub::meter_mod_env() const{
    return ampEnv.meter_output();
}

float np::synth::FMSub::meter_pitch() const{
    return modulator.meter_pitch();
}

pdsp::Patchable& np::synth::FMSub::in_trig(){
    return in("trig");
}

pdsp::Patchable& np::synth::FMSub::in_pitch(){
    return in("pitch");
}


ofParameterGroup & np::synth::FMSub::label (std::string name ){
    parameters.setName( name );
    return parameters;
}
