
#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetWindowTitle( "joystick drones");
    
    ofxGuiSetFillColor(ofColor(255, 80, 80));
    
    drone.setup( 420, 80 );
    
    samples.resize(4);
    for( size_t i=0; i<samples.size(); ++i ){
        drone.addSample( samples[i] );
    }
    drone.smoothing( 100.0f );
        
    js.setup();
    js.addListener( this );    
    
    drone.ch(0) >> engine.audio_out(0);
    drone.ch(1) >> engine.audio_out(1);
    
    angleMod >> drone.in_position();
    angleMod.set(0.0f);
    
    droneGUI.setup( "droneGUI", "joystick.xml");
    droneGUI.add( drone.parameters );
    droneGUI.add( angleModAmount.set("axis2 angle to pos", 0.5f, -1.0f, 1.0f ) );
    droneGUI.setPosition( 20, 120 );
    droneGUI.loadFromFile("joystick.xml");
   
    samplesGUI.setup( "samples", "samples.xml", 240, 120);   
    for( size_t i=0; i<samples.size(); ++i ){
        samplesGUI.add( samples[i].label( "sample" + ofToString(i) ) );
    }
    samplesGUI.loadFromFile( "samples.xml");
    
    selector.add( droneGUI, { '1', '2', '3', '4'} ); 
    selector.setPosition( 20, ofGetHeight() -70, 50 );
    
    //------------SETUPS AND START AUDIO-------------
    engine.listDevices();
    engine.setDeviceID( 0 );
    engine.setup( 44100, 512, 3); 

}

//--------------------------------------------------------------
void ofApp::update(){
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofBackground( 0 ); 
    ofSetColor( 255, 80, 80 );
    droneGUI.draw();
    samplesGUI.draw();
    drone.draw( 20, 20 );
    selector.draw();
}

//--------------------------------------------------------------
void ofApp::exit(){
    js.exit();
}

//--------------------------------------------------------------
void ofApp::axisMoved( int axis, int value ) {
    
    switch( axis ){
        case 0:
            drone.resonators_control[0].getOFParameterFloat().set( ofMap(value, 0, js.axisMin, 0.0f, 1.0f, true) );
            drone.resonators_control[2].getOFParameterFloat().set( ofMap(value, 0, js.axisMax, 0.0f, 1.0f, true) );
        break;
      
        case 1:
            drone.resonators_control[1].getOFParameterFloat().set( ofMap(value, 0, js.axisMax, 0.0f, 1.0f, true) );
            drone.resonators_control[3].getOFParameterFloat().set( ofMap(value, 0, js.axisMin, 0.0f, 1.0f, true) );
        break;
        
        case 2:
            x2 = value;
            recalculateAxis2Angle();
        break;
        
        case 3: 
            y2 = value;
            recalculateAxis2Angle();
        break;
    }
    
}

void ofApp::recalculateAxis2Angle() {
    
    float angle = (atan2( y2, x2 ) + PI) / TWO_PI;
    
    if(angle>0.5f) angle = 1.0f - angle;
    angle *= 2.0f;
    
    angleMod.set( angle * angleModAmount );

}

//--------------------------------------------------------------
void ofApp::buttonPressed( int button, int value ) {
    if(value == 1){
        if(button>=0 && button <4) selector.load(button);
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){ }

//--------------------------------------------------------------
void ofApp::keyReleased(int key){}
void ofApp::mouseDragged(int x, int y, int button){}
void ofApp::mousePressed(int x, int y, int button){}
void ofApp::mouseReleased(int x, int y, int button){}
void ofApp::mouseMoved(int x, int y ){}
void ofApp::mouseEntered(int x, int y){}
void ofApp::mouseExited(int x, int y){}
void ofApp::windowResized(int w, int h){}
void ofApp::gotMessage(ofMessage msg){}
void ofApp::dragEvent(ofDragInfo dragInfo){ }
//--------------------------------------------------------------
