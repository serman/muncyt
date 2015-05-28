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

    
	int modo;
	// MODO 1
	// http://forum.openframeworks.cc/t/universal-multitouch-wall-using-microsoft-sdk-openframeworks-and-ofxkinectnui/9908/37
    	
	void updateModo1();
	void updateModo2(); /// dic-GGG
    bool    bModoAdapt;
    
	void	drawMarco(ofRectangle rect);
	void	drawMarco(float xx, float yy, float ww, float hh);
	
	ofxKinect kinect;
	
	int wk, hk;		// Dimensiones de las imagenes de trabajo y de kinect	
	int wk1, hk1;	// Dimensiones para la presentaci—n en pantalla
	
	ofRectangle	rect_RGB, rect_Depth, rect_Proy, rect_Blobs, rect_Bckgnd;
	
	ofxCvGrayscaleImage		grayImage; // grayscale depth image
    
	ofImage					colorImg, depthImg;
	
	ofxCvGrayscaleImage		tempGrayImage;
	ofxCvGrayscaleImage		backgroundImage;	// para ver el background
	bool                    bLearnBakground;
    
    ofxCv::RunningBackground background;
    ofImage thresholded, bckCvImg;
    float   thres, lr, lrUI;
    
    // temporizador para coger background al arrancar la aplicacion
    int numFramesMax_newBckgrnd;
    
    
	// background substraction
	// mejorara con shortImages o floatImages
	ofxCvGrayscaleImage	grayThreshNear;
	ofxCvGrayscaleImage	grayThreshFar;
	float				thresholdLow, thresholdHigh;

	ofxCvFloatImage	fThreshNear, fThreshFar;	// Aœn no lo uso
	
    bool    swDistMax;
	float	distMax;	// a partir de esta distancia se pone la imagen en negro
	bool    swErodeMask;
    
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
	
    bool swErode;
    
	bool	bWarp;
	
	//
	// Calibracion
	//
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