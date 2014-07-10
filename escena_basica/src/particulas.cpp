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
	colorExcitado = ofColor(193,242,149);
	escale = 3.0;
	
	// por defecto utilizamos modo de dibujo: MODO_PARTIC
	modoDraw = MODO_PARTIC;
	
	tLifeMax = floor(ofRandom(120,200));
}

void Particula::setCarga(float _q) {
	q = _q;
}

void Particula::setColor(ofColor _color) {
	color = _color;
}

void Particula::setColorExcitado(ofColor _color) {
	colorExcitado = _color;
}

void Particula::setTexture(ofTexture & _tex) {
	tex = _tex;
}
void Particula::setEscala(float _escale) {
	escale = _escale;
}

void Particula::setExcitado(bool _swEx) {
	swExcitado = _swEx;
}

void Particula::setTipo(int _tipo) {
	tipo = _tipo;
}

void Particula::setModoDraw(int _modo) {
	modoDraw = _modo;
}

void Particula::update() {
	pos = this->getPosition();
	vel = this->getVelocity();
	// update values de pos, veloc, radio...
	if(modoDraw==MODO_PARTIC) {
		trail.addVertex(pos);
	}
	else if(modoDraw==MODO_PATH) {
		trail.addVertex(pos);
	}
	else if(modoDraw==MODO_BOX2D) {
	}
	
	if(trail.size()>tLifeMax) 		trail.clear();
//	float rnd = ofRandom(1);
//	if(trail.size()>100 && rnd<0.1) {
//		trail.clear();
//	}
}

void Particula::draw() {
	ofSetColor(100,0,0);	// cambia bastante con esto
	
	if(modoDraw==MODO_PARTIC) {
//		ofLogVerbose("Partic modoDraw PARTIC: " + ofToString(modoDraw));
		
		ofPushStyle();
		ofEnableAlphaBlending();
	//	ofSetColor(0,100,100, 100);
	//	ofLine(this->getPosition().x, this->getPosition().y,W_WIDTH/2.0, ofGetHeight()/2.0);
		ofPushMatrix();
			ofTranslate(this->getPosition().x, this->getPosition().y);

	//		ofSetColor(255,0,255, 60);		
	//		ofLine(-W_WIDTH,0,W_WIDTH,0);
	//		ofLine(0,-ofGetHeight(),0,ofGetHeight());
		
			this->radio = this->getRadius(); 
			ofPushMatrix();
		
				if(this->swExcitado) 			ofSetColor(colorExcitado, 255);
				else							ofSetColor(color, 100);
		
				ofScale(escale,escale, 0);
				tex.draw(-radio,-radio,2*radio,2*radio);
		
//				if(this->swExcitado) {
//					ofSetColor(255,0,0);
//					ofLine(-20,0,20,0);
//				}
//				else {
//					ofSetColor(255,0,0);
//					ofLine(-5,-5,-5,5);					
//					ofLine(5,-5,5,5);					
//				}
				
					
			ofPopMatrix();
		ofPopMatrix();
		

		ofPopStyle();
	}
	else if(modoDraw==MODO_PATH) {
//		ofLogVerbose("Partic modoDraw PATH: " + ofToString(modoDraw));

		drawTrail();
		
	}
	else if(modoDraw==MODO_BOX2D) {
//		ofLogVerbose("Partic modoDraw BOX2D: " + ofToString(modoDraw));
		ofxBox2dCircle::draw();
	}
	
}

void Particula::drawTrail() {
	// dibujar traza
	ofSetColor(color);
	trail.draw();
	
}
