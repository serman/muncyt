/*
 *  ParticleTipos.h
 *  Nuclear_Fuerte_1
 *
 *  Created by guillermo casado on 10/10/14.
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef PART_TIPOS
#define PART_TIPOS

struct ParticleData {
    ofVec3f position;
    ofVec3f velocity;
	
	float	m;
	float	q;
    ofColor color;
    int		nColor;
	int		tpPartic;
	
	float amortV;
	float tLife;
//	int	  mode_MESH;	// Si pongo esto peta el programa !!!
	
	void setTipoPart(int tipo) {
		//	// devuelve un ParticleData para un tipo de particula.
		//	// No da valor a posicion ni velocidad
		//
		tpPartic = tipo;
		nColor = tipo;
		if(tipo==0) {
			m = 1.0;
			q = 0;
			amortV = 1.0;
			tLife = 2.0;
			//			mode_MESH = GL_LINE_STRIP;
		}
		else if(tipo==1) {
			m = 1.5;
			q = 0;
			amortV = 2.0;
			tLife = 4.0;
			//			mode_MESH = GL_LINES;
		}
		else if(tipo==2) {
			m = 1.5;
			q = -1.0;
			amortV = 5.0;
			tLife = 1.2;
			//			mode_MESH = GL_LINE_STRIP;
		}
		else if(tipo==3) {
			m = 2.0;
			q = -1.5;
			amortV = 2.0;
			tLife = 2.0;
			//			mode_MESH = GL_LINE_STRIP;
		}
		else if(tipo==4) {
			m = 2.2;
			q = 2.0;
			amortV = 3.0;
			tLife = 2.0;
			//			mode_MESH = GL_LINE_STRIP;
		}
		else if(tipo==5) {
			m = 3.0;
			q = -1.8;
			amortV = 4.0;
			tLife = 1.0;
			//			mode_MESH = GL_LINE_STRIP;
		}
		tLife*=5;		
		
	}
	
//	ParticleData() {
//		
//		
//	}
};

#endif