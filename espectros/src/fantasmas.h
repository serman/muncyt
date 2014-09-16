#pragma once

#include "ofxUI.h"
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
#include "ofxScene.h"

#define MAX_SEQUENCES 3
class fantasmas : public ofxScene{
	public:
		void setup();
		void update(float );
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

        void sceneWillAppear( ofxScene * fromScreen );
        void sceneWillDisappear( ofxScene * toScreen );
        void init_Escena();
        void exit_Escena();
    
    
    	ofxSyphonClient *mSyphonClient;
        ofxSyphonClient *mSyphonClient2;
    
        void setSyphonClients( ofxSyphonClient *mSyphonClient_,ofxSyphonClient *mSyphonClient2_ ){
            cout << "settings syphon client" <<endl;
            mSyphonClient=mSyphonClient_;
            mSyphonClient2=mSyphonClient2_;
        }
    
	    cheapComm myOSCcomm;
    //32 bits red, 32 bits green, 32 bits blue, from 0 to 1 in 'infinite' steps
    ofFbo fbo; // with alpha
    ofPixels cameraPixels;
    ofImage cameraImg;
    
    ofxTuioClient *tuioclient;
    void setTuioClient(ofxTuioClient *mtuioclient){
        tuioclient=mtuioclient;
    }
    
/*    void	tuioAdded(ofxTuioCursor & tuioCursor);
	void	tuioRemoved(ofxTuioCursor & tuioCursor);
	void	tuioUpdated(ofxTuioCursor & tuioCursor);
    */
    moveAndRecord moveandrecord;
    mosaicRecorder mRecorder;
    ofPixels remoteBlobImgPxl;
    ofImage testImg;
    ofImage feedImg;
    ofxUICanvas *gui2;
    std::map<string, int> appStatuses;
     void setupStatus();
    enum mode{CAPTURE, MOSAIC};
    
    
    void setupSequences();
    void setupSequence(int seq);
    vector<ofxImageSequence> sequences;
    void showDebug();
    int RecordingBlobId;
    
    vector<ofPoint> points[MAX_SEQUENCES];
    
    void onRecordingFinished(int &num);
    void playImage(int i);
    int playImgIndex[MAX_SEQUENCES];
    void gui2Event(ofxUIEventArgs &e);
    int currentImg=0;
    int maxFrame=50;
    
    
};
