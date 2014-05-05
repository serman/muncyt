/*
 *  particulas.cpp
 *  escena_basica
 *
 *  Created by guillermo casado on 02/05/14.
 *  Copyright 2014 __peripecio.com__. All rights reserved.
 *
 */

#include "particulas.h"

void Particula::setCarga(float _q) {
	q = _q;
}

void Particula::setTexture(ofTexture & _tex) {
	tex = _tex;
}

void Particula::setExcitado(bool _swEx) {
	swExcitado = _swEx;
}

void Particula::draw() {
//	ofxBox2dCircle::draw();
	
	ofPushStyle();
	ofSetColor(0,100,100, 100);
//	ofLine(this->getPosition().x, this->getPosition().y,ofGetWidth()/2.0, ofGetHeight()/2.0);
	ofPushMatrix();
		ofTranslate(this->getPosition().x, this->getPosition().y);

		ofSetColor(255,0,255, 90);
//		ofLine(-30,0,30,0);
//		ofLine(0,-30,0,30);
		ofLine(-ofGetWidth(),0,ofGetWidth(),0);
		ofLine(0,-ofGetHeight(),0,ofGetHeight());
		float rtmp = this->getRadius();
		ofPushMatrix();
			ofScale(4.2, 4.2, 0);
			tex.draw(-rtmp,-rtmp,2*rtmp,2*rtmp);
		ofPopMatrix();
	ofPopMatrix();
	ofPopStyle();
	
	
}
