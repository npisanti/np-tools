
#pragma once

#include "ofMain.h"
#include "ofxPDSP.h"
#include "ModalTable.h"

// wavetable based polysynth

namespace np2 { namespace synth {

class WaveSynth {

public:
    // class to rapresent each synth voice ------------
    class Voice : public pdsp::Patchable {
        friend class WaveSynth;
    
    public:
        Voice(){}
        Voice(const Voice& other){}
        
        float meter_env() const;
        float meter_pitch() const;
        
        void oscMapping( pdsp::osc::Input & osc, std::string address, np::tuning::ModalTable * table );
    
        std::atomic<bool> bTrig;
        std::atomic<int> m1;
        std::atomic<int> m2;
        std::atomic<int> m3;

    private:
        void setup(WaveSynth & m);

        pdsp::TableOscillator   oscillator;
        pdsp::VAFilter          filter;
        pdsp::Amp               voiceAmp;

        pdsp::AHR           envelope;    
        pdsp::Amp           envToTable;
        pdsp::Amp           envToFilter;  
                
        pdsp::Amp               driftAmt;
        pdsp::LFOPhazor         phazorFree;
        pdsp::TriggeredRandom   rnd;
        pdsp::OnePole           randomSlew;
        
        pdsp::Panner            pan;
    }; // end voice class -----------------------------


    // synth public API -------------------------------
    void setup(  std::string path  );
    
    void oscMapping( pdsp::osc::Input & osc, np::tuning::ModalTable * table );

    vector<Voice>       voices;
    
    ofParameterGroup    parameters;
    ofParameterGroup & label (std::string name );

    pdsp::WaveTable  wt;

    pdsp::Patchable& ch( size_t index );
    
    pdsp::ParameterGain gain;

private: // --------------------------------------------------
    
    pdsp::LowCut lowcut;

    pdsp::Parameter     drift;
    pdsp::Parameter     filterBase;
    pdsp::Parameter     lowcutControl;
    
    // stereo clipper
    pdsp::SoftClip clip0;
    pdsp::IIRUpSampler2x upsampler0;
    pdsp::IIRDownSampler2x downsampler0; 
    pdsp::SoftClip clip1;
    pdsp::IIRUpSampler2x upsampler1;
    pdsp::IIRDownSampler2x downsampler1; 

    void loadWaves( std::string path );
};

}} // end namspaces 
