
#pragma once 

#include "ofxPDSP.h"

namespace folderkit {

class IRVerb{
public:
    bool load( std::string path );

    pdsp::Patchable & ch( int i );

    pdsp::LowCut          cut0;
    pdsp::LowCut          cut1;

    pdsp::FDLConvolver    rev0;
    pdsp::FDLConvolver    rev1;
    
    pdsp::SampleBuffer    impulse0;   
    pdsp::SampleBuffer    impulse1;   
};

}



