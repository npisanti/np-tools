#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetWindowTitle("eye - webcam " +ofToString(id) );
    ofSetWindowShape( width, height );    
    
    fbo.allocate( width, height );
    
    mirror.allocate( fbo );
    mirror.active = false;
    
    monochrome.allocate( fbo );
    monochrome.active = false;
    
    hsb.allocate( fbo );
    hsb.active = false;
    
    tones.allocate( fbo );
    tones.active = false;
    
    webcam.setDeviceID( id );
    webcam.setDesiredFrameRate(60);
    webcam.initGrabber(width, height);

    ofBackground(0);

    bDrawGui = false;

    gui.setup( "", "settings.xml", 20, 20 );
    gui.setName( "GUI" );
    gui.add( mirror.parameters );
    gui.add( hsb.parameters );
    gui.add( monochrome.parameters );
    gui.add( tones.parameters );
    gui.loadFromFile( "settings.xml" );
}

//--------------------------------------------------------------
void ofApp::update(){

    webcam.update();
    if(webcam.isFrameNew()){
        fbo.begin();
            ofClear(0,0,0,0);
            webcam.draw( 0, 0 );
        fbo.end();
        mirror.apply( fbo );
        hsb.apply( fbo );
        monochrome.apply( fbo );
        tones.apply( fbo );
    }					
}

//--------------------------------------------------------------
void ofApp::draw(){
	fbo.draw( 0, 0 );
	//frag.draw( 0, 0 );
	if(bDrawGui) gui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch( key ){
		case 'g': bDrawGui = !bDrawGui;	break;
        case 'm': monochrome.active = ! monochrome.active; break;
        case 's': gui.saveToFile("settings.xml"); break;
        case 'h': 
            mirror.horizontal = true;
            mirror.active = ! mirror.active;
        break;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}
