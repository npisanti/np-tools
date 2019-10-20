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

#pragma once

#include "ofMain.h"
#include "ofxPDSP.h"
#include "StereoDelay.h"
#include "effect/BasiVerb.h"
#include "StereoRMS.h"
#include "dynamics/Brickwall.h"
#include "LiveParameters.h"


class ofApp : public ofBaseApp{

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

        pdsp::Engine engine;   
        int device;
        
        np::meter::StereoRMS rms;
        
        np::effect::BasiVerb  reverb;
        np::dynamics::Brickwall limiter;
        
        ofParameterGroup parameters;
        np::LiveParameters live;
        std::string path;
        
        ofxOscSender        sender;
        int outputPort;
        std::string outputIP;
        
        int clock;
        
};
