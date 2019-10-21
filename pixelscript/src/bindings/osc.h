/*====================================================================

	pixelscript - little 2d graphics scripting playground
  
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

namespace osc{
    
    struct TaggedSender{
        std::string name;
        std::string address;
        int port;
        ofxOscSender sender;
    };
    
    void setMessage( const ofxOscMessage & msg );
    
    void resources( ofxOscReceiver & rec, std::vector<TaggedSender> & send );
    
    void setup_receiver( int port );
    void setup_sender( const char * name, const char * addr, int port );

    const char * address();
    double number( int i );
    const char * string( int i );
    
    void begin( const char * name );
    void set_address( const char * str );
    void add_string( const char * str );
    void add_float( double value );
    void add_int( double num );
    void send();
    
    void route( const char * name );
    void route( const char * name, const char * address );

}
