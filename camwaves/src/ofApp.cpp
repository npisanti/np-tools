/*====================================================================

	camwaves - export waveforms from webcam 
  
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

#define WAVELEN 512 

#define PLOTHEIGHT 150
#define SAVETIMER 15

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetWindowTitle("cam waveforms");
    
    camWidth = 1280;  // try to grab at this size.
    camHeight = 720;
    camPosX = 230;
    camPosY = 10;
    savecounter = 0;
    		
    savedPath = "";
    savedShow = 0;

    vidGrabber.setDeviceID(0);
    vidGrabber.setDesiredFrameRate(60);
    vidGrabber.initGrabber( camWidth, camHeight );
    
    waveplot.allocate( camWidth, PLOTHEIGHT );
    savedPlot.allocate( camWidth, PLOTHEIGHT );
    
    fbo.allocate( camWidth, camHeight );
    
    mirror.allocate( fbo );
    mirror.active = false;
    
    monochrome.allocate( fbo );
    monochrome.active = false;    
    
    values.resize( WAVELEN );
    for( auto & value : values ){ value = 0.0f; }
    
    
    //patching-------------------------------
    keyboard.setPolyMode( 8 );

    int voicesNum = keyboard.getVoicesNumber();
    
    synth.datatable.setup( WAVELEN, WAVELEN ); // as many samples as the webcam width
	//synth.datatable.smoothing(0.5f);

    synth.setup( voicesNum );
    for(int i=0; i<voicesNum; ++i){
        // connect each voice to a pitch and trigger output
        keyboard.out_trig(i)  >> synth.voices[i].in("trig");
        keyboard.out_pitch(i) >> synth.voices[i].in("pitch");
    }
    
    // patch synth to the engine
    synth.ch(0) >> engine.audio_out(0);
    synth.ch(1) >> engine.audio_out(1);

    // graphic setup---------------------------
    ofSetVerticalSync(true);
    ofDisableAntiAliasing();
    ofBackground(0);
    ofSetColor(ofColor(0,100,100));
    ofNoFill();
    ofSetLineWidth(1.0f);
    ofSetFrameRate(24);

    // GUI -----------------------------------
    gui.setup("GUI", "settings.xml", 10, camPosY);
    gui.add( synth.parameters );
    gui.add( mirror.parameters );
    gui.add( monochrome.parameters );
   
    position.setName("position");
        position.add( px.set("x", 0, 0, camWidth-WAVELEN) );
        position.add( py.set("y", 50, 0, camHeight) );
    gui.add( position );
    gui.loadFromFile("settings.xml");
    
    // audio / midi setup----------------------------
#ifdef USE_MIDI_KEYBOARD
    midiIn.listPorts();
    midiIn.openPort(1); //set the right port !!!
    engine.addMidiController( keyboard, midiIn ); 
#else
    keyboard.setKeysToggle( true );
#endif 
    engine.listDevices();
    engine.setDeviceID(0); // REMEMBER TO SET THIS AT THE RIGHT INDEX!!!!
    engine.setup( 44100, 512, 3);     
    
}

//--------------------------------------------------------------
void ofApp::update(){
    vidGrabber.update();
    
    if(vidGrabber.isFrameNew()){
        fbo.begin();
            ofSetColor( 255 );
            vidGrabber.draw( 0, 0 );
        fbo.end();
        mirror.apply( fbo );
        monochrome.apply( fbo );
    }
    
    if(synth.datatable.ready()){
		
        fbo.readToPixels( pixels );
        
        // ------------------ GENERATING THE WAVE --------------------
        for(int n=0; n<WAVELEN; ++n){
            float sample = ofMap(pixels.getData()[(px+n)*4 + py*4*camWidth], 0, 255, -1.0f, 1.0f);
            values[n] = sample;
        }

        synth.datatable.begin();
        for(int n=0; n<WAVELEN; ++n){
            synth.datatable.data(n, values[n]);
        }
        synth.datatable.end(false);

		
		// ----------------- PLOTTING THE WAVEFORM -------------------
		waveplot.begin();
            ofClear(0, 0, 0, 0);
            
            ofSetColor(255);
            ofDrawRectangle(1, 1, waveplot.getWidth()-2, waveplot.getHeight()-2);
            ofTranslate(2, 2);

            ofBeginShape();
            for(int n=0; n<WAVELEN; ++n){
                float y = ofMap(pixels.getData()[(px+n)*4 + py*4*camWidth], 0, 255, PLOTHEIGHT, 0);
                ofVertex( ofMap( n, 0, WAVELEN, 0, waveplot.getWidth()), y );
            }
            ofEndShape();
		waveplot.end();
		
    }
    
}

//--------------------------------------------------------------
void ofApp::draw(){

    gui.draw();

	ofPushMatrix();
        ofSetColor( 255 );
        ofTranslate(camPosX, camPosY);
        fbo.draw(0, 0);
        
        ofTranslate( 0, camHeight + 10 );

        if( savedShow > 0 ){
            savedPlot.draw( 0, 0 );
            ofDrawBitmapString( "saved to : " + savedPath, 20, 20 );
            savedShow--;
        }else{
            waveplot.draw(0, 0);      
        }
    ofPopMatrix();

#ifndef USE_MIDI_KEYBOARD
    keyboard.draw( 10, ofGetHeight()-130, 200, 110);
#endif

	ofTranslate( camPosX, camPosY );
    ofSetColor( 200, 0, 0 );
    ofDrawLine( px, py, px+WAVELEN, py );
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
#ifndef USE_MIDI_KEYBOARD
    // sends key messages to ofxPDSPComputerKeyboard
    keyboard.keyPressed( key ); 
#endif
    switch(key){
		case ' ':
            savedPath = "waves/wave_"+ ofToString(savecounter, 3, '0') + ".xml";
			np::serialize_floats( savedPath, values );
            savecounter++;
            
            savedPlot.begin();
                ofClear( 0, 0, 0, 0 );
                ofSetColor( 255 );
                waveplot.draw(0,0);
            savedPlot.end();
            
            savedShow = SAVETIMER;
		break;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
#ifndef USE_MIDI_KEYBOARD
    // sends key messages to ofxPDSPComputerKeyboard
    keyboard.keyReleased( key );
#endif
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){}


//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){}
//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){}
//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){}
//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){}
//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){}
//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){}
//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){}
