
#include "IRVerb.h"

bool folderkit::IRVerb::load( std::string path ){
    
    ofDirectory dir;
	dir.listDir( path );
	dir.allowExt("wav");
	dir.sort(); 
    
    if( dir.size()==0 ){
        return false;
    }
    
    100.0f >> cut0.in_freq();
    100.0f >> cut1.in_freq();
    
    cut0 >> rev0;
    cut1 >> rev1;
    
    impulse0.load( dir.getPath(0) );
    
    if( impulse0.channels > 1 ){
        rev0.loadIR( impulse0, 0 );
        rev1.loadIR( impulse1, 1 );   
    }else{
        if( dir.size()>1 ){
            impulse1.load( dir.getPath(1) );
            rev0.loadIR( impulse0, 0 );
            rev1.loadIR( impulse1, 0 );   
        }else{
            rev0.loadIR( impulse0, 0 );
            rev1.loadIR( impulse0, 0 );   
        }
    }
    std::cout<< "[ folderkit ] found impulse responses in /IR folder\n";
    return true;
}

pdsp::Patchable & folderkit::IRVerb::ch( int i ){
    switch( i ){
        default:
        case 0: return rev0; break;
        case 1: return rev1; break;
    }
}
