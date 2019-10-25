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

#include "Sensor.h"

np::piezo::Sensor::Sensor() {
    oldValue = 0;
    a2d = nullptr;
    sender = nullptr;
    ngate = true;
    
    parameters.setName( "piezo sensor" );
    parameters.add( active.set("active", true ) );
    parameters.add( envLow.set("envelope_low", 5, 0, 1023 ) );
    parameters.add( envHigh.set("envelope_high", 200, 0, 1023 ) );
    parameters.add( trigLow.set("trigger_low", 5, 0, 1023 ) );
    parameters.add( trigHigh.set("trigger_high", 20, 0, 1023 ) );
}

ofParameterGroup & np::piezo::Sensor::label( std::string name ){
    parameters.setName( name );
    return parameters;
}

void np::piezo::Sensor::setup( int channel, MCP & mcp, ofxOscSender & sender, std::string oscAddress ){
    this->channel = channel;
    this->a2d = &mcp;
    this->sender = &sender;
    trigmsg.setAddress( oscAddress + "/trig" );
    envmsg.setAddress( oscAddress + "/env" );
}

void np::piezo::Sensor::update(){
#ifdef __ARM_ARCH
    if( active ){
        unsigned char data[3];
        data[0] = 1;
        data[1] = 0b10000000 |( ((channel & 7) << 4));
        data[2] = 0;
        a2d->readWrite(data);//sizeof(data) );
        int value = 0;
        value = (data[1]<< 8) & 0b1100000000;
        value |=  (data[2] & 0xff);     
        // now 'value' holds the ADC in value

        if( value!=oldValue && value>=envLow && value<=envHigh ){
            float range = envHigh - envLow; // non-checked, possible divide by zero 
            float v = value - envLow;
            v = v / range;
            envmsg.addIntArg( channel );
            envmsg.addFloatArg( v );
            sender->sendMessage(envmsg, false);
        }
        
        if( ngate ){
            if( value > trigHigh ){
                trigmsg.addIntArg( channel );
                sender->sendMessage(trigmsg, false);                
                ngate = false;
            }
        }else{
            if( value > trigHigh ){
                ngate = true;
            }
        }
    }
#endif
}
