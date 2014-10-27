//
//  generativo1.h
//  mainScreen
//
//  Created by Sergio Galan on 7/28/14.
//
//

#ifndef __mainScreen__generativo1__
#define __mainScreen__generativo1__

#include <iostream>
#include "ofxUI.h"
#include "ofMain.h"
#include "ofxSyphon.h"
#include "ofxCv.h"
#include "mosaicPlayer.h"
#include "ofxTuio.h"
#include <ofMath.h>
#include "ofxScene.h"
#include "consts.h"
class generativo1 : public ofxScene{
    
public:
    void setup();
    void update(float );
    void draw();
    
    ofxSyphonClient *mSyphonClient;
    ofxSyphonClient *mSyphonClient2;
    
    ofFbo fbo; // with alpha
    ofPixels cameraPixels;
    ofImage cameraImg;
    
    
    ofxTuioClient *tuioclient;
    
    void setTuioClient(ofxTuioClient *mtuioclient){
        tuioclient=mtuioclient;
    }
    
    void setSyphonClients( ofxSyphonClient *mSyphonClient_,ofxSyphonClient *mSyphonClient2_ ){
        cout << "settings syphon client" <<endl;
        mSyphonClient=mSyphonClient_;
        mSyphonClient2=mSyphonClient2_;
    }
    
    
    
    
        void	tuioAdded(ofxTuioCursor & tuioCursor);
     void	tuioRemoved(ofxTuioCursor & tuioCursor);
     void	tuioUpdated(ofxTuioCursor & tuioCursor);
     
    void sceneWillAppear( ofxScene * fromScreen );
    void sceneWillDisappear( ofxScene * toScreen );
    void init_Escena();
    void exit_Escena();
    
    void keyPressed(int k);
private:
    int rotateAng=0;
     ofImage backgroundImg;
};


#endif /* defined(__mainScreen__generativo1__) */
