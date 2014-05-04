#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include <math.h>       /* ceil */
#include "findEyeCenter.h"

class testApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
    
    void keyPressed(int );
	int thresholdI;
	
	ofVideoGrabber cam;
	ofxCv::ObjectFinder finder;
	ofxCv::ObjectFinder finderEyeL;
    	ofxCv::ObjectFinder finderEyeR;
    ofxCv::ObjectFinder finderEyes;
    ofImage eye1;
    ofImage eye2;
    ofImage eyeGray;
    cv::Point leftPupil;
};
