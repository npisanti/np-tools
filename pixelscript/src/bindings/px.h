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

#pragma once

#include <iostream>
#include "ofMain.h"
#include "../Buffer.h"

namespace px {

    void resources( np::pixelscript::Buffer & resource );
    void setRotated( bool rotate );
    
    void canvas( int w, int h, int layers = 1 );
    
    void framerate( int value );
    void blendmode( int value );
    
    void layer( int l );
    
    void begin();
    void finish();
    
    void clear();
    void clear( int r, int g, int b, int a );
    void clear( int r, int g, int b );
    void clear( int gray, int a );
    void clear( int gray );
    
    void background( int r, int g, int b, int a );
    void background( int r, int g, int b );
    void background( int gray, int a );
    void background( int gray );
    
    void fade( int speed );

    void color( int r, int g, int b, int a );
    void color( int r, int g, int b);
    void color( int grey, int a );
    void color( int grey );

    void fill( bool active = true );

    void stroke( double width );
    
    void point( int x, int y );
    void line( int x0, int y0, int x1, int y1 );
    void arcl( int cx, int cy, int radius, double angle_begin, double angle_end );

	void rect( int x, int y, int w, int h );
    void rect( int x, int y, int w, int h, int r );
    void circle( int x, int y, int radius );
	void ellipse( int cx, int cy, int width, int height );
    void poly( int cx, int cy, int radius, int N, double
     theta=-M_PI_2 );
    void triangle( int x0, int y0, int x1, int y1, int x2, int y2 );
     
    void begin_shape();
    void next( bool close = false );
    void end_shape( bool close = false );
	void vertex( int x, int y );
	void curve( int x, int y );
	void bezier( int c1x, int c1y, int c2x, int c2y, int x, int y );
    void arc( int cx, int cy, int radius, double angle_begin, double angle_end );
    void polypath( int cx, int cy, int radius, int N, double
     theta=-M_PI_2 );
    
    double map( double input, double inmin, double inmax, double outmin, double outmax );
    
    double random();
    double random( double max );
    double random( double min, double max );
    bool chance( double value );
        
    int width();
    int height();
        
    void push();
    void pop();
    void translate( int x, int y );
    void rotate( double rad );
    void scale( double x, double y );
    
}
