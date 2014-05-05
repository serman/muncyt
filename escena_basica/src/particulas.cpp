/*
 *  particulas.cpp
 *  escena_basica
 *
 *  Created by guillermo casado on 02/05/14.
 *  Copyright 2014 __peripecio.com__. All rights reserved.
 *
 */

#include "particulas.h"

Particula::Particula() {
	ofxBox2dCircle::ofxBox2dCircle();
	color = ofColor(93,202,49);
	scale = 3.0;
}

void Particula::setCarga(float _q) {
	q = _q;
}

void Particula::setColor(ofColor _color) {
	color = _color;
}

void Particula::setTexture(ofTexture & _tex) {
	tex = _tex;
}

void Particula::setExcitado(bool _swEx) {
	swExcitado = _swEx;
}

void Particula::draw() {
	ofxBox2dCircle::draw();
	
	ofPushStyle();
	ofSetColor(0,100,100, 100);
//	ofLine(this->getPosition().x, this->getPosition().y,ofGetWidth()/2.0, ofGetHeight()/2.0);
	ofPushMatrix();
		ofTranslate(this->getPosition().x, this->getPosition().y);

//		ofSetColor(255,0,255, 60);
//		ofLine(-30,0,30,0);
//		ofLine(0,-30,0,30);
	
//		ofLine(-ofGetWidth(),0,ofGetWidth(),0);
//		ofLine(0,-ofGetHeight(),0,ofGetHeight());
	
		this->radio = this->getRadius(); 
		ofPushMatrix();
	
//			ofSetColor(93,202,49, 250);
			float alfa = (this->swExcitado)? 255 : 100;
			ofSetColor(color, alfa);
	
			ofScale(scale,scale, 0);
			tex.draw(-radio,-radio,2*radio,2*radio);
		ofPopMatrix();
	ofPopMatrix();
	ofPopStyle();
	
	
}
