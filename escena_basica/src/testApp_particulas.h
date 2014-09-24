/*
 *  testApp_particulas.h
 *  escena_basica
 *
 *  Created by guillermo casado on 24/09/14.
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */





void testApp::addParticleLateral() {
	
	
	//	float rnd = ofRandom(1.0);
	//	if(rnd<0.15) {
	if(ofGetFrameNum()%ratePartic==0) {
		//		ParticleX( ofVec3f _position , ofColor _color, float mass = 1.0, float charge = 0.0 )
		//		ParticleX( ofVec3f _position , ofVec3f _vel , ofColor _color, float mass = 1.0, float charge = 0.0 )
		float v = ofMap(mouseX,0,ofGetHeight(), 4, 10);
		ParticleX p = ParticleX(ofVec3f(0,ofRandom(ofGetHeight()),0), ofVec3f(v,0,0), ofColor(255,255,255) );
		particulas.push_back(p);
		
		
		// pos, vel, color, mass, charge
		//		ParticleX pTmp = ParticleX(ofVec3f(0,ofRandom(ofGetHeight())) , ofVec3f(ofRandom(10.0, 30.0), 0) , ofColor::white, 1.0, 0.0);
		
	}
	
	
}
