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

#include "ofMain.h"
#include "ofxPDSP.h"
#include "Library.h"
#include "dynamics/SoftClip.h"
#include "meter/RMS.h"

namespace motore {
    
class Sampler : public pdsp::Patchable {
    
public:
    Sampler() { patch(); } 
    Sampler( const Sampler & other ) { patch(); } 
    
    ofParameterGroup parameters;
    ofParameterGroup & label( std::string name );
    
    pdsp::Patchable & in_trig();
    pdsp::Patchable & in_select();
    pdsp::Patchable & in_pitch();
    
    void linkToLibrary( motore::Library & library );
        
    void oscMapping( pdsp::osc::Input & osc, std::string address );
    
    pdsp::Patchable & ch( int i );  
    
    np::meter::RMS rms;
    
    int checkTrigger();
    
private:
    void patch ();
    
    pdsp::PatchNode     selectNode;
    pdsp::PatchNode     triggers;
    pdsp::Sampler       sampler;
    pdsp::AHR           env;
    pdsp::AHR           fEnv;
    pdsp::Amp           amp;

    pdsp::TriggeredRandom   drift;
    pdsp::Amp               driftAmt;
    pdsp::Parameter         driftControl;
    
    pdsp::PatchNode         pitchNode;
    pdsp::Parameter         pitchControl;
        
    pdsp::Parameter        attackControl;
    pdsp::Parameter        holdControl;
    pdsp::Parameter        releaseControl;
    pdsp::Parameter        lowCutControl;
    pdsp::Parameter        panControl;
    
    pdsp::ParameterGain    sendDelay;
    pdsp::ParameterGain    sendRev;
    
    pdsp::Panner           pan;
    
    pdsp::LowCut lowcut;
    
    np::dynamics::SoftClip softclip;
    
    // -----------------------------
    motore::Library * pLibrary;
    
    int cursor;    
    int tPast;

    std::atomic<int> instrument;


};   
    
} // end namespaces
