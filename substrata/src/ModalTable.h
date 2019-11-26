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

#pragma once

#include "ofMain.h"
#include "ofxPDSP.h"

// pure tuning using integer ratios

namespace np { namespace tuning {

class ModalTable {

private:

    // private internal classes ------------------------------------------------
    struct RatioUI {

        RatioUI(){
            numerator.addListener(this, &RatioUI::ratioChange);
            denominator.addListener(this, &RatioUI::ratioChange);
        }

        void ratioChange( int & value ) {
            double ratio = double(numerator) / double (denominator);
            double bp = (double) (*basePitch);
            double freq = pdsp::p2f(bp);
            freq *= ratio;
            pitch = pdsp::f2p(freq);
        }
        
        void setBasePitch( ofParameter<int> & bp) {
            basePitch = &bp;
        }
        
        ofParameter<int>*   basePitch;
        ofParameter<int>    numerator;
        ofParameter<int>    denominator;
        
        float pitch;
    };

public: // ------------------- PUBLIC API ----------------------------

    ModalTable(){}
    ModalTable( const ModalTable & other ) { }
    
    ofParameterGroup & setup( std::string name="integer ratio mode" );
    ofParameterGroup & label( std::string name );  
    
    ofParameterGroup parameters;    
        std::vector<float> pitches;
        ofParameter<int> degrees;
    
    std::atomic<int> deg;
    std::atomic<int> base;
    
    pdsp::ValueControl tonalControl;
    
private: // ----------------------------------------------------------

    void updateAll( int & value );
    
    ofParameter<int>    masterPitchControl;
    vector<RatioUI>     ratios;

    
};

}} // end namespaces
