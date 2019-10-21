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
#include "../Shader.h"
#include "../Buffer.h"

namespace frag{

    void resources( np::pixelscript::Buffer & buff, std::vector<np::pixelscript::Shader> & reso, float playhead );
    
    void load( const char * name, const char * path );
        
    void apply( const char * name );
    
    void begin( const char * name );
    void uniform( const char * name, float value );
    void uniform( const char * name, float x, float y );
    void uniform( const char * name, float x, float y, float z );
    void uniform( const char * name, float x, float y, float z, float w );
    void finish();
}
