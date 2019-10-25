/*====================================================================

	rpiezos - raspberry pi piezo device 
  
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

namespace np {
    
class LiveParameters {

public:
    LiveParameters();

    void setup( ofParameterGroup & parameters, std::string path );
    void setInterval( float time ){ interval=time; }
    
    void enableWatching();
    
private: 
    ofParameterGroup * parameters;
    
    std::string name;
    std::string path;
    float interval = 0.05f;
    time_t writeTimestamp = 0.0f;
    float timePassed = 0.0f;;
    
    void checkFile(ofEventArgs &args);
    void reload();
};
    
}
