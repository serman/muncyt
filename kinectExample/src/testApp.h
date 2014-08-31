#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxCv.h"
#include "particleClouds.h"
#include "ofxUI.h"
#include "cheapCommRcv.h"
#include "ofxDelaunay.h"
//#include "serialized.h"
//#include "ofxMsgPacker.h"
//#include "ofxMsgUnPacker.h"
#include <msgpack.hpp>
#include "sendContour.h"
//#define EASYCAM

#ifndef EASYCAM
	#include "ofxGrabCam.h"
#endif
//#include "ofxGrabCam.h"

// uncomment this to read from two kinects simultaneously
//#define USE_TWO_KINECTS

#define USEFILE


	#include "ofxOpenNI2Grabber.h"
	#include "ofxOpenNI2Recorder.h"
	#include "extendedDepthSource.h"


#define IRCAMERAWIDTH 640
#define IRCAMERAHEIGHT 480

using namespace ofxCv;
using namespace cv;
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
	
	ofxCvColorImage grayImage; // grayscale depth image
	ofxCvGrayscaleImage grayThreshNear; // the near thresholded image
	ofxCvGrayscaleImage grayThreshFar; // the far thresholded image
	
	
    bool debug;
	bool bDrawPointCloud;

	
	int angle;
	
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

	bool explosion;

    //function
    
    void    drawCountours();
    
    int		stepCloudPoint;
	int		stepLines;

    //shaders
    ofShader shader;
    void	setupShader();
   	void	drawNoise();
    void    drawCVSilhouettes();
    bool	boolDrawNoise;
    int     alphaNoise;
	
    int		alphaLines;
	ofColor colorfondo;
    long lastExplosionTime;
    //osc
    cheapCommRcv myOSCrcv;

    //test
    int maxForce;
    void cambioEscena();
    
    std::map<string, int> appStatuses;
    
//********CV
    ofxCv::ContourFinder contourFinder;
    ofPolyline resampledContour;
    ofImage depthImg;
    bool doTriang;
	ofxDelaunay triangulation;
	bool	bAddPts;		// si a–ade puntos a la triangulacion
	bool	bSoloEnContorno;	// si muestra solo los triangulos de dentro del contorno
	ofMesh	triangContMesh;
	
	bool doTessel;
	ofTessellator tessel;
	ofMesh contornoTes;	// contorno teselado
    
    enum  {EM, NUCLEAR_DEBIL, NUCLEAR_FUERTE, GRAVEDAD};
    
    int status_mode=EM;


    void setupStatus();
    // declaraciones
//    bool	bAddPts;		// si a–ade puntos a la triangulacion
//    bool	bSoloEnContorno;	// si muestra solo los triangulos de dentro del contorno
//    ofMesh	triangContMesh;
    ofMesh	triangContMesh_old;
    int numPointsXtra = 100;
    
    // Colores
    vector<ofColor> colorFluor;
    void cargaColores();
    
    
    bool bFill;
    
    int modoFill;
    
    bool bDrawOld;
    sendContour sender;

    particleClouds particleCloud;
    
};
