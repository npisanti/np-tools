/*====================================================================

	wolframkit - cellular automata playground
  
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

#include "RuleSeq.h"
#include <boost/filesystem.hpp>
#include "ofJson.h"
#include "ofAppRunner.h"

np::RuleSeq::Entry::Entry(){
    rule = 60;
    density = 0.5;
    for( int i=0; i<NUMSYNTHS; ++i ) thresholds[i] = 0;
}

np::RuleSeq::Buffer::Buffer(){
    tempo = 120;
    division = 16;
    num_bars = 8;
    first = 0;
    iEntry = 0;
}

np::RuleSeq::RuleSeq(){
    bChanged = false;
    iBuffer = 1;
}

bool np::RuleSeq::setup( std::string path ){

    this->path = path;
    if( ! ofFilePath::isAbsolute( path )){
        path = ofFilePath::getCurrentWorkingDirectory() + "/" + path;
    }
    this->name = ofFilePath::getFileName( path );
    
    ofFile file( path );
    if( file.exists() ){
        ofAddListener(ofEvents().update, this, &np::RuleSeq::checkFile);
        writeTimestamp = boost::filesystem::last_write_time( path );    
        reload();
        return true;
    }else{
        ofLogError() << "[np::RuleSeq] file does not exists\n"; 
        return false;   
    }
}

void np::RuleSeq::checkFile(ofEventArgs &args){
    timePassed += ofGetLastFrameTime();
    if( timePassed > interval ){
        time_t checkTimestamp = boost::filesystem::last_write_time( path );
        if( checkTimestamp != writeTimestamp){
            writeTimestamp = checkTimestamp;
            reload();
            ofLogNotice() << "["<<name<<"] file reloaded";
        }
    }
}

void np::RuleSeq::reload(){
    ofJson json = ofLoadJson( path );
    int b = (iBuffer==0) ? 1 : 0;  

    try {
        ofJson general = json["general"];
        
        buffers[b].tempo = general["tempo"];
        buffers[b].division = general["division"];
        if( buffers[b].division<= 0 ){
            std::cout<<"[ "<<name<<" ] division must be greater than 0, setting do default (16)\n";
            buffers[b].division = 16;
        }
        buffers[b].num_bars = general["num_bars"];
        if( buffers[b].num_bars<= 0 ){
            std::cout<<"[ "<<name<<" ] num_bars must be greater than 0, setting do default (8)\n";
            buffers[b].num_bars = 8;
        }
        buffers[b].first = general["first_seq"];
        
//        std::cout<< "settings loaded: tempo="<<buffers[b].tempo<<" | division="<<buffers[b].division<<" | num_bars="<<buffers[b].num_bars<<" | first_seq="<<buffers[b].first<<"\n";

        ofJson seqs = json["sequences"];
        buffers[b].entries.resize( seqs.size() );
        auto & e = buffers[b].entries;
        
        for( size_t i=0; i<e.size(); ++i ){
            auto & seq = seqs[i];
            e[i].rule = seq["rule"];
            e[i].density = seq["density"];
//            std::cout<<"entry "<<i<<" density = "<< e[i].density<<"\n";
//            std::cout <<"  thresholds = ";        
            ofJson t = seq["thresholds"];
            for( size_t k=0; k<NUMSYNTHS; ++k  ){
                if( k<t.size() ){
                    e[i].thresholds[k] = t[k];
//                    std::cout<<" "<<t[k];
                }else{
                    e[i].thresholds[k] = 8;
//                    std::cout<<" d8";
                }
            }
//            std::cout<<"\n";
        }
//        std::cout<<"RULESET LOADED!!!!!!!!!!!! entries="<<buffers[b].entries.size()<<"\n";
        bChanged = true;
    }
    catch ( ofJson::exception& e)
    {
         // output exception information
         std::cout << "[ "<<name<<" ] message: " << e.what() << '\n'
                   << "exception id: " << e.id << std::endl;
     }
}

    
bool np::RuleSeq::changed(){
    if(bChanged){
        bChanged = false;
        swapBuffers();
        buffers[iBuffer].iEntry = buffers[iBuffer].first;
        return true;
    }
    return false;
}

void np::RuleSeq::swapBuffers(){
    iBuffer = (iBuffer==0) ? 1 : 0;    
}
        
int np::RuleSeq::division() const{
    return buffers[iBuffer].division;
}

int np::RuleSeq::tempo() const{
    return buffers[iBuffer].tempo;
}

int np::RuleSeq::numBars() const{
    return buffers[iBuffer].num_bars;
}

const np::RuleSeq::Entry & np::RuleSeq::entry() const{
    int i = buffers[iBuffer].iEntry;
    return buffers[iBuffer].entries[i];
}

void np::RuleSeq::next(){
    buffers[iBuffer].iEntry++;
    if (buffers[iBuffer].iEntry >= (int) buffers[iBuffer].entries.size()){
        buffers[iBuffer].iEntry = 0;
    }
}


