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

#include "ofParameterGroup.h"
#include "Wolfram.h"
#include "flags.h"

namespace np {
    
class RuleSeq {

public:
    struct Entry{
        Entry();
        int rule;
        int thresholds[ NUMSYNTHS ];
        float density;
    };

    struct Buffer{
        Buffer();
        float tempo;
        int division;
        int num_bars;
        int first;
        std::vector<Entry> entries;
        int iEntry;
    };

    RuleSeq();

    bool setup( std::string path );
    
    void setInterval( float time ){ interval=time; }
    
    bool changed();

    void next();
    int division() const;
    int tempo() const;
    int numBars() const;
    const Entry & entry() const;

private: 
    Buffer buffers[2];
    std::atomic<int> iBuffer;
    std::atomic<bool> bChanged; 
    int iEntry;
    
    std::string name;
    std::string path;
    float interval = 0.05f;
    time_t writeTimestamp = 0.0f;
    float timePassed = 0.0f;;
    
    void checkFile(ofEventArgs &args);
    void reload();
    void swapBuffers();


};
    
}
