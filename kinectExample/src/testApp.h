#pragma once

#include "ofMain.h"
#include "particleClouds.h"
#include "ofxUI.h"
#include "cheapCommRcv.h"
#include "ofxDelaunay.h"
//#include "serialized.h"
//#include "ofxMsgPacker.h"
//#include "ofxMsgUnPacker.h"
#include <msgpack.hpp>
#include "sendContour.h"
#include "contours.h"

//#define EASYCAM

#ifndef EASYCAM
	#include "ofxGrabCam.h"
#endif


#define USEFILE
//#define TESTMODE

	#include "ofxOpenNI2Grabber.h"
	#include "ofxOpenNI2Recorder.h"
	#include "extendedDepthSource.h"


#define IRCAMERAWIDTH 640
#define IRCAMERAHEIGHT 480


class testApp : public ofBaseApp {
public:
	
	void setup();
	void update();
	void draw();
	void exit();
	
	void drawPointCloud(int step = 1);
	
	void keyPressed(int key);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
    void guiEvent(ofxUIEventArgs &e);

	void	drawLinesH(float step = 5.0);
	void	drawLinesV(float step = 5.0);
    void	showDebug();
    void setRandomBG();
    void fadeBG();
	

    ofxOpenNI2Grabber oniCamGrabber;
    ofxOpenNI2GrabberSettings oniSettings;
	ofxOpenNI2Recorder oniCamrecorder;
    extendedDepthSource depthGenerator;
    RGBSource rgbGenerator;
    bool isReady;

	bool	pulso;
#ifdef TESTMODE
	bool	bDrawPoints;
	bool	bDrawLinesH, bDrawLinesV,bDrawNativePointCloud;
    bool 	bDrawContours;
    bool bDrawPointCloud;
    bool	boolDrawNoise;
#endif
   // bool 	bRealColors;

	int 	distanciaLineasK;
	// GUI
	void		setupGUI();
   	ofxUICanvas *gui1;
    ofxUITabBar *guiTabBar;
    
	void		grabarScreen();
	void drawAxis();
	ofxCvColorImage colorImg;
	
	//ofxCvColorImage grayImage; // grayscale depth image
	ofxCvGrayscaleImage grayThreshNear; // the near thresholded image
	ofxCvGrayscaleImage grayThreshFar; // the far thresholded image
    bool debug;
    

	
	int angle;
	
#ifdef EASYCAM
	ofEasyCam easyCam;
#else 
	ofxGrabCam camera;	
	void saveCameraPose();
	void loadCameraPose();
#endif

    float zMin, zMax;

    float low;
    int    incrDistance;
    ofImage Img;
    ofMesh mesh;

	//bool explosion;

    //function
    
    int		stepCloudPoint;
	int		stepLines;

    //shaders
    ofShader shader;
    void	setupShader();
   	void	drawNoise();

    
    int     alphaNoise;
	
    int		alphaLines;
	ofColor colorfondo;
    long lastExplosionTime;

    //osc
    cheapCommRcv myOSCrcv;
    void parseOSC(int );

    //test
    void cambioEscena();
    
    std::map<string, int> appStatuses;
    
//********CV
   
    enum  {EM, NUCLEAR_DEBIL, NUCLEAR_FUERTE, GRAVEDAD};
    
    int status_mode=EM;

    void setupStatus();
    
    sendContour sender;

    particleClouds particleCloud;
    contours mcontour;
    
};
