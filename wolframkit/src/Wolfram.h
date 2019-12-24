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

#include "ofMain.h"
#include "ofxPDSP.h"
#include "flags.h"
#include "Automaton1D.h"

namespace np { namespace sequence {

class Wolfram2 : public pdsp::Sequence {

public:
    Wolfram2();

    void draw( int ca_side, int bars_h, ofColor fg, ofColor bg );
    void setRule(int rule){ ca.setRule( rule); }
    
    int currentStep() const;
    float getStep( int step, int out ) const;

    std::vector<int> thresholds;

    std::atomic<int>   steps;
    std::atomic<int>   division;
    std::atomic<float> density;
    std::atomic<bool>  regenerate;
    
    vector<float>   stepbars;    

private:
    Automaton1D     ca;

};

}} // end namespaces
