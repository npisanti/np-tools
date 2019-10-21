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

#include "ofParameterGroup.h"
#include "pdspCore.h"
#include "envelopes/AHR.h"
#include "ofx/Parameter.h"
#include "ofx/ParameterAmp.h"
#include "ofx/ParameterGain.h"
#include "modules/oscillators/FMOperator.h"
#include "modules/sources/BitNoise.h"
#include "modules/signal/Panner.h"
#include "modules/filters/VAFilter.h"
#include "random/TriggeredRandom.h"

namespace np { namespace synth {

class Percussion : public pdsp::Patchable{ 
public:
    Percussion(){ patch(); }
    Percussion(const Percussion & other){ patch(); }

    float meter() const ;

    pdsp::Patchable & in_trig();
    pdsp::Patchable & in_pitch();

    ofParameterGroup parameters;

    ofParameterGroup & label( string name );

    void setPitch( float value ){ pitchControl.getOFParameterFloat() = value; }
    
    pdsp::Amp solo;
    
private:
    void patch();

    pdsp::PatchNode     trigger;
    pdsp::FMOperator    osc;
    pdsp::BitNoise      noise;
    
    pdsp::AHR           aEnv;
    pdsp::AHR           mEnv;
    pdsp::Amp           amp;

    pdsp::Panner        panner;

    pdsp::ParameterAmp  sineamp;
    pdsp::ParameterAmp  noiseamp;
    
    pdsp::ParameterAmp noiseFM;
    pdsp::ParameterAmp noiseRM;
    pdsp::ParameterAmp oscMod;
    
    pdsp::Amp           rm;
    
    ofParameterGroup filterGroup;
    pdsp::VAFilter             filter;
    pdsp::Parameter            filterTypeControl;
    pdsp::Parameter            filterCutoffControl;
    pdsp::Parameter            filterResoControl;

    pdsp::Parameter        pitchControl;    
    pdsp::Parameter        fbControl;
    pdsp::Parameter        noisePitchControl;
    pdsp::Parameter        noiseDecimateControl;
    pdsp::Parameter        bitsControl;

    pdsp::Parameter        veloToEnv;
    pdsp::Parameter        mEnvAmtControl;
    pdsp::Parameter        mEnvAttackControl;
    pdsp::Parameter        mEnvReleaseControl;

    ofParameterGroup       ampEnvelope;
    pdsp::Parameter        envAttackControl;
    pdsp::Parameter        envHoldControl;
    pdsp::Parameter        envReleaseControl;

    ofParameterGroup       oscGroup;
    ofParameterGroup       noiseGroup;

    ofParameterGroup       modEnvelope;
    pdsp::ParameterAmp         modFilterAmt;
    pdsp::ParameterAmp         modNoisePitchAmt;
    pdsp::ParameterAmp         modDecimateAmt;
    pdsp::ParameterAmp         fbModControl;
    pdsp::ParameterAmp         oscPitchModControl;


    pdsp::Parameter            randomControl;
    pdsp::Parameter            randomCutoffControl;
    pdsp::TriggeredRandom   random0;    
    pdsp::TriggeredRandom   random1;    
    pdsp::TriggeredRandom   random2;    
    pdsp::Amp               randomPitch;
    pdsp::Amp               randomCutoff;
    pdsp::Amp               randomDecimate;
    
    ofParameterGroup mixer;
    pdsp::Parameter         panControl;
    pdsp::ParameterGain     gain;
    
  
    pdsp::ParameterGain sendDel;
    pdsp::ParameterGain sendRev;


};

}} // end namespaces
