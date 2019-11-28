
#pragma once

#include "ofMain.h"
#include "ofxPDSP.h"
#include <atomic>

namespace substrata {
    
class Correlation{
public:
    Correlation();
    int sampleMin;
    int sampleMax;
    int now;
    int range;
};

static std::string const orcanums[] = {
    "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "a", "b", //  0 - 11
    "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", // 12 - 23
    "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z", // 24 - 35
};

class Library{
public:
    
    void init( std::string path );

    float correlate( int instrument, int subfolder );
    
    std::vector<std::vector<Correlation>> correlations;
    
    std::vector<pdsp::SampleBuffer*> samples;

    std::vector<int> availables;
    
    void free();
    
private:
    void addInstrumentFolder( std::string path, int index );
    void correlateFolder( std::string path, int index, int dyn );
    
};
    
}
