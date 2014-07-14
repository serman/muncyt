#pragma once

#include "ofMain.h"
#include "ofxAnimatableFloat.h"

#include "ofxSceneManager.h"
#include "nuclear_debil.h"
#include "electromagnetica.h"
#include "nuclear_fuerte/nuclear_fuerte.h"
#include "gravedad/gravedad.h"
#include "ofxTuio.h"

class testApp : public ofBaseApp{

	enum Scenes{ SCENE_1 = 1, SCENE_2, SCENE_3,SCENE_4 };
	
	public:
		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void mousePressed( int x, int y, int button );


		void windowResized(int w, int h);
	
		ofxSceneManager *		sceneManager;
        ofxTuioClient tuioClient;
    
        void tuioAdded(ofxTuioCursor &tuioCursor);
        void tuioUpdated(ofxTuioCursor &tuioCursor);
        void tuioRemoved(ofxTuioCursor &tuioCursor);
    ofPath borde;
    
};
