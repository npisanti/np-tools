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

#include "HQSaturator.h"

void np2::effect::HQSaturator::patch(){

    addModuleInput( "signal", inputGain );
    addModuleOutput( "signal", outputGain );
    
    
    inputGain >> saturator >> outputGain;
    
    parameters.setName( "saturator" );
    parameters.add( inputGain.set("input dB", 0, -48, 48) );
    parameters.add( outputGain.set("output dB", 0, -48, 0) );
}

ofParameterGroup & np2::effect::HQSaturator::label( std::string name ){
    parameters.setName(name);
    return parameters;
}

