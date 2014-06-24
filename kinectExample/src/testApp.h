#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"

#include "Particle.h"
#include "ofxUI.h"
#include "cheapCommRcv.h"

//#define EASYCAM

#ifndef EASYCAM
	#include "ofxGrabCam.h"
#endif
//#include "ofxGrabCam.h"

// uncomment this to read from two kinects simultaneously
//#define USE_TWO_KINECTS

#define ASUS
//#define USEFILE

#ifndef ASUS
	#include "ofxKinect.h"
#else
	#include "ofxOpenNI2Grabber.h"
	#include "ofxOpenNI2Recorder.h"
	#include "extendedDepthSource.h"
#endif

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
	
#ifndef ASUS
	ofxKinect kinect;
#else
    ofxOpenNI2Grabber oniCamGrabber;
    ofxOpenNI2GrabberSettings oniSettings;
	ofxOpenNI2Recorder oniCamrecorder;
    extendedDepthSource depthGenerator;
    RGBSource rgbGenerator;
    bool isReady;
#endif
	bool	pulso;
	bool	bDrawPoints;
	bool	bDrawLinesH, bDrawLinesV,bDrawNativePointCloud;
    bool 	bDrawContours;
    bool 	bRealColors;
	int 	distanciaLineasK;
	// GUI
	void		setupGUI();
   	ofxUICanvas *gui1;
    
	void		grabarScreen();
	
	ofxCvColorImage colorImg;
	
	ofxCvGrayscaleImage grayImage; // grayscale depth image
	ofxCvGrayscaleImage grayThreshNear; // the near thresholded image
	ofxCvGrayscaleImage grayThreshFar; // the far thresholded image
	
	ofxCvContourFinder contourFinder;
	
    bool debug;
	bool bDrawPointCloud;

	
	int angle;
    //int particlesSize=640*480;
    //ofxParticle3D particles[particlesSize];
	 vector<Particle> particles ;
	// used for viewing the point cloud
	
#ifdef EASYCAM
	ofEasyCam easyCam;
#else 
	ofxGrabCam camera;	
	void saveCameraPose();
	void loadCameraPose();
#endif
	
    
    
    // zMax
    float zMin, zMax;

    float low;
    int    incrDistance;
    ofImage Img;
    ofMesh mesh;
    ofMesh meshParticles;
	bool explosion;

    //function
    void	setupParticles();
	void	drawParticles();
    void	updateParticles();
	void    resetParticles();
    void    drawCountours();
    void 	explosionParticles();
    int		numParticles;
    enum	{NUBE, ESPEJO};
    int		particleMode;
    
    float	speed;
    float accTest;
    int		stopUmbral;
    
    int		stepCloudPoint;
	int		stepLines;

    //shaders
    ofShader shader;
    void	setupShader();
   	void	drawNoise();
    bool	boolDrawNoise;
    int		alphaParticles;
	
    int		alphaLines;
	ofColor colorfondo;
    long lastExplosionTime;
    //osc
    cheapCommRcv myOSCrcv;

    //test
    int maxForce;
    
};
