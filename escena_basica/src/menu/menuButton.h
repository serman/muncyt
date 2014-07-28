/*
 *  menuButton.h
 *  escena_basica
 *
 *  Created by guillermo casado on 24/07/14.
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */


#ifndef escena_basica_menuButton_h
#define escena_basica_menuButton_h
#define INTERACTIONTIMEOUT
#include <ofMain.h>
#include <math.h>

enum BUTTON_TYPE1 {TYPE_ACC1,TYPE_CRASH1};

class menuButton : public tangibleObject{
    
public:
    bool status_handIsOn;
    bool status_active;
    long lastCollision;
	
	float timeTouch;
	float timePressed;
	float timeIntervActivacion;	// en millis
	bool	pulsado;
	
	ofColor	cF;
	
	bool last_hadOn;
	
	string nombre;
	
    ofEvent<BUTTON_TYPE1> buttonEvent;
	
    menuButton(){
        status_handIsOn=false;
        status_active=true;
        
		timeTouch = 0.0;
		timePressed = 0.0;
		timeIntervActivacion = 2000.0;
        
		pulsado = false;
    }
    
    void setup(){
//        if(x<100 && y>100){//izquierda
//            img.rotate90(2);
//        }
//        else if (y<100 && x<500){ //arriba
//            img.rotate90(3);
//        }
    }
	
    void setup(string _name, ofColor _cF){
		nombre = _name;		
		cF = _cF;
    }
    
	
    void draw(){
        ofPushStyle();
        if(status_active ==false ){
            ofSetColor(cF,220);
            ofRect(x,y,width,height);
        }
        
        else{ //non active Status
            if(status_handIsOn ==true){
                ofSetColor(cF, 100);
                ofFill();
                ofRect(x,y,width,height);
            }else{ 
				//active bug hand is not on
				
                //ofSetColor(cF, 160);
                //ofFill();
				// ofRect(x,y,width,height);
            }
        }
        ofPopStyle();
        //
    }
	
    void update(){}
	
//    void update_prev(ofPoint p){
//        
//        if(p.distance(ofPoint(x,y)) <200){
//            status_active=true;            
//        }
//        else{
//            status_active=false;
//        }
//    }
    
    void touch(float x, float y, int s_id){
        if(inside(x, y)){
            status_handIsOn=true;
        }else {
            status_handIsOn=false; 
		}
    }
    
    void slide(float x, float y, int s_id, int acc){
        if(canInteract()==false) return;
        if(inside(x, y)){
            status_handIsOn=true;
            if(status_active) {
				
				// si  el tiempo que ha estado pulsado es > timeIntervActivacion ==>
				// ==> pulsado = true;
				//	pulsado = true;
			}
        }else{
            status_handIsOn=false;
		}
    }
	
    void touchUp(int s_id){
		status_handIsOn=false;
		pulsado = false;
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
