
#include "WaveSynth.h"

#include "routines/serialize.h"

#define WAVELEN 512

void np2::synth::WaveSynth::setup( std::string path ){

    // -------------- WAVES ------------------------------
    wt.setup( WAVELEN, 128 ); // 512 samples, 128 max partials
   
    // ------ null wave ------
    wt.addEmpty(); 
   
    // ------ organ waves ----
    wt.addAdditiveWave ( { 1.0f, 0.0f } ); 
    wt.addAdditiveWave ( { 1.0f, 1.0f } ); 
    wt.addAdditiveWave ( { 1.0f, 0.0f, 0.0f, 1.0f } ); 
    wt.addAdditiveWave ( { 1.0f, 1.0f, 1.0f, 1.0f } ); 
    wt.addAdditiveWave ( { 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f } ); 
    wt.addAdditiveWave ( { 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f } ); 

    for( int i=1; i<8; ++i ){
        
        float correct = 1.0f;
        switch( i ){ // some volume corrections 
            case 1: correct = 0.5; break;
            case 2: correct = 0.6; break;
            case 3: correct = 0.7; break;
            case 4: correct = 0.8; break;
        }
        
        for(int n=0; n<wt.tableLength(); n++){
            wt.table(i)[n] *=  correct;
        }
    }

    // --- NOISY WAVEs ---
    wt.addEmpty(); 
    int index = wt.size() -1;
    for(int n=0; n<wt.tableLength(); n++){
        wt.table(index)[n] = pdsp::brand();  
    }
    
    wt.addEmpty(); 
    index = wt.size() -1;
    int pulse = wt.tableLength() / 4;
    // setting the wavetable buffer
    for(int n=0; n<wt.tableLength(); n++){
        if(n<pulse){
            wt.table(index)[n] = pdsp::brand(); 
        }else{
            wt.table(index)[n] = 0.0f;
        }
    }
    
    wt.addEmpty(); 
    index = wt.size() -1;
    pulse = wt.tableLength() / 16;
    // setting the wavetable buffer
    for(int n=0; n<wt.tableLength(); n++){
        if(n<pulse){
            wt.table(index)[n] = pdsp::brand(); 
        }else{
            wt.table(index)[n] = 0.0f;
        }
    }
    
    // --- SAVED WAVES ----
    loadWaves( path + "/waves" );

    // -------------- SYNTH ------------------------------
    
    -2.0f >> clip0.in_threshold();
    -2.0f >> clip1.in_threshold();
    clip0.setOversampleLevel(2);
    clip1.setOversampleLevel(2);
    lowcut.ch(0) >> upsampler0 >> clip0 >> downsampler0 >> gain.ch(0);
    lowcut.ch(1) >> upsampler1 >> clip1 >> downsampler1 >> gain.ch(1);
    
    lowcutControl >> lowcut.in_freq();
    
    voices.resize( 9 );
    
    for( size_t i=0; i<voices.size(); ++i ){
        voices[i].setup( *this );
        float pan = -1.0 + 0.25f*i;
        voices[i] * (pdsp::panL( pan ) * dB(-6.0f) ) >> lowcut.ch(0);
        voices[i] * (pdsp::panR( pan ) * dB(-6.0f) ) >> lowcut.ch(1);
    }
    
    parameters.setName( "wavetable_synths" );
    parameters.add( gain.set("output_gain", -12, -48, 12 ) );
    parameters.add( drift.set("drift", 0.05f, 0.0f, 1.0f) );
    parameters.add( filterBase.set("filter_base", 60, 20, 132) );
    parameters.add( lowcutControl.set("low_cut", 100, 20, 1000) );
    
}

void np2::synth::WaveSynth::oscMapping( pdsp::osc::Input & osc, np::tuning::ModalTable * table ){
    for( size_t i=0; i<voices.size(); ++i ){
        std::string address = "/";
        address += ofToString( i+1 );
        voices[i].oscMapping( osc, address, table );
    }
}

void np2::synth::WaveSynth::loadWaves( std::string path ){
    ofDirectory dir;
    dir.allowExt("xml");
	dir.listDir( path );
	dir.sort(); 

    if( dir.size() > 0 ){
        std::vector<float> values;
        values.reserve( WAVELEN );
        
        for( size_t i=0; i<dir.size(); ++i ){
            np::deserialize_floats( dir.getPath(i), values );           
            wt.addEmpty(); 
            int index = wt.size() -1;
            for(int n=0; n<wt.tableLength(); n++){
                wt.table(index)[n] = values[n];
            }
        }        
    }
}

void np2::synth::WaveSynth::Voice::setup(WaveSynth & m){

    bTrig = false;
    envelope.set( 0.0f, 0.0f, 0.0f );
    envelope.setCurve( 1.0f ); 

    addModuleInput("trig", envelope.in_trig());
    addModuleInput("pitch", oscillator.in_pitch());
    addModuleInput("table", oscillator.in_table());
    addModuleInput("cutoff", filter.in_pitch());
    addModuleOutput("signal", voiceAmp);

    oscillator.setTable( m.wt );

    // SIGNAL PATH
    oscillator >> filter >> voiceAmp;
    
    // MODULATIONS AND CONTROL
    envelope >> envToTable >> oscillator.in_table();

    envelope >> voiceAmp.in_mod();
    m.filterBase >> filter.in_pitch();
    
    0.2f    >> phazorFree;
    0.05f  >> randomSlew.in_freq();
                                       m.drift >> driftAmt.in_mod();        
    phazorFree.out_trig() >> rnd >> randomSlew >> driftAmt;
                                                  driftAmt >> oscillator.in_pitch();
                                                  driftAmt * 7.0f >> filter.in_pitch();
    
    envelope >> envToFilter >> filter.in_pitch();
    m.filterBase >> filter.in_pitch();
    
}

void np2::synth::WaveSynth::Voice::oscMapping( pdsp::osc::Input & osc, std::string address, np::tuning::ModalTable * table ){
    
    osc.out_value( address, 0 ) >> oscillator.in_table();
    osc.parser( address, 0 ) = [&, table]( float value ) noexcept {
        m2 = value;
        return value;
    };      
       
    osc.out_value( address, 1 ) >> oscillator.in_pitch();
    osc.parser( address, 1 ) = [&, table]( float value ) noexcept {
        int i = value;
        m1 = i;
        bTrig = true;
        float p = table->pitches[i%table->degrees];
        int o = i / table->degrees;
        p += o*12.0f;
        return p;
    };       

    osc.out_trig( address, 2 ) >> envelope.in_trig();
    osc.out_trig( address, 2 ) >> envelope.in_attack();
    osc.parser( address , 2) = [&]( float value ) noexcept {
        return 1.0f + value * pdsp::Clockable::getOneBarTimeMs() * (1.0f/16.0f);
    };
    
    osc.out_trig( address, 3 ) >> envelope.in_release();
    osc.parser( address , 3) = [&]( float value ) noexcept {
        return 5.0f + value * pdsp::Clockable::getOneBarTimeMs() * (2.0f/16.0f);
    };
    
    osc.out_value( address, 4 ) * 12.0f >> envToFilter.in_mod();
    osc.parser( address, 4 ) = [&, table]( float value ) noexcept {
        m3 = value;
        return value;
    };      
    
    osc.out_value( address, 5 ) >> envToTable.in_mod();
    
    osc.out_value( address, 6 ) * 12.0f >> filter.in_pitch();
    
}
