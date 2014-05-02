#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxBackground.h"
#include "ofxBlobTracker.h"
#include "ofxUI.h"
#define _USE_LIVE_VIDEO		// uncomment this to use a live camera

class testApp : public ofBaseApp{
    private:
        ofImage bg;
        #ifdef _USE_LIVE_VIDEO
            ofVideoGrabber 		cam;
        #else
            ofVideoPlayer 		vidPlayer;
        #endif
        bool isNewFrame;
        ofxCvColorImage		colorImg;			//a place to save the live video frame
    ofxCvGrayscaleImage grayImage;
        ofxBackground		backgroundAddon;	//the addon that allows you to use a variety of methods for background/foreground segmentation
		ofxBlobTracker          blobTracker;
        bool bLearnBackground;	//a boolean to indicate whether to instruct the addon to learn the background again, defaults to true
    	ofTrueTypeFont consoleFont;
    
    	ofxUICanvas *gui1;
        ofxUICanvas *gui2;
    
    	int blobThreshold;
    
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
		
};
