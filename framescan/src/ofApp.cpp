/*====================================================================

	framescan - scans and sequences animation framews from webcam
  
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

#define CAMW 1280
#define CAMH 720
#define CUTMARGIN 20

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetWindowTitle( "(-)");
    
    bCleaning = false;
    select = 0;
 
    frames.reserve(99);

    loadFrames();
    
    cam.setDeviceID(0);
    cam.setDesiredFrameRate(60);
    cam.initGrabber( CAMW, CAMH );
    
    fbo.allocate( cam.getWidth(), cam.getHeight() );
  
    invertfrag.name( "invert cut" );
    invertfrag.uniform( iLow.set("u_low", 0.5f, 0.0f, 1.0f) );
    invertfrag.uniform( iHigh.set("u_high", 1.0f, 0.0f, 1.0f) );
    invertfrag.load( ofToDataPath("invertcut.frag") );
    
    borderfrag.name( "border" );
    borderfrag.uniform( iBorder.set("u_border", 0.05f, 0.0f, 0.4f) );
    borderfrag.load( ofToDataPath("border.frag") );

    gui.setup( "GUI", "settings.xml", ofGetWidth()-220, 20 );
    gui.add( mirror.parameters );
    gui.add( mono.parameters );
    gui.add( hsb.parameters );
    gui.add( invertfrag.parameters  );
    gui.add( borderfrag.parameters  );
    gui.add( eSide.set("eraser side", 20, 0,50 )  );
    gui.add( cutx.set("cut x", 700, 0, CAMW ));
    gui.add( cuty.set("cut y", 160, 0, CAMH ));
    gui.add( cutw.set("cut width", 200, 0, 640 ) );
    gui.add( cuth.set("cut height", 200, 0, 640 ) );
    gui.add( offsetX.set("offset x", 0, -CUTMARGIN, CUTMARGIN) );
    gui.add( offsetY.set("offset y", 0, -CUTMARGIN, CUTMARGIN) );
    gui.add( speed.set("speed", 0.3f, 0.0f, 0.5f ) );
    gui.add( pMode.set("preview mode", 2, 0, 2 ) );
    gui.add( cMode.set("crosshair mode", 0, 0, 1 ) );
    gui.loadFromFile( "settings.xml" );
    
    reallocate();

    ofBackground( 0 );
    
    cursor = 0.0f;
    cutoverlay = 0;
}

//--------------------------------------------------------------
void ofApp::update(){
 
    cam.update();

    if( cam.isFrameNew() ){
        fbo.begin();
            ofSetColor( 255 );   
            cam.draw( 0, 0 );
        fbo.end();
        
        mirror.apply( fbo );
        if( ! invertfrag.active ){
            mono.apply( fbo );
            hsb.apply( fbo );          
        }else{
            invertfrag.apply( fbo );
        }
            
    }
}

//--------------------------------------------------------------
void ofApp::draw(){

    ofSetColor( 255 );
   ofColor alt = ofColor( 255, 0, 0 );

    if( !bCleaning ){ // -------- draws acquisition UI -----
        fbo.draw( 0, 0 );

        switch( pMode ){
            case 0: break;
            case 1: case 2:
                if( invertfrag.active ){
                    ofEnableBlendMode(OF_BLENDMODE_ADD);
                    ofSetColor( 255, 0, 0 );
                }else{
                    ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);
                    ofSetColor( 180 );
                }
                
            break;
        }

        switch( pMode ){
            case 0: break;
            
            case 1:
                if( ! frames.empty() ){
                    frames.back().draw( cutx, cuty );
                }                    
            break;
            
            case 2:
                if( ! frames.empty() ){
                    frames.front().draw( cutx, cuty );
                }
            break;
        }

        ofEnableBlendMode(OF_BLENDMODE_ALPHA);
 
        ofNoFill();    
        
        switch( cMode ){
            case 0:
                ofSetColor( alt );
                ofDrawLine( cutx, cuty+(cuth/2), cutx+cutw, cuty+(cuth/2) );
                ofDrawLine( cutx+(cutw/2), cuty, cutx+(cutw/2), cuty+cuth );
                ofDrawRectangle( cutx, cuty, cutw, cuth );        
            break;
            
            case 1:
            {
                ofPushMatrix();
                ofNoFill();
                int cx = cutx + cutw/2;
                int cy = cuty + cuth/2;
                int rad = cutw * 0.3;
                static const float step = TWO_PI / 3.0f;
                ofSetColor( alt );
                    ofTranslate( cx, cy );
                    for( int i=0; i<3; ++i ){
                        int x = cos( float(i) * step + PI*0.5) * rad;
                        int y = sin( float(i) * step + PI*0.5) * rad;
                        ofDrawCircle( x, y, 4 );
                    }
                ofPopMatrix();
                ofDrawRectangle( cutx, cuty, cutw, cuth );   
            }
            break;
        }
                
    }else{ // -------- draws cleaning UI -------
        
        if( ! frames.empty() ){
            ofSetColor(255);
            frames[select].draw( 0, 0, cutw*2, cuth*2 );
        }   
        ofSetColor( alt );
        ofNoFill();
        ofDrawRectangle( ofGetMouseX(), ofGetMouseY(), eSide*2, eSide*2);
        
    }
    

    gui.draw();
    
    // ------------ draws cut preview -----
    
    ofTranslate( fbo.getWidth() + 20, 20 );
    
    switch( cutoverlay ){
        case 2 : case 4: break;
        
        case 1 : case 3:
            ofSetColor(255, 0, 0);
            cutfbo.draw( 0, 0 );
        break;
        
        default : 
            ofSetColor( 255 );
            cutfbo.draw( 0, 0 );
        break;
    }
    
    if( cutoverlay==1 || cutoverlay==3 ){
        if( invertfrag.active ){
            ofEnableBlendMode(OF_BLENDMODE_ADD);
            ofSetColor( 0, 255, 0 );
        }else{
            ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);
            ofSetColor( 180 );
        }
    }else{
        ofSetColor( 255 );
    }

    switch( cutoverlay ){
        case 0: break;
        
        case 1: case 2:
            if( ! frames.empty() ){
                frames.back().draw( 0, 0 );
            }                    
        break;
        
        case 3: case 4:
            if( ! frames.empty() ){
                frames.front().draw( 0, 0 );
            }
        break;
    }
    
    
    // --------- draw frames --------------
    
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    
    ofSetColor( alt );
    ofDrawRectangle( 0, 0, cutw, cuth );
    ofDrawLine( 0, cuth/2, cutw, cuth/2 );
    ofDrawLine( cutw/2, 0, cutw/2, cuth );
    
    ofTranslate( 0, cuth+20 );
    ofSetColor(255);
    
    int counter = cursor;
    
    if( ! frames.empty() ){
        frames[counter].draw( 0, 0 );

        cursor += speed;
        float max = frames.size();
        if( cursor >= max){
            cursor -= max;
        } 
    }    
    ofSetColor( alt );
    ofDrawRectangle( 0, 0, cutw, cuth );
}

void ofApp::offcut(){
    ofDisableAlphaBlending();
    cutfbo.begin();
        ofClear( 0, 0, 0, 0 );
        ofSetColor(255);
        precut.draw( offsetX - CUTMARGIN, offsetY - CUTMARGIN );
    cutfbo.end();
    borderfrag.apply( cutfbo );
    ofEnableAlphaBlending();
}

void ofApp::loadFrames(){
    ofDirectory dir;
    dir.allowExt("png");
    dir.listDir( ofToDataPath("frames") );
    dir.sort(); 
    frames.resize( dir.size() );
    for( size_t i = 0; i < dir.size(); i++){
        frames[i].load( dir.getPath(i ));
    }
}

void ofApp::reallocate(){
    precut.allocate( cutw + CUTMARGIN*2, cuth + CUTMARGIN*2 );
    precut.begin();
        ofClear( 0, 0, 0, 0 );
    precut.end();
    
    cutfbo.allocate( cutw, cuth );
    cutfbo.begin();
        ofClear( 0, 0, 0, 0 );
    cutfbo.end();
    
    borderfrag.allocate( cutfbo );
}

void ofApp::erase( int mx, int my ){
    if( mx<cutw*2 && my < cuth*2 ){
        mx = mx/2;
        my = my/2;

        ofPixels & pixels = frames[select].getPixels();
        
        int stopx = mx + eSide;
        int stopy = my + eSide;
        
        if( stopx >= cutw ) stopx = cutw;
        if( stopy >= cuth ) stopy = cuth;
        
        for( int x = mx; x<stopx; ++x ){
            for( int y = my; y<stopy; ++y ){
                pixels.setColor( x, y, ofColor(255, 255, 255, 0) );
            }
        }
        frames[select].update();
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

    switch( key ){
        case 'c': case ' ':
            ofDisableAlphaBlending();
            precut.begin();
                ofClear( 0, 0, 0, 0 );
                ofSetColor(255);
                fbo.draw( -cutx +CUTMARGIN, -cuty +CUTMARGIN );
            precut.end();
            ofEnableAlphaBlending();
            
            offsetX = 0;
            offsetY = 0;
            offcut();
        break;
        
        case 'a' : case OF_KEY_RETURN:
        {
            ofPixels pixels;
            cutfbo.readToPixels( pixels );
            frames.emplace_back();
            frames.back().setFromPixels( pixels );
        }
        break;
        
        case 's':
            for( int i=0; i< (int)frames.size(); ++i ){
                std::string name = std::string("frames/frame_") + ofToString(i, 3, '0') + ".png";
                frames[i].save( name );
            }
        break;
        
        case 'l':
            loadFrames();
        break;
        
        case 'x':
            erase( ofGetMouseX(), ofGetMouseY() );
        break;
        
        case 'z':
            if( !frames.empty() ) frames.pop_back();
        break;
        
        case 'p':
            pMode++;
            if( pMode==3 ) pMode = 0;
        break;
        
        case 'r':
            reallocate();
        break;
        
        case OF_KEY_TAB:
            bCleaning = !bCleaning;
        break;
        
        case OF_KEY_RIGHT:
            if( bCleaning ){
                select++;
                if( select >= (int)frames.size() ){
                    select=0;
                } 
            }else{
                offsetX++;
                offcut();                
            }
        break;

        case OF_KEY_LEFT:
            if( bCleaning ){
                select--;
                if( select < 0 ){
                    select=frames.size()-1;
                } 
            }else{
                offsetX--;
                offcut();                
            }
        break;
        
        case OF_KEY_UP: 
            offsetY++;
            offcut();
        break;
        
        case OF_KEY_DOWN: 
            offsetY--;
            offcut();
        break;
        
        case '1': cutoverlay = 1; break;
        case '2': cutoverlay = 2; break;
        case '3': cutoverlay = 3; break;
        case '4': cutoverlay = 4; break;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    switch( key ){
        case '1': cutoverlay = 0; break;
        case '2': cutoverlay = 0; break;
        case '3': cutoverlay = 0; break;
        case '4': cutoverlay = 0; break;
    }
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    erase( x, y );
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    erase( x, y );
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
