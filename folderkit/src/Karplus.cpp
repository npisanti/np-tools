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

#include "Karplus.h"

void np::folderkit::Karplus::setup( pdsp::Engine & engine ){
    
    synth.setup( NUMKARPL, 0.8f, false );
    tunings.resize( NUMKARPL );
    
    parameters.setName("karplus");
    parameters.add( synth.label("synth") );
    tuningGroup.setName("tuning");
    for( size_t i=0; i<tunings.size(); ++i ){
        tuningGroup.add( tunings[i].set( "p "+ofToString(i+1), 36.0f + i*7.0f, 0.0f, 128.0f ));
    }
    parameters.add( tuningGroup );
    
    synth.ch(0) >> engine.audio_out(0);
    synth.ch(1) >> engine.audio_out(1);
}
    
void np::folderkit::Karplus::oscMapping( pdsp::osc::Input & osc ){
        
    int numsynths = synth.size();
    
    for( int index = 0; index< numsynths; ++index ){
        
        tunings[index] >> synth.in_pitch( index );
        
        std::string address = "/";
        address += ofToString( index );
        
        osc.out_trig( address, 0 ) >> synth.in_trig( index );  
        osc.out_trig( address, 0 ) >> synth.voices[index].in("pluck_decay");
        osc.parser(address, 0) = [&]( float value ) noexcept {
            value *= 0.112;
            value = (value<1.0) ? value : 1.0;
            value = value * value;
            value = 1 + value * 199;
            return value;  
        };          
    }
}
    

