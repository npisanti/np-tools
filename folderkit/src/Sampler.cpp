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

void motore::Sampler::patch (){
    
    addModuleInput("trig", triggers );
    addModuleInput("select", selectNode );
    addModuleInput("pitch", pitchNode );
    addModuleOutput("signal", softclip );
    addModuleOutput("L", pan.out_L() );
    addModuleOutput("R", pan.out_R() );
    addModuleOutput("rev", sendRev );
    addModuleOutput("delay", sendDelay );
    
    0.0f >> env.in_velocity();
    
    drift        >> driftAmt;
    driftControl >> driftAmt.in_mod();
                    driftAmt >> sampler.in_pitch();
                   pitchNode >> sampler.in_pitch();
                pitchControl >> sampler.in_pitch();

    triggers >> drift.in_trig();
    triggers >> sampler  >> amp;
    triggers >> env      >> amp.in_mod();

    sampler >> amp >> lowcut >> softclip >> pan;
                                softclip >> rms;
                                softclip >> sendRev;
                                softclip >> sendDelay;
    
    attackControl  >> env.in_attack();
    holdControl    >> env.in_hold();
    releaseControl >> env.in_release();

    selectNode >> sampler.in_select();
    
    rms.attackControl.getOFParameterInt() = 0;
    rms.releaseControl.getOFParameterInt() = 800;
    
    lowCutControl >> lowcut.in_freq();
    panControl >> pan.in_pan();
    
    parameters.setName( "rename me");
    parameters.add( pitchControl.set("pitch", 0, -24, 24));
    parameters.add( driftControl.set("pitch drift", 0.0f, 0.0f, 0.5f));
    parameters.add( attackControl.set("attack time", 0, 0, 1000));
    parameters.add( holdControl.set("hold time", 5000, 0, 5000));
    parameters.add( releaseControl.set("release time", 5000, 0, 5000));
    parameters.add( lowCutControl.set("low cut hz", 30, 20, 500) );
    parameters.add( softclip.parameters );
    parameters.add( panControl.set( "pan control", 0.0f, -1.0f, 1.0f ));
    parameters.add( sendRev.set( "send to reverb", -48, -48, 12 ));
    parameters.add( sendDelay.set( "send to delay", -48, -48, 12 ));
   
    
    pLibrary = nullptr;
    tPast = 0;
    cursor = 0;
    instrument = 0;
}

ofParameterGroup & motore::Sampler::label( std::string name ){
    parameters.setName( name );
    return parameters;
}

pdsp::Patchable & motore::Sampler::in_trig() {
    return in("trig");
}

pdsp::Patchable & motore::Sampler::in_select() {
    return in("select");
}

pdsp::Patchable & motore::Sampler::in_pitch() {
    return in("pitch");
}

pdsp::Patchable & motore::Sampler::ch( int i ){
    switch( i ){
        case 0: return out("L"); break;
        case 1: return out("R"); break;
        default: break;
    }
    return out("L");
}

void motore::Sampler::linkToLibrary( motore::Library & library ){
    
    std::cout<<"[motore::Sampler] adding "<<library.samples.size()<<" samples\n";
    
    for( size_t i=0; i<library.samples.size(); ++i ){
        sampler.addSample( library.samples[i], 0 );
    }
    
    pLibrary = &library;
}

void motore::Sampler::oscMapping( pdsp::osc::Input & osc, std::string address ){
    
    osc.out_trig( address, 0 ) >> sampler.in_select();
    osc.out_trig( address, 0 ) >> triggers;
    osc.parser( address , 0) = [&]( float value ) noexcept {
        //std::cout<<"received message\n";
        instrument = value;
        if( instrument && pLibrary->availables[instrument] ){
            pLibrary->correlate( instrument, value );
            return value;
        }else{
            return pdsp::osc::Ignore;
        }
    };
}

int motore::Sampler::checkTrigger(){
    int trigs = env.meter_triggers();
    if( tPast != trigs ){
        tPast = trigs;
        return instrument;
    }
    return -1;
}

