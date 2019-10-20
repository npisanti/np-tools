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

#include "Wolfram.h"

np::sequence::Wolfram2::Wolfram2(){

    stepbars.resize( CA_WIDTH );
    thresholds.resize( NUMSYNTHS );
    
    this->steps = 16;
    this->division = 16;
    this->density = 0.5;
    this->regenerate = true;
    
    code = [&] () noexcept {
        
        if(regenerate){
            ca.random( density );
            regenerate = false;
        }else{
            ca.advance();
        }

        for(int x=0; x < CA_WIDTH; ++x){
            int sum = 0;
            sum += ca.CA[0][x];
            sum += ca.CA[1][x];
            sum += ca.CA[2][x];
            sum += ca.CA[3][x];
            sum += ca.CA[4][x];
            sum += ca.CA[5][x];
            sum += ca.CA[6][x];
            sum += ca.CA[7][x];
            
            int sect = x / steps;
            float value = 0.0f;
                        
            if( sect <NUMSYNTHS ){
                int tr = thresholds[sect];
                sum -= tr;
                int range = CA_HEIGHT - tr;
       
                if( sum>0 && range>0 ){
                    value = float( sum ) / float ( range );
                }              
            }

            stepbars[x] = value;
        } 
        
        // seqs array to messages ---------------------------
        this->bars = double(steps) / double(division);
        
        double d = division;
        
        this->begin();       
            for(int s=0; s<steps; ++s){ // steps
                for(int o=0; o < NUMSYNTHS; ++o) { // outputs
                    float outval = stepbars[ (o*steps) + s ];
                    if ( outval > 0.0f ){
                        this->delay( s / d ).out( o ).bang( outval );
                    }
                }
            }
        this->end();
    };    
    
}

int np::sequence::Wolfram2::currentStep() const {
    return meter_percent() * steps;
}

float np::sequence::Wolfram2::getStep( int step, int out ) const{
    return stepbars[ step + out*steps ];
}

void np::sequence::Wolfram2::draw( int ca_side, int bars_h, ofColor fg, ofColor bg ){
    
    ofSetColor( fg );
    
    for( int y=0; y<CA_HEIGHT; ++y ){
        for( int x=0; x<CA_WIDTH; ++x ){
            if( ca.CA[y][x] ){
                ofFill();
            }else{
                ofNoFill();
            }
            ofDrawRectangle( x*ca_side, y*ca_side, ca_side, ca_side );
        }
    }

    int playhead = meter_percent() * steps; 

    ofPushMatrix();
    ofTranslate( 0, ca_side * (CA_HEIGHT+1));
    
    ofSetColor( bg );
    for( int x=0; x<CA_WIDTH; ++x ){
        if( x%steps == playhead ){
            ofSetColor( fg );
        }else{
            ofSetColor( bg );
        }
        ofFill();
        ofDrawRectangle( ca_side*x, bars_h, ca_side, -(stepbars[x]*bars_h) );
        ofSetColor( bg );
        ofNoFill();
        ofDrawRectangle( ca_side*x, 0, ca_side, bars_h );
    }
    
    ofTranslate( 0, bars_h );
    
    ofSetColor( fg );

    int sw = ca_side*steps;
    float soff = meter_percent() * sw;
    int sh = ca_side*2;
    for( int i=0; i<NUMSYNTHS; ++i ){
        int sx = sw*i;
        ofNoFill();
        ofDrawRectangle( sx, 0, sw, sh );
        ofDrawLine( sx + soff, 0, sx+soff, sh );
    }

    ofPopMatrix();
}

