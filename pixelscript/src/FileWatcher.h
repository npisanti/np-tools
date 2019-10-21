/*====================================================================

	pixelscript - little 2d graphics scripting playground
  
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

#ifndef NP_FILEWATCHER_DEFINED
#define NP_FILEWATCHER_DEFINED

#include "ofMain.h"

namespace np{
    
class FileWatcher{

public:
    FileWatcher();
    
    void load( std::string path, bool watch=true );
    std::string getPath() const { return filepath; };

    float interval;

    ofParameterGroup parameters;

protected:
    std::string filepath;
    std::string filename;

    virtual void reload(){};

private:
    // --- variables for live check ---
    time_t writeTimestamp;
    float timePassed;
    void checkFile(ofEventArgs &args);
};
    
    
}

#endif // NP_FILEWATCHER_DEFINED
