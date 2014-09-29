//
//  menu.h
//  kinectExample
//
//  Created by Sergio Galan on 9/29/14.
//
//

#ifndef kinectExample_menu_h
#define kinectExample_menu_h
#include "consts.h"
#include <ofMain.h>
#include <math.h>
class menu{
public:
    void setup(){
        emImg.loadImage("emImg.png");
        weakImg.loadImage("weakImg.png");
        strongImg.loadImage("strongImg.png");
        gravityImg.loadImage("gravityImg.png");
        flabImg.loadImage("flabImg.png");
        start(MENU);
    }
    
    void start(int _state){
        initTime=ofGetElapsedTimeMillis();
        state=_state;
        
    }
    void draw(){
        ofPushStyle();
        ofEnableAlphaBlending();
        ofSetColor(255,min( (int)(ofGetElapsedTimeMillis()-initTime)/2,255) ) ;
        switch(state){
            case EM:
                emImg.draw(0,0,ofGetWidth(),ofGetHeight());
                ofDrawBitmapString (ofToString(ofGetElapsedTimeMillis()-initTime),500,500);
                
            break;
            case NUCLEAR_DEBIL:
                weakImg.draw(0,0,ofGetWidth(),ofGetHeight());
            break;
            case NUCLEAR_FUERTE:
                strongImg.draw(0,0,ofGetWidth(),ofGetHeight());
                break;
                
            case GRAVEDAD:
                gravityImg.draw(0,0,ofGetWidth(),ofGetHeight());
                break;

            case MENU:
                flabImg.draw(0,0);
            break;
        }
        ofDisableAlphaBlending();
        ofPopStyle();
        
        
    }

private:
    ofImage emImg, weakImg,strongImg,gravityImg,flabImg;
    long initTime;
    int state;
    
    
};

#endif
