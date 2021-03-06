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

#include "SinePercussion.h"

void np2::synth::SinePercussion::patch(){
    addModuleInput("trig", triggers); 
    addModuleInput("pitch", sine.in_pitch()); 
    addModuleOutput("signal", gain ); 
    addModuleOutput("env", aEnv ); 

    aEnv.setReleaseCurve( 1.0f );

    //patching
    sine >> amp >> gain;
    triggers >> aEnv >> amp.in_mod();    
    triggers >> pEnv.set( 0.0f, 0.0f, 0.0f ) >> pModAmt >> sine.in_pitch();

    bTrig = false;

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
        m1 = i;
        return p;  
    };       

    osc.out_trig( address, 1 ) >> triggers;
    osc.out_trig( address, 1 ) * 0.3333f >> aEnv.in_hold();
    osc.out_trig( address, 1 ) * 0.6666f >> aEnv.in_release();
    osc.parser( address , 1) = [&]( float value ) noexcept {
        bTrig = true;
        value *= (1.0f/16.0f);
        value = (value<1.0) ? value : 1.0;
        value = value * value;
        value = 5 + value * 500.0f;
        return value;  
    };
    
    osc.out_value( address, 2 ) * 12.0f >> pModAmt.in_mod();

    osc.out_trig( address, 3 ) >> pEnv.in_release();
    osc.parser( address , 3 ) = [&]( float value ) noexcept {
        m2 = value;
        value *= (1.0f/16.0f);
        value = (value<1.0) ? value : 1.0;
        value = value * value;
        value = 5 + value * 500.0f;
        return value;  
    };
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

