/*
 *  gravedad_SolMgr.cpp
 *  Gravedad_1
 *
 *  Created by guillermo casado on 25/09/14.
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#include "gravedad.h"

//ofSpherePrimitive sol;
//float		rSol;
//ofVec2f		posSol;
//float		masaSol;
//void		addMasaSol(float m);

void	gravedad::initSol(float m) {
	densSol = 10.5;
	masaSol = m;
	setRfromM();
}

void	gravedad::addMasaSol(float m) {
	masaSol += m;
	setRfromM();
}

void	gravedad::setRfromM() {
	rSol = sqrt(masaSol/densSol);
	sole.setRadius(rSol);	
}

void	gravedad::setMfromR() {
	masaSol = densSol*rSol*rSol;
}

void gravedad::drawSol() {
	ofPushStyle();
	ofSetColor(255,255,0,  100+120*ofNoise(ofGetElapsedTimef() / 10.0) );
	sole.setPosition(0,0,0);
	ofPushMatrix();
	ofRotateX(90);
	sole.draw();
	ofPopMatrix();
	ofPopStyle();
}
