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

#include "Automaton1D.h"

#include "ofxPDSP.h"

np::Automaton1D::Automaton1D(){
    setRule( 60 );
    clear();
}

void np::Automaton1D::setRule( int rule ) noexcept { 
    // init wolfram rules from number bits
    this->rule = rule;
    int rulebits = rule;
    for(int i=0; i<8; ++i){             // inits the rules
        wrules[i] = rulebits & 0x0001;  // take just the lowest bit
        rulebits = rulebits>>1;         // bitshift by one
    }
}

void np::Automaton1D::advance() noexcept{ // update the automaton
    for( int y=CA_HEIGHT-1; y>0; --y ){
        for( int x=0; x<CA_WIDTH; ++x){
            CA[y][x] = CA[y-1][x];    
        }
    }

    for( int x=0; x<CA_WIDTH; ++x){
        int xprev = x-1;
        if (xprev==-1) xprev = CA_WIDTH-1;
        int xnext = x+1;
        if(xnext==CA_WIDTH) xnext = 0;

        int cellRule = CA[1][xprev] * 4 
                    + CA[1][x] * 2 
                    + CA[1][xnext];
                    
        CA[0][x] = wrules[cellRule]; 
    }
}

void np::Automaton1D::random( float density ) noexcept{
    clear();
    for( int x=0; x<CA_WIDTH; ++x ){
        if(x%2==0){
            CA[0][x] = pdsp::chance(density) ? 1 : 0;  // just odd    
        }else{
            CA[0][x] = 0;         
        }
    }        

    for( int i=0; i<CA_HEIGHT-1; ++i ){
        advance();
    }
}

void np::Automaton1D::clear() noexcept{
    for( int y=0; y<CA_HEIGHT; ++y ){
        for( int x=0; x<CA_WIDTH; ++x ){
            CA[y][x] = 0;
        }        
    }    
}


