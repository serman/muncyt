//
//  joy2key.h
//  mainScreen
//
//  Created by Sergio Galan on 9/9/14.
//
//

#ifndef mainScreen_joy2keyLocal_h
#define mainScreen_joy2keyLocal_h
#include "ofMain.h"
#include <GLFW/glfw3.h>
class joy2key{
public:
    int previousval0;
    int previousval1;
    
    int previousValueButton;
    int previousValueButton1,previousValueButton2,previousValueButton3;
    bool joystickPresent=false;
    long timeLastTouch; //tiempo desde el ultimo toque en segundos
    void setup(){
       // int serverRecvPort = 12000;
       // myosc.setup(serverRecvPort);
        previousval0=0;
        previousval1=0;
        previousValueButton=0;
        previousValueButton1=0;
        previousValueButton2=0;
        previousValueButton3=0;
        if(glfwJoystickPresent(0)==false){
            joystickPresent=false;
            cout << "NOT PRESENT";
        }
        else{ joystickPresent=true;
            cout << "PRESENT";
        }
        timeLastTouch=0;
        
    }
    
    
    int update(){
        if(joystickPresent==false) return;
        int c;
        const  float* ejes=glfwGetJoystickAxes(0, &c);
        //cout <<ejes[0]<<endl;
        //cout <<ejes[1]<<endl;
        const unsigned char* botones=glfwGetJoystickButtons(0, &c);
        
        int positionReponse=0;
        int value;
//eje 0
                value = (int)ejes[0];
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

                    }
                }
                if(value==-1){
                    ofKeyEventArgs arg;
                    arg.key='a';
                    ofNotifyEvent(ofEvents().keyPressed,arg);
                }
                if(value!=0 && previousval0<1) previousval0++;
                if(value==0 && previousval0>0) previousval0--;

        //eje 1

                value = (int)ejes[1];
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
                        timeLastTouch=ofGetElapsedTimef();
                    }
                }
                if(value==-1){
                    ofKeyEventArgs arg;
                    arg.key='w';
                    ofNotifyEvent(ofEvents().keyPressed,arg);
                }
                if(value!=0 && previousval1<1) previousval1++;
                if(value==0 && previousval1>0) previousval1--;
        

/******* botones **/
                value = (int)botones[0];
                if(value==0 && previousValueButton==1){
                    ofKeyEventArgs arg;
                    arg.key='e';
                    ofNotifyEvent(ofEvents().keyReleased,arg);
                    previousValueButton=0;
                }
                else if(value==1 && previousValueButton==0){
                    ofKeyEventArgs arg;
                    arg.key='e';
                    ofNotifyEvent(ofEvents().keyPressed,arg);
                    previousValueButton=1;
                }
        
                value = (int)botones[1];
                if(value==0 && previousValueButton1==1){
                    ofKeyEventArgs arg;
                    arg.key='r';
                    ofNotifyEvent(ofEvents().keyReleased,arg);
                    previousValueButton1=0;
                }
                else if(value==1 && previousValueButton1==0){
                    ofKeyEventArgs arg;
                    arg.key='r';
                    ofNotifyEvent(ofEvents().keyPressed,arg);
                    previousValueButton1=1;
                }
        value = (int)botones[2];

        if(value==0 && previousValueButton2==1){
            ofKeyEventArgs arg;
            arg.key='t';
            ofNotifyEvent(ofEvents().keyReleased,arg);
            previousValueButton2=0;
        }
        else if(value==1 && previousValueButton2==0){
            ofKeyEventArgs arg;
            arg.key='t';
            ofNotifyEvent(ofEvents().keyPressed,arg);
            previousValueButton2=1;
        }
        
        value = (int)botones[3];
        if(value==0 && previousValueButton3==1){
            ofKeyEventArgs arg;
            arg.key='y';
            ofNotifyEvent(ofEvents().keyReleased,arg);
            previousValueButton3=0;
        }
        else if(value==1 && previousValueButton3==0){
            ofKeyEventArgs arg;
            arg.key='y';
            ofNotifyEvent(ofEvents().keyPressed,arg);
            previousValueButton3=1;
        }
        
        
                        //}
        return positionReponse;
    }
    
    
private:
    
};


#endif
