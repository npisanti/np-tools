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
#include <atomic>

namespace substrata {
    
class Correlation{
public:
    Correlation();
    int sampleMin;
    int sampleMax;
    int now;
    int range;
};

static std::string const orcanums[] = {
    "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "a", "b", //  0 - 11
    "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", // 12 - 23
    "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z", // 24 - 35
};

class Library{
public:
    
    void init( std::string path );

    void correlate( int instrument, float & select );
    
    std::vector<Correlation> correlations;
    
    std::vector<pdsp::SampleBuffer*> samples;
    
    std::vector<int> availables;
    
    void free();
    
private:
    void correlateFolder( std::string path, int index );
    
};
    
}
