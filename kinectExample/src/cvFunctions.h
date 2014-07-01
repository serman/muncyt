//
//  cvFunctions.h
//  kinectExample
//
//  Created by Sergio Galan on 6/26/14.
//
//
#include "ofMain.h"
#include "ofxCv.h"

#ifndef kinectExample_cvFunctions_h
#define kinectExample_cvFunctions_h

using namespace ofxCv;
using namespace cv;

vector<ofPolyline>  getSmoothSilhouete1( ofxCv::ContourFinder &contourFinder, ofImage &grayimg,float threshold );
#endif
