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

    osc.setVerbose( true );
    osc.openPort( inputPort );
    std::cout<<"[ folderkit ] receiving osc on port "<<inputPort<<"\n";
    
    if( outputPort != -1 ){
        sender.setup( outputIP, outputPort);
        std::cout<<"[ folderkit ] sending osc to port "<<outputPort<<" on "<<outputIP<<"\n";
    
    }
    
    library.init( path ) ;
    
    // --------------------------------------------
    200.0f >> delaycut.in_freq();
    
    delaycut.ch(0) >> delays.ch(0);
    delaycut.ch(1) >> delays.ch(1);   

    delays.out("send") >> reverb;
    
    reverb.ch(0) >> limiter.ch(0);
    reverb.ch(1) >> limiter.ch(1);
 
    delays.ch(0) >> limiter.ch(0);
    delays.ch(1) >> limiter.ch(1);
    
    limiter.ch(0) >> engine.audio_out(0);
    limiter.ch(1) >> engine.audio_out(1);
    
    for( int i=0; i<NUMSAMPLERS; ++i ){
        std::cout<< "initializating sampler "<<i<<"\n";
        samplers[i].linkToLibrary( library );    
        //samplers[i].rms >> engine.blackhole();
        samplers[i].out("delay") >> delays.ch(0);
        samplers[i].out("delay") >> delays.ch(1);
        samplers[i].out("rev") >> reverb;
        samplers[i].ch(0) >> limiter.ch(0);
        samplers[i].ch(1) >> limiter.ch(1);
    }

    
    std::vector<std::string> addresses = { "a", "b", "c", "d", "e", "f", "g", "h" };

    // OSC mapping -----------------------------
    osc.linkTempo( "/orca/bpm" );

    for(size_t i=0; i<NUMSAMPLERS; ++i ){
        std::string address = "/";
        address += addresses[i];
        samplers[i].oscMapping( osc, address );
    }
    
    // graphic setup---------------------------
    ofSetVerticalSync(true);
    ofDisableAntiAliasing();
    ofBackground(0);
    ofSetColor(ofColor(0,100,100));
    ofNoFill();
    ofSetLineWidth(1.0f);
    ofSetFrameRate(60);

    // GUI -----------------------------------
    //gui.setup( "folderkit", path + "/settings.xml", 20, 20 );
    parameters.setName("folderkit");
        for( size_t i=0; i<NUMSAMPLERS; ++i ){
            std::string name = "sampler ";
            name += addresses[i];
            parameters.add( samplers[i].label( name ) );
        }
        parameters.add( percbus.label( "perc bus comp") );
        parameters.add( delays.parameters );
        parameters.add( reverb.parameters );
        parameters.add( limiter.parameters );
    live.watch( parameters, path + "/settings.json");
    
    //gui.loadFromFile( path + "/settings.xml" );
    //gui.minimizeAll();
 
    // audio setup----------------------------
    engine.sequencer.play();
    engine.addOscInput ( osc );

    const auto & devices = engine.listDevices();
    engine.setDeviceID( device ); 
    
    if(devices[device].outputChannels == 4 ){
        limiter.ch(0) >> engine.audio_out(2);
        limiter.ch(1) >> engine.audio_out(3);
        
        engine.setChannels( 0, 4 );
        engine.setup( 44100, 512, 2);    
    }else{
        engine.setup( 44100, 512, 2);    
    }
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
    /*
    if(bDrawGui){
        gui.draw();
    }
    */
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    /*
    switch( key ){
        case 'g': bDrawGui = !bDrawGui; break;
        case 's': gui.saveToFile(path + "/settings.xml"); break;
        case 'l': gui.loadFromFile(path + "/settings.xml"); break;
    }
    */
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
