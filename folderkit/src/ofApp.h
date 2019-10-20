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

#pragma once

#define NUMSAMPLERS 8

#include "ofMain.h"
#include "ofxMidi.h"
#include "ofxPDSP.h"
#include "ofxOsc.h"
#include "StereoDelay.h"
#include "synth/WaveSynth.h"
#include "effect/Chorus.h"
#include "effect/Filter.h"
#include "effect/BasiVerb.h"
#include "effect/StereoDelay.h"
#include "meter/RMS.h"

#include "dynamics/Brickwall.h"

#include "dynamics/Comp.h"

#include "Library.h"
#include "Sampler.h"
#include "LiveParameters.h"


class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void exit();
        
        void drawMeter(float value, float min, float max, int x, int y, int w, int h);
        
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

        pdsp::Engine        engine;   
        pdsp::osc::Input    osc;
        
        ofxOscSender        sender;
          
        np::effect::BasiVerb  reverb;
        np::effect::StereoDelay delays;
        
        ofParameterGroup parameters;
        np::LiveParameters live;

        motore::Sampler samplers [ NUMSAMPLERS ];
        
        np::dynamics::Brickwall limiter;
        
        np::dynamics::Compressor percbus;
        
        pdsp::LowCut delaycut;
        
        bool bDrawGui;
        
        motore::Library library;
        
        std::string path;
        int inputPort;
        int outputPort;
        std::string outputIP;
        int device;
        
};