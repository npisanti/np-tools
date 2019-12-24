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

#pragma once

#include "ofMain.h"
#include "ofxPDSP.h"
#include "ofxGui.h"

#include "Percussion.h"
#include "Wolfram.h"
#include "effect/BasiVerb.h"
#include "effect/StereoDelay.h"
#include "dynamics/Brickwall.h"
#include "LiveParameters.h"
#include "RuleSeq.h"
#include "flags.h"

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

        void solo( int i );

        // pdsp modules
        pdsp::Engine   engine; 
        
        pdsp::Sequence masterplan;        
        np::RuleSeq ruleseq;
        np::sequence::Wolfram2 wolframSeq;

        np::effect::StereoDelay dub;
        np::effect::BasiVerb reverb;
        np::dynamics::Brickwall limiter;
        
        std::vector<np::synth::Percussion> percs;        
        ofParameterGroup synths;
        np::LiveParameters live;
   
        ofColor brightColor;
        ofColor darkColor;
        
        // ------------------
        ofxOscSender sender;
        int laststep;
        int lastplayhead;
        
        // ------------------   
        std::string seqPath;
        std::string synthPath;
        std::string outputIP;
        int outputPort;
        int device;
        bool headless;
     
};

