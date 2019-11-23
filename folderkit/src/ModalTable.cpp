
// ModalTable.cpp
// Nicola Pisanti, GPLv3 License, 2016

#include "ModalTable.h"

#define MAX_DEGREES 12

ofParameterGroup & np::tuning::ModalTable::setup( std::string name ) {
    
    ratios.resize( MAX_DEGREES );
    pitches.resize( MAX_DEGREES );
    
    parameters.setName( name );
    parameters.add( masterPitchControl.set( "master_pitch",  29,   0,    60  ) );
    parameters.add( degrees.set( "used_degrees",  12,   3,    12  ) );

    int index = 0;

    for( RatioUI & r : ratios ){
        r.setBasePitch( masterPitchControl );  
        std::string label = "d";
        label += ofToString(index);
        label += " numerator";
        parameters.add( r.numerator.set( label, MAX_DEGREES + index, 1, 63 ) );
        label = "d";
        label += ofToString(index);
        label += " denominator";
        parameters.add( r.denominator.set( label, MAX_DEGREES, 1, 32 ) );
        
        r.numerator.addListener( this, &ModalTable::updateAll );
        r.denominator.addListener( this, &ModalTable::updateAll);

        index++;
    }
    
    masterPitchControl.addListener( this, &ModalTable::updateAll);
    
    deg = degrees;
    base = masterPitchControl;
    
    tonalControl.set( masterPitchControl );
    
    int dummy = 0;
    updateAll( dummy );
    
    return parameters;
}

ofParameterGroup & np::tuning::ModalTable::label( std::string name ){
    parameters.setName(name);
    return parameters;
}

void np::tuning::ModalTable::updateAll( int & value ) {
    int dummy = 0;
    for( RatioUI & r : ratios ){
        r.ratioChange( dummy );
    } 
    for( size_t i=0; i<pitches.size(); ++i){
        pitches[i] = ratios[i].pitch;
    }
    base = masterPitchControl;
    deg = degrees;
    
    tonalControl.set( masterPitchControl );
}
