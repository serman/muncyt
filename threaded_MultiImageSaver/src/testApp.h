#pragma once

#include "ofMain.h"
#include "ofxImageSequenceRecorder.h"

class testApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
        void exit();
    

	ofVideoGrabber cam;

	// Grabacion de la imagen entera
    ofxImageSequenceRecorder recorder;
	
	// Numero de zonas a grabar
	int nZonas;
	vector<ofRectangle> zonasSave;
	void	initZonasSave();
	// Grabadores
    vector<ofPtr <ofxImageSequenceRecorder> > recordersPtr;

	// Esto afecta a todos por igual
    bool recording;
	
	float periodoSaveFrame;
	float tLastSave;
	
	// Para activar o desactivar la grabacion mediante thread
	void toggleThreads();
	void activaThreads();
	void desactivaThreads();
};
