//
//  joy2key.h
//  mainScreen
//
//  Created by Sergio Galan on 9/9/14.
//
//

#ifndef mainScreen_joy2key_h
#define mainScreen_joy2key_h
#include "ofxOsc.h"
#include "ofMain.h"
class joy2key{
    public:
    ofxOscReceiver myosc;
    int previousval0;
    int previousval1;
    
    void setup(){
        int serverRecvPort = 12000;
        myosc.setup(serverRecvPort);
        previousval0=0;
        previousval1=0;
    }
    
    int update(){
        int positionReponse=0;
        int value;
        while(myosc.hasWaitingMessages()){
            // get the next message
            ofxOscMessage m;
            myosc.getNextMessage(&m);
            // check for mouse moved message
            
            if(m.getAddress() == "/joystick/0/axis/0"){
                value = (int)m.getArgAsFloat(0);
                if(value==1){
                    ofKeyEventArgs arg;
                    arg.key='d';
                    ofNotifyEvent(ofEvents().keyPressed,arg);
                   
                }
                else if(value==0){
                    
                    if(previousval0>0){
                        ofKeyEventArgs arg;
                        arg.key='d';
                        ofNotifyEvent(ofEvents().keyReleased,arg);
                        arg.key='a';
                        ofNotifyEvent(ofEvents().keyReleased,arg);
                        ofLog() << m.getAddress() << "  "<<ofToString( m.getArgAsFloat(0));
                    }
                }
                if(value==-1){
                    ofKeyEventArgs arg;
                    arg.key='a';
                    ofNotifyEvent(ofEvents().keyPressed,arg);
                }
                if(value!=0 && previousval0<2) previousval0++;
                if(value==0 && previousval0>0) previousval0--;
            }
            else if(m.getAddress() == "/joystick/0/axis/1"){
                value = (int) m.getArgAsFloat(0);
                if(value==1){
                    ofKeyEventArgs arg;
                    arg.key='s';
                    ofNotifyEvent(ofEvents().keyPressed,arg);
                }
                else if(value==0){
                    if(previousval1>0){
                    ofKeyEventArgs arg;
                    arg.key='s';
                    ofNotifyEvent(ofEvents().keyReleased,arg);
                    arg.key='w';
                    ofNotifyEvent(ofEvents().keyReleased,arg);
                    ofLog() << m.getAddress() << "  "<<ofToString( m.getArgAsFloat(0));
                    }
                }
                if(value==-1){
                    ofKeyEventArgs arg;
                    arg.key='w';
                    ofNotifyEvent(ofEvents().keyPressed,arg);
                }
                if(value!=0 && previousval1<2) previousval1++;
                if(value==0 && previousval1>0) previousval1--;
            }
            else if(m.getAddress() == "/joystick/0/JOYBUTTONDOWN"){
                 value = (int) m.getArgAsFloat(0);
                if(value==0){
                    ofKeyEventArgs arg;
                    arg.key='e';
                    ofNotifyEvent(ofEvents().keyPressed,arg);
                }
                else if(value==1){
                    ofKeyEventArgs arg;
                    arg.key='r';
                    ofNotifyEvent(ofEvents().keyPressed,arg);
                }
                else if(value==2){
                    ofKeyEventArgs arg;
                    arg.key='t';
                    ofNotifyEvent(ofEvents().keyPressed,arg);
                }
                else if(value==3){
                    ofKeyEventArgs arg;
                    arg.key='y';
                    ofNotifyEvent(ofEvents().keyPressed,arg);
                }
            }
            else if(m.getAddress() == "/joystick/0/JOYBUTTONUP"){
                value = (int) m.getArgAsFloat(0);
                if(value==0){
                    ofKeyEventArgs arg;
                    arg.key='e';
                    ofNotifyEvent(ofEvents().keyReleased,arg);
                }
                else if(value==1){
                    ofKeyEventArgs arg;
                    arg.key='r';
                    ofNotifyEvent(ofEvents().keyReleased,arg);
                }
                else if(value==2){
                    ofKeyEventArgs arg;
                    arg.key='t';
                    ofNotifyEvent(ofEvents().keyReleased,arg);
                }
                else if(value==3){
                    ofKeyEventArgs arg;
                    arg.key='y';
                    ofNotifyEvent(ofEvents().keyReleased,arg);
                }
            }
        }
        return positionReponse;
    }
    

    private:
    
};


#endif
