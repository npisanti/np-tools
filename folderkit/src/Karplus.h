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

#pragma once

#include "ofMain.h"
#include "synth/KarplusStrong.h"

#define NUMKARPL 9

namespace np{ namespace folderkit{
    
class Karplus {
public:

    void setup( pdsp::Engine & engine );

    void oscMapping( pdsp::osc::Input & osc );
        
    np::synth::KarplusStrong synth;
    
    ofParameterGroup tuningGroup;
    std::vector<pdsp::Parameter> tunings;
    
    ofParameterGroup parameters;
    
};

}}
