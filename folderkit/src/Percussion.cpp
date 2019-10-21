/*====================================================================

	folderkit - companion drumkit for orca-c
  
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

#include "Percussion.h"

void np::synth::Percussion::patch(){
    addModuleInput("trig", trigger); 
    addModuleInput("pitch", osc.in_pitch()); 
    addModuleOutput("signal", gain ); 
    addModuleOutput("L", panner.out_L() ); 
    addModuleOutput("R", panner.out_R() ); 
    addModuleOutput("send_delay", sendDel ); 
    addModuleOutput("send_rev", sendRev ); 

    //patching
    osc >> sineamp >> filter >> amp >> solo >> gain >> panner;
    gain >> sendDel;
    gain >> sendRev;
    noise >> noiseamp >> filter >> amp;
    noise >> noiseFM >> osc.in_fm();
    noise >> rm.in_mod();
    osc >> rm >> noiseRM >> filter;
    osc >> oscMod >> noise.in_pitch();
    solo.set(1.0f);
    
    
    trigger >> aEnv >> amp.in_mod();
    trigger >> mEnv.set(0.0f, 0.0f,  50.0f);

    envAttackControl  >> aEnv.in_attack();
    envHoldControl    >> aEnv.in_hold();
    envReleaseControl >> aEnv.in_release();

    veloToEnv          >> mEnv.in_velocity();    
    mEnvAttackControl  >> mEnv.in_attack();
    mEnvReleaseControl >> mEnv.in_release();
    
    filterTypeControl   >> filter.in_mode();
    filterCutoffControl >> filter.in_pitch();
    filterResoControl   >> filter.in_reso();

    trigger >> noise.in_trig();
    trigger >> random0 >> randomPitch >> noise.in_pitch();
    trigger >> random1 >> randomDecimate >> noise.in_decimation();
    trigger >> random2 >> randomCutoff >> filter.in_cutoff();

    pitchControl >> osc.in_pitch(); 
    fbControl >> osc.in_fb();
    noisePitchControl    >> noise.in_pitch();
    noiseDecimateControl >> noise.in_decimation();
    bitsControl >> noise.in_bits();
    
    mEnv >> fbModControl >> osc.in_fb();
    mEnv >> oscPitchModControl >> osc.in_pitch();
    mEnv >> modNoisePitchAmt >> noise.in_pitch();
    mEnv >> modDecimateAmt >> noise.in_decimation();
    mEnv >> modFilterAmt >> filter.in_pitch();
    
    panControl >> panner.in_pan();

    parameters.setName("percussion");

    oscGroup.setName("osc");    
        oscGroup.add( sineamp.set("amp", 1.0f, 0.0f, 1.0f ));
        oscGroup.add( pitchControl.set("pitch", 36.0f, 0.0f, 132.0f ));
        oscGroup.add( fbControl.set( "feedback", 0.0f, 0.0f, 0.5f ) );
        oscGroup.add( noiseFM.set( "noise_fm", 0.0f, 0.0f, 1.0f ) );
    parameters.add( oscGroup );
    
    noiseGroup.setName("noise");
        noiseGroup.add( noiseamp.set("amp", 0.0f, 0.0f, 1.0f ));
        noiseGroup.add( noiseRM.set( "ring_mod", 0.0f, 0.0f, 1.0f ) );
        noiseGroup.add( noisePitchControl.set ("pitch", -36.0f, -36.0f, 148.0f) );
        noiseGroup.add( oscMod.set ("osc_mod", 0.0f, 0.0f, 48.0f) );
        noiseGroup.add( noiseDecimateControl.set ("decimate", 160.0f, 1.0f, 160.0f) );
        noiseGroup.add( bitsControl.set ("bits", 24, 24, 1) );
    parameters.add( noiseGroup );
    
    filterGroup.setName("filter");
        filterGroup.add( filterTypeControl.set("mode", 0, 0, 5 ) );

        filterGroup.add( filterCutoffControl.set("cutoff", 136.0f, 20.0f, 136.0f ) );
        filterGroup.add( filterResoControl.set("reso", 0.0f, 0.0f, 1.0f ) );
    parameters.add( filterGroup );
    
    ampEnvelope.setName("amp envelope");
        ampEnvelope.add( envAttackControl.set("attack",  3, 0, 6000 ) );
        ampEnvelope.add( envHoldControl.set( "hold",  40, 0, 6000 ) );
        ampEnvelope.add( envReleaseControl.set("release", 20, 5, 6000 ) );
    parameters.add( ampEnvelope );

    modEnvelope.setName("mod envelope");
        modEnvelope.add( mEnvAttackControl.set("attack",  0, 0, 6000 ) );
        modEnvelope.add( mEnvReleaseControl.set("release", 50, 0, 6000 ) );
        modEnvelope.add( veloToEnv.set( "dynamics", 0.5f, 0.0f, 1.0f ) );
        modEnvelope.add( fbModControl.set( "to_osc_fb", 0.f, -1.f, 1.f ) );
        modEnvelope.add( oscPitchModControl.set( "to_osc",  48.0f, -86.0f, 86.0f ) );
        modEnvelope.add( modNoisePitchAmt.set("to_noise_pitch", 0.0f, -120.0f, 120.0f ) );    
        modEnvelope.add( modDecimateAmt.set("to_decimation", 0.0f, -120.0f, 120.0f ) );    
        modEnvelope.add( modFilterAmt.set("to_filter", 0.0f, -120.0f, 120.0f ) );  
        modEnvelope.add( randomControl.set("random_noise", 0.0f, 0.0f, 66.6f ) );    
        modEnvelope.add( randomCutoffControl.set("random_cutoff", 0.0f, 0.0f, 66.6f ) );    
    parameters.add( modEnvelope );
        
    mixer.setName("mixer");
        mixer.add( gain.set("gain", -12.0f, -48.0f, 24.0f) );
        mixer.add( panControl.set("pan", 0.0f, -1.0f, 1.0f) );
        mixer.add( sendDel.set("send_to_delay", -48.0f, -48.0f, 24.0f) );
        mixer.add( sendRev.set("send_to_reverb", -48.0f, -48.0f, 24.0f) );
    parameters.add( mixer );
        
    aEnv.enableDBTriggering( -24.0f, 0.0f);
}   

ofParameterGroup & np::synth::Percussion::label( string name ){
    parameters.setName( name );
    return parameters;
}

float np::synth::Percussion::meter() const {
    return aEnv.meter_output();
}

pdsp::Patchable & np::synth::Percussion::in_trig() {
    return in("trig");
}
pdsp::Patchable & np::synth::Percussion::in_pitch() {
    return in("pitch");
}

