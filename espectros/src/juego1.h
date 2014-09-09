//
//  juego1.h
//  mainScreen
//
//  Created by Sergio Galan on 7/28/14.
//
//

#ifndef __mainScreen__juego1__
#define __mainScreen__juego1__

#include <iostream>
#include "ofxUI.h"
#include "ofMain.h"
#include "ofxSyphon.h"
#include "ofxCv.h"
#include "mosaicPlayer.h"
#include "ofxTuio.h"
#include <ofMath.h>
#include "ofxScene.h"
#include "ofxBox2d.h"
#include "ofMain.h"
#include "ofxCv.h"
#include "ofxFilterLibrary.h"

#define SCREEN_W 768
#define SCREEN_H 384
#define VIDEO_W 640
#define VIDEO_H 480

class juego1 : public ofxScene{
    
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
    void keyPressed(int key);
    void keyReleased(int key);
    void showDebug()    ;

private:
    ofxBox2d                            box2d;
    vector		<ofPtr<ofxBox2dRect> >	circles;		  //	default box2d circles
    void addObstacle();
    void addObstacle(ofPoint p1, int m_id);
    void updateObstacle(ofPoint p1, int m_id);
    void removeObstacle(int);
        void hideObstacle(int);
    void throwBall();
    ofxBox2dCircle ball ;
    void drawControls();
    ofVec2f throwDirection;
    int throwForce;
    int amountForce;
    bool forceDecrease;
    ofxBox2dRect goal;
    void contactStart(ofxBox2dContactArgs &e);
    bool isGoal;
    void moveGoal();
    ofPoint convertPoint(float, float);
    /** escalado video ajuste pantala */
    float VIDEO_scale=SCREEN_W/640;
    float VIDEO_offset=((VIDEO_scale*480)-SCREEN_H)/2;
    std::map<string, int> appStatuses;
    ofPixels gray;
	ofImage edge;
    
    ofPixels remoteBlobImgPxl;
    ofImage feedImg;
        AbstractFilter *filter;
    typedef enum tipos {BALL=1, GOAL =2, OBSTACLE} tipoObj;
    enum game_status{WAIT, PLAYING, WIN};
    typedef struct  {
        tipoObj tipo;
        int id ;
        bool isHidden;
    } datoObjeto;


};




#endif /* defined(__mainScreen__juego1__) */
