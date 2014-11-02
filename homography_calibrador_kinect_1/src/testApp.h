#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxKinect.h"

#include "ofxCv.h"
#include "Calibrador.h"

enum modos {M320x240, M400x300, M640x480};

class testApp : public ofBaseApp {
public:
	
	void setup();
	void update();
	void draw();
	void exit();
	
	void drawPointCloud();
	
	void keyPressed(int key);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
    
	int modo;
    
	bool movePoint(vector<ofVec2f>& points, ofVec2f point);
    
	ofxKinect kinect;
	
	bool reg;
	
	ofxCvColorImage colorImg;
	
	ofxCvGrayscaleImage grayImage; // grayscale depth image
	
	int angle;
	
	Calibrador	calib1, calib2;
	
	string		nombre1, nombre2;
	
	ofRectangle c1_Src, c1_Dst;
	ofRectangle c2_Src, c2_Dst;
	
	ofImage		imgRGB, imgBN;
	ofImage		imgCalib;
	ofImage		warped1, warped2;
    //	vector<ofVec2f> leftPoints_c1, rightPoints_c2;
	bool movingPoint, isDstPt_c1, isDstPt_c2;
	ofVec2f* curPoint;
	
};

