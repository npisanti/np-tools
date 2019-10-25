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

#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetVerticalSync(false);
    
    // ------------- settings ----------------------------------
    parameters.setName( "rpiezos" );

    settings.setName("settings");
        settings.add( destinationIP.set("destination_ip", "localhost") );
        settings.add( destinationPort.set("destination_port", 12345, 0, 40000 ) );
        settings.add( prefix.set("osc_prefix", "/piezos" ) );
        settings.add( autoreload.set("autoreload", true ) );
        settings.add( sleep.set("sleep_nanosecs", 100, 1, 2000 ) );
    parameters.add( settings );
    
    piezos.resize( 6 );
    
    for( size_t i=0; i<piezos.size(); ++i ){
        parameters.add( piezos[i].label( "sensor "+ofToString(i) ) );
    }
    
    live.setup( parameters, path );
    
    if( autoreload ){
        live.enableWatching();
    }

    // ------------------ OSC setup ----------------------------
        
    sender.setup( destinationIP, destinationPort );
    std::cout<<"[cb_rpiezos] sending OSC to "<<destinationIP<<" on port "<<destinationPort<<"\n";   
       

    // ------------------ inputs setup -------------------------
#ifdef __ARM_ARCH        
    a2d.setup("/dev/spidev0.0", SPI_MODE_0, 1000000, 8);

    for( int i=0; i<(int)piezos.size(); ++i ){
        piezos[i].setup( i, a2d, sender, prefix );
    }
#endif
}

//--------------------------------------------------------------
void ofApp::update(){
    
    for( size_t i=0; i<piezos.size(); ++i ){
        piezos[i].update();
    }
    
    usleep( sleep ); // 0.1 ms 
}
