/*====================================================================

	substrata - companion drumkit for orca-c
  
	Copyright (c) 2019 Nicola Pisanti <nicola@npisanti.com>

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program. If not, see <http://www.gnu.org/licenses/>.

====================================================================*/

#include "FMSub.h"

void np::synth::FMSub::patch(){
    
    addModuleInput("trig", voiceTrigger);
    addModuleInput("pitch", pitchNode);
    addModuleInput("modulator", modulator.in_pitch() );
    addModuleOutput("signal", saturator);

    modEnv.setReleaseCurve( 1.0f );
    ampEnv.setReleaseCurve( 1.0f );


    pitchNode >> carrierA.in_pitch();
    pitchNode >> carrierB.in_pitch();
    pitchNode >> modulator.in_pitch();
    
    detune_ctrl * -1.0f >> carrierA.in_pitch();
    detune_ctrl         >> carrierB.in_pitch();


    // SIGNAL PATH
    modulator >> fmAmp >> carrierA.in_fm() >> oscXFader.in_A();
                 fmAmp >> carrierB.in_fm() >> oscXFader.in_B();
                 
    osc_mix >> oscXFader.in_fade();
    
    //oscXFader >> voiceAmp >> gain;
    oscXFader >> voiceAmp >> highcut >> saturator;
    
    highcutControl >> highcut.in_freq();
    
    // MODULATIONS AND CONTROL
    modEnv >> fm_mod >> fmAmp.in_mod();

    
    voiceTrigger >> ampEnv >> voiceAmp.in_mod();
    
    envAttackControl  >> ampEnv.in_attack();
    1.0f  >> ampEnv.in_sustain();
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

    parameters.add( drift.set("drift", 0.0f, 0.0f, 1.0f) );    
    
    parameters.add( envAttackControl.set("attack",  3, 0, 200 ) );
    parameters.add( envReleaseControl.set("release", 20, 5, 600 ) );

    parameters.add( modEnvAttackControl.set("mod attack",  0, 0, 200 ) );
    parameters.add( modEnvHoldControl.set( "mod hold",  40, 0, 300 ) );
    parameters.add( modEnvReleaseControl.set("mod release", 50, 5, 600 ) );    
    parameters.add( saturator.parameters );
    parameters.add( highcutControl.set("high_cut", 200, 20, 20000) );
    
    bTrig = false;
    bPitch = false;
    gate = false;
}

void np::synth::FMSub::oscMapping( pdsp::osc::Input & osc, std::string address, np::tuning::ModalTable * table ){
    
    osc.out_value( address, 0 ) >> pitchNode;
    osc.parser( address, 0 ) = [&, table]( float value ) noexcept {
        if( value == 33.0f ) return pdsp::osc::Ignore;
        int i = value;
        if( m1 != i ){
            bPitch = true;
        }
        m1 = i;
        float p = table->pitches[i%table->degrees];
        int o = i / table->degrees;
        p += o*12.0f;
        return p;  
    };   

    osc.out_trig( address, 1 ) >> ampEnv.in_attack();
    osc.out_trig( address, 1 ) >> ampEnv.in_release();
    osc.parser( address , 1) = [&]( float value ) noexcept {
        return value * pdsp::Clockable::getOneBarTimeMs() * (0.5f/16.0f);
    };
    
    osc.out_trig( address, 2 ) >> voiceTrigger;
    osc.parser( address , 2) = [&]( float value ) noexcept {
        if( gate && value == 33.0f ){
            gate = false;
            return 0.0f;
        }else if( !gate && value!=33.0f ){
            gate = true;
            bTrig = true;
            return 1.0f;
        }else{
            return pdsp::osc::Ignore;
        }
    };

    osc.out_value( address, 3 ) >> modulator.in_ratio();
    osc.parser( address , 3) = [&]( float value ) noexcept {
        if( value == 0.0f ){
            return 0.5f;
        }else{
            return value;
        }
    };
    
    osc.out_value( address, 4 ) * (1.0f/16.0f) >> fm_mod.in_mod();

    osc.out_value( address, 5 ) >> modEnv.in_release();
    osc.parser( address , 5) = [&]( float value ) noexcept {
        return value * pdsp::Clockable::getOneBarTimeMs() * (0.5f/16.0f);
     };
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
