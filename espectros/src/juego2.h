//
//  juego2.h
//  mainScreen
//
//  Created by Sergio Galan on 7/28/14.
//
//

#ifndef __mainScreen__juego2__
#define __mainScreen__juego2__

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
#include "cv.h"
#include <math.h> 
#include "enemy.h"
#include "particleSourceHandler.h"

#include "consts.h"

class juego2 : public ofxScene{
    
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
    vector		<ofPtr<ofxBox2dCircle> >	circles;		  //	default box2d circles
    void addObstacle();
    void addObstacle(ofPoint p1, int m_id);
    void updateObstacle(ofPoint p1, int m_id);
    void removeObstacle(int);
    void hideObstacle(int);

    ofxBox2dCircle player;
    void drawControls();
    void reset();

    void contactStart(ofxBox2dContactArgs &e);
    void drawEnemies();

    //ofPoint convertPoint(float, float);
    /** escalado video ajuste pantala
    float VIDEO_scale=SCREEN_W/640;
    float VIDEO_offset=((VIDEO_scale*480)-SCREEN_H)/2;*/
    std::map<string, int> appStatuses;
    ofPixels gray;
    ofImage imgRecortada;

    
    ofPixels remoteBlobImgPxl;
    ofImage feedImg;
    
    int getDistance(){
        int minDist=100000000;
      /*  for(int i=0; i< allPolis.size(); i++){
            ofPoint p= poli.getClosestPoint(player.getPosition());
            
            int d= round( p.distance(player.getPosition()));
            if(d<minDist)
        }*/
    }
    std::vector<ofPoint> newPoints;
    ofPoint prevPos;
    ofPolyline poli;
    

    enum game_status{WAIT, PLAYING, WIN};
    enum moveDir{LEFT,RIGHT,UP,DOWN,NONE};
    
        void move(int);
    int movingSpeed =4;

    int distanceToLine=0;
    ofImage mask1;
    ofImage mask1gray;
    ofFbo maskFbo;
    
    bool    fillthis=false;
    ofxCv::ContourFinder contourFinder;
    float threshold=125;
    ofPoint findRegionToPaint();
    int currentKey=NONE;
    int previousKey=NONE;
    bool startProcessing=false;
    bool prepareReset = false;
    
    void drawBgRegion();
    void drawNewArea();
    void explode(ofPoint);
    void drawExplode();
    ofPolyline l;
    int start, end;
    bool drawExplodeb;
    int calcMarcador();
 ParticleSourceHandler mparticles;
    ofxBox2dCircle bigEnemy;
    float xoff=0;
    float yoff=1;
    ofPoint newPosition(){
        ofPoint p1;
        xoff+=0.01;
        yoff+=0.01;
        p1.x=ofNoise(xoff,yoff)*768;
        p1.y=ofNoise(yoff,yoff)*384;
        return p1;
    }
    void createBigEnemy();

};


#endif /* defined(__mainScreen__juego2__) */
