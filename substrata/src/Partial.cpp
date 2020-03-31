
#include "Partial.h"

#include "routines/serialize.h"

#define WAVELEN 512

void np2::synth::Partial::setup( std::string path ){
   // -------------- SYNTH ------------------------------
    voices.resize( 18 );
    
    for( size_t i=0; i<voices.size(); ++i ){
        voices[i].setup( *this );
        
        voices[i].pan.out_L() >> gain.ch(0); 
        voices[i].pan.out_R() >> gain.ch(1);
    }

	for( size_t i=1; i<voices.size(); ++i ){
		voices[i-1].out("fm") >> voices[i].in("fm");
    }

	for( size_t i=0; i<9; ++i ){
		voices[i].out("fm") >> voices[i+9].in("up");
    }
    
    parameters.setName( "wavetable_synths" );
    parameters.add( gain.set("output_gain", -12, -48, 12 ) );
    parameters.add( drift.set("drift", 0.05f, 0.0f, 1.0f) );
    
}

void np2::synth::Partial::oscMapping( pdsp::osc::Input & osc, np::tuning::ModalTable * table ){
    
    for( size_t i=0; i<voices.size(); ++i ){
        std::string address;
        switch( i ){
            case 0: address = "/q"; break;
            case 1: address = "/w"; break;
            case 2: address = "/e"; break;
            case 3: address = "/r"; break;
            case 4: address = "/t"; break;
            case 5: address = "/y"; break;
            case 6: address = "/u"; break;
            case 7: address = "/i"; break;
            case 8: address = "/o"; break;
            
            case 9: address = "/a"; break;
            case 10: address = "/s"; break;
            case 11: address = "/d"; break;
            case 12: address = "/f"; break;
            case 13: address = "/g"; break;
            case 14: address = "/h"; break;
            case 15: address = "/j"; break;
            case 16: address = "/k"; break;
            case 17: address = "/l"; break;
            
            default: address = "/boh"; break;
        }
        voices[i].oscMapping( osc, address, table );
    }
}

void np2::synth::Partial::Voice::setup(Partial & m){

    bTrig = false;
    envelope.set( 0.0f, 0.0f, 0.0f );
    envelope.setReleaseCurve( 0.5f ); 

    addModuleInput("trig", envelope.in_trig());
    addModuleInput("pitch", oscillator.in_pitch());
    addModuleInput("fm", fmAmp );
    addModuleInput("up", upAmp );
    addModuleOutput("signal", multAmp);
    addModuleOutput("fm", voiceAmp);

    // SIGNAL PATH
    oscillator >> voiceAmp  >> multAmp >> pan;

    fmAmp >> oscillator.in_fm(); 
    upAmp >> oscillator.in_fm(); 
    
    // MODULATIONS AND CONTROL

    envelope >> voiceAmp.in_mod();
    
    0.2f    >> phazorFree;
    0.05f  >> randomSlew.in_freq();
                                       m.drift >> driftAmt.in_mod();        
    phazorFree.out_trig() >> rnd >> randomSlew >> driftAmt;
                                                  driftAmt >> oscillator.in_pitch();
    
}

void np2::synth::Partial::Voice::oscMapping( pdsp::osc::Input & osc, std::string address, np::tuning::ModalTable * table ){
       
    12.0f >> oscillator.in_pitch();
    osc.out_value( address, 0 ) >> oscillator.in_pitch();
    osc.parser( address, 0 ) = [&, table]( float value ) noexcept {
        int i = value;
        m1 = i;
        bTrig = true;
        float p = table->pitches[i%table->degrees];
        int o = i / table->degrees;
        p += o*12.0f;
        return p;
    };      
    
    osc.out_value( address, 1 ) >> multAmp.in_mod();
    osc.parser( address, 1 ) = [&]( float value ) noexcept {
	if( value == 0.0f ) return 0.0f;
        return 1.0f/value;
    };

    osc.out_value( address, 2 ) >> oscillator.in_ratio(); 
    osc.parser( address, 2 ) = [&, table]( float value ) noexcept {
    	if( value==0.0f ) return 0.5f;
		return value;
    };

	osc.out_value( address, 3 ) * 0.25 >> fmAmp.in_mod();
	osc.out_value( address, 4 ) * 0.25 >> upAmp.in_mod();

    osc.out_trig( address, 5 ) >> envelope.in_trig();
    osc.out_trig( address, 5 ) >> envelope.in_attack();
    osc.parser( address , 5) = [&]( float value ) noexcept {
        return 1.0f + value * pdsp::Clockable::getOneBarTimeMs() * (1.0f/16.0f);
    };
    
    osc.out_trig( address, 6 ) >> envelope.in_release();
    osc.parser( address , 6) = [&]( float value ) noexcept {
        return 5.0f + value * pdsp::Clockable::getOneBarTimeMs() * (1.0f/16.0f);
    };

    osc.out_value( address, 7 ) >> pan.in_pan();
    osc.parser(address, 7) = [&]( float value ) noexcept {
        int pan = value;
        if( pan > 9 ) pan = 9;
        switch( pan ){
            default: case 0: case 5: return 0.0f; break;
            case 1: return -1.0f; break;
            case 2: return -0.75f; break;
            case 3: return -0.5f; break;
            case 4: return -0.25f; break;
            case 6: return 0.25f; break;
            case 7: return 0.5f; break;
            case 8: return 0.75f; break;
            case 9: return 1.0f; break;
        }
    };     
}
