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

#include "ofMain.h"

namespace np{ namespace pixelscript {
    
class Buffer{

    struct Layer{
        Layer();
        
        void swap();
        void begin(){ fbos[now].begin(); }
        void end(){ fbos[now].end(); }
        void draw( int x, int y ){ fbos[now].draw( x, y ); }
        void draw( int x, int y, int w, int h ){ fbos[now].draw( x, y, w, h ); }
        
        std::vector<ofFbo> fbos;
        int now;
        int then;
    };

public:

    Buffer();
    
    void allocate( int w, int h, int numLayers );
    void begin();
    void end();
    void swap();
    
    void setLayer( int i );
    
    void draw( int x, int y );
    void draw( int x, int y, int w, int h );
    
    const ofTexture & getTexture() const;
    
    int getWidth() const { return w; }
    int getHeight() const { return h; }

private:
    int layer;
    int w; 
    int h;
    std::vector<Layer> layers;
    
};
    
}}
