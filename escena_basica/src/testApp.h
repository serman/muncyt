#pragma once
#include "ofMain.h"
#include "ofxBox2d.h"

#include "particulas.h"

// -------------------------------------------------

#define COLOR_VERDE 0x5dca31
const int tipoNucleo  = 0;
const int tipoNeutron = 1;


#define N_SOUNDS 6

// 

class testApp : public ofBaseApp {
	
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
	void resized(int w, int h);
	
    
	ofxBox2d                            box2d;			  //	the box2d world

	// Borde circular
	vector <ofPolyline>                 lines;
	vector <ofPtr<ofxBox2dEdge> >       edges;

	ofxBox2dEdge			borde;
	ofPolyline              bordeLine;
	
//	vector    <ofPtr<ofxBox2dCircle> >	circles;  //	default box2d circles
	vector    <ofPtr<Particula> >	nucleos;	//	
	vector    <ofPtr<Particula> >	neutrones;  //	
	vector	  <ofPtr<ofxBox2dRect> >	boxes;	//	defalut box2d rects

	void addNucleo(int xx, int yy);
	void addNucleo(int xx, int yy, float r);
	void addBox(int xx, int yy);

	void addNeutron(int xx, int yy);

	// Fuerza
	float		fuerzaVal;
	float		fuerzaAng;
	float		fuerzaWAng;
	ofPoint		fuerza;
	
	bool		swFuerza;
	void		drawFuerza(ofPoint p, ofPoint gravity);
	void		drawFuerzaSelector();
	
	// TUT en https://www.iforce2d.net/b2dtut/fixtures
	
	// Textura de particulas
	ofTexture	texPartic;
	
	// Color Radioactivo
	// #5dca31 = RGB(93,202,49)  = HSB(103,76,79)
	// Ver mas en: http://rgb.to/hex/5dca31
	ofColor colorVerde;
	
	float rNucleo;
	float rNeutron;
	
	// when the ball hits we play this sound
	ofSoundPlayer  sounds[N_SOUNDS];
	void cargaSounds();
	
};

