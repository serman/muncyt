/*
 *  particulas.h
 *  escena_basica
 *
 *  Created by guillermo casado on 02/05/14.
 *  Copyright 2014 __peripecio.com__. All rights reserved.
 *
 */

#ifndef PARTICULAS
#define PARTICULAS

#include "ofxBox2d.h"

class Particula : public ofxBox2dCircle {
	public:
	
	float radio;
	float q;
	float energia;
	float energExcitado;
	bool  swExcitado;
	ofTexture tex;
	ofColor	color;
	ofColor	colorExcitado;
	int tipo;
	
	float scale;
	
	Particula();
	
	void setCarga(float _q);
	void setTexture(ofTexture & _tex);
	void setColor(ofColor _color);
	void setColorExcitado(ofColor _color);
	void setExcitado(bool _swEx);
	void setTipo(int _tipo);

	bool isExcitado() {return this->swExcitado;}

	void draw();
	
};

#endif
