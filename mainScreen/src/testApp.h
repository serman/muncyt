#pragma once

#include "ofMain.h"
#include "cheapComm.h"
#include "ofxSyphon.h"
#include "ofxCv.h"
class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    	ofxSyphonClient mSyphonClient;
	    cheapComm myOSCcomm;
    //32 bits red, 32 bits green, 32 bits blue, from 0 to 1 in 'infinite' steps
    	ofFbo fbo; // with alpha
    ofPixels cameraPixels;
    ofImage cameraImg;
};
