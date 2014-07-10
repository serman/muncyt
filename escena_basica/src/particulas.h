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

enum modo_Draw {
	MODO_BOX2D,
	MODO_PARTIC,
	MODO_PATH
};

enum tipoPart { tipoNucleo, tipoNeutron };


class Particula : public ofxBox2dCircle {
	public:
	
	ofPoint pos;
	ofPoint vel;
	float radio;
	
	float masa;		// masa
	float q;		// carga
	float car1;		// caract1
	float car2;		// caract2
	int nivel1;		// 1 o -1
	
	float energia;
	float energExcitado;
	bool  swExcitado;
	
	ofTexture tex;
	
	ofColor	color;
	ofColor	colorExcitado;
	int tipo;
	
	float escale;
	
	int modoDraw;
	
	Particula();
	
	void setCarga(float _q);
	void setCaract1(float _c);
	void setCaract2(float _c);
	
	void setTexture(ofTexture & _tex);
	
	void setEscala(float _escale);
	void setColor(ofColor _color);
	void setColorExcitado(ofColor _color);
	void setExcitado(bool _swEx);
	void setTipo(int _tipo);
	void setModoDraw(int _modo);

	bool isExcitado() {return this->swExcitado;}

	void update();
	void draw();
	
	bool		swAcumula;
	vector<ofPoint> trailV;
	ofPolyline	trail;
	
	int		tLifeMax;
	
	void updateTrail(ofPoint _pos);
	void drawTrail();	
};

#endif
