/*
 *  testApp_SolMgr.cpp
 *  Gravedad_1
 *
 *  Created by guillermo casado on 25/09/14.
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#include "testApp.h"

//ofSpherePrimitive sol;
//float		rSol;
//ofVec2f		posSol;
//float		masaSol;
//void		addMasaSol(float m);

void	testApp::initSol(float m) {
	densSol = 10.5;
	masaSol = m;
	setRfromM();
}

void	testApp::addMasaSol(float m) {
	masaSol += m;
	setRfromM();
}

void	testApp::setRfromM() {
	rSol = sqrt(masaSol/densSol);
	sol.setRadius(rSol);	
}

void	testApp::setMfromR() {
	masaSol = densSol*rSol*rSol;
}

void testApp::drawSol() {
	ofPushStyle();
	ofSetColor(255,255,0,  100+120*ofNoise(ofGetElapsedTimef() / 10.0) );
	sol.setPosition(0,0,0);
	ofPushMatrix();
	ofRotateX(90);
	sol.draw();
	ofPopMatrix();
	ofPopStyle();
}

