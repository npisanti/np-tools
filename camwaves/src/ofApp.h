/*====================================================================

	camwaves - export waveforms from webcam 
  
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

#include "ofMain.h"
#include "ofxMidi.h"
#include "ofxPDSP.h"
#include "ofxGui.h"
#include "synth/DataSynth.h"

#include "dotfrag/Monochrome.h"
#include "dotfrag/Mirror.h"

#include "routines/serialize.h"

// decomment to use a midi keyboard instead of the computer keys
//#define USE_MIDI_KEYBOARD

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
        
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
        
        pdsp::Engine   engine;

        np::synth::DataSynth synth;
        
#ifdef USE_MIDI_KEYBOARD
		pdsp::midi::Keys        keyboard;
		pdsp::midi::Input       midiIn;
#else
        pdsp::ComputerKeyboard  keyboard;
#endif

        ofxPanel                    gui;

        ofVideoGrabber vidGrabber;
        int camWidth;
        int camHeight;
        int camPosX;
        int camPosY;
        
        ofParameterGroup position;
        ofParameter<int> px;
        ofParameter<int> py;
        
        std::vector<float> values;
        
        ofPixels pixels;
        ofFbo fbo;
        ofx::dotfrag::Mirror mirror;
        ofx::dotfrag::Monochrome monochrome;
		
		ofFbo waveplot;
		
        ofFbo savedPlot;
        std::string savedPath;
        int savedShow;  
        
        int savecounter;
};
