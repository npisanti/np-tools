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
#include "ofx/Parameter.h"
#include "ofx/ParameterAmp.h"
#include "utility/DBtoLin.h"
#include "modules/oscillators/FMOperator.h"
#include "ModalTable.h"

namespace np2 { namespace synth {

class SinePercussion : public pdsp::Patchable{ 
public:
    SinePercussion(){ patch(); }
    SinePercussion(const SinePercussion & other){ patch(); }

    void enableDB( float minValue=-18.0f );
    void disableDB( );
    
    float meter() const ;

    pdsp::Patchable & in_trig();
    pdsp::Patchable & in_pitch();
    
    pdsp::Patchable & out_env();

    ofParameterGroup parameters;

    ofParameterGroup & label( string name );

    void oscMapping( pdsp::osc::Input & osc, std::string address, np::tuning::ModalTable * table );
    
private:
    void patch();

    pdsp::PatchNode     triggers;
    pdsp::FMOperator    sine;
    pdsp::AHR           aEnv;
    pdsp::AHR           pEnv;
    pdsp::Amp           amp;
    pdsp::Amp           pModAmt;


    pdsp::Parameter        envAttackControl;
    pdsp::Parameter        envHoldControl;
    pdsp::Parameter        envReleaseControl;

    pdsp::Amp              gain;
    pdsp::DBtoLin          dBtoLin;
    pdsp::Parameter        gainControl;  

};

}} // end namespaces
