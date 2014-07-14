//
//  Header.h
//  escena_basica
//
//  Created by Sergio Galan on 7/10/14.
//
//

#ifndef escena_basica_anilloButton_h
#define escena_basica_anilloButton_h
#define INTERACTIONTIMEOUT
#include <ofMain.h>
#include <math.h>
enum BUTTON_TYPE {TYPE_ACC,TYPE_CRASH};

class anilloButton : public tangibleObject{
    
public:
  
    bool status_handIsOn;
    bool status_active;
    BUTTON_TYPE btype;
    long lastCollision;
    ofEvent<BUTTON_TYPE> buttonEvent;
    //ofNotifyEvent(onVolumeChange, 10.0);
    anilloButton(){
        status_handIsOn=false;
        status_active=true;
        btype= TYPE_ACC;
    }

    
    
    void setup(){}
    
    void draw(){
        ofNoFill();
        if(status_active ==false ){
            ofSetColor(ofColor::gray);
            
            ofRect(x,y,width,height);
        }
        else{ //non active Status
            if(status_handIsOn ==true){
                ofSetColor(ofColor::green);
            ofRect(x,y,width,height);
                
            }else{ //active bug hand is not on
                ofSetColor(ofColor::blue);
            ofRect(x,y,width,height);
            }
            
        }
        //
        
    }
    void update(){}

    void update_prev(ofPoint p){
        
        if(p.distance(ofPoint(x,y)) <200){
            status_active=true;            
        }
        else{
            status_active=false;
        }
        
    }
    
    
    
    
    void touch(float x, float y, int s_id){
        if(inside(x, y)){
            status_handIsOn=true;
        }else
            status_handIsOn=false;
        
    }
    
    void slide(float x, float y, int s_id, int acc){
        if(canInteract()==false) return;
        if(inside(x, y)){
            status_handIsOn=true;
            if(status_active)
                ofNotifyEvent(buttonEvent, btype);
        }else
            status_handIsOn=false;
        
    }
    void touchUp(int s_id){
            status_handIsOn=false;
    }
    
    bool canInteract(){
        if(ofGetElapsedTimeMillis()-lastCollision>400){
            lastCollision=ofGetElapsedTimeMillis();
            return true;
        }
        else return false;
    }
    
};

#endif