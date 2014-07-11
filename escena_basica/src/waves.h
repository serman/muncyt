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
class waves{
public:
    vector<ofVec3f> points;
    float freq;
    float freq2;
    ofPolyline curvas;

    void update(){
        curvas.clear();
        points.clear();
        for (int i=0; i < W_WIDTH; i+=1){
            ofVec3f vector=ofVec3f(i,200+100*sin( (ofGetFrameNum()*freq +2*PI*i*1 /freq2) /( W_WIDTH) ),3);
            points.push_back(vector);
            curvas.addVertex(vector);
        }
        
    }
    
    void setup(){
        freq=30;
        freq2=0.5;
        for (int i=0; i < W_WIDTH; i++){
            ofVec3f vector=ofVec3f(i,200+100*sin((ofGetFrameNum()*50 +2*PI)* (i/768.0)),3);
                        points.push_back(vector);
            curvas.addVertex(vector);
        }

    }
    
    void draw(){
        ofPushMatrix();
        
        curvas.draw();
        ofPopMatrix();
    }
    
    
    
};