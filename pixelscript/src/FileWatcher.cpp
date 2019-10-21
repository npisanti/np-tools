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

#include "FileWatcher.h"

np::FileWatcher::FileWatcher(){
    
    parameters.setName("label me");
    interval = 0.05f;
    timePassed = 0.0f;
    
}

void np::FileWatcher::load( std::string path, bool watch ) {
    filepath = path;
    filename = ofFilePath::getFileName( filepath ); 

    if( ! ofFilePath::isAbsolute( filepath )){
        filepath = ofFilePath::getCurrentWorkingDirectory() + "/" + filepath;
    }

    ofFile file( filepath );
    if( file.exists() ){
        if( watch ){
            ofAddListener(ofEvents().update, this, &FileWatcher::checkFile);
            writeTimestamp = boost::filesystem::last_write_time( filepath );
        }
        reload();
        ofLogNotice() << "["<<filename<<"] file loaded";
    }else{
        ofLogError() << "[np::FileWatcher] file to load at "<<filepath <<" does not exists\n";
    }
    
    parameters.setName( filename );
}


void np::FileWatcher::checkFile(ofEventArgs &args){
    timePassed += ofGetLastFrameTime();
    if( timePassed > interval ){
        time_t checkTimestamp = boost::filesystem::last_write_time( filepath );
        if( checkTimestamp != writeTimestamp){
            writeTimestamp = checkTimestamp;
            reload();
            ofLogNotice() << "["<<filename<<"] file reloaded";
        }
    }
}
