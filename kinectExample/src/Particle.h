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

#define MAX_SPEED 30.f
#define MAX_STEER 0.9f
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
    
    void applyForce(float a_x, float a_y, float a_z) {
        acceleration.x += a_x;
        acceleration.y += a_y;
        acceleration.z += a_z;
    };
    
    void applyForce(ofVec3f a_loc) {
        acceleration += a_loc;
    }
    
    void stopAcc() {
        acceleration.x = 0;
        acceleration.y = 0;
        acceleration.z = 0;
    };
    

    
    void update(){
    	velocity += acceleration;
        position += velocity;
        acceleration=acceleration*0;
    }
    
    
//ready made forces
    void steer(ofVec3f& a_target, bool a_slowdown, float a_scale, float a_minDist ) {
        ofVec3f steer;
        ofVec3f desired = a_target - position;
        float d = desired.length();
        
        // If the distance is greater than 0, calc steering (otherwise return zero vector)
        if (d > 0.f) {
            desired.normalize();
            if(a_minDist == 0) a_slowdown = false;
            // Two options for desired vector magnitude (1 -- based on distance, 2 -- maxspeed)
            if ((a_slowdown) && (d < a_minDist)) {desired *= (MAX_SPEED * (d/a_minDist));} // This damping is somewhat arbitrary
            else {desired *= (MAX_SPEED); }
            
            steer = (desired - velocity);
            steer.limit(MAX_STEER);
            steer *= a_scale;
        } else {
            steer.set(0.f, 0.f, 0.f);
        }
        acceleration += steer;
    }
    
    void gravityTowards(ofVec3f& a_target,float a_minDist, float speed){
            ofVec3f diff =  a_target - position;
            int sqdistance =  diff.length();
            if(sqdistance>a_minDist){
              diff.normalize();
            int force=sqdistance/speed;
            diff *= force;
            applyForce( diff );
         }//espejo
        else{
         diff.normalize();
         int force=sqdistance/speed;
         diff = diff*-1;
         diff= diff*force;
         applyForce(diff);
        }
    }
    
    bool sandDown(int acc,int floor){
        ofVec3f dPoint = ofVec3f( floor,position.y,position.z);
        ofVec3f diff =  dPoint - position;
        if(position.x<floor){ //no point can be "below our floor
        	position.x=floor;
            return false;
        }
        
        if(abs(diff.x)>1){
        	diff.normalize();
        	applyForce(diff*acc);
            return true;
        }else{
            velocity*=0;
            acceleration*=0;
        	return false;
        }
    }
    //este no rula añade una fuerza en al direccion de las agujas del reloj
    void rotateAround(ofVec3f& a_target, float speed,float a_minDist){
        ofVec3f dir = position - a_target;
        float d = dir.length();
        if (d < a_minDist && d > 0) {
            float pct = 1 - (d / a_minDist);
            dir.normalize();
            dir *= (MAX_SPEED);
            dir = (dir - velocity);
            dir *= (2 * pct);
            
            acceleration.x = acceleration.x - dir.y;
            acceleration.y = acceleration.y + dir.x;
        }
    }
    

    
};
#endif