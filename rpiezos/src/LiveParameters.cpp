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

#include "LiveParameters.h"
#include <boost/filesystem.hpp>
#include "ofJson.h"
#include "ofAppRunner.h"

np::LiveParameters::LiveParameters(){
    parameters = nullptr;
}

void np::LiveParameters::setup( ofParameterGroup & parameters, std::string path ){
    
    this->parameters = &parameters;
    
    this->path = path;
    if( ! ofFilePath::isAbsolute( path )){
        path = ofFilePath::getCurrentWorkingDirectory() + "/" + path;
    }

    name = ofFilePath::getFileName( path );

    ofFile file( path );
    if( !file.exists() ){
        ofLogNotice() << "[ rpiezos ] "<<name<<" file does not exists, creating with default parameters\n";
        ofJson json;
        ofSerialize( json, parameters );
        ofSavePrettyJson( path, json );        
    }
    
    reload();
}

void np::LiveParameters::checkFile(ofEventArgs &args){
    timePassed += ofGetLastFrameTime();
    if( timePassed > interval ){
        time_t checkTimestamp = boost::filesystem::last_write_time( path );
        if( checkTimestamp != writeTimestamp){
            writeTimestamp = checkTimestamp;
            reload();
            ofLogNotice() << "[ "<<name<<" ] file reloaded";
        }
    }
}

void np::LiveParameters::enableWatching(){
    ofAddListener(ofEvents().update, this, &np::LiveParameters::checkFile);
    writeTimestamp = boost::filesystem::last_write_time( path );    
}
    
void np::LiveParameters::reload(){
    ofJson json = ofLoadJson( path );
    ofDeserialize( json, *parameters );
}
