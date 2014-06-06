#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxKinect.h"
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
	
	ofxKinect kinect;

	bool	pulso;
	bool	bDrawPoints;
	bool	bDrawLinesH, bDrawLinesV;
	
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
	bool bThreshWithOpenCV;
	bool bDrawPointCloud;
	
	int nearThreshold;
	int farThreshold;
	
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
    int		numParticles;
    enum	{NUBE, ESPEJO};
    int		particleMode;
    
    float	speed;
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
	
    //osc
    cheapCommRcv myOSCrcv;
};
