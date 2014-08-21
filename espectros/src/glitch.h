//
//  glitch.h
//  mainScreen
//
//  Created by Sergio Galan on 7/28/14.
//
//

#ifndef __mainScreen__glitch__
#define __mainScreen__glitch__

#include <iostream>
#include "ofxUI.h"
#include "ofMain.h"
#include "ofxSyphon.h"
#include "ofxCv.h"
#include "mosaicPlayer.h"
#include "ofxTuio.h"
#include <ofMath.h>
#include "ofxScene.h"

class glitch : public ofxScene{
    
public:
    void setup();
    void update(float );
    void draw();
    
    ofxSyphonClient *mSyphonClient;
    ofxSyphonClient *mSyphonClient2;
    
    ofFbo fbo; // with alpha
    ofFbo fboGeneral; // with alpha
    ofFbo fboGlitches;
    ofPixels remoteBlobImgPxl;
    
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
    
    /*    void	tuioAdded(ofxTuioCursor & tuioCursor);
     void	tuioRemoved(ofxTuioCursor & tuioCursor);
     void	tuioUpdated(ofxTuioCursor & tuioCursor);
     */



    void sceneWillAppear( ofxScene * fromScreen );
    void sceneWillDisappear( ofxScene * toScreen );
    void init_Escena();
    void exit_Escena();
    
    void drawBigPixels();
    void drawBigPixels(int x1, int y1, int sizex, int sizey);
    
    void drawSectionAlpha();
    
private:
    
};


#endif /* defined(__mainScreen__glitch__) */
