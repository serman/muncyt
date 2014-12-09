//
//  testApp.h
//  mainScreen
//
//  Created by Sergio Galan on 7/28/14.
//
//

#pragma once

#include "ofMain.h"


#include "ofxSceneManager.h"

#include "ofxTuio.h"
#include "ofxSyphon.h"

/*** ESCENAS **/
#include "fantasmas.h"
#include "glitch.h"
#include "faces.h"
#include "MosaicoCaras.h"
#include "generativo1.h"
#include "generativo2.h"
#include "juego1.h"
#include "juego2.h"
#include "joy2KeyLocal.h"
class testApp : public ofBaseApp{
    
	enum Scenes{ FANTASMAS=0, BOLA, COMEPANTALLA, JOY_WAVES ,CARAS3D, MOSAICOCARAS };
#define NUM_SCENES 5
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed  (int key);
    void keyReleased  (int key);
    void mousePressed( int x, int y, int button );
    
    
    void windowResized(int w, int h);
	void exit();
    ofxSceneManager *		sceneManager;
    ofxTuioClient tuioClient;
    
    void tuioAdded(ofxTuioCursor &tuioCursor);
    void tuioUpdated(ofxTuioCursor &tuioCursor);
    void tuioRemoved(ofxTuioCursor &tuioCursor);


    ofxSyphonClient mSyphonClient;
    ofxSyphonClient mSyphonClient2;
    joy2key j2key;
    void gotMessage(ofMessage m);
    long timeToEndScene;
    int chooseScene();
    
};
