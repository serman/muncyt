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
	
	tLifeMax = floor(ofRandom(80,150));
}

void Particula::setCarga(float _q) {
	q = _q;
}

void Particula::setCaract1(float _c) {
	car1 = _c;
}
void Particula::setCaract2(float _c) {
	car2 = _c;
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
		updateTrail(pos);
	}
	else if(modoDraw==MODO_PATH) {
		updateTrail(pos);
	}
	else if(modoDraw==MODO_BOX2D) {
	}
	
}

void Particula::updateTrail(ofPoint _pos) {
//	ofLogNotice("update pos: " + ofToString(trailV.size())+"/"+ofToString(tLifeMax));
	trailV.push_back(pos);
	if(trailV.size()>tLifeMax) {
//		float rnd = ofRandom(1);
//		if(rnd<0.4) 
			trailV.erase(trailV.begin());//erase(0);
	}
	trail.clear();
	for(int i=0; i<trailV.size(); i++) {
		trail.addVertex(trailV[i]);
	}
		//	
		//	if(trail.size()>100 && ) {
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
				float radioT = radio+ofRandom(2);
				if(this->swExcitado) 			ofSetColor(colorExcitado, 255);
				else							ofSetColor(color, 100);
		
				ofScale(escale,escale, 0);
				tex.draw(-radioT,-radioT,2*radioT,2*radioT);
		
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
