#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxBackground.h"
#include "ofxBlobTracker.h"
#include "ofxUI.h"
#include "ofxSyphon.h"
#include "ofxCv.h"
#include "cheapComm.h"
#include "mosaicRecorder.h"
//#define _USE_LIVE_VIDEO		// uncomment this to use a live camera
#define VIDEOWITH 640
#define VIDEOHEIGHT 480
#define CAMERA_EXPOSURE_TIME  2200.0f

class testApp : public ofBaseApp{
    private:
        ofImage bg;
        #ifdef _USE_LIVE_VIDEO
            ofVideoGrabber 		cam;
        #else
            ofVideoPlayer 		vidPlayer;
        #endif
        bool isNewFrame;
/*** ADDONS **/
    	ofxBackground		backgroundAddon;	//the addon that allows you to use a variety of methods for background/foreground segmentation
		ofxBlobTracker          blobTracker;
    
    
    	ofxUICanvas *gui1;
        ofxUICanvas *gui2;
    
        ofTrueTypeFont consoleFont;
    	/** intermediate images for tracking ***/
	    ofxCvColorImage		sourceColorImg;			//a place to save the live video frame
    	ofxCvGrayscaleImage grayImage;
        ofxCvShortImage		floatBgImg;
	    ofxCvGrayscaleImage grayBg;
        ofImage blobImgOF;
        ofImage blobImgOF_min;
	    bool adminMode;
	    long exposureStartTime;
    
	    ofFbo fbo;
    /*** tracking parameters ***/
    	int minBlobSize=150;
	    int maxBlobSize=3000;
    	int blobThreshold=11;
	    int amplify=10;
    	int smooth=1;
        bool refreshBackground=false;
	    bool adaptativeBackground=false;
	    float fLearnRate= 0.005f;
        bool bLearnBackground=true;	//a boolean to indicate whether to instruct the addon to learn the background again, defaults to true
    	cheapComm myComm;
    	mosaicRecorder mRecorder;
    
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
    
        void blobAdded(ofxBlob &_blob);
        void blobDeleted(ofxBlob &_blob);
    	ofxUITextArea *msgArea;
	    void cleanBackgrounds();
        string textString;
    	ofxSyphonServer individualTextureSyphonServer;
		ofTexture tex;
};
