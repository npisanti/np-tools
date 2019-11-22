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

#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetWindowTitle("|::|");
    engine.score.setTempo( 120.0f); // the delay times are clocked

    bDrawGui = true;

    osc.openPort( inputPort );
    std::cout<<"[ folderkit ] receiving osc on port "<<inputPort<<"\n";
    
    if( outputPort != -1 ){
        sender.setup( outputIP, outputPort);
        std::cout<<"[ folderkit ] sending osc to port "<<outputPort<<" on "<<outputIP<<"\n";
    }
    
    library.init( path ) ;
    table.setup( "pitch_table" );
    
    // --------------------------------------------
    
    for( int i=0; i<NUMSAMPLERS; ++i ){
        std::cout<< "[ folderkit ] initializating sampler "<<i<<"\n";
        samplers[i].linkToLibrary( library );    
        //samplers[i].rms >> engine.blackhole();
        samplers[i].out("signal") >> reverb;
        samplers[i].ch(0) >> limiter.ch(0);
        samplers[i].ch(1) >> limiter.ch(1);
    }
    
    
    reverb.ch(0) >> limiter.ch(0) >> engine.audio_out(0);
    reverb.ch(1) >> limiter.ch(1) >> engine.audio_out(1);
 
    std::vector<std::string> addresses = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "a", "b", "c", "d", "e", "f" };

    // OSC mapping -----------------------------
    osc.linkTempo( "/orca/bpm" );

    for(size_t i=0; i<NUMSAMPLERS; ++i ){
        std::string address = "/";
        address += addresses[i];
        samplers[i].oscMapping( osc, address, &table );
    }
    
    // PARAMETERS ------------------------------
    parameters.setName("folderkit");
        for( size_t i=0; i<NUMSAMPLERS; ++i ){
            driveControl >> samplers[i].in("clip_input");
            clipControl >> samplers[i].in("clip_threshold");
            gainControl >> samplers[i].in("clip_output");
        }
        parameters.add( driveControl.set("samplers_drive", 12, -48, 24) );
        parameters.add( clipControl.set("clippers_threshold", -2, -12, 1) );
        parameters.add( gainControl.set("samplers_gain", -12, -48, 24) );        
        parameters.add( limiter.parameters );
        parameters.add( reverb.parameters );
        parameters.add( table.parameters );
        
    live.watch( parameters, path + "/settings.json");

    // audio setup----------------------------
    engine.sequencer.play();
    engine.addOscInput ( osc );

    engine.setDeviceID( device ); 
    engine.setup( 44100, 512, 2);    
    
}

//--------------------------------------------------------------
void ofApp::update(){
    if( outputPort!=-1 ){
        for( int i=0; i<NUMSAMPLERS; ++i ){
            int t = samplers[i].checkTrigger();
            if( t!=-1 ){
                ofxOscMessage m;
                m.setAddress("/sample");
                m.addIntArg( t ); // instrument 
                m.addIntArg( i ); // samoler
                sender.sendMessage(m, false);
            }
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
    library.free();
    std::cout<<"[ folderkit ] resources cleanly released\n";
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
