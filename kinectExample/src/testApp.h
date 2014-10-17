#ifndef __kinect__app__
#define __kinect__app__

#include "ofMain.h"
#include "particleClouds.h"
#include "dela.h"
#include "ofxUI.h"
#include "cheapCommRcv.h"
#include "ofxDelaunay.h"
//#include "serialized.h"
//#include "ofxMsgPacker.h"
//#include "ofxMsgUnPacker.h"
#include <msgpack.hpp>
#include "sendContour.h"
#include "rcvContour.h"
#include "videoMask.h"
#include "contours.h"
#include "gridView.h"
#include "ofxPostProcessing.h"
#include "highBloomPass.h"
#include "tunnel.h"
#include "rayoSilueta.h"
#include "menu.h"
#include <math.h>
//#define EASYCAM
#include "consts.h"
#ifndef EASYCAM
	#include "ofxGrabCam.h"
#endif


//#define USEFILE
//#define TESTMODE

	#include "ofxOpenNI2Grabber.h"
	#include "ofxOpenNI2Recorder.h"
	#include "extendedDepthSource.h"


#define IRCAMERAWIDTH 640
#define IRCAMERAHEIGHT 480

static unsigned char GLOW_ON = 0x0002;
static unsigned char ZOOM_ON = 0x0001;
static unsigned char VERTICAL_ON = 0x0004;
static unsigned char CONV_ON = 0x0008;


class cheapCommRcv;
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

//	void	drawLinesH(float step = 5.0);
//	void	drawLinesV(float step = 5.0);
    void	showDebug();
    void setRandomBG();
    void fadeBG();
	void startFBO();

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
    cheapCommRcv *myOSCrcv;

    //test
    void toScene(int );
    void endScene(int );
    void parseStrongOsc(string s,ofxOscMessage &m);
    void parseWeakOsc(string s,ofxOscMessage &m);
    void parseEmOsc(string s,ofxOscMessage &m);
    void parseGravityOsc(string s,ofxOscMessage &m);
    void parseMenuOsc(string s,ofxOscMessage &m);
    
    std::map<string, int> appStatuses;
    
//********CV
   
    
    
    
    int status_mode=EM;

    void setupStatus();
    
    sendContour sender;
    rcvContour rcvCont;
    particleClouds particleCloud;
    contours mcontour;
    gridView mgrid;
    tunnel   mtunnel;
    rayoSilueta mrayoSil;
    videoMask mvideoMask;
    menu mmenu;
    ofVec3f tr, tl, bl, br;
    ofMesh mesh1;
    ofLight light;
    int lx,ly,lz=0;
    ofxPostProcessing post;
    ofxPostProcessing post2D;
    ofFbo pfbo;
    void loadScreenId();
    
    void setLightOri(ofLight &light, ofVec3f rot)
    {
        ofVec3f xax(1, 0, 0);
        ofVec3f yax(0, 1, 0);
        ofVec3f zax(0, 0, 1);
        ofQuaternion q;
        q.makeRotate(rot.x, xax, rot.y, yax, rot.z, zax);
        light.setOrientation(q);
    }
    dela mdela;
    
    void setShaders(unsigned int  shaderFlags);

};
#endif