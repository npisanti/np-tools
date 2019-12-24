
#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main(int argc, char *argv[]){

    ofSetupOpenGL( 640, 480, OF_WINDOW);

    ofApp *app = new ofApp();
    
    app->width = 640;
    app->height = 480;
    app->id = 0;

    if(argc>1){		
        for( int i=1; i<argc; ++i ){
            switch( i ){
                case 1: app->width = ofToInt( argv[i] ); break;
                case 2: app->height = ofToInt( argv[i] );  break;
                case 3: app->id = ofToInt( argv[i] );  break;
                default : break;
            }
        }   
    }
  
    ofRunApp( app );
}
