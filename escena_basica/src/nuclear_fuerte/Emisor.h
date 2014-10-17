/*
 *  Emisor.h
 *  nuclear_Fuerte
 *
 *  Created by guillermo casado on 03/09/14.
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _EMISOR
#define _EMISOR

#include "nuclear_fuerte.h"
//#include "ParticleTipos.h"

class Emisor {

public:
	int idEmisor;
	
	// posicion
	float rho;	// controlara la energia de las particulas emitidas
	float ang;	// radians
	ofVec2f posXY;
	
	// hacia donde van las particulas
	ofVec2f pointTo;
	ofVec2f dir;
	
	// Tipo Particula
	int		tpPartic;
	
	// Emision de particulas
	ParticleData	pData;
	int				ratePartic;

	// para dibujar
	float		lado;
	ofVboMesh	icono;
	ofColor		c;

	// activo
	bool	bActivo;
	
	Emisor() {
		setPos_Radial(0,0);
		
		tpPartic = 0;

		c = ofColor::red;

		ratePartic = 3;
		
		bActivo = true;
		
		// preparar forma a dibujar
		setupIcono();
	}
	
	Emisor(float _rho, float _ang, int _tpPartic = 0) {
		
		setPos_Radial(_rho, _ang);
		
		tpPartic = _tpPartic;

		c = ofColor::red;
		
		ratePartic = 3;

		bActivo = true;
		
		// preparar forma a dibujar
		setupIcono();
		
	}
	
	void setId(int id) {
		idEmisor = id;
	}
	
	void setColor(ofColor cc) {
		c = cc;
		setupIcono();
	}
	
	void setupIcono() {
		// También puede ser un png!
		
		lado = 30.0;
		float altura = lado*sin(PI/3.0);
		icono.clear();
		icono.addVertex(ofVec3f(-lado/2.0, -0.2*altura, 0.0));
		icono.addColor(ofColor::fromHsb(c.getHue(), c.getSaturation(), c.getBrightness()*0.2));
		icono.addVertex(ofVec3f(+lado/2, -0.2*altura, 0.0));
		icono.addColor(ofColor::fromHsb(c.getHue(), c.getSaturation(), c.getBrightness()*0.2));
		icono.addVertex(ofVec3f(0,0.8*altura,0.0));
		icono.addColor(c);
	}
	
	void setPos_Radial(float _rho, float _ang) {
		rho = _rho;
		ang = _ang;	
		calcPosXY();
	}
	void setPos_XY(float _x, float _y) {
		posXY = ofVec2f(_x, _y);
		
		rho = posXY.length();
		ang = atan2(posXY.y, posXY.x);	// -PI, PI
//		ofLogNotice(ofToString(ang));
	}
	
	ofVec2f calcPosXY() {
		posXY = ofVec2f(rho*cos(ang), rho*sin(ang));
		return posXY;
	}	
	
	void draw() {

		ofPushStyle();
		// linea de union
		ofSetColor(c);
//		ofLine(0,0, posXY.x, posXY.y);
		
		// dibuja el icono
		ofPushMatrix();
		// translate
		ofTranslate(posXY.x, posXY.y);
		// id
//		ofSetColor(200);
//		ofDrawBitmapString(ofToString(idEmisor), 10,10);
		// rotate
		ofRotateZ(90+RAD_TO_DEG*(ang));
		
		// draw
		icono.draw();
		ofPopMatrix();
		ofPopStyle();
		
	}
	
	// Da valores de particulas que emite
	ParticleData getParticleData() {
		float maxRnd = 2;
		ofVec2f pos = posXY+ofVec2f(ofRandom(maxRnd)-maxRnd/2.0,ofRandom(maxRnd)-maxRnd/2.0);
		pData.position = pos;
		
		// velocidad
		// se da mas velocidad cuanto más lejos este del centro 
		// (resize de pos bastaría)
		float ddMax = 384;
		float ddMin = 50.0;	// radio de la zona central
		float dd = pos.length();
		pos.normalize();	
		pos*=-ofMap(dd,ddMin,ddMax, 10,18);
		pData.velocity = pos;

		int nRadio = floor(ofMap(dd, ddMin, ddMax, 0,3));
		nRadio = ofClamp(nRadio, 0,3);
		
		// dependendo del sector en el que se esta, y de la distancia se crea una u otra particula
		float angulo = (ang<0)? ang+TWO_PI : ang;
		int nSector = floor(angulo/ofDegToRad(30.0));	// 12 sectores

		int nnTipo = (nSector+nRadio)%6;
		
		// tipo va a depender de la distancia o del angulo
		//		pData.tpPartic = floor(ofRandom(3));
		// tipos 0, 1, 2, 3, 4, 5
//		pData.tpPartic = floor(ofClamp(dd/(ddMax*0.75)*6,0,5)); 
		pData.tpPartic = nnTipo; 

		
		if(false) {
		// - - - devolver solo con pos, veloc y tipo - - - 

			// Segun el tipo asinar color
	//		pData.nColor = floor(ofRandom(6));
			pData.nColor = floor(dd/ddMax*6);
			
					
			// que dependa del tipo!
			pData.m = 1+ofRandom(5);
			pData.q = floor(ofRandom(5))-2;
		}
		
		return pData;
		
	}
	

	
	
	
};

#endif
