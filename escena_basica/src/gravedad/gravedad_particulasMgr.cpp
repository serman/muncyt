/*
 *  gravedad_particulasMgr.cpp
 *  tests_Light_Grav
 *
 *  Created by guillermo casado on 24/09/14.
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#include "gravedad.h"

void gravedad::addParticleOut() {
	
	//	float rnd = ofRandom(1.0);
	//	if(rnd<0.15) {
	if(ofGetFrameNum()%ratePartic==0) {
		
		float d0 = W_WIDTH/2;// * (1+ofRandom(0.2));
		float ang0 = ofRandom(TWO_PI);
		float dir0 = ofDegToRad(ofRandom(90-20,90+20));
		float v = 5.0;
		ofColor cc = ofColor::indigo;
		
//		ofVec3f p0 = ofVec3f(W_WIDTH,W_HEIGHT/15.0,0);
		ofVec3f p0 = ofVec3f(d0*cos(ang0),d0*sin(ang0),0);
		p0 += zentro;
		dir0+=ang0;
		ofVec3f v0 = ofVec3f(v*cos(dir0),v*sin(dir0),0);
//		v0.rotateRad(0,0,ang0);
		
		float masa = ofRandom(3);
		
		ParticleX p = ParticleX(p0, v0, cc, masa );
		particulas.push_back(p);
		
		// pos, vel, color, mass, charge
		//		ParticleX pTmp = ParticleX(ofVec3f(0,ofRandom(ofGetHeight())) , ofVec3f(ofRandom(10.0, 30.0), 0) , ofColor::white, 1.0, 0.0);
		cheapComm::getInstance()->sendAudio1("/audio/gravity/new_particle_event",ofMap(ang0,0,TWO_PI,0,1));
        
	}
	
}

void gravedad::updateParticlesX() {
	// add alguna particula desde un lateral
	addParticleOut();
	
	// fuerza SOL
	for(int i=0; i<particulas.size(); i++) {
//		gravityTowards(ofVec3f& a_target,float a_minDist, float masaAttrac)
		ofVec3f pSol = ofVec3f(W_WIDTH/2.0, W_HEIGHT/2.0,0);
		particulas[i].gravityTowards(pSol, 10.0f,  masaSol);
        	}
	
	
	// fuerzas TUIO
    for (int j=0; j<hands.objectsCol.size(); j++){
        ofVec3f pTUIO(hands.objectsCol[j]->x,hands.objectsCol[j]->y);
        for(int i=0; i<particulas.size(); i++) {
            //ofVec3f pTUIO = ofVec3f(ofGetMouseX()-ofGetWidth()/2, -(ofGetMouseY()-ofGetHeight()/2), 0);
            pTUIO+=ofVec3f(W_WIDTH/2.0, W_HEIGHT/2.0 ,0);
            particulas[i].gravityTowards(pTUIO, 10.0f,  masaTUIO);
        }
    }
    //esto hay que hacerlo para cada TUIO

	
    
	
	// update
	for(int i=0; i<particulas.size(); i++) {
		particulas[i].update();
	}
	
	// chequear si han chocado con el Sol
	for(int i=particulas.size()-1; i>=0; i--) {
		ofVec3f pSol = ofVec3f(W_WIDTH/2.0, W_HEIGHT/2.0,0);
		ofVec2f zDist = ofVec2f(particulas[i].position.x, particulas[i].position.y);
		zDist-=	pSol;
		if( zDist.lengthSquared() < (rSol*rSol) ) {
			// CHOQUE!!!
			// sumar masa a Sol
			addMasaSol(600);
			
			// eliminar particula
			particulas.erase(particulas.begin()+i);
			
			cheapComm::getInstance()->sendAudio0("/audio/gravity/sun_collision_event");
			
			
		}
		else if(zDist.lengthSquared() > (4*W_WIDTH*4*W_WIDTH)) {
			particulas.erase(particulas.begin()+i);			
		}

	}
	
	
}


void gravedad::drawParticlesX() {
	// draw
	for(int i=0; i<particulas.size(); i++) {
		particulas[i].draw();
		
	}
}
