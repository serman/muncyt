//
//  generativo2.h
//  mainScreen
//
//  Created by Sergio Galan on 7/28/14.
//
//

#ifndef __mainScreen__generativo2__
#define __mainScreen__generativo2__

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
#include "ofxFilterLibrary.h"
class generativo2 : public ofxScene{
    
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

#define lineW  768 //ancho de la linea
#define  lineS  60
#define  NLINES 52
    
    float  lines[NLINES][lineW+1];
     float  filterr[lineW];
    
    
    int tesela=1;
    float px2,px1,py2,py1;
    

    int lineDistance=8;
    ofPolyline p;
    float tx1;
    float ty1,tx2,ty2 ;
    AbstractFilter *filter;
    
    vector<ofPoint> people;
    
    
};


#endif /* defined(__mainScreen__generativo2__) */
