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

#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetWindowTitle(">|<");
    
    engine.setChannels(2, 2); // activate 2 input channels
    
    clock = 0;
    if( outputPort != -1 ){
        sender.setup( outputIP, outputPort);
        std::cout<<"[ masterbus ] sending osc to port "<<outputPort<<" on "<<outputIP<<"\n";
        limiter.ch(0) >> rms.ch(0) >> engine.blackhole();
        limiter.ch(1) >> rms.ch(1) >> engine.blackhole();
    }
    
    engine.audio_in(0) >> limiter.ch(0);
    engine.audio_in(1) >> limiter.ch(1);

    engine.audio_in(0) >> reverb;
    engine.audio_in(1) >> reverb;

    reverb.ch(0) >> limiter.ch(0);
    reverb.ch(1) >> limiter.ch(1);
 
    limiter.ch(0) >> engine.audio_out(0);
    limiter.ch(1) >> engine.audio_out(1);
    
    // PARAMETERS ----------------------------
    parameters.setName( "masterbus" );
    parameters.add( reverb.parameters );
    parameters.add( limiter.parameters );
    parameters.add( rms.parameters );
    
    // encoded defaults -------- 
    limiter.thresholdControl.getOFParameterInt() = -9;
    limiter.makeup.getOFParameterInt() = 9;
    limiter.clipThreshold.getOFParameterInt() = -2;
    
    // JSON settings --------
    if( path!="" ){
        live.watch( parameters, path );        
    }

    engine.setDeviceID( device ); 
    engine.setup( 44100, 512, 2);    
    
    // disconnect from audio input to be used as JACK input
    ofSystem("jack_disconnect system:capture_1 RtApiJack:inport\\ 0");
    ofSystem("jack_disconnect system:capture_2 RtApiJack:inport\\ 1");
}

//--------------------------------------------------------------
void ofApp::update(){
    if( outputPort!=-1 ){
        if(clock==0){
            ofxOscMessage m;
            m.setAddress("/masterbus");
            m.addFloatArg( rms.meter(0) ); // instrument 
            m.addFloatArg( rms.meter(1) ); // samoler
            sender.sendMessage(m, false);        
        }

        clock++;
        if(clock>=rms.refresh){
            clock = 0;
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){    

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
}

//--------------------------------------------------------------
void ofApp::exit(){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
