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

#include "Sampler.h"

void folderkit::Sampler::patch (){
    
    float sample_drift_amount = 0.05f;
    float comb_drift_amount = 0.06f;
    
    addModuleInput("clip_input", inputControl );
    addModuleInput("clip_threshold", clip.in_threshold() );
    addModuleInput("clip_output", outputControl );

    addModuleOutput("signal", outputGainStage );

    addModuleOutput("L", pan.out_L() );
    addModuleOutput("R", pan.out_R() );
    
    env.set( 0.0f, 0.0f, 0.0f );
    0.0f >> env.in_velocity();
    0.5f >> comb.in_damping();

    triggers >> samplerDrift.in_trig();
    triggers >> env      >> amp.in_mod();
    
    flips[0].init( true );
    for( int i=0; i<2; ++i ){
        triggers >> flips[i] >> fadeEnvs[i].in_trig();
        float fadeout = 15.0f;
        0.0f >>fadeEnvs[i].in_attack();
        1.0f >>fadeEnvs[i].in_sustain();
        fadeout >> fadeEnvs[i].in_decay();
        fadeout >> fadeEnvs[i].in_release();
        fadeEnvs[i] >> fadeAmps[i].in_mod();
        samplers[i] >> fadeAmps[i] >> amp;
        flips[i] >> samplers[i];
        
        samplerDrift * sample_drift_amount >> samplers[i].in_pitch();
        selectNode >> samplers[i].in_select();
        startNode >> samplers[i].in_start();
    }

    0.2f    >> phazorFree;
    0.05f  >> randomSlew.in_freq();   
    phazorFree.out_trig() >> rnd >> randomSlew * comb_drift_amount >> comb.in_pitch(); 
    amp >> comb;
                           
    comb >> lowcut >> inputGainStage;
    lowCutControl >> lowcut.in_freq();

    outputGainStage >> pan;
    outputGainStage >> rms;

    clip.setOversampleLevel(2);
    inputGainStage >> upsampler >> clip >> downsampler >> outputGainStage;
    inputControl >> inputGainStage.in_mod();
    outputControl >> outputGainStage.in_mod();


    rms.attackControl.getOFParameterInt() = 0;
    rms.releaseControl.getOFParameterInt() = 800;

    parameters.setName( "rename me");

    parameters.add( lowCutControl.set("low cut hz", 30, 20, 500) );

    pLibrary = nullptr;
    tPast = 0;
    cursor = 0;
    instrument = 0;
}

ofParameterGroup & folderkit::Sampler::label( std::string name ){
    parameters.setName( name );
    return parameters;
}

pdsp::Patchable & folderkit::Sampler::in_trig() {
    return in("trig");
}

pdsp::Patchable & folderkit::Sampler::in_select() {
    return in("select");
}

pdsp::Patchable & folderkit::Sampler::in_pitch() {
    return in("pitch");
}

pdsp::Patchable & folderkit::Sampler::ch( int i ){
    switch( i ){
        case 0: return out("L"); break;
        case 1: return out("R"); break;
        default: break;
    }
    return out("L");
}

void folderkit::Sampler::linkToLibrary( folderkit::Library & library ){
    
    std::cout<<"[folderkit::Sampler] adding "<<library.samples.size()<<" samples\n";
    
    for( size_t i=0; i<library.samples.size(); ++i ){
        samplers[0].addSample( library.samples[i], 0 );
        samplers[1].addSample( library.samples[i], 0 );
    }
    
    pLibrary = &library;
}

void folderkit::Sampler::oscMapping( pdsp::osc::Input & osc, std::string address, np::tuning::ModalTable * table ){
    
    osc.out_trig( address, 0 ) >> selectNode;
    osc.out_trig( address, 0 ) >> triggers;
    osc.parser( address , 0) = [&]( float value ) noexcept {
        //std::cout<<"received message\n";
        instrument = value;
        if( value!=0.0f && pLibrary->availables[instrument] ){
            pLibrary->correlate( instrument, value );
            return value;
        }else{
            return pdsp::osc::Ignore;
        }
    };

    osc.out_value( address, 1 ) >> samplers[0].in_pitch();
    osc.out_value( address, 1 ) >> samplers[1].in_pitch();
    osc.parser( address, 1 ) = [&, table]( float value ) noexcept {
        int i = value;
        float p = table->pitches[i%table->degrees];
        int o = i / table->degrees;
        if( i > 24 ){
            o-=3;
        }
        p += o*12.0f;
        p -= table->base;
        return p;  
    };       

    osc.out_value( address, 2 ) * (1.0f/32.0f) >> startNode;
    
    osc.out_value( address, 3 ) >> pan.in_pan();
    osc.parser(address, 3) = [&]( float value ) noexcept {
        int pan = value;
        if( pan > 9 ) pan = 9;
        switch( pan ){
            default: case 0: case 5: return 0.0f; break;
            case 1: return -1.0f; break;
            case 2: return -0.75f; break;
            case 3: return -0.5f; break;
            case 4: return -0.25f; break;
            case 6: return 0.25f; break;
            case 7: return 0.5f; break;
            case 8: return 0.75f; break;
            case 9: return 1.0f; break;
        }
    };      

    osc.out_value( address, 4 ) >> env.in_attack();
    osc.parser(address, 4) = [&]( float value ) noexcept {
        value *= (1.0f/16.0f);
        value = (value<1.0) ? value : 1.0;
        value = value * value;
        value = value * 1000;
        return value;               
        
    };      
        
    osc.initTo( address, 5, 10000.0f );
    osc.out_value( address, 5 ) >> env.in_release();
    osc.parser(address, 5) = [&]( float value ) noexcept {
        if( value == 0.0f ){
            return 7500.0f;
        }else{
            value *= (1.0f/16.0f);
            value = (value<1.0) ? value : 1.0;
            value = value * value;
            value = value * 2000;
            return value;  
        }
    };      

    osc.out_value( address, 6 ) >> comb.in_feedback();
    osc.parser(address, 6) = [&]( float value ) noexcept {
        value *= (1.0f/9.0f);
        value = (value<1.0f) ? value : 1.0f;
        value = 1.0f-value;
        value = value * value;
        value = 1.0f-value;      
        value *= 0.999f;  
        return value;  
    };      
    
    osc.out_value( address, 7 ) >> comb.in_pitch();
    osc.parser( address, 7 ) = [&, table]( float value ) noexcept {
        int i = value;
        float p = table->pitches[i%table->degrees];
        int o = i / table->degrees;
        p += o*12.0f;
        return p;  
    };          

}

int folderkit::Sampler::checkTrigger(){
    int trigs = env.meter_triggers();
    if( tPast != trigs ){
        tPast = trigs;
        return instrument;
    }
    return -1;
}

