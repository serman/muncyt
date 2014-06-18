#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
//#include "ofxBackground.h"
#include "ofxBlobTracker.h"
#include "ofxBlob.h"
#include "ofxUI.h"
#include "ofxSyphon.h"
#include "ofxCv.h"
#include "ofxOsc.h"
#include "cheapComm.h"
#include "mosaicRecorder.h"
#include <math.h>
//#define _USE_LIVE_VIDEO		// uncomment this to use a live camera
#define VIDEOWITH 640
#define VIDEOHEIGHT 480
#define VIDEO_PROC_SCALE 2;
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
    	//ofxBackground		backgroundAddon;	//the addon that allows you to use a variety of methods for background/foreground segmentation
		ofxBlobTracker          blobTracker;
    
    
    	ofxUICanvas *gui1;
        ofxUICanvas *gui2;
    
        ofTrueTypeFont consoleFont;
    	/** intermediate images for tracking ***/
	    ofxCvColorImage		sourceColorImg;			//a place to save the live video frame
        ofxCvGrayscaleImage		previousImg;			//a place to save the live video frame
    	ofxCvGrayscaleImage grayImage;
        ofxCvShortImage		floatBgImg;
	    ofxCvGrayscaleImage grayBg;
        ofImage maskedImageOF;
        ofImage blobImgOF_min;
    ofImage contourMaskOF;
    	void setMaskedImageBlobs();
	    bool adminMode;
	    long exposureStartTime;
    int nonZero=0;
        int movementAverage[50];
    int counterAverage=0;
	    ofFbo fbo;
    /*** tracking parameters ***/
    	int minBlobSize=300;
	    int maxBlobSize=4500;
    	int blobThreshold=30;
	    int amplify=10;
    	int smooth=2;
        bool refreshBackground=false;
	    bool adaptativeBackground=false;
	    float fLearnRate= 0.005f;
        bool bLearnBackground=true;	//a boolean to indicate whether to instruct the addon to learn the background again, defaults to true
    	cheapComm myComm;
    	mosaicRecorder mRecorder;
	    vector <ofPoint> positions;
    	int rectCounter=0;
    int alphaCounter=0;
	public:
		void setup();
	    void setupStatus();
		void update();
		void draw();
	    void drawCoolGui();
    void showDebug();
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    	void gui2Event(ofxUIEventArgs &e);
        void blobAdded(ofxBlob &_blob);
        void blobDeleted(ofxBlob &_blob);
    	ofxUITextArea *msgArea;
	    void cleanBackgrounds();
        string textString;
    	ofxSyphonServer individualTextureSyphonServer;
        ofxSyphonServer onlyBlobsImageSyphonServer;
		ofTexture tex;
    
    	std::map<string, int> appStatuses;
    	

    /***TUIO **/
	    void sendTUIO(std::vector<ofxBlob> * objectBlobs);
    	int frameseq;
    	ofxOscSender	TUIOSocket;
    	void setupTUIO();
    
};
