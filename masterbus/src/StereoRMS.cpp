/*====================================================================

	masterbus - hub for all the instruments
  
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

#include "StereoRMS.h"

float np::meter::StereoRMS::meter( int ch ) const { 
    return envelopes[ch].meter_output(); 
}

ofParameterGroup & np::meter::StereoRMS::label( string name ){
    parameters.setName( name );
    return parameters;
}

void np::meter::StereoRMS::patch( ){

    channels.resize(2);
        
    addModuleInput("0", channels[0] ); 
    addModuleInput("1", channels[1] ); 
    addModuleOutput("0", channels[0] );
    addModuleOutput("1", channels[1] );
    
    channels[0] >> rms[0] >> gain.ch(0) >> envelopes[0] >> channels[0];
    channels[1] >> rms[1] >> gain.ch(1) >> envelopes[1] >> channels[1];

    rms[0].set( 50.0f );
    rms[1].set( 50.0f );
    
    attackControl  >> envelopes[0].in_attack();
    attackControl  >> envelopes[1].in_attack();
    releaseControl >> envelopes[0].in_release();
    releaseControl >> envelopes[1].in_release();
        
    parameters.setName( "rms envelope" );
    parameters.add( gain.set( "band input gain", 0, -48, 36 ));  
    parameters.add( attackControl.set( "attack ms", 5, 1, 80 ));
    parameters.add( releaseControl.set( "release ms", 500, 1, 5000 ));
    parameters.add( refresh.set( "refresh_clock", 2, 1, 50 ));
    
}

pdsp::Patchable & np::meter::StereoRMS::ch( int index ){
    return channels[index];
}
