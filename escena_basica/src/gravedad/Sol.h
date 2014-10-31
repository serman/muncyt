/*
 *  Sol.h
 *  Gravedad_1
 *
 *  Created by guillermo casado on 28/10/14.
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _SOL
#define _SOL

#include "ofMain.h"

class Sol {
public:
	
	ofSpherePrimitive esfera;
	float		radio;
	ofVec3f		pos3D;
	ofVec2f		posScreen;
	float		masa;
	float		densidad;
	
	float masa_Tgt;
	float easing;
	
	Sol() {
		easing = 0.1;
		densidad = 5.0;//10.5;
	}

	void	init(float m, ofVec3f _pos3D, ofVec2f _posScreen) {
		pos3D = _pos3D;
		posScreen = _posScreen;
		masa = m;
		masa_Tgt = m;
		setRfromM();
	}
	
	void	addMasa(float m) {
		masa_Tgt += m;
//		masa += m;
//		setRfromM();
	}
	
	void setRadio(float r) {
		radio = r;
		esfera.setRadius(radio);
		setMfromR();
	}
	
	void	setRfromM() {
		radio = sqrt(masa/densidad);
		esfera.setRadius(radio);	
	}
	
	void	setMfromR() {
		masa = densidad*radio*radio;
	}
	
	void update() {
		if(masa!=masa_Tgt) {
			if(abs(masa_Tgt-masa)>10) {
				masa += easing*(masa_Tgt-masa);
			}
			else masa = masa_Tgt;

			setRfromM();
		}
		
	}
	
	void	draw(){
		ofPushStyle();
		ofSetColor(255,255,0,  100+120*ofNoise(ofGetElapsedTimef() / 10.0) );
		esfera.setPosition(pos3D.x,pos3D.y,pos3D.z);
		ofPushMatrix();
		ofRotateX(90);
		ofTranslate(0,-100,0);
		esfera.draw();
		ofPopMatrix();
		ofPopStyle();
	}
	
	
	
	
	
};

#endif
