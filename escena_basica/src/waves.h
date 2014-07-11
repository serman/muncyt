//
//  waves.h
//  escena_basica
//
//  Created by Sergio Galan on 7/11/14.
//
//

#ifndef __escena_basica__waves__
#define __escena_basica__waves__

#include <iostream>
#include <ofMain.h>

#include "consts.h"
#endif /* defined(__escena_basica__waves__) */
#define PI 3.14159265
class waves{
public:
    vector<ofVec3f> points;
    int freq=2;
    ofPolyline curvas;

    void update(){
        curvas.clear();
        for (int i=0; i < W_WIDTH; i++){
            ofVec3f vector=ofVec3f(i,100*sin( (ofGetFrameNum()*200.0 +2*PI*i*1) / (768.0) ),3);
            curvas.addVertex(vector);
        }
        
    }
    
    void setup(){
        for (int i=0; i < W_WIDTH; i++){
            ofVec3f vector=ofVec3f(i,100*sin((ofGetFrameNum()*50 +2*PI)* (i/768.0)),3);
            curvas.addVertex(vector);
        }

    }
    
    void draw(){
        ofPushMatrix();
        ofTranslate(0,200);
        curvas.draw();
        ofPopMatrix();
    }
    
    
    
};