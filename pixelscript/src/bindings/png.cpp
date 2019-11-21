/*====================================================================

	pixelscript - little 2d graphics scripting playground
  
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

#include "png.h"

namespace png{
    
    Pair::Pair(){
        index = 0;
        name = "";
        path = "";
    }
    
    std::vector<Pair> * images;
    Pair * current;
    int alignMode;
      
    void init(){
        current = nullptr;
        images = nullptr;
    }
    
    void resources( std::vector<Pair> & reso ){
        images = &reso;
        alignMode = 1;
        auto pback = &images->back();
        current = pback;            
    }
    
    void mode( int value ){
        alignMode = value;
    }
 
    void load( const char * name, const char * path ){

        for(size_t i=0; i<images->size(); ++i ){
            if( images->at(i).name == name ){
                if( images->at(i).path != path ){
                    std::cout<<"[pixelscript] "<<name<<" name already taken by another folder!\n";
                }
                return; // already loaded for that name
            }
        }        
        
        images->back().name = name;
        images->back().path = path;

        if( ofFilePath::getFileExt( ofFilePath::getFileName( std::string( path ) )) == "png" ){
            std::string ap (path);
            if( ! ofFilePath::isAbsolute(ap)  ){
                ap = ofFilePath::getCurrentWorkingDirectory() + "/"+ ap;
            }
            images->back().folder[0].load( ap );
        }else{
            // directory 
            ofDirectory dir;
            dir.openFromCWD( path );
            dir.listDir();
            dir.allowExt("png");
            dir.sort();
            
            if( dir.size() ){
                images->back().folder.resize( dir.size() );
            }

            for(int i = 0; i < (int)dir.size(); i++){
                std::string ap = dir.getPath(i);
                if( ! ofFilePath::isAbsolute(ap)  ){
                    ap = ofFilePath::getCurrentWorkingDirectory() + "/"+ ap;
                }
                images->back().folder[i].load(ap);
            }
        }
        current = &images->back();
        current->index = 0;
        
        images->emplace_back();
        images->back().folder.resize(1);
    }
 
    void select( const char * name ){
        for(size_t i=0; i<images->size(); ++i ){
            if( strcmp(images->at(i).name.c_str(), name) == 0 ){
                current = &(images->at(i));
                return;
            }
        }
        current = &images->back();
        current->index = 0;
        std::cout<<"[pixelscript] "<<name<<" folder not loaded\n";
    }
    
    void frame( int index ){
        current->index = index % int( current->folder.size() );
    }
    
    void pct( double value ){
        if( value >= 1.0 ) value = 0.99999999;
        if( value < 0.0 ) value = 0.0;
        double max = current->folder.size();
        current->index = value * max;
    }
    
    void draw( int x, int y ){
        auto & img = current->folder[current->index];
        switch( alignMode ){
            case 0: 
                img.draw( x, y );                
            break;
            
            case 1:
                img.draw( x - img.getWidth()/2, y - img.getHeight()/2 );
            break;
            
            case 2: default:
                img.draw( x, y - img.getHeight() );
            break;
        }
    }
    
    int next(){
        current->index++;
        int max = current->folder.size();
        if( current->index >= max ) {
            current->index = 0;
        }
        return current->index;
    }
    
    int prev(){
        current->index--;
        if( current->index < 0 ){
            current->index = current->folder.size()-1;
        }
        return current->index;
    }
    
    int step( int step ){
        int index = current->index + step;
        int max = current->folder.size();
        index = (index%max + max )%max;
        current->index = index;
        return current->index;
    }
    
    int random(){
        int max = current->folder.size();
        int index = current->index;
        int r = index;
        while( r == index && max!=1 ){
            r = rand()%max;
        }
        current->index = r;
        return current->index;
    }
    
    int randjump( int stepmax ){
        int s = 1 + rand()%stepmax;
        step( s );
        return current->index;
    }
    
    int width(){
        return current->folder[current->index].getWidth();
    }
    
    int height(){
        return current->folder[current->index].getHeight();
    }

}
