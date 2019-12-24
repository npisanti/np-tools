
// class for using a Thrustmaster Dual Analog 4 gamepad as input for ofxPDSP, based on kashimAstro's ofxJoystick
// this joystick has a problem: the axis sometimes get stuck. It is a linux input problem, also replicated with jstest

#pragma once

#include "ofMain.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <string>
#include <sstream>
#include "unistd.h"

#define JS_EVENT_BUTTON 0x01 // button pressed/released
#define JS_EVENT_AXIS   0x02 // joystick moved
#define JS_EVENT_INIT   0x80 // initial state of device
 
#define OF_DPAD_HORIZ 12 
#define OF_DPAD_VERT 13 

namespace np {

class DA4Event {
    
	public:
  	static const short MIN_AXES_VALUE = -32767;
	static const short MAX_AXES_VALUE = 32767;
  
  	unsigned int    time;
  	short           value;
  	unsigned char   type;
    unsigned char   number;

	bool isButton() {
	    return (type & JS_EVENT_BUTTON) != 0;
	}

	bool isAxis() {
	    return (type & JS_EVENT_AXIS) != 0;
	}

	bool isInitialState() {
	    return (type & JS_EVENT_INIT) != 0;
	}
};

    // here starts all the API you need ---------------------------------------------------------------------------------


class DA4Listener // your ofApp has to extend this class
{
public: 
    // add this method to ofApp to manage buttons (othewise it will just print out messages on debug )
    virtual void buttonPressed( int button, int value ) 
    { 
        #ifndef NDEBUG
        cout <<"button="<<button<<" value="<<value<<"\n"; 
        #endif
    } 

    virtual void axisMoved( int axis, int value ) 
    { 
        #ifndef NDEBUG
        cout <<"axis="<<axis<<" value="<<value<<"\n"; 
        #endif
    } 
};


class DA4Joystick : public ofThread {
public:
    
    // this is for the setup
	void setup(DA4Listener* app, std::string devicePath="/dev/input/js0", int sleep=1000); 
	
    void setup( std::string devicePath="/dev/input/js0", int sleep=1000); 
    
    void addListener( DA4Listener* app );
    
    // this has to be called on ofApp::exit()
	void exit();
    
	~DA4Joystick();
	DA4Joystick();
	DA4Joystick(int joystickNumber);
	DA4Joystick(std::string devicePath);
        

  	static const short axisMin = -32767;
	static const short axisMax = 32767;

private:

	bool isFound();
	void openPath(std::string devicePath);
	int _fd;

	bool sample(DA4Event* event);
	void threadedFunction();    

    vector<DA4Listener*> listeners;
    int sleepValue;

    int counter;
};


}
