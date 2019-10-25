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

#include "ofMain.h"
#include "ofApp.h"
#include "ofAppNoWindow.h"  

//========================================================================
int main( int argc, char *argv[] ){

    ofAppNoWindow window;  
    
    ofApp *app = new ofApp();

    if(argc>1){		
        std::string path = std::string( argv[1] );
        if( ! ofFilePath::isAbsolute( path )){
            path = ofFilePath::getCurrentWorkingDirectory() + "/" + path;
        }
        app->path = path;
        std::string ext = ofFilePath::getFileExt( path );
        
        if( ext!="json" ){
            std::cout<<"[rpiezos] input is not a .json file!\n";
            return 0;
        }else{
            ofSetupOpenGL(&window, 256,256,OF_WINDOW);  
            ofRunApp( app );            
        }        
    }
}
