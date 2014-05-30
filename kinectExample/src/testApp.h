#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxKinect.h"
#include "Particle.h"
#include "ofxUI.h"
#include "cheapCommRcv.h"

// uncomment this to read from two kinects simultaneously
//#define USE_TWO_KINECTS
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
    void guiEvent(ofxUIEventArgs &e);

	void	drawLines();
    void showDebug();
	ofxKinect kinect;
	bool pulso;
#ifdef USE_TWO_KINECTS
	ofxKinect kinect2;
#endif
	
   	ofxUICanvas *gui1;
    
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
	ofEasyCam easyCam;
    
    
    // zMax
    int zMin;
    int zMax;
    float low;
    int    incrDistance;
    ofImage Img;
    ofMesh mesh;
    ofMesh meshParticles;
	bool explosion;
    //function
    void setupParticles();
     void drawParticles();
    void updateParticles();
	void    resetParticles();
    int numParticles;
    enum  {NUBE, ESPEJO};
    int particleMode;
    
    float speed;
    int stopUmbral;
    
    
    //shaders
    ofShader shader;
    void setupShader();
   	void drawNoise();
    bool boolDrawNoise;
    int alphaParticles;
    //osc
    cheapCommRcv myOSCrcv;
};
