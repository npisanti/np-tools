/*====================================================================

	masterbus - hub for all the instruments
  
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

    ofAppNoWindow window;  
    ofSetupOpenGL( &window, 240, 240, OF_WINDOW);	

    ofApp *app = new ofApp();
    
    app->path = "";
    app->outputPort = -1;
    app->outputIP = "localhost";
    app->device = 0;

    for( int i=1; i<argc; ++i ){
        std::string cmd = std::string( argv[i] );

        if( cmd == "--path" || cmd == "-p" ){
            if( i+1 < argc ){
                std::string path = std::string( argv[i+1] );
                if( ! ofFilePath::isAbsolute( path )){
                    path = ofFilePath::getCurrentWorkingDirectory() + "/" + path;
                }
                path = ofFilePath::removeTrailingSlash( path );
                app->path = path;
            }
        }
        if( cmd == "--device" || cmd == "-d" ){
            if( i+1 < argc ){
                app->device = std::stoi( argv[i+1] );
            }
        }
        if( cmd == "--out-port" || cmd == "-o" ){
            if( i+1 < argc ){
                app->outputPort = std::stoi( argv[i+1] );
            }
        }
        if( cmd == "--out-address" || cmd == "-a" ){
            if( i+1 < argc ){
                app->outputIP = std::string( argv[i+1] );
            }
        }
        if( cmd == "--list-devices" || cmd == "-l" ){
            ofSoundStreamListDevices();
            return 0;
        }
    }   
    
    ofRunApp( app );

	
}
