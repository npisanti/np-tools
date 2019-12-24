
// DA4Joystick.cpp
// Nicola Pisanti, MIT License, 2016

#include "DA4Joystick.h"

namespace np {

DA4Joystick::DA4Joystick() {
    openPath("/dev/input/js0");
    sleepValue = 1000;
    
    counter = 0;
}

DA4Joystick::DA4Joystick(int joystickNumber)
{
    std::stringstream sstm;
    sstm << "/dev/input/js" << joystickNumber;
    openPath(sstm.str());
}

DA4Joystick::DA4Joystick(std::string devicePath)
{
    openPath(devicePath);
}

void DA4Joystick::setup( std::string devicePath, int sleep ) 
{
    sleepValue = sleep;
    openPath(devicePath);
    if( isFound() ) {
        startThread(true);
        ofLog(OF_LOG_NOTICE, "DA4Joystick : starting joystick thread" );
    }else{
        ofLog(OF_LOG_NOTICE, "DA4Joystick : joystick not found" );
    }
}

void DA4Joystick::setup( DA4Listener* app, std::string devicePath, int sleep) 
{
    setup( devicePath, sleep );
    addListener(app);
}

void DA4Joystick::addListener( DA4Listener* app ) {
    listeners.push_back( app );
}

void DA4Joystick::exit()
{
    if( isFound() ){
        ofLog(OF_LOG_NOTICE, "DA4Joystick : stopping joystick thread" );
        stopThread();
        ofLog(OF_LOG_NOTICE, "DA4Joystick : joystick thread closed" );
    }
}


void DA4Joystick::threadedFunction() 
{
   
    while(isThreadRunning()) {
   
        DA4Event event;
        
        while ( sample(&event) ) 
        {
            if (event.isButton()) {
                for(size_t i=0; i<listeners.size(); ++i) {
                    listeners[i]->buttonPressed( event.number, event.value );
                }
            }
            else if (event.isAxis()) {
            
                switch( event.number ){
                    
                    case 4: 
                        switch( event.value){
                            case 0:
                                for(size_t i=0; i<listeners.size(); ++i) {
                                    listeners[i]->buttonPressed( OF_DPAD_HORIZ, 0 );
                                }
                            break;
                            
                            case -32767:
                                for(size_t i=0; i<listeners.size(); ++i) {
                                    listeners[i]->buttonPressed( OF_DPAD_HORIZ, -1 );
                                }
                            break;
                            
                            case 32767:
                                for(size_t i=0; i<listeners.size(); ++i) {
                                    listeners[i]->buttonPressed( OF_DPAD_HORIZ, 1 );
                                }
                            break;
                        }
                    break;
                    
                    case 5:
                        switch( event.value){
                            case 0:
                                for(size_t i=0; i<listeners.size(); ++i) {
                                    listeners[i]->buttonPressed( OF_DPAD_VERT, 0 );
                                }
                            break;
                            
                            case -32767:
                                for(size_t i=0; i<listeners.size(); ++i) {
                                    listeners[i]->buttonPressed( OF_DPAD_VERT, -1 );
                                }
                            break;
                            
                            case 32767:
                                for(size_t i=0; i<listeners.size(); ++i) {
                                    listeners[i]->buttonPressed( OF_DPAD_VERT, 1 );
                                }
                            break;
                        }
                    break;
                    
                    default: 
                        for(size_t i=0; i<listeners.size(); ++i) {
                            listeners[i]->axisMoved( event.number, event.value );
                        }
                    break;
                }
            }
        } 

        usleep( sleepValue );
    }
}

void DA4Joystick::openPath(std::string devicePath)
{
    _fd = open(devicePath.c_str(), O_RDONLY | O_NONBLOCK);
}

bool DA4Joystick::sample(DA4Event* event)
{
    int bytes = read(_fd, event, sizeof(*event)); 

    if (bytes == -1) return false;
    // NOTE if this condition is not met, we're probably out of sync and this
    // Joystick instance is likely unusable
    return bytes == sizeof(*event);
}

bool DA4Joystick::isFound()
{
    return _fd >= 0;
}

DA4Joystick::~DA4Joystick()
{
    close(_fd);
};


} // end namespace np

