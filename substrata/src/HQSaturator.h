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

#pragma once

#include "ofMain.h"
#include "ofxPDSP.h"

namespace np2 { namespace effect {
    
class HQSaturator : public pdsp::Patchable {

public:    
    HQSaturator() { patch(); }
    HQSaturator( const HQSaturator & other ) { patch (); }
    
    ofParameterGroup parameters;

    ofParameterGroup & label( std::string name );

private:    
    void patch();
    
    pdsp::ParameterGain    inputGain;
    pdsp::ParameterGain    outputGain;

    pdsp::IIRUpSampler2x upsampler;
    pdsp::IIRDownSampler2x downsampler; 
    pdsp::Saturator2 saturator;
};
    
}}
