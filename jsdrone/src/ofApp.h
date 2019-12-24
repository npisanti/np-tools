#pragma once

#include "ofMain.h"
#include "ofxPDSP.h"
#include "ofxGui.h"
#include "DA4Joystick.h"
#include "ofxGuiPresetSelector.h"
#include "synth/GrainDrone.h"
#include "util/SampleControl.h"

class ofApp : public ofBaseApp, public np::DA4Listener {

public:
    void setup();
    void update();
    void draw();
    void exit();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    void buttonPressed( int button, int value );
    void axisMoved( int axis, int value );
    
    void recalculateAxis2Angle();
    
    pdsp::Engine        engine;
    ofxPanel            droneGUI;
    ofxPanel            samplesGUI;
    
    np::DA4Joystick                         js;
    np::synth::GrainDrone                   drone;
    std::vector<np::util::SampleControl>    samples;
    
    pdsp::Parameter angleMod;
    float x2;
    float y2;
    ofParameter<float> angleModAmount;

    ofxGuiPresetSelector selector;
};
