
#include "Library.h"

substrata::Correlation::Correlation(){
    sampleMin = 0;
    sampleMax = 0;
    now = 0;
}

float substrata::Library::correlate( int instrument, int subfolder ){
    if( instrument < 0 ) instrument = 0;
    if( instrument > 35 ) instrument = 35;
    if( subfolder < 0 ) subfolder = 0;
    if( subfolder > 35 ) subfolder = 35;    
    auto & corr = correlations[instrument][subfolder];
    int r = corr.range;
    int x = corr.now;
    
    switch ( r ){
        case 1: break;
        
        case 2:
            if( x == corr.sampleMin ){
                x++;
            }else{
                x = corr.sampleMin;
            }
            corr.now = x;
        break;
        
        default :
            while( x == corr.now ){
                x = pdsp::dice(r) + corr.sampleMin;
            }
            corr.now = x;
        break;
    }

    return float(x);
}
    
void substrata::Library::init( std::string path ){
    
    std::cout<< "[Library] parsing "<<path<<"\n";
    std::cout<< "adding files";
    
    correlations.resize( 36 );
    for( auto & v : correlations ){
        v.resize(36);
    }
    availables.resize(36);
    for( auto & val : availables ){
        val = 0;
    }
    
    ofDirectory dir;

	dir.listDir( path );
	//dir.allowExt("jpg");
	dir.sort(); 

    //std::cout << "READING DIR -------------------\n";
	for(int i = 0; i < (int)dir.size(); i++){

        std::string name = ofFilePath::getFileName( dir.getPath(i));
        //std::cout << name << "\n";
        
        for( int n=0; n<36; ++n ){
            if( orcanums[n]==name && name!="0" ){
                addInstrumentFolder( dir.getPath(i), n );
            }
        }
	}
    
    std::cout<<"\n";
    std::cout<<"[ substrata ] loaded "<<samples.size()<<" samples\n";
}

void substrata::Library::addInstrumentFolder( std::string path, int index ){
    //std::cout << "--- adding instrument folder  \n";
    
    ofDirectory dir;

	dir.listDir( path );
	dir.sort(); 

    std::string name = ofFilePath::getFileName( dir.getPath(0));
    if( name == "0" ){
        
        std::string innerpath = dir.getPath(0);
        correlateFolder( innerpath, index, 0 );
        
        for(int i = 1; i < (int)dir.size(); i++){
           
            innerpath = dir.getPath(i);
            name = ofFilePath::getFileName( dir.getPath(i));
            //std::cout << name << "\n";
            //std::cout << dir.getPath(i) << "\n";
            
            for( int i=1; i<36; ++i ){
                if( name == orcanums[i] ){
                    correlateFolder( innerpath, index, i );
                }
            }
        }
    }else{
        std::cout<<"[ substrata ] subfolder 0 is mandatory, error for directory "<<path<<"\n";
    }


}

void substrata::Library::correlateFolder( std::string path, int index, int subfolder ){
    //std::cout << "----- correlating folder  \n";
    float amp = 1.0f;
    ofDirectory dbfile;
	dbfile.allowExt("dB");
	dbfile.listDir( path );
    if( dbfile.size()>0 ){
         std::string name = ofFilePath::getFileName( dbfile.getPath(0) );
         name = ofFilePath::removeExt( name );
         std::cout << "\nx";
         amp = dB( ofToInt( name ) );
    }else{
        std::cout << "\n";
    }

    //std::cout << "----- adding wavs\n";
    ofDirectory wavs;
	wavs.allowExt("wav");
	wavs.listDir( path );
	wavs.sort(); 
    int smin = samples.size();
	for( size_t i = 0; i < wavs.size(); i++){
        pdsp::SampleBuffer * sample = new pdsp::SampleBuffer();
        sample->load( wavs.getPath(i) );
        samples.push_back( sample );
        
        if( amp!= 1.0f ){
            for (int c=0; c<sample->channels; ++c ){
                for (int n=0; n< sample->length; ++n ){
                    sample->buffer[c][n] *= amp;
                }
            }
        }
        //std::cout<< "loaded wav file: "<<wavs.getPath(i)<<"\n";
        //std::cout<< "sample len is "<< samples.back()->length <<"\n";
        std::cout<<"."<< std::flush;
    }
    int smax = samples.size();

    int range = smax - smin;

    correlations[index][subfolder].now = smin;
    correlations[index][subfolder].sampleMin = smin;
    correlations[index][subfolder].sampleMax = smax;
    correlations[index][subfolder].range = range;
    
    if( subfolder == 0 ){
        // update all thd unassigned correlation to point to 0 
        for( size_t i=0; i<correlations[index].size(); ++ i ){
            correlations[index][i].now = smin;
            correlations[index][i].sampleMin = smin;
            correlations[index][i].sampleMax = smax;
            correlations[index][i].range = range;
        }        
    }
    
    availables[index] = 1;
}

void substrata::Library::free(){
    for( auto & pointer : samples ){
        if ( pointer != nullptr ){
            delete pointer;
        }
    }
}
