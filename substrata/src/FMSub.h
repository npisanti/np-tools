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
#include "HQSaturator.h"
#include "ModalTable.h"

namespace np { namespace synth {

class FMSub : public pdsp::Patchable {

public:

    FMSub(){ patch(); }
    FMSub( const FMSub& other ){ patch(); }

    pdsp::Patchable& in_trig();
    pdsp::Patchable& in_pitch();
    
    float meter_mod_env() const;
    float meter_pitch() const;

    std::atomic<int> m1;
    std::atomic<bool> bTrig;
    std::atomic<bool> bPitch;

    ofParameterGroup    parameters;

    ofParameterGroup & label (std::string name );

    void oscMapping( pdsp::osc::Input & osc, std::string address, np::tuning::ModalTable * table );

    void patch();
    
    // ------- parameters ------------------------------------

    pdsp::PatchNode     voiceTrigger;
    pdsp::PatchNode     pitchNode;
    
    pdsp::FMOperator    carrierA;
    pdsp::FMOperator    carrierB;
    pdsp::FMOperator    modulator;

    pdsp::Amp           fmAmp;
    pdsp::Amp           voiceAmp;

    pdsp::AHR           ampEnv;
    pdsp::Parameter         envAttackControl;
    pdsp::Parameter         envReleaseControl;

    pdsp::AHR           modEnv;
    pdsp::Parameter         modEnvAttackControl;
    pdsp::Parameter         modEnvHoldControl;
    pdsp::Parameter         modEnvReleaseControl;            

    pdsp::Amp               driftAmt;
    pdsp::LFOPhazor         phazorFree;
    pdsp::TriggeredRandom   rnd;
    pdsp::OnePole           randomSlew;

    pdsp::Amp  fm_mod;

    pdsp::Parameter     ratio_ctrl;

    pdsp::Parameter     drift;
    
    pdsp::Parameter     detune_ctrl;
    pdsp::Parameter     osc_mix;
    
    pdsp::LinearCrossfader oscXFader;

    np2::effect::HQSaturator saturator;
};

}} // end namspaces 
