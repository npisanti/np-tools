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

#include "Library.h"

substrata::Correlation::Correlation(){
    sampleMin = 0;
    sampleMax = 0;
    now = 0;
}

void substrata::Library::correlate( int instrument, float & select ){
    if( instrument < 0 ) instrument = 0;
    if( instrument > 35 ) instrument = 35;  
    auto & corr = correlations[instrument];
    int r = corr.range;
    int x = corr.now;
    if( r < 3 ){ // rule : at least 3 samples for round robin 
        select = x;
    }else{
        while( x == corr.now ){
            x = pdsp::dice(r) + corr.sampleMin;
        }
        corr.now = x;
        select = x;
    }
}
    
void substrata::Library::init( std::string path ){
    
    std::cout<< "[Library] parsing "<<path<<"\n";
    std::cout<< "adding files";
    
    correlations.resize( 36 );
    availables.resize( correlations.size() );
    for( auto & val : availables ){
        val = 0;
    }
        
    ofDirectory dir;

	dir.listDir( path );
	//dir.allowExt("jpg");
	dir.sort(); 

    //std::cout << "READING DIR -------------------\n";
	for(int i = 0; i < (int)dir.size(); i++){
        //std::cout << dir.getPath(i) << "\n";
        std::string name = ofFilePath::getFileName( dir.getPath(i));
        //std::cout << name << "\n";
        
        for( int n=0; n<36; ++n ){
            if( orcanums[n]==name && name!="0" ){                
                correlateFolder( dir.getPath(i), n);
            }
        }
	}
    
    std::cout<<"\n";
}


void substrata::Library::correlateFolder( std::string path, int index ){

    float amp = 1.0f;
    ofDirectory dbfile;
	dbfile.allowExt("dB");
	dbfile.listDir( path );
    if( dbfile.size()>0 ){
         std::string name = ofFilePath::getFileName( dbfile.getPath(0) );
         name = ofFilePath::removeExt( name );
         std::cout << "\n found dB file, gain = "<<name<<" dB\n";
         amp = dB( ofToInt( name ) );
    }

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
        std::cout<<"."<< std::flush;
    }
    int smax = samples.size();

    int range = smax - smin;

    correlations[index].now = smin;
    correlations[index].sampleMin = smin;
    correlations[index].sampleMax = smax;
    correlations[index].range = range;
    
    availables[index] = 1;
}

void substrata::Library::free(){
    for( auto & pointer : samples ){
        if ( pointer != nullptr ){
            delete pointer;
        }
    }
}
