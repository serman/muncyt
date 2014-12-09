#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxCv.h"
#include "ofxFaceTracker.h"
//#include "ofxFaceTrackerThreaded.h"

#define LADO_CARA 400

struct faceData {
	ofRectangle rect;
	// aqui se pueden meter mas datos
	
	// carga y save en xml
	
};

enum modos_deteccion {
	MODO_HAAR,
	MODO_OFXCV,
	MODO_FCTRACKer
};

class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
	
	
		vector<string>		fileNames;
		vector<ofImage>		images;
		int					iImgAct;
		void cargarFiles();
		
		vector<ofImage>		images_faces;
		int					iFaceAct;
		int					lastTime;

	void drawBanda(int idFace, int idBanda, vector<ofRectangle> _bandas);
	
	vector<ofImage>		images_soloCara;

	
		bool			isAnalisisDone;
		faceData		rectFace;
		vector<faceData> rectsFacesAct;
	
		// FINDERS
		void setupFinders();
	
		int modoDetect;
	
		// MODO OFXOPENCV
		ofxCvHaarFinder finder;
	
		// MODO OFXCV
		ofxCv::ObjectFinder	finder_ofxcv;

		// MODO FACETRACKER
//		ofxFaceTrackerThreaded tracker, trackerFT;
		ofxFaceTracker tracker, trackerFT;
	
		void			update_faceTracker();	
		ofMesh			LEyeRect, REyeRect, mouthRect;
		ofVec2f			position;
		float			scale;
		ofMatrix4x4		rotationMatrix;
		void			drawFaceTracker();
	
	ofRectangle	rectCara, rectCara_X;	
	
	void				setupBandas();
	vector<ofRectangle> bandas1;
	vector<ofRectangle> bandas2;
	vector<int> ids_1;
	vector<int> ids_2;
	bool	draw_bandas2;
	
};
