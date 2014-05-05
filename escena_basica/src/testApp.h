#pragma once
#include "ofMain.h"
#include "ofxBox2d.h"

#include "particulas.h"

// -------------------------------------------------

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
	
//	vector    <ofPtr<ofxBox2dCircle> >	circles;		  //	default box2d circles
	vector    <ofPtr<Particula> >	circles;		  //	default box2d circles
	vector	  <ofPtr<ofxBox2dRect> >	boxes;			  //	defalut box2d rects
	
	void addCircle(int xx, int yy);
	void addCircle(int xx, int yy, float r);
	void addBox(int xx, int yy);
	
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
};

