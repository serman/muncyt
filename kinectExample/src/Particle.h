//
//  Particle.h
//  PixelForces
//
//  Created by Ben McChesney on 7/2/11.
//  Copyright 2011 Helios Interactive. All rights reserved.
//
//  A Simple Particle Class

#ifndef _PARTICLE
#define _PARTICLE

#include "ofMain.h"

class Particle
{
    public :
    Particle() ;
    
    Particle( ofVec3f _position , ofColor _color )
    {
        position = _position ;
        color = _color ;
        velocity = ofVec3f ( ofRandom ( -2 , 2 ) , ofRandom ( -2 , 2 ) , ofRandom ( -2 , -2 ) ) ;
        spawnPoint = _position ;
        recentlyUsed=0;
    };
    
    Particle( ofVec3f _position , ofColor _color, int mx, int my )
    {
        position = _position ;
        color = _color ;
        velocity = ofVec3f ( ofRandom ( -2 , 2 ) , ofRandom ( -2 , 2 ) , ofRandom ( -2 , -2 ) ) ;
        spawnPoint = _position ;
        _x=mx;
        _y=my;
        recentlyUsed=0;
    };
    
    
    ofVec3f position , velocity ;
    ofVec3f acceleration ;          //smoothing applied to velocity
    ofVec3f spawnPoint ;            //original location to line up the picture
    ofColor color ;
    int _x;
    int _y;
    char recentlyUsed;
    
    void addForce(float a_x, float a_y, float a_z) {
        acceleration.x += a_x;
        acceleration.y += a_y;
        acceleration.z += a_z;
    };
    
    void addForce(ofVec3f a_loc) {
        acceleration += a_loc;
    }
    
    void stopAcc() {
        acceleration.x = 0;
        acceleration.y = 0;
        acceleration.z = 0;
    };
    
    void setForce(ofVec3f a_loc){
        stopAcc();
        addForce(a_loc);
    }
    
    void update(){
    	velocity += acceleration;
        position += velocity;
    }
    
    
    
    
    
};
#endif