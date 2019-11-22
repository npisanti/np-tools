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
    float comb_drift_amount = 0.07f;
    
    addModuleInput("clip_input", inputControl );
    addModuleInput("clip_threshold", clip.in_threshold() );
    addModuleInput("clip_output", outputControl );

    addModuleOutput("signal", outputGainStage );

    addModuleOutput("L", pan.out_L() );
    addModuleOutput("R", pan.out_R() );
    
    env.set( 0.0f, 0.0f, 0.0f );
    0.0f >> env.in_velocity();
    0.5f >> comb.in_damping();
    
    samplerDrift * sample_drift_amount >> sampler.in_pitch();
    
    triggers >> samplerDrift.in_trig();
    triggers >> sampler  >> amp;
    triggers >> env      >> amp.in_mod();

    0.2f    >> phazorFree;
    0.05f  >> randomSlew.in_freq();   
    phazorFree.out_trig() >> rnd >> randomSlew * comb_drift_amount >> comb.in_pitch(); 

    sampler >> amp >> comb;
                           
#ifdef SAMPLER_USE_LOWCUT
    comb >> lowcut >> inputGainStage;
    lowCutControl >> lowcut.in_freq();
#else
    comb >>inputGainStage;
#endif
    outputGainStage >> pan;
    outputGainStage >> rms;

    clip.setOversampleLevel(2);
    inputGainStage >> upsampler >> clip >> downsampler >> outputGainStage;
    inputControl >> inputGainStage.in_mod();
    outputControl >> outputGainStage.in_mod();

    selectNode >> sampler.in_select();
    
    rms.attackControl.getOFParameterInt() = 0;
    rms.releaseControl.getOFParameterInt() = 800;

    parameters.setName( "rename me");
/*
    parameters.add( pitchControl.set("pitch", 0, -24, 24));
#ifdef SAMPLER_USE_LOWCUT
    parameters.add( lowCutControl.set("low cut hz", 30, 20, 500) );
#endif
*/
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
        sampler.addSample( library.samples[i], 0 );
    }
    
    pLibrary = &library;
}

void folderkit::Sampler::oscMapping( pdsp::osc::Input & osc, std::string address, np::tuning::ModalTable * table ){
    
    osc.out_trig( address, 0 ) >> sampler.in_select();
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

    osc.out_value( address, 1 ) >> sampler.in_pitch();
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

    osc.out_value( address, 2 ) * (1.0f/32.0f) >> sampler.in_start();
    
    osc.out_value( address, 3 ) >> pan.in_pan();
    osc.out_value( address, 3 ).enableSmoothing(20.0f);
    osc.parser(address, 3) = [&]( float value ) noexcept {
        if( value > 8.0f ){
            value = value - 9.0f;
            value /= 8.0f;
            value = (value<1.0) ? value : 1.0;
            return value-1.0f;
        }else{
            value /= 8.0f;
            return value;
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
        value *= (1.0f/16.0f);
        value = (value<1.0f) ? value : 1.0f;
        value = 1.0f-value;
        value = value * value;
        value = 1.0f-value;        
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

