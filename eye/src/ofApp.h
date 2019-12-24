#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxDotFrag.h"

class ofApp : public ofBaseApp{
    public:
        void setup();
        void update();
        void draw();
        
        void keyPressed(int key);
        void keyReleased(int key);

        ofVideoGrabber  webcam;
        ofFbo           fbo;
        
        bool bDrawGui;
        ofxPanel gui;
        
        ofx::dotfrag::Mirror mirror;
        ofx::dotfrag::Monochrome monochrome;
        ofx::dotfrag::ThreeTones tones;
        ofx::dotfrag::HSB hsb;
        
        int width;
        int height;
        int id;
};
