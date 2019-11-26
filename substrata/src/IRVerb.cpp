/*====================================================================

	substrata - companion drumkit for orca-c
  
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

#include "IRVerb.h"

bool substrata::IRVerb::load( std::string path ){
    
    ofDirectory dir;
	dir.listDir( path );
	dir.allowExt("wav");
	dir.sort(); 
    
    if( dir.size()==0 ){
        return false;
    }
    
    100.0f >> cut0.in_freq();
    100.0f >> cut1.in_freq();
    
    cut0 >> rev0;
    cut1 >> rev1;
    
    impulse0.load( dir.getPath(0) );
    
    if( impulse0.channels > 1 ){
        rev0.loadIR( impulse0, 0 );
        rev1.loadIR( impulse1, 1 );   
    }else{
        if( dir.size()>1 ){
            impulse1.load( dir.getPath(1) );
            rev0.loadIR( impulse0, 0 );
            rev1.loadIR( impulse1, 0 );   
        }else{
            rev0.loadIR( impulse0, 0 );
            rev1.loadIR( impulse0, 0 );   
        }
    }
    std::cout<< "[ substrata ] found impulse responses in /IR folder\n";
    return true;
}

pdsp::Patchable & substrata::IRVerb::ch( int i ){
    switch( i ){
        default:
        case 0: return rev0; break;
        case 1: return rev1; break;
    }
}
