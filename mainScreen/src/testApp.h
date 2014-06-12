#pragma once

#include "ofMain.h"
#include "cheapComm.h"
#include "ofxSyphon.h"
#include "ofxCv.h"
#include "mosaicPlayer.h"
#include "mosaicRecorder.h"
#include "ofxTuio.h"
#include "moveAndRecord.h"
#include "ofxImageSequence.h"
#include <ofMath.h>
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
    ofxSyphonClient mSyphonClient2;
	    cheapComm myOSCcomm;
    //32 bits red, 32 bits green, 32 bits blue, from 0 to 1 in 'infinite' steps
    	ofFbo fbo; // with alpha
    ofPixels cameraPixels;
    ofImage cameraImg;
    ofxTuioClient tuioclient;
    
/*    void	tuioAdded(ofxTuioCursor & tuioCursor);
	void	tuioRemoved(ofxTuioCursor & tuioCursor);
	void	tuioUpdated(ofxTuioCursor & tuioCursor);
    */
    moveAndRecord moveandrecord;
    mosaicRecorder mRecorder;
    ofPixels remoteBlobImgPxl;
    ofImage testImg;
    ofImage feedImg;
    
    std::map<string, int> appStatuses;
     void setupStatus();
    enum mode{CAPTURE, MOSAIC};
    
    
    void setupSequences();
    vector<ofxImageSequence> sequences;
    void showDebug();
};
