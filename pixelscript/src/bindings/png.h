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

namespace png{

    struct Pair {
        Pair();
        std::string name;
        std::string path;
        std::vector<ofImage> folder;
        int index;
    };

    void init();
    void resources( std::vector<Pair> & reso );
    
    void load( const char * name, const char * path );
    
    // modes: 0/def = top left corner, 1 = center, 2 = bottom left corner  
    void mode( int value );
    
    void select( const char * name );
    
    void frame( int index );
    void pct( double value );
    
    int next();
    int prev();
    int step( int step );
    int random(); // don't repeat the same frame two time 
    int randjump( int stepmax ); // jumps a number of step between 1 and stepmax included
    
    
    
    void draw( int x, int y );
    
    int width();
    int height();
}
