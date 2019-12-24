/*====================================================================

	wolframkit - cellular automata playground
  
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

#define PLAYHEAD_GRANULARITY 64

//--------------------------------------------------------------
void ofApp::setup(){
    
    if( outputPort!=-1 ){
        sender.setup( outputIP, outputPort );
        laststep = -1;
        lastplayhead = -1;
    }
    
    //------------------- graphics --------------
    if(!headless){
        ofBackground(0);
        ofSetFrameRate(60);
        ofDisableAntiAliasing();        
        brightColor = ofColor(255);
        darkColor = ofColor(134);
        ofSetWindowTitle( "::::::" );
        //brightColor = ofColor( 255, 95, 95);
        //darkColor = ofColor( 90, 35, 35);
    }

    ruleseq.setup( seqPath );

    //----------------- SEQUENCING ---------------------
    engine.score.setTempo(172.0f);
    engine.score.sections.resize(2);
    
    engine.score.sections[0].setCell( 0, &masterplan );
    
    // masterplan coding
    masterplan.bars = 1.0;
    
    masterplan.code = [&] () {
        if( ruleseq.changed() ){
            masterplan.resetCount();
            wolframSeq.division = ruleseq.division();
            engine.sequencer.setTempo( ruleseq.tempo() );
        }
        
        if( masterplan.counter() % ruleseq.numBars() == 0 ){
            
            for( int i=0; i<NUMSYNTHS; ++i ){
                wolframSeq.thresholds[i] = ruleseq.entry().thresholds[i];                                
            }

            wolframSeq.setRule( ruleseq.entry().rule );
            wolframSeq.density = ruleseq.entry().density;
            wolframSeq.regenerate = true;
            
            ruleseq.next();
        }

    }; // masterplan end


    engine.score.sections[1].setCell( 0, &wolframSeq );
    engine.score.launchMultipleCells(0); 
    
    
    // ----------- PATCHING -----------
    
    percs.resize( NUMSYNTHS );
    
    for ( int i=0; i<NUMSYNTHS; ++i ) {
        percs[i].setPitch( 36.0f + i*7.0f );
        engine.score.sections[1].out_trig(i) >> percs[i]; // patch the sequence outputs to the percs
        percs[i].out("L") >> limiter.ch(0);
        percs[i].out("R") >> limiter.ch(1);
    
        percs[i].out("send_rev") >> reverb;
        percs[i].out("send_delay") >> dub.ch(0);
        percs[i].out("send_delay") >> dub.ch(1);
    }
    // connect the percs to the stereo delay
                  dub.ch(0) >> limiter.ch(0);
                  dub.ch(1) >> limiter.ch(1);
    
    reverb.ch(0) >> limiter.ch(0);
    reverb.ch(1) >> limiter.ch(1);
    dub.out("send") >> reverb;
    
    limiter.ch(0) >> engine.audio_out(0);
    limiter.ch(1) >> engine.audio_out(1);
    
    // ------------ SERIALIZATION ------------

    synths.setName("wolframkit");    
    for ( int i=0; i<NUMSYNTHS; ++i ) {
        synths.add( percs[i].label("synth_"+ofToString(i)) );
    }
    synths.add( dub.parameters );
    synths.add( reverb.parameters );
    synths.add( limiter.parameters );
    
    live.watch( synths, synthPath );
    
    //---------------------- audio setup -------------
    engine.setDeviceID( device );
    engine.setup( 44100, 512, 3); 
        
}

//--------------------------------------------------------------
void ofApp::update(){
    // sends out OSC 
    if( outputPort!=-1 ){
        float playhead = wolframSeq.meter_percent();
        int coarse = playhead * PLAYHEAD_GRANULARITY;
        if( coarse != lastplayhead ){
            ofxOscMessage m;
            m.setAddress("/wolfram/playhead");
            m.addFloatArg( playhead ); 
            sender.sendMessage(m, false);
            lastplayhead = coarse;
        }
        
        int step = wolframSeq.currentStep();
        if( step!= laststep ){
            for( int i=0; i<NUMSYNTHS; ++i){
                float value = wolframSeq.getStep( step, i );
                ofxOscMessage m;
                m.setAddress("/wolfram");
                m.addIntArg( i ); // instrument
                m.addFloatArg( value ); // value 
                sender.sendMessage(m, false);
            }
            laststep = step;
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    if(!headless){
        ofBackground(0);
        int side = ofGetWidth() / CA_WIDTH;
        int w = side * CA_WIDTH;
        int off = (ofGetWidth() - w)/2;
        
        ofTranslate( off, 30 );
        wolframSeq.draw( side, 50, brightColor, darkColor );
    }
}

void ofApp::solo( int select ){
    for( int i=0; i<NUMSYNTHS; ++i){
        if( i == select ){
            percs[i].solo.set(1.0f);
        }else{
            percs[i].solo.set(0.0f);
        }
    }
    std::cout<<"[ wolframkit ] soloed synth "<<select<<"\n";
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    switch ( key ){
        case ' ':
            if(engine.score.isPlaying()){
                engine.score.stop();
            }else{
                engine.score.play();
            }        
        break;
        
        case '\\': case '0': solo(0); break;
        case '1': solo(1); break;
        case '2': solo(2); break;
        case '3': solo(3); break;
        case '4': solo(4); break;
        case '5': solo(5); break;
        
        case 'a':
            for( int i=0; i<NUMSYNTHS; ++i){
                percs[i].solo.set(1.0f);
            }
            std::cout<<"[ wolframkit ] all synths active\n";
        break;
    }

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

