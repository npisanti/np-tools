/*====================================================================

	substrata - companion drumkit for orca-c
  
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
#include "Library.h"
#include "dynamics/SoftClip.h"
#include "meter/RMS.h"
#include "ModalTable.h"
#include "FlipSwitch.h"

namespace substrata {
    
class Sampler : public pdsp::Patchable {
    
public:
    Sampler() { patch(); } 
    Sampler( const Sampler & other ) { patch(); } 
    
    ofParameterGroup parameters;
    ofParameterGroup & label( std::string name );
    
    pdsp::Patchable & in_trig();
    pdsp::Patchable & in_select();
    pdsp::Patchable & in_pitch();
    
    void linkToLibrary( substrata::Library & library );
        
    void oscMapping( pdsp::osc::Input & osc, std::string address, np::tuning::ModalTable * table );
    
    pdsp::Patchable & ch( int i );  
    
    np::meter::RMS rms;
    
    int checkTrigger();

    pdsp::Parameter        lowCutControl;    
    
    pdsp::ADSR          fadeEnvs[2];

    std::atomic<int> instrument;
    std::atomic<int> subfolder;
    
private:
    void patch ();
    
    pdsp::PatchNode     selectNode;
    pdsp::PatchNode     startNode;
    pdsp::PatchNode     triggers;
    
    pdsp::Sampler       samplers[2];
    pdsp::Amp           fadeAmps[2];
    np::FlipSwitch      flips[2];
    
    pdsp::AHR           env;
    pdsp::Amp           amp;

    pdsp::TriggeredRandom   samplerDrift;

    pdsp::CombFilter comb;     
    pdsp::LFOPhazor         phazorFree;
    pdsp::TriggeredRandom   rnd;
    pdsp::OnePole           randomSlew;
    
    pdsp::Panner           pan;

    pdsp::LowCut lowcut;


    // -----------------------------
    pdsp::DBtoLin inputControl;
    pdsp::Amp inputGainStage;
    pdsp::SoftClip clip;
    pdsp::Amp outputGainStage; 
    pdsp::DBtoLin outputControl;    
    
    // -----------------------------
    substrata::Library * pLibrary;
    
    int cursor;    
    int tPast;
};   
    
} // end namespaces
