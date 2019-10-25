/*====================================================================

	rpiezos - raspberry pi piezo device 
  
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
#include "ofxOsc.h"
#include "Sensor.h"
#include "ofxGPIO.h"
#include "LiveParameters.h"

class ofApp : public ofBaseApp{
	public:
		void setup();
		void update();

        ofParameterGroup parameters;        
        
        ofParameterGroup settings;
        ofParameter<std::string> destinationIP;
        ofParameter<int> destinationPort;
        ofParameter<bool> autoreload;
        ofParameter<int> sleep;
        ofParameter<std::string> prefix;
        
        vector<np::piezo::Sensor> piezos;
            
        ofxOscSender sender;
        
        std::string path;
        np::LiveParameters live;
        
#ifdef __ARM_ARCH   
        MCP a2d;
#endif

};
