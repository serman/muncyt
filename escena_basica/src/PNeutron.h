/*
 *  PNeutron.h
 *  escena_basica
 *
 *  Created by guillermo casado on 08/07/14.
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef PNEUTRON
#define PNEUTRON

#include "particulas.h"

class PNeutron : public Particula {
public:

	PNeutron(b2World * world2d, ofPoint _pos) {
		Particula();
		float dens  = 50.5;
		float bounc = 0.53;
		float fricc = 0.1;
		setPhysics(dens, bounc, fricc);	//setPhysics(float density, float bounce, float friction);
		
		radio = 2;
		masa  = 1;
		setup(world2d, _pos.x, _pos.y, radio);		// pos, rr
		
//		masa;		// masa
		q = 0;
		car1 = 0.5;
		car2 = 10.0;
		nivel1 = 0;
		
		tipo = tipoNeutron;	
		
/*

 nucleos.push_back(ofPtr<Particula>(new Particula));
 nucleos.back().get()->setPhysics(50.5, 0.53, 0.1); //setPhysics(float density, float bounce, float friction);
 nucleos.back().get()->setup(box2d.getWorld(), xx, yy, r);		// pos, rr
 nucleos.back().get()->setRotation(ofRandom(360));
 
 nucleos.back().get()->setTexture(texPartic);
 
 // test de Exitado o no
 nucleos.back().get()->setExcitado(true);
 //	if(ofRandom(1)<0.5)	nucleos.back().get()->setExcitado(true);
 //	else				nucleos.back().get()->setExcitado(false);
 
 
 nucleos.back().get()->setEscala(4.0);
 nucleos.back().get()->setTipo(tipoNucleo);
 
 nucleos.back().get()->setColorExcitado(ofColor::fromHsb(60,250,250));
 nucleos.back().get()->setColor(ofColor::fromHsb(60,250,100));
 
 nucleos.back().get()->setModoDraw(modoDrawParticulas);
 // * * * * * * *
 // A corregir:
 // http://forum.openframeworks.cc/t/box2d-contact-listening-and-userdata/3441/3
 //
 nucleos.back().get()->setData(nucleos.back().get());	
 // * * * * * * *
 
 
*/
		
	}
	
	// choques
	
	
protected:
	
	
private:
	

};

#endif