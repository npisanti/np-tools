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

#include "osc.h"

#include <iostream>

namespace osc{
    
    ofxOscReceiver * receiver;
    std::vector<TaggedSender> * senders;
    const ofxOscMessage * message;
    ofxOscMessage toSend;
    int selected;
    std::string _address;
    
    void setMessage( const ofxOscMessage & msg ){
        message = &msg;
    }
    
    void resources( ofxOscReceiver & rec, std::vector<TaggedSender> & send){
        senders = &send;
        receiver = &rec;
        selected = 0;
    }
    
    void setup_receiver( int port ){
        receiver->setup( port );
        std::cout<<"[ osc ] receiving OSC on port "<<port<<"\n";
    }
    
    void setup_sender( const char * name, const char * addr, int port ){
        
        for(size_t i=0; i<senders->size(); ++i ){
            if( senders->at(i).name == name ){
                if( senders->at(i).address!=addr || senders->at(i).port!=port ){
                    std::cout<<"[ osc.setup_sender ] "<<name<<" name already taken by another sender!\n";
                }
                return;
            }
        }        

        senders->emplace_back();
        std::string a = addr;
        senders->back().sender.setup( a, port );
        std::cout<<"[ osc ] sending OSC to port "<<port<<", address "<<a<<"\n";
    }

    const char * address(){ 
        _address = message->getAddress();
        return _address.c_str(); 
    }
    
    double number( int i ){
        auto t = message->getArgType(i); 
        if( t == OFXOSC_TYPE_INT32 ){
            return double(  message->getArgAsInt32(i) );
        }else if( t == OFXOSC_TYPE_FLOAT ){
            return double(  message->getArgAsFloat(i) );
        }
        return 0.0;
    }
    
    const char * string( int i ){
        auto t = message->getArgType(i); 
        if( t == OFXOSC_TYPE_STRING ){
            return message->getArgAsString(i).c_str();
        }
        return "";
    }

    void begin( const char * name ){
        for(size_t i=0; i<senders->size(); ++i ){
            if( senders->at(i).name == name ){
                selected = i;
                toSend = ofxOscMessage();
                return;
            }
        }
        std::cout<<"[ osc.begin ] sender with given name not found!\n";
    }
    
    void set_address( const char * str ){
        toSend.setAddress( str );
    }
    
    void add_string( const char * str ){
        toSend.addStringArg( str );
    }
    
    void add_float( double value ){
        toSend.addFloatArg( value );
    }
    
    void add_int( double num ){
        toSend.addIntArg( int(num) );
    }
    
    void send(){
        senders->at(selected).sender.sendMessage( toSend, false );
    }
    
    void route( const char * name ){
        for(size_t i=0; i<senders->size(); ++i ){
            if( senders->at(i).name == name ){
                senders->at(i).sender.sendMessage( *message, false );
                return;
            }
        }
        std::cout<<"[ osc.route ] sender with given name not found!\n";
    }
    
    void route( const char * name, const char * address ){
        for(size_t i=0; i<senders->size(); ++i ){
            if( senders->at(i).name == name ){
                toSend = *message;
                toSend.setAddress( address );
                senders->at(i).sender.sendMessage( toSend, false );
                return;
            }
        }
        std::cout<<"[ osc.route ] sender with given name not found!\n";
    }
}
