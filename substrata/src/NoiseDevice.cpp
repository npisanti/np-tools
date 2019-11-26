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

#include "NoiseDevice.h"

void np2::synth::NoiseDevice::patch() {
  
    addModuleInput ( "trig", trigger );
    addModuleInput ( "hold", env.in_hold() );
    addModuleInput ( "release", env.in_release() );

    addModuleOutput( "L", gain0 );
    addModuleOutput( "R", gain1 );
    addModuleOutput( "env", env );
    
    noise.ch(0) >> filter.ch(0) >> amp0 >> lowcut.ch(0) >> gain0;
    noise.ch(1) >> filter.ch(1) >> amp1 >> lowcut.ch(1) >> gain1;
    
    trigger >> noise.in_trig();
    trigger >> env >> amp0.in_mod();
               env >> amp1.in_mod();
    
    lowcutControl >> lowcut.in_freq();
    
    // map controls 
    noiseDecimateControl >> noise.in_decimation();
    
    env.set( 0.0f, 0.0f, 0.0f );
    
    env >> modFilterAmt >> filter.in_cutoff();

    filterTypeControl   >> filter.in_mode();
    filterResoControl   >> filter.in_reso();

    bitsControl >> noise.in_bits();
    
    gainControl >> dBtoLin  >> gain0.in_mod();    
                   dBtoLin  >> gain1.in_mod();    
    
    parameters.setName( "noise device");
    parameters.add( gainControl.set("gain", -12, -48, 24) );
    parameters.add( noiseDecimateControl.set ("decimate", 160, 1, 160) );
    parameters.add( bitsControl.set ("bits", 24, 24, 1) );
    parameters.add( filterTypeControl.set("filter mode", 0, 0, 5 ) );
    parameters.add( filterResoControl.set("filter reso", 0.0f, 0.0f, 1.0f ) );
    parameters.add( lowcutControl.set("lowcut", 100, 20, 1000 ) );
}

void np2::synth::NoiseDevice::oscMapping( pdsp::osc::Input & osc, std::string address, np::tuning::ModalTable * table ){
    
    -60.0f >> noise.in_pitch();
    osc.out_value( address, 0 ) >> noise.in_pitch();
    osc.out_value( address, 0 ) >> noise.in_pitch();
    osc.parser( address, 0 ) = [&, table]( float value ) noexcept {
        int i = value;
        float p = table->pitches[i%table->degrees];
        int o = i / table->degrees;
        p += o*12.0f;
        return p;  
    };       

    osc.out_trig( address, 1 ) >> trigger;
    osc.out_trig( address, 1 ) >> env.in_release();
    osc.parser( address , 1) = [&]( float value ) noexcept {
        return 5 + value * pdsp::Clockable::getOneBarTimeMs() * (1.0f/16.0f);
    };
    
    osc.out_value( address, 2 ) * 12.0f >> modFilterAmt.in_mod();
    osc.out_value( address, 2 ) * -12.0f >> filter.in_pitch();
                                  132.0f >> filter.in_pitch();
    
    osc.out_value( address, 3 ) * 12.0f >> noise.in_pitch();
    
    osc.out_value( address, 4 ) * (-6.0f) >> noise.in_decimation();
    
}

ofParameterGroup & np2::synth::NoiseDevice::label( string name ){
    parameters.setName(name);
    return parameters;
}    
    
float np2::synth::NoiseDevice::meter() const {
    return env.meter_output();
}

pdsp::Patchable & np2::synth::NoiseDevice::in_trig(){
    return in("trig");
}
pdsp::Patchable & np2::synth::NoiseDevice::in_mod(){
    return in("mod");
}
pdsp::Patchable & np2::synth::NoiseDevice::in_hold(){
    return in("hold");
}
pdsp::Patchable & np2::synth::NoiseDevice::in_release(){
    return in("release");
}

pdsp::Patchable & np2::synth::NoiseDevice::ch( size_t index ) {
    pdsp::wrapChannelIndex( index );
    switch( index ){
        case 0: return out("L"); break;
        case 1: return out("R"); break;
    }
    return out("L"); 
}
