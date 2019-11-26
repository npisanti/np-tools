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

#include "ofParameterGroup.h"
#include "pdspCore.h"
#include "envelopes/AHR.h"
#include "modules/filters/VAFilter.h"
#include "random/TriggeredRandom.h"
#include "ofx/Parameter.h"
#include "ofx/ParameterAmp.h"
#include "utility/DBtoLin.h"
#include "modules/sources/BitNoise.h"
#include "ModalTable.h"

namespace np2 { namespace synth {
    
class NoiseDevice : public pdsp::Patchable {

public:
    NoiseDevice(){ patch(); };
    NoiseDevice(const NoiseDevice & other){ patch(); };
    
    ofParameterGroup parameters;

    float meter() const;
    
    pdsp::Patchable & in_trig();
    pdsp::Patchable & in_mod();
    pdsp::Patchable & in_hold();
    pdsp::Patchable & in_release();

    pdsp::Patchable & ch( size_t index );

    ofParameterGroup & label( string name );

    void oscMapping( pdsp::osc::Input & osc, std::string address, np::tuning::ModalTable * table );
    
    

private:
    void patch();
    
    pdsp::BitNoise          noise;

    pdsp::AHR               env;
    pdsp::Amp               amp0;
    pdsp::Amp               amp1;
    pdsp::PatchNode         trigger;
    
    pdsp::VAFilter             filter;
    pdsp::Parameter            filterTypeControl;
    pdsp::Parameter            filterResoControl;
    
    pdsp::Parameter            noiseDecimateControl;
    
    pdsp::Amp                  modFilterAmt;
    
    pdsp::Parameter            lowcutControl;


    pdsp::Parameter            bitsControl;
    
    pdsp::LowCut               lowcut;
    

    pdsp::Amp           gain0;
    pdsp::Amp           gain1;
    pdsp::DBtoLin       dBtoLin;
    pdsp::Parameter     gainControl;  

};

}} // end namespaces
