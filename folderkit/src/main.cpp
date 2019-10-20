/*====================================================================

	folderkit - companion drumkit for orca-c
  
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

#include "ofMain.h"
#include "ofApp.h"
#include "ofAppNoWindow.h"  

//========================================================================
int main( int argc, char *argv[] ){

    if(argc>1){		

        ofAppNoWindow window;  
        ofSetupOpenGL( &window, 240, 240, OF_WINDOW);	


        ofApp *app = new ofApp();
        
        app->path = "not assigned";
        app->inputPort = 4444;
        app->outputPort = -1;
        app->outputIP = "localhost";
        app->device = 0;

        std::string path = std::string( argv[1] );
        
        if( ! ofFilePath::isAbsolute( path )){
            path = ofFilePath::getCurrentWorkingDirectory() + "/" + path;
        }
        path = ofFilePath::removeTrailingSlash( path );
        app->path = path;
        
        for( int i=2; i<argc; ++i ){
            std::string cmd = std::string( argv[i] );

            if( cmd == "--device" || cmd == "-d" ){
                if( i+1 < argc ){
                    app->device = std::stoi( argv[i+1] );
                }
            }
            if( cmd == "--input-port" || cmd == "-i" ){
                if( i+1 < argc ){
                    app->inputPort = std::stoi( argv[i+1] );
                }
            }
            if( cmd == "--out-port" || cmd == "-o" ){
                if( i+1 < argc ){
                    app->outputPort = std::stoi( argv[i+1] );
                }
            }
            if( cmd == "--out-ip" || cmd == "-p" ){
                if( i+1 < argc ){
                    app->outputIP = std::string( argv[i+1] );
                }
            }

        }   
        
        ofRunApp( app );
    }else{
        std::cout<< "[ folderkit ] no folder path given! \n";
    }

	
}
