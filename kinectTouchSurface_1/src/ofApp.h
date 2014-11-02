#ifndef _TEST_APP
#define _TEST_APP

#pragma once

#include "ofMain.h"

#include "ofxOpenCv.h"
#include "ofxCv.h"
#include "ofxKinect.h"
#include "ofxXmlSettings.h"
#include "ofxUI.h"
#include "ofxOsc.h"

#include "BlobX.h"
#include "TUIO.h"

#include "Calibrador.h"

class testApp : public ofBaseApp{

public:
	void setup();
	void update();
	void draw();
	void exit();
    
	void keyPressed  (int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
    
	ofxKinect kinect;
	
	int wk, hk;	// domensiones de la imagen de la kinect
	
	int modo;
	
	// MODO 0
	int W, H;						//Screen size
	vector<ofPoint> corners; 		//Marked corners of the surface
	ofShortPixels backgroundDepth;	//Background depth image
	
	ofRectangle	rectCalib;	// zona donde se podra hacer la calibracion
	
	ofPixels outPixels;	//Pixels array holding current drawing
	ofImage  outImage;	//Output image built from outPixels
	
	bool	calibrating;	//Working mode selector : calibrate or draw
	ofPoint shift;			//Point used for drawing shifted color image
	
	bool getBackground;	// update background para bck-substraction
	
	float minV;		//Minimal distance in mm
	float maxV; 	//Maximal distance in mm
	
	void updateModo0();
	void updateModo1();
    
	void	drawMarco(ofRectangle rect);
	void	drawMarco(float xx, float yy, float ww, float hh);
    
	
	// MODO 1
	// http://forum.openframeworks.cc/t/universal-multitouch-wall-using-microsoft-sdk-openframeworks-and-ofxkinectnui/9908/37
    
	// Presentacion
	int wk1, hk1;
	ofRectangle	rect_RGB, rect_Depth, rect_Proy, rect_Blobs, rect_Bckgnd;

	
	ofxCvGrayscaleImage		grayImage; // grayscale depth image
    
	ofImage					colorImg;
	
	ofxCvGrayscaleImage		tempGrayImage;
	ofxCvGrayscaleImage		paso_openCV;
	ofxCvGrayscaleImage		grayImage_No_Blur;
	ofxCvGrayscaleImage		backgroundImage;	// para ver el background
    
    
	// background substraction
	// mejorara con shortImages o floatImages
	ofxCvGrayscaleImage	grayThreshNear;
	ofxCvGrayscaleImage	grayThreshFar;
	float				thresholdLow, thresholdHigh;
	bool				bLearnBakground;
    
	ofxCvFloatImage	fThreshNear, fThreshFar;
	
    bool    swDistMax;
	float	distMax;	// a partir de esta distancia se pone la imagen en negro
	
	ofShader				maskShader;
	ofxCvGrayscaleImage		maskImage;
	ofFbo					maskFbo;
	ofImage					imageMasked;
	ofxCvColorImage			maskImageColor;
	
	
	// ContourFinder CF
	ofxCv::ContourFinder contourFinderX;
	void	drawBlobs(int wk1, int hk1);
	int		max_blobs;
	float	min_blob_size, max_blob_size;
	bool	bFindHoles;
	float				threshold;			// contraste
	int					blurcv;
	bool bt;	// activar o no el threshold antes de cfX
	
	bool	bWarp;
	
	
	//
	// MODO 2 CALIBR
	//
	
	// Calibracion
	Calibrador	calib2;
	bool		cargaCalib;
	string		nombreCalib;
	bool		bCalibActiva2;
	bool		movePoint(vector<ofVec2f>& points, ofVec2f point);
	bool		movingPoint, isDstPt_c1, isDstPt_c2;
	ofVec2f*	curPoint;
	
	ofImage		proyImg;
	
	//
	// GUI
	//
	void	setupGUI();
	ofxUISuperCanvas *gui1;
	void	guiEvent(ofxUIEventArgs &e);
    
	// modo reducido
	bool	bPeque;
	
	// Settings en XML
	void loadXMLSettings();
	void saveSettings();
	
	
	
	//Communication
	// TUIO viene con la clas de tracking/contourFinder de ofxNCore (que no la cargo)
	// Si se quiere reutilizar hay que sustituir una clase llamada Blob
	bool	bTUIO;
	TUIO	myTUIO;
	vector<BlobX>	blobs;
    
};


#endif