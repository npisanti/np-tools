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

#pragma once

#include "ofxPDSP.h"

namespace np { namespace meter {
	
class StereoRMS : public pdsp::Patchable {

public:
    StereoRMS(){ patch(); }
    StereoRMS(const StereoRMS & other){ patch(); }

    void patch();

    pdsp::Patchable & ch( int index );
    
    ofParameterGroup & label( string name );

    ofParameterGroup parameters;

    float meter( int ch ) const;

    pdsp::Parameter     attackControl;
    pdsp::Parameter     releaseControl;
    pdsp::ParameterGain gain;
    
    ofParameter<int>    refresh;
    
private:
    std::vector<pdsp::ChannelNode> channels;
    pdsp::RMSDetector       rms [2];
    pdsp::EnvelopeFollower  envelopes[2];

};

}} // end namespaces
