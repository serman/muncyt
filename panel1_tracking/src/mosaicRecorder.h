//
//  mosaicRecorder.h
//  mainScreen
//
//  Created by Sergio Galan on 5/13/14.
//
//

#ifndef __mainScreen__mosaicRecorder__
#define __mainScreen__mosaicRecorder__

#include <iostream>

#endif /* defined(__mainScreen__mosaicRecorder__) */

#include "ofMain.h"
#include "ofxVideoRecorder.h"


class mosaicRecorder {
public:
    void setup();
    mosaicRecorder();
	ofxVideoRecorder videoSaver;
    void start(int _time, int _FPS,  ofImage *srcImage,int _w=0, int _h=0);
    void update();
    
private:
    ofVideoPlayer recordedVideoPlayback;
    int MAX_VIDEOS=10;
    string filename="videoRecorded";
    int current_video=0;
    bool isRecording=false;
    ofImage *currentImage;
    int FPS=25;
    int time=3000;
    int frameCount=0;
    
};