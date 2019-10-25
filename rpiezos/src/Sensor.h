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
#include "ofxGPIO.h"

namespace np{ namespace piezo{
    
class Sensor {
public:
    Sensor();
    
    void setup ( int channel, MCP & mcp, ofxOscSender & sender, std::string oscAddress="/piezos" );
    void update();

    ofParameterGroup parameters;        
        ofParameter<bool>   active;
        ofParameter<int>    envLow;
        ofParameter<int>    envHigh;
        ofParameter<int>    trigLow;
        ofParameter<int>    trigHigh;

    ofParameterGroup & label( std::string name );

private:
    int channel;
    MCP* a2d;
    ofxOscSender * sender;
    
    int oldValue;
    bool ngate;
    
    ofxOscMessage trigmsg;
    ofxOscMessage envmsg;
};

}}
