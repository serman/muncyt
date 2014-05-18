/*
 *  Efectos.h
 *  escena_basica
 *
 *  Created by guillermo casado on 06/05/14.
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef DESTELLO
#define DESTELLO

// No se porque pero tengo que poner esto sino no reconoce elementos de OF!
#include "ofMain.h"

class Destello {
	public:
	float radio, radioMax;

	ofPoint pos;

	float t;
	float t0;
	float tVida;	// en segundos (float)
	ofTexture tex;
	
	bool isStarted;
	bool isFinished;
	
	Destello() {}
	Destello(float px, float py, float _radioMax, float _tVida) {
		pos = ofPoint(px,py);
		radioMax = _radioMax;
		radio = 0;
		tVida = _tVida;
		t = 0.0;
		
		isStarted = false;
		isFinished = false;
	}
	
	void start() {
		t=0.0;
		t0 = ofGetElapsedTimef();
		isStarted = true;
		isFinished = false;
	}
	
	
	void update() {
		if(isStarted && !isFinished) {
			// update radio
			float dt = ofGetElapsedTimef()-t0;
			radio = ofMap(dt, 0, tVida, 0, radioMax);
			if(dt>tVida) {isFinished = true;}
		}
		
	}
	
	void draw() {
		// dibujar la textura o lo que sea
		if(isStarted && !isFinished) {
			ofPushStyle();
				ofNoFill();
				ofSetCircleResolution(120);
				ofSetColor(ofColor::fromHsb(0, 200, 200, 100));
				ofSetLineWidth(4);
				ofCircle(pos.x, pos.y, radio);
			ofPopStyle();
		}
	}

	void setTexture(ofTexture &_tex) { tex = _tex; }

};
				  
#endif