/*====================================================================

	wolframkit - cellular automata playground
  
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

#include "flags.h"
#define MWR 8

namespace np{

class Automaton1D  {
    
public:

    Automaton1D();
        
    void random( float density ) noexcept;
    void clear() noexcept;

    void advance() noexcept;
    
    void setRule( int rule ) noexcept;
    
    int CA[CA_HEIGHT][CA_WIDTH];    

private:    
    int rule;
    int wrules [MWR];
    
};

}
