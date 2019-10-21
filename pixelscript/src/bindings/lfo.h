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

namespace lfo{
    
    void setPlayHead( double value );

    double phasor( double speed );
    
    bool clock( int division );
    
    double tick( double speed );
     
    double triangle( double speed );
    double ramp( double speed );
    double saw( double speed );
    double sine( double speed );
    double pulse( double speed, double width );
    double square( double speed );
    
    double triangle( double speed, double phase );
    double ramp( double speed, double phase );
    double saw( double speed, double phase );
    double sine( double speed, double phase );
    double pulse( double speed, double width, double phase );  
    double square( double speed, double phase );  
    
    double noise( double speed );
    double noise( double speed, double x );
    double noise( double speed, double x, double y );
    double noise( double speed, double x, double y, double z );

}
