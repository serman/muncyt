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
#include "effects.h"
#include "tr1/boost_shared_ptr.h"
#include "consts.h"
#define SCREEN_W 768
#define SCREEN_H 384


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
    void setupUI();

private:
    ofxBox2d                            box2d;
    vector		<ofPtr<ofxBox2dRect> >	circles;		  //	default box2d circles
    void addObstacle();
    void addObstacle(ofPoint p1, int m_id,  float w=0.05, float h=0.05);
    void updateObstacle(ofPoint p1, int m_id);
     void updateObstacle(ofPoint p1, int m_id, float, float);
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
    void randomMoveGoal();
    //ofPoint convertPoint(float, float);
    /** escalado video ajuste pantala
    float VIDEO_scale=SCREEN_W/640;
    float VIDEO_offset=((VIDEO_scale*480)-SCREEN_H)/2;*/
    std::map<string, int> appStatuses;
    ofPixels gray;
	ofImage edge;
    
    ofxUICanvas *gui2;
    void gui2Event(ofxUIEventArgs &);
    ofPixels remoteBlobImgPxl;
    ofImage feedImg;
        AbstractFilter *filter;
    typedef enum tipos {BALL=1, GOAL =2, OBSTACLE} tipoObj;
    enum game_status{WAIT, PLAYING, WIN,LOSE,WINWIN};
    enum game_sub_status{THROW, BOUNCING};
    typedef struct  {
        tipoObj tipo;
        int id ;
        bool isHidden;
    } datoObjeto;

    effectsHandler eff;
    
    void reset();
    void setLevel();
        int helpLinesCount;
    
    //Parametros tocables float density, float bounce, float friction)
    float ballDensity=0.89;
    float ballBounce=0.65;
    float ballFriction=0.8;
    
    float enemyDensity=0.0; //static object
    float enemyBounce=1.5;
    float enemyFriction=0.7;
    
    float gravityY=10.0;
    
    float goalHeight=80;
    bool goalDown=false;
    ofImage flecha;
        ofImage ballImg;
    ofImage goalImg;
    
};




#endif /* defined(__mainScreen__juego1__) */
