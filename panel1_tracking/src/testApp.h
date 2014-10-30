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
#include "ofxSimpleMask.h"
#include <math.h>

//#define _USE_LIVE_VIDEO		// uncomment this to use a live camera
#define VIDEOWITH 720
#define VIDEOHEIGHT 576
#define VIDEO_PROC_SCALE 2;
#define CAMERA_EXPOSURE_TIME  2200.0f

#define NCAMARAS 2
#define CEIL_INDEX 0
#define FRONT_INDEX 1
//#define TESTMODE
class cheapComm;
class testApp : public ofBaseApp{
    private:
        ofImage bg;
        #ifdef _USE_LIVE_VIDEO
            ofVideoGrabber 		camCeil;
            ofVideoGrabber 		camFront;
            ofVideoGrabber      *currentCam;
        #else
            ofVideoPlayer 		vidPlayerCeil;
            ofVideoPlayer 		vidPlayerFront;
            ofVideoPlayer 		*currentVid;
        #endif
        bool isNewFrame;
/*** ADDONS **/
    	//ofxBackground		backgroundAddon;	//the addon that allows you to use a variety of methods for background/foreground segmentation
		ofxBlobTracker          blobTracker;
    
        ofxUICanvas *gui0;
        ofxUICanvas *gui1;
        ofxUITabBar *guiTabBar;
    
        ofTrueTypeFont consoleFont;
    	/** intermediate images for tracking ***/
	    ofxCvColorImage		sourceColorImg;			//a place to save the live video frame
    	ofxCvGrayscaleImage sourceGrayImage;
        ofxCvShortImage		*floatBgImg;
        ofxCvShortImage		floatBgImgCameraCeil;
        ofxCvShortImage		floatBgImgCameraFront;
    
	    ofxCvGrayscaleImage grayBg; //
       // ofxCvGrayscaleImage grayBgCameraCeil; //
       // ofxCvGrayscaleImage grayBgCameraFront; //
        ofImage maskedImageOF;
  
    ofImage contourMaskOF;
    	void setMaskedImageBlobs();
	    bool adminMode;
	    long exposureStartTime;
    int nonZero=0;
        int movementAverage[50];
    int counterAverage=0;
	    ofFbo fbo;
    /*** tracking parameters ***/
    	int minBlobSize[NCAMARAS];
        int maxBlobSize[NCAMARAS];
    	int blobThreshold[NCAMARAS];
	    int amplify[NCAMARAS];
    	int smooth[NCAMARAS];
                        
	    bool adaptativeBackground=false;
	    float fLearnRate= 0.005f;
        bool bCaptureBackground=true;	//a boolean to indicate whether to instruct the addon to capture the background again, defaults to true
    	cheapComm *myComm;
	    //vector <ofPoint> positgui_settingsCamera0.xml"guiions;
    	int rectCounter=0;
    int alphaCounter=0;
	public:
		void setup();
	    void setupStatus();
		void update();
		void draw();
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
    void setupGui();
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
    ofxSimpleMask maskMaker;
    ofFbo fbo1;
    void setFrontCamera();
    void setCeilCamera();
    bool firstTimeFrontCamera;
    bool firstTimeCeilCamera;
    int configIndex;
    
    
};
