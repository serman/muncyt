/*
 *  testApp_Collisions.cpp
 *  escena_basica
 *
 *  Created by guillermo casado on 17/05/14.
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#include "nuclear_debil.h"

//--------------------------------------------------------------
void nuclear_debil::contactStart(ofxBox2dContactArgs &e) {
	if(e.a != NULL && e.b != NULL) { 
		// Si chocan dos objetos Circle (dos particulas) suena un tipo de sonido
		// Se puede hacer dependiente de 
		if(e.a->GetType() == b2Shape::e_circle && e.b->GetType() == b2Shape::e_circle) {
			//			sounds[0].play();			
		}
		else {
			// Si el choque no es entre dos particulas (circles), suena otra cosa.
			// Por ejemplo el choque con la pared de la cámara
			//			sounds[1].play();
			//			ofLogVerbose("Choque xx-xx");
		}
	}
}

//--------------------------------------------------------------
void nuclear_debil::contactEnd(ofxBox2dContactArgs &e) {
	if(e.a != NULL && e.b != NULL) { 
		// si chocan dos particulas
		if(e.a->GetType() == b2Shape::e_circle && e.b->GetType() == b2Shape::e_circle) {
			Particula * pA = (Particula*)e.a->GetBody()->GetUserData();
			Particula * pB = (Particula*)e.b->GetBody()->GetUserData();
			
			// Aquí indica como recoger el punto de contacto en box2D:
			// https://www.iforce2d.net/b2dtut/collision-anatomy
			//
			
			if((pA->tipo == tipoNeutron && pB->tipo == tipoNucleo) ||
			   (pA->tipo == tipoNucleo  && pB->tipo == tipoNeutron)) {
				
				//ofLogVerbose("Fin choque. pa-pb: " + ofToString(pA->tipo) + "-" + ofToString(pB->tipo));
				// add un destello en el punto de colision
				
				ofVec2f ptChoque = (pA->getPosition() + pB->getPosition())/2.0;
				
				if(pA->tipo == tipoNucleo) {
					colisiona_N_n(pA, pB);
				} 
				else {
					colisiona_N_n(pB, pA);
				}
			}
			else if((pA->tipo == tipoNeutron || pB->tipo == tipoNeutron)) {
				// neutron choca contra otra cosa
				ofLogVerbose("CHOQUE >>>>>>>>>>>>> n <-> algo <<<<<<<<<<<<<<");
				sounds[1].play();
				if(pA->tipo == tipoNeutron) {
					
				}
				else {
					
				}
			}
			
		}
		
	}
}

void nuclear_debil::colisiona_N_n (Particula * pN, Particula * pn) {
	if(pN->swExcitado) {
		ofVec2f ptChoque = (pN->getPosition() + pn->getPosition())/2.0;
		
		// sonido
		sounds[0].play();
		
		// desexcitar el nucleo
		pN->setExcitado(false);
		
		addDestello(ptChoque.x, ptChoque.y);
		
		// add un nuevo neutron, y tal vez duplicar el nucleo
		// Calcular el punto para que no choque con el nucleo
		ofVec2f velNucleo = pN->getVelocity();
		ofVec2f posNucleo = pN->getPosition();
		ofVec2f velNeutron = pn->getVelocity();
		ofVec2f posNeutron = pn->getPosition();
		
		ofVec2f posNeutron2 = pn->getPosition();
		ofVec2f velNeutron2 = pn->getVelocity();
		velNeutron2.rotate( -2.0 * velNeutron2.angleRad(velNucleo) );
		
		ofVec2f distPBA = posNeutron - posNucleo;
		float angPBA_velNucleo = distPBA.angleRad(velNucleo);
		distPBA.rotate(-2*angPBA_velNucleo);
		
		posNeutron2 = posNucleo + distPBA;
		
		// Da error al crear el objeto
		// porque esta en proceso de choque ==> world's blocked
		// Hay que añadirlo a una lista de creacion de neutrones
		//						addNeutron(posNeutron2.x, posNeutron2.y, 
		//								   velocNeutronLanz*0.95, 
		//								   atan2(dirNeutron2.y, dirNeutron2.x));
		CineticData cd = CineticData(posNeutron2.x, 
									 posNeutron2.y,
									 velNeutron2.length(),
									 atan2(velNeutron2.y, velNeutron2.x),
									 tipoNeutron);
		nuevasPartics.push_back(cd);
		ofLogVerbose("add CineticData por Nueva colision +++++++++++++++++++++++++++++ N n ++++");
		
		addDestello(posNeutron2.x, posNeutron2.y);
		
	}
}

