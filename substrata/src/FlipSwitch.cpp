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

#include "FlipSwitch.h"

np::FlipSwitch::FlipSwitch(){
    gate = false;
}

void np::FlipSwitch::init( bool value ){
    gate = value;
}

float np::FlipSwitch::formula(const float &x) noexcept {
    if( pdsp::checkTrigger(x) ){
        gate = !gate;
        if( gate ){
            return 1.0f;
        }else{
            return PDSP_TRIGGER_OFF;
        }
    }
    return x;
}

void np::FlipSwitch::formulaAudioRate(float* &output, const float* &input, const int &bufferSize) noexcept {

    ofx_Aeq_Zero( output, bufferSize );
    
    for( int n=0; n<bufferSize; ++n ){
        if( pdsp::checkTrigger( input[n] ) ){
            gate = !gate;
            if( gate ){
                output[n] = 1.0f;
            }else{
                output[n] = PDSP_TRIGGER_OFF;
            }
        }
    }
}
