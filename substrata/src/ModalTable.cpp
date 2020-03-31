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

#include "ModalTable.h"

np::tuning::ModalTable::ModalTable(){
    degrees = TABLE_DEGREES;
   
    for( int i=0; i<TABLE_DEGREES; ++i ){
        ratios[i].numerator = i + 12;
        ratios[i].denominator = 12;
    }
    base = 29;
    recalculate();
}
    
void np::tuning::ModalTable::recalculate(){
    for( int i=0; i<TABLE_DEGREES; ++i ){
        double ratio = double(ratios[i].numerator) / double (ratios[i].denominator);
        double bp = double( base );
        double freq = pdsp::p2f( bp );
        freq *= ratio;
        pitches[i] = pdsp::f2p(freq);
    }
}

void np::tuning::ModalTable::oscMapping( pdsp::osc::Input & osc ){
    for( int index=0; index<TABLE_DEGREES; ++index ){
        osc.parser( "/p", index*2 ) = [&, index]( float value ) noexcept {
            ratios[index].numerator = value;
            return pdsp::osc::Ignore;
        };
        osc.parser( "/p", index*2 +1 ) = [&, index]( float value ) noexcept {
            ratios[index].denominator = value;
            return pdsp::osc::Ignore;
        };
    }
    osc.parser( "/p", TABLE_DEGREES*2 ) = [&]( float value ) noexcept {
        base = 24+value;
        recalculate();
        return pdsp::osc::Ignore;
    };
}
    
