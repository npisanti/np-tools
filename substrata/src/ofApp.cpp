/*====================================================================

	substrata - companion drumkit for orca-c
  
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
    ofSetFrameRate( 200 ); // faster OSC mirroring 
    
    ofSetWindowTitle("|::|");
    engine.score.setTempo( 120.0f); // the delay times are clocked

    bDrawGui = true;

    osc.openPort( inputPort );
    std::cout<<"[ substrata ] receiving osc on port "<<inputPort<<"\n";
    
    if( outputPort != -1 ){
        sender.setup( outputIP, outputPort);
        std::cout<<"[ substrata ] sending osc to port "<<outputPort<<" on "<<outputIP<<"\n";
    }
    
    library.init( path ) ;
    table.setup( "pitch_table" );
    
    bool bUseIR = ireverb.load( path + "/IR" );
    
    // --------------------------------------------

    //synths.setup( path );
    //synths.gain.ch(0) >> limiter.ch(0);
    //synths.gain.ch(1) >> limiter.ch(1);

    clipControl >> clip0.in_threshold();
    clipControl >> clip1.in_threshold();
    clip0.setOversampleLevel(2);
    clip1.setOversampleLevel(2);
    upsampler0 >> clip0 >> downsampler0 >> clipOutput.ch(0) >> limiter.ch(0);
    upsampler1 >> clip1 >> downsampler1 >> clipOutput.ch(1) >> limiter.ch(1);
    
    std::cout<< "[ substrata ] initializating samplers ";
    
    for( int i=0; i<NUMSAMPLERS; ++i ){
        
        samplers[i].linkToLibrary( library );        
        
        std::cout<<"."<< std::flush;
        
        //samplers[i].rms >> engine.blackhole();
        
        if( bUseIR ){
            samplers[i].ch(0) * dB(-12.0f) >> reverbSend.ch(0);
            samplers[i].ch(1) * dB(-12.0f) >> reverbSend.ch(1);
        }else{
            samplers[i].out("signal") >> reverbSend;
        }
        samplers[i].ch(0) >> limiter.ch(0);
        samplers[i].ch(1) >> limiter.ch(1);
    }
    std::cout<<"\n";
    
    if(bUseIR){
        //synths.gain.ch(0) * dB( -12.0f ) >> reverbSend.ch(0);
        //synths.gain.ch(1) * dB( -12.0f )  >> reverbSend.ch(1);
        sub * dB(-30.0f) >> reverbSend.ch(0);
        sub * dB(-30.0f) >> reverbSend.ch(1);
        zap * dB(-30.0f) >> reverbSend.ch(0);
        zap * dB(-30.0f) >> reverbSend.ch(1);
        noise.out("L") * dB(-30.0f) >> reverbSend.ch(0);
        noise.out("R") * dB(-30.0f) >> reverbSend.ch(1);
        reverbSend.ch(0) >> ireverb.cut0;
        reverbSend.ch(1) >> ireverb.cut1; 
        ireverb.rev0 * dB(-24.0f) >> limiter.ch(0);
        ireverb.rev1 * dB(-24.0f) >> limiter.ch(1);
    }else{
        //synths.gain.ch(0) >> reverbSend;
        //synths.gain.ch(1) >> reverbSend;
        sub * dB(-21.0f) >> reverbSend >> reverb;
        zap * dB(-21.0f) >> reverbSend >> reverb;
        noise.out("L") * dB(-27.0f) >> reverbSend >> reverb;
        reverb.ch(0) >> limiter.ch(0);
        reverb.ch(1) >> limiter.ch(1);
    }
    
    sub >> limiter.ch(0);
    sub >> limiter.ch(1);

    zap >> upsampler0;
    zap >> upsampler1;
    noise.out("L") >> upsampler0;
    noise.out("R") >> upsampler1;

    table.tonalControl >> sub.in("modulator");
    
    limiter.ch(0) >> engine.audio_out(0);
    limiter.ch(1) >> engine.audio_out(1);
 
    std::vector<std::string> addresses = { "1", "2", "3", "4", "5", "6", "7", "8", "9", "a", "b", "c", "d", "e", "f" };

    // OSC mapping -----------------------------
    osc.linkTempo( "/orca/bpm" );

    sub.oscMapping( osc, "/s", &table );
    zap.oscMapping( osc, "/z", &table );
    noise.oscMapping( osc, "/x", &table );
    //synths.oscMapping( osc, &table );
    
    for(size_t i=0; i<NUMSAMPLERS; ++i ){
        std::string address = "/";
        address += addresses[i];
        samplers[i].oscMapping( osc, address, &table );
    }
    
    // PARAMETERS ------------------------------
    parameters.setName("substrata");
        for( size_t i=0; i<NUMSAMPLERS; ++i ){
            driveControl >> samplers[i].in("clip_input");
            clipControl >> samplers[i].in("clip_threshold");
            gainControl >> samplers[i].in("clip_output");
            
            cuts.add( samplers[i].lowCutControl.set( std::string("samp_") + addresses[i], 30, 20, 500 ) );
        }
        cuts.setName( "low_cuts" );
        parameters.add( cuts );
        
        general.setName("general_settings");
        general.add( driveControl.set("samplers_drive", 12, -48, 24) );
        general.add( clipControl.set("clippers_threshold", -2, -12, 1) );
        general.add( gainControl.set("samplers_gain", -12, -48, 24) );
        general.add( reverbSend.set("reverb_send", -18, -48, 24) );    
        general.add( clipOutput.set("synthpercs_out", -12, -48, 24) );parameters.add( general );        
        
        parameters.add( limiter.parameters );
        parameters.add( reverb.parameters );
        parameters.add( table.parameters );
        parameters.add( sub.parameters );
        parameters.add( zap.parameters );
        parameters.add( noise.parameters );
        //parameters.add( synths.parameters );

        
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
                m.addIntArg( i ); // samoler
                m.addIntArg( t ); // instrument 
                m.addIntArg( samplers[i].subfolder ); // instrument 
                sender.sendMessage(m, false);
            }
        }
        /*
        for( size_t i=0; i<synths.voices.size(); ++i ){
            if( synths.voices[i].bTrig ){
                synths.voices[i].bTrig = false;
                ofxOscMessage m;
                m.setAddress("/synth");
                m.addIntArg( i ); // instrument 
                m.addIntArg( synths.voices[i].m1 ); // pitch
                m.addIntArg( synths.voices[i].m2 ); // wave
                m.addIntArg( synths.voices[i].m3 ); // env amt
                sender.sendMessage(m, false);
            }
        }
        */
        if( zap.bTrig ){
            zap.bTrig = false;
            ofxOscMessage m;
            m.setAddress("/zap");
            m.addIntArg( zap.m1 );
            m.addIntArg( zap.m2 ); 
            sender.sendMessage(m, false);
        }
        if( noise.bTrig ){
            noise.bTrig = false;
            ofxOscMessage m;
            m.setAddress("/noise");
            m.addIntArg( noise.m1 );
            m.addIntArg( noise.m2 ); 
            sender.sendMessage(m, false);
        }
        if( sub.bTrig ){
            sub.bTrig = false;
            ofxOscMessage m;
            m.setAddress("/sub/trigger");
            sender.sendMessage(m, false);
        }
        if( sub.bPitch ){
            sub.bPitch = false;
            ofxOscMessage m;
            m.setAddress("/sub/pitch");
            m.addIntArg( noise.m1 ); 
            sender.sendMessage(m, false);
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
    std::cout<<"[ substrata ] resources cleanly released\n";
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
