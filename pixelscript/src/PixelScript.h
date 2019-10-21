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

#pragma once

#include "ofMain.h"

#include "ofxLua/src/ofxLua.h"
#include "FileWatcher.h"
#include "bindings/png.h"
#include "Buffer.h"
#include "Shader.h"
#include "ofxOsc.h"
#include "osc.h"

namespace np{ 
    
class PixelScript : public FileWatcher, public ofxLuaListener {
    
public:
    PixelScript();
    ~PixelScript();

    void update();
    
    void draw( int x, int y );
    void draw( int x, int y, int w, int h );
    
    void setTime( float value );
    
    void variable( std::string name, double value ){ lua.setNumber( name, (lua_Number) value ); }
   
    void variable( std::string name, bool value ){ lua.setBool( name, value ); }

    ofxLua lua;    

    ofParameter<float> speed;
    
    int getWidth() const { return buffer.getWidth(); };
    int getHeight() const { return buffer.getHeight(); };
    
    void oscReceived(const ofxOscMessage& message);
    
    void headless( bool active, int usecs=100 );

private:
    
    ofTrueTypeFont font;
    
    std::vector<osc::TaggedSender> senders;
    ofxOscReceiver receiver;

    std::vector<png::Pair> images;
    std::vector<np::pixelscript::Shader> shaders;
    
    np::pixelscript::Buffer buffer;

    bool bHeadless;
    bool loaded;
    int usec;

    void errorReceived(std::string& msg);
    
    float clock;
    float before; 
    
    void reload() override;
    
};
    
}
