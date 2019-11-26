
// SinePercussion.cpp
// Nicola Pisanti, MIT License, 2018

#include "SinePercussion.h"

void np2::synth::SinePercussion::patch(){
    addModuleInput("trig", triggers); 
    addModuleInput("pitch", sine.in_pitch()); 
    addModuleOutput("signal", gain ); 
    addModuleOutput("env", aEnv ); 

    //patching
    sine >> amp >> gain;
    triggers >> aEnv >> amp.in_mod();    
    triggers >> pEnv.set( 0.0f, 0.0f, 0.0f ) >> pModAmt >> sine.in_pitch();

    

    gainControl >> dBtoLin  >> gain.in_mod();
    
    
    envAttackControl  >> aEnv.in_attack();
    envHoldControl    >> aEnv.in_hold();
    envReleaseControl >> aEnv.in_release();

    parameters.setName("zap");
    parameters.add( gainControl.set("gain", -12, -48, 24) );
    parameters.add( envAttackControl.set("attack",  1, 0, 200 ) );
    parameters.add( envHoldControl.set( "hold",  0, 0, 300 ) );
    parameters.add( envReleaseControl.set("release", 5, 5, 600 ) );
    
}   

void np2::synth::SinePercussion::oscMapping( pdsp::osc::Input & osc, std::string address, np::tuning::ModalTable * table ){

    osc.out_value( address, 0 ) >> sine.in_pitch();
    osc.parser( address, 0 ) = [&, table]( float value ) noexcept {
        int i = value;
        float p = table->pitches[i%table->degrees];
        int o = i / table->degrees;
        p += o*12.0f;
        return p;  
    };       

    osc.out_trig( address, 1 ) >> triggers;
    osc.out_trig( address, 1 ) >> aEnv.in_hold();
    osc.out_trig( address, 1 ) >> pEnv.in_release();
    osc.parser( address , 1) = [&]( float value ) noexcept {
        value *= (1.0f/16.0f);
        value = (value<1.0) ? value : 1.0;
        value = value * value;
        value = 5 + value * 1000;
        return value;  
    };
    
    osc.out_value( address, 2 ) * 12.0f >> pModAmt.in_mod();
    
}

ofParameterGroup & np2::synth::SinePercussion::label( string name ){
    parameters.setName( name );
    return parameters;
}

void np2::synth::SinePercussion::enableDB( float minValue ){
    aEnv.enableDBTriggering( minValue, 0.0f);
}
void np2::synth::SinePercussion::disableDB( ){
    aEnv.disableDBTriggering( );
}

float np2::synth::SinePercussion::meter() const {
    return aEnv.meter_output();
}

pdsp::Patchable & np2::synth::SinePercussion::in_trig() {
    return in("trig");
}
pdsp::Patchable & np2::synth::SinePercussion::in_pitch() {
    return in("pitch");
}

pdsp::Patchable & np2::synth::SinePercussion::out_env() {
    return out("env");
}

