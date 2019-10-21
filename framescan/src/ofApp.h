/*====================================================================

	framescan - scans and sequences animation framews from webcam
  
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
#include "dotfrag/Mirror.h"
#include "dotfrag/Monochrome.h"
#include "dotfrag/HSB.h"
#include "dotfrag/Live.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
        
        ofVideoGrabber cam;
        
        ofFbo fbo;
        ofx::dotfrag::Mirror mirror;
        ofx::dotfrag::Monochrome mono;
        ofx::dotfrag::HSB hsb;
        
        ofx::dotfrag::Live invertfrag;
        ofParameterGroup invert;
        ofParameter<bool> iActive;
        ofParameter<float> iLow;
        ofParameter<float> iHigh;
        ofParameter<float> iBorder;
        
        
        ofx::dotfrag::Live borderfrag;
        
        ofxPanel gui;
        
        ofParameter<int> cutx;
        ofParameter<int> cuty;
        ofParameter<int> cutw;
        ofParameter<int> cuth;
        ofParameter<int> offsetX;
        ofParameter<int> offsetY;
        ofParameter<float> speed;
        ofParameter<int> pMode;
        ofParameter<int> cMode;
        
        void offcut();
        
        ofFbo precut;
        ofFbo cutfbo;
        
        std::vector<ofImage> frames;
        
        float cursor;
        
        int cutoverlay;
};
