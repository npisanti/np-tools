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

#include "ofMain.h"
#include "ofApp.h"
#include "ofAppNoWindow.h"  


//====================================================================
int main( int argc, char *argv[] ){

    if(argc>1){		
        
        ofApp *app = new ofApp();
        
        app->headless=false;
        app->seqPath = "";
        app->synthPath = "";
        app->outputPort = -1;
        app->outputIP = "localhost";
        app->device = 0;

        for( int i=1; i<argc; ++i ){
            std::string cmd = std::string( argv[i] );

            if( cmd == "--device" || cmd == "-d" ){
                if( i+1 < argc ){
                    app->device = std::stoi( argv[i+1] );
                }
            }
            if( cmd == "--headless" || cmd == "-h" ){
                app->headless = true;
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
            if( cmd == "--seq" || cmd == "--sequences" || cmd == "-e" ){
                if( i+1 < argc ){
                    std::string path = std::string( argv[i+1] );
                    if( ! ofFilePath::isAbsolute( path )){
                        path = ofFilePath::getCurrentWorkingDirectory() + "/" + path;
                    }
                    app->seqPath=path;
                }
            }
            if( cmd == "--synths" || cmd == "--synth" || cmd == "-y" ){
                if( i+1 < argc ){
                    std::string path = std::string( argv[i+1] );
                    if( ! ofFilePath::isAbsolute( path )){
                        path = ofFilePath::getCurrentWorkingDirectory() + "/" + path;
                    }
                    app->synthPath=path;
                }
            }
            if( cmd == "--list-devices" || cmd == "-l" ){
                ofSoundStreamListDevices();
                return 0;
            }
        }   
        
        if( app->synthPath=="" ){
            std::cout<< "[ wolframkit ] no synths path given! \n";
            //return 0;
        }
        if( app->seqPath=="" ){
            std::cout<< "[ wolframkit ] no sequences path given! \n";
            //return 0;
        }
        
        ofAppNoWindow window; 
        if( app->headless ){
            ofSetupOpenGL( &window, 240, 240, OF_WINDOW);	
        }else{
            ofSetupOpenGL( 1910/2, 210, OF_WINDOW);	
        }

        ofRunApp( app );
    }else{
        std::cout<< "[ wolframkit ] no folder path given! \n";
    }	
}
