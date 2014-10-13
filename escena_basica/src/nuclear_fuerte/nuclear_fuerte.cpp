//
//  nuclear_fuerte.cpp
//  escena_basica
//
//  Created by Sergio Galan on 7/11/14.
//
//

#include "nuclear_fuerte.h"


void nuclear_fuerte::setup(){

	colorp1=0; //red
    colorp2=1; //blue
    colorp3=2;//green
	
    
}

void nuclear_fuerte::update(float d1){
    
}

void nuclear_fuerte::draw(){
    
}

void nuclear_fuerte::mousePressed( int x, int y, int button ){}

void nuclear_fuerte::tuioAdded(ofxTuioCursor &tuioCursor){
	ofPoint loc = ofPoint(tuioCursor.getX()*W_WIDTH,tuioCursor.getY()*W_HEIGHT);

    int tipo=(int)ofRandom(0,4);
    exchangeColors(tipo);
    if(tipo==1){
        cheapComm::getInstance()->sendSync4("/sync/strong_nuclear/hand_on_event",1.0,tipo,colorp1,(float)colorp2);
        cheapComm::getInstance()->sendSync2("/sync/strong_nuclear/setColor",1.0,colorp1);
        cheapComm::getInstance()->sendSync2("/sync/strong_nuclear/setColor",2.0,colorp2);
    }
    else if(tipo==2){
        cheapComm::getInstance()->sendSync4("/sync/strong_nuclear/hand_on_event",1.0,tipo,colorp2,(float)colorp3);
        cheapComm::getInstance()->sendSync2("/sync/strong_nuclear/setColor",2.0,colorp2);
        cheapComm::getInstance()->sendSync2("/sync/strong_nuclear/setColor",3.0,colorp3);
    }
    else if(tipo==3){
        cheapComm::getInstance()->sendSync4("/sync/strong_nuclear/hand_on_event",1.0,tipo,(float)colorp3,(float)colorp1);
        cheapComm::getInstance()->sendSync2("/sync/strong_nuclear/setColor",3.0,colorp3);
        cheapComm::getInstance()->sendSync2("/sync/strong_nuclear/setColor",1.0,colorp1);
    }    
}
void nuclear_fuerte::tuioRemoved(ofxTuioCursor &tuioCursor){
}

void nuclear_fuerte::tuioUpdated(ofxTuioCursor &tuioCursor){
}

void nuclear_fuerte::exchangeColors(int tipo){
    int ctmp;
    if(tipo==1){
        ctmp=colorp1;
        colorp1=colorp2;
        colorp2=ctmp;
    }
    else if (tipo==2){
        ctmp=colorp2;
        colorp2=colorp3;
        colorp3=ctmp;
    }
    else if (tipo==3){
        ctmp=colorp3;
        colorp3=colorp1;
        colorp1=ctmp;
    }


}



