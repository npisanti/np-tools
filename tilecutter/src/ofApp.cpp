/*====================================================================

	tilecutter - cuts .png tiles from an image
  
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
    ofSetLogLevel(OF_LOG_VERBOSE);
    input.load("/home/nicola/resources/motore/sources/cut4.png");
    
    gui.setup("", "settings.xml", 1400, 20);
    gui.add( cutSide.set("cut side", 40, 0, 300));
    gui.add( cutX.set("cut x", 0.0f, 0.0f, 1.0f) );
    gui.add( cutY.set("cut y", 0.0f, 0.0f, 1.0f) );
    gui.add( scale.set("scale preview", 0.2f, 0.0001f, 2.0f) );
    gui.add( threshold.set("alpha threshold", 128, 0, 255) );
	gui.loadFromFile("settings.xml");
    bCutDone = false;

    rescale();
    
    counter = 0;
}

//--------------------------------------------------------------        
void ofApp::rescale() {
    resized = input;
    resized.resize( resized.getWidth() * scale, resized.getHeight() * scale );  
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0);

    ofSetColor(255);
    resized.draw( 0, 0 );
    
    int cx = resized.getWidth() * cutX;
    int cy = resized.getHeight() * cutY;
    int side = cutSide;
    ofSetColor(255, 0, 0);
    ofNoFill();
    ofDrawRectangle( cx-side/2, cy-side/2, side, side );
    ofDrawLine( cx-side/2, cy, cx+side/2, cy);
    ofDrawLine( cx, cy-side/2, cx, cy+side/2);
    
    gui.draw();
    
    if(bCutDone){
        ofSetColor( 255 );
        int xx = 1200;
        int yy = 400;
        ofDrawRectangle(xx, yy, cutResized.getWidth(), cutResized.getHeight());
        cutResized.draw( xx, yy );
        ofSetColor(0);
        ofDrawLine( xx, yy + cutResized.getHeight()/2, xx + cutResized.getWidth(), yy + cutResized.getHeight()/2 );
        ofDrawLine( xx+cutResized.getWidth()/2, yy, xx+cutResized.getWidth()/2, yy + cutResized.getHeight() );
    }
}

//--------------------------------------------------------------
void ofApp::exit(){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    if ( x < resized.getWidth() && y<resized.getHeight() ){
        cutX = static_cast<float>(x) / static_cast<float> ( resized.getWidth());
        cutY = static_cast<float>(y) / static_cast<float> ( resized.getHeight());
    }    
}

void ofApp::mouseDragged(int x, int y, int button){
    if ( x < resized.getWidth() && y<resized.getHeight() ){
        cutX = static_cast<float>(x) / static_cast<float> ( resized.getWidth());
        cutY = static_cast<float>(y) / static_cast<float> ( resized.getHeight());
    }    
}

void ofApp::mouseReleased(int x, int y, int button){
    if ( x < resized.getWidth() && y<resized.getHeight() ){
        cutX = static_cast<float>(x) / static_cast<float> ( resized.getWidth());
        cutY = static_cast<float>(y) / static_cast<float> ( resized.getHeight());
        cut();
    }    
}

void ofApp::cut(){
	int side = cutSide;
	cutImage.cropFrom( input, input.getWidth() * cutX - (side*0.5f), input.getHeight() * cutY - (side*0.5f), side, side );
	cutImage.setImageType(OF_IMAGE_COLOR_ALPHA);
	
	for (int x = 0; x<(int)cutImage.getWidth(); x++){
		for (int y = 0; y<(int)cutImage.getHeight(); y++){
				
			const ofColor & pixelColor = cutImage.getColor( x, y );
			
			//if(  pixelColor.r <= threshold && pixelColor.g <= threshold && pixelColor.b <= threshold ){
			if( pixelColor.g <= threshold ){
				cutImage.setColor( x, y, ofColor( 255, 255, 255, 255 ) );
			}else{
				cutImage.setColor( x, y, ofColor( 0, 0, 0, 0 ) );
			}

		}
	}
	cutImage.update();
	
	cutResized = cutImage;
	cutResized.resize( cutResized.getWidth() * scale * 3.0f, cutResized.getHeight() * scale * 3.0f );  
	bCutDone = true;
}

static const float arrowStep = 0.0005f;

void ofApp::keyPressed(int key){
    switch(key){
        case 'c': cut(); break;
        
        case 'r': rescale(); break;
        
        case 's':
        {
            string filename = "tile" + ofToString(ofGetFrameNum()) + ".png"; 
            cutImage.save( filename );
            cout << "saved " << filename << "\n";
        }
        break;
        
        case OF_KEY_LEFT    : cutX-=arrowStep; cut(); break;
        case OF_KEY_RIGHT   : cutX+=arrowStep; cut(); break;
		case OF_KEY_UP		: cutY-=arrowStep; cut(); break;
		case OF_KEY_DOWN	: cutY+=arrowStep; cut(); break;
        
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){}
void ofApp::mouseMoved(int x, int y){}
void ofApp::mouseEntered(int x, int y){}
void ofApp::mouseExited(int x, int y){}
void ofApp::windowResized(int w, int h){}
void ofApp::gotMessage(ofMessage msg){}
void ofApp::dragEvent(ofDragInfo dragInfo){ }
//--------------------------------------------------------------
