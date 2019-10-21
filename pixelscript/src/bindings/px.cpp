/*====================================================================

	pixelscript - little 2d graphics scripting playground
  
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

#include "px.h"
#include "ofMain.h"

#define PIXELSCRIPT_ARC_RESO 30 

namespace px {
    
    np::pixelscript::Buffer * buffer;
    bool _rotated = false;
    
    void resources( np::pixelscript::Buffer & resource ){
        buffer = &resource;
    }
    
    void setRotated( bool rotate ){
        _rotated = rotate;
    }
    
    void canvas( int w, int h, int layers ){
        buffer->allocate( w, h, layers );
        if( _rotated ){
             ofSetWindowShape( h, w );
        }else{
             ofSetWindowShape( w, h );
        }
    }
        
    void layer( int l ){
        buffer->setLayer( l );
    }
    
    void framerate( int value ){
        ofSetFrameRate( value );
    }
    
    void begin(){
        buffer->begin();
    }
    
    void finish(){
        buffer->end();
    }
    
    void blendmode( int value ){
        switch( value ){
            case 1: ofEnableBlendMode( OF_BLENDMODE_ADD ); break;
            case 2: ofEnableBlendMode( OF_BLENDMODE_MULTIPLY ); break;
            case 3: ofEnableBlendMode( OF_BLENDMODE_SUBTRACT ); break;
            case 4: ofEnableBlendMode( OF_BLENDMODE_SCREEN ); break;
            default: ofEnableBlendMode( OF_BLENDMODE_ALPHA ); break;
        }
    }
    
    void background( int r, int g, int b, int a ){
        ofBackground( r, g, b, a );
    }

    void background( int r, int g, int b ){
        ofBackground( r, g, b, 255 );
    }
    void background( int gray, int a ){
        ofBackground( gray, gray, gray, a );
    }
    
    void background( int gray ){
        ofBackground( gray, gray, gray, 255 );
    }
    
    void clear( int r, int g, int b, int a ){
        ofClear( r, g, b, a );
    }

    void clear( int r, int g, int b ){
        ofClear( r, g, b, 255 );
    }
    void clear( int gray, int a ){
        ofClear( gray, gray, gray, a );
    }
    
    void clear( int gray ){
        ofClear( gray, gray, gray, 255 );
    }

    void clear(){
        ofClear( 0, 0, 0, 0 );
    }
    
    void fade( int speed ){
        ofPushStyle();
            ofSetColor( 0, 0, 0, speed );
            ofFill();
            ofDrawRectangle( -1, -1, buffer->getWidth()+1, buffer->getHeight()+1 );
        ofPopStyle();
    }

    void color( int r, int g, int b, int a ){
        ofSetColor( r, g, b, a );
    }
    
    void color( int r, int g, int b){
        ofSetColor( r, g, b );
    }
    
    void color( int grey, int a ){
        ofSetColor( grey, a );
    }
    
    void color( int grey ){
        ofSetColor( grey );
    }

    void fill( bool active ){
        if( active ){
            ofFill();
        }else{
            ofNoFill();
        }
    }

    void stroke( double width ){
        ofSetLineWidth( width );
    }
    
    void point( int x, int y ){
        ofDrawLine( x, y, x+1, y+1 );
    }
    
    void line( int x0, int y0, int x1, int y1 ){
        ofDrawLine( x0, y0, x1, y1 );
    }

	void arcl( int cx, int cy, int radius, double angle_begin, double angle_end ){
        ofBeginShape();
            arc( cx, cy, radius, angle_begin, angle_end );
        ofEndShape();
    }

	void rect( int x, int y, int w, int h ){
        ofDrawRectangle( x, y, w, h );
    }
    
    void rect( int x, int y, int w, int h, int r ){
        ofDrawRectRounded( x, y, w, h, r );
    }
    
    void circle( int x, int y, int radius ){
        ofDrawCircle( x, y, radius );
    }
    
	void ellipse( int cx, int cy, int width, int height ){
        ofDrawEllipse( cx, cy, width, height );
    }
    
    void triangle( int x0, int y0, int x1, int y1, int x2, int y2 ){
        ofDrawTriangle( x0, y0, x1, y1, x2, y2 );
    }
    
    void poly( int cx, int cy, int radius, int N, double
     theta ){
        float step = M_TWO_PI / float(N);
        
        ofBeginShape();
        for(int i=0; i<N; ++i ){
            ofVertex( cx + cos(theta)*radius, cy + sin(theta)*radius );
            theta += step;
        }
        ofEndShape( true );
    }
     
    void polypath( int cx, int cy, int radius, int N, double
     theta ){
        float step = M_TWO_PI / float(N);

        for(int i=0; i<N; ++i ){
            ofVertex( cx + cos(theta)*radius, cy + sin(theta)*radius );
            theta += step;
        }
    }
     
    void begin_shape(){
        ofBeginShape();
    }
    
    void next( bool close ){
        ofNextContour( close );
    }
    
    void end_shape( bool close ){
        ofEndShape( close );
    }
    
	void vertex( int x, int y ){
        ofVertex( x, y );
    }
    
	void curve( int x, int y ){
        ofCurveVertex( x, y );
        
    }
	void bezier( int c1x, int c1y, int c2x, int c2y, int x, int y ){
        ofBezierVertex( c1x, c1y, c2x, c2y, x, y );
    }
    
	void arc( int cx, int cy, int radius, double angle_begin, double angle_end ){
        double step = ( angle_end - angle_begin ) / PIXELSCRIPT_ARC_RESO;

        for(int i=0; i<=PIXELSCRIPT_ARC_RESO; ++i ){
            ofVertex( cx + cos(angle_begin)*radius, cy + sin(angle_begin)*radius );
            angle_begin += step;
        }
    }

    bool chance( double value ){ return (ofRandomuf()<value); }
    
    double map( double input, double inmin, double inmax, double outmin, double outmax ){
        return ofMap( input, inmin, inmax, outmin, outmax, true );
    }
    
    double random(){
        return ofRandomuf();
    }
    
    double random( double max ){
        return ofRandom( max );
    }
    
    double random( double min, double max ){
        return ofRandom( min, max );
    }
        
    void push(){
        ofPushMatrix();
    }
    void pop(){
        ofPopMatrix();
    }
    
    void translate( int x, int y ){
        ofTranslate( x, y );
    }
    
    void rotate( double rad ){
        ofRotateRad( rad );
    }
    void scale( double x, double y ){
        ofScale( x, y );
    }
    
    int width(){
        return buffer->getWidth();
    }

    int height(){
        return buffer->getHeight();
    }
}
