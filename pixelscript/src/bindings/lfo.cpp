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

#include "lfo.h"

namespace lfo{
    double playhead;
    
    void setPlayHead( double value ){
        playhead = value;
    }
    
    bool clock( int division ){
        if( ofGetFrameNum()%division == 0 ){
            return true;
        }
        return false;
    }
    
    double tick( double speed ){
        return floor( playhead*speed );
    }
    
    double phasor( double speed ){
        double ph = playhead*speed;
        ph = ph - (long) ph;
        return ph;
    }
     
    double triangle( double speed ){
        double ph = playhead*speed;
        ph = ph - (long) ph;
        return 1.0 - (abs( ph - 0.5 ) * 2.0 );
    }
    
    double ramp( double speed ){
        double ph = playhead*speed;
        ph = ph - (long) ph;
        return ph;
    }
    
    double saw( double speed ){
        double ph = playhead*speed;
        ph = ph - (long) ph;
        return 1.0f - ph;
    }
    
    double sine( double speed ){
        double ph = playhead*speed;
        ph = ph - (long) ph;
        return 1.0 - (cos(ph*TWO_PI)*0.5 + 0.5);
    }
    
    double pulse( double speed, double width ){
        double ph = playhead*speed;
        ph = ph - (long) ph;
        double value = ( ph>width ) ? 1.0 : 0.0;
        return value;
    }
    
    double square( double speed ){
        double ph = playhead*speed;
        ph = ph - (long) ph;
        double value = ( ph>0.5 ) ? 1.0 : 0.0;
        return value;
    }
    

    double triangle( double speed, double phase ){
        double ph = playhead*speed;
        ph += phase;
        ph = ph - (long) ph;
        return 1.0 - (abs( ph - 0.5 ) * 2.0 );
    }
    
    double ramp( double speed, double phase ){
        double ph = playhead*speed;
        ph += phase;
        ph = ph - (long) ph;
        return ph;
    }
    
    double saw( double speed, double phase ){
        double ph = playhead*speed;
        ph += phase;
        ph = ph - (long) ph;
        return 1.0f - ph;
    }
    
    double sine( double speed, double phase ){
        double ph = playhead*speed;
        ph += phase;
        ph = ph - (long) ph;
        return 1.0 - (cos(ph*TWO_PI)*0.5 + 0.5);
    }
    
    double pulse( double speed, double width, double phase ){
        double ph = playhead*speed;
        ph += phase;
        ph = ph - (long) ph;
        double value = ( ph>width ) ? 1.0 : 0.0;
        return value;
    }
    
    double square( double speed, double phase ){
        double ph = playhead*speed;
        ph += phase;
        ph = ph - (long) ph;
        double value = ( ph>0.5 ) ? 1.0 : 0.0;
        return value;
    }
    
    double noise( double speed ){ return ofNoise( playhead*speed, 0.0f ); }   
    
    double noise( double speed, double x ){ return ofNoise( playhead*speed, x ); }
    
    double noise( double speed, double x, double y ){ return ofNoise( playhead*speed, x, y ); }
    
    double noise( double speed, double x, double y, double z ){ return ofNoise( playhead*speed, x, y, z ); }    
    
}
