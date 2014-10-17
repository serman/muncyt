/*
 *  Centro.h
 *  Nuclear_Fuerte_1
 *
 *  Created by guillermo casado on 04/09/14.
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 
 Centro sirve para:
 - detectar qué Emisores estan lanzando partículas 
 - obtener la direccion neta resultado del choque de los emisores
 
 - Interesante cuando la neta = 0 
 - Hay que tener en cuenta que la energia de cada rayo sea lo suficientemente alta (distancia al centro)
 
*/

#include "ParticleS.h"

class Centro {
public:
	ofVec2f pos;
	float radio;
	
	ofPolyline	circulo;
	ofPath		circuloPath;	// Para fill
	
	bool	bOcupado;
	ofVec2f dirNeta;	
	
	bool	bVariosEmitters;
	int		contadorPartics;
	vector	<int> idsEmitters;
	vector  <ParticleS> particsInside;
	
	ofVec2f	pTot, pTot_prev;	// momento total por frame = SUMA masa*veloc
	ofVec2f pMean, pMean_prev; // momento total / contadorPartics
	float	energy, energy_prev;	// energia por frame = SUMA masa*veloc^2

	int nMuestrasT;	// numero de muestras para calcular el momento y energia medios
	vector<ofVec2f>	pMeans;
	vector<float>	energies;
	ofVec2f pMeanT;	// media temporal del momento total
	float	energyMeanT;
	
	
	Centro() {
		dirNeta = ofVec2f(0,0);
		set(ofVec2f(0,0), 30);
		bOcupado = false;
		
		nMuestrasT = 30;
	}
	
	void set(ofVec2f _pos, float r) {
		pos = _pos;
		radio = r;
		
		circulo.clear();
		circulo.arc(pos, radio, radio, 0, 360, true, 60);
		circulo.close();
		
		circuloPath.clear();
		circuloPath.arc(pos, radio, radio, 0, 360);
		circuloPath.close();
	}

	void reset() {
		pTot = ofVec2f(0,0);
		energy = 0.0;
		
		bVariosEmitters = false;
		
		contadorPartics = 0;		
		particsInside.clear();
	}
	
	void updateEnd() {
		// Para llamar al final del update
		
		pMeans.push_back(pTot);
		energies.push_back(energy);
		if(pMeans.size()>nMuestrasT) {
			pMeans.erase(pMeans.begin(), pMeans.begin()+1);
			energies.erase(energies.begin(), energies.begin()+1);
		}
		
		pMeanT = ofVec2f(0,0);
		energyMeanT = 0;
		for(int i=0; i<pMeans.size(); i++) {
			pMeanT+=pMeans[i];
			energyMeanT+=energies[i];
		}
		pMeanT=pMeanT/pMeans.size();
		energyMeanT/=energies.size();
		
		
		
		// emitir particulas
		// segun el angulo, el pMeanT y la energyMeanT
		vector<ParticleData> newPartics;
		
		// partics a crear
		int nPartics = 1;
		
		// Si p < pMin:		
		// Si la energia es peque, emitir partics lentas en dir media
		// Si la energia es grande, emitir partics de todo tipo en todas direcciones
		// para energias medias, distribucion experimental: 
		// hay un minEnerg y maxEnerg: funcion sin(angulo) para asignar direccion de salida (o similar)

		// generar PDatas
		

//			float maxRnd = 2;
//			ofVec2f pos = posXY+ofVec2f(ofRandom(maxRnd)-maxRnd/2.0,ofRandom(maxRnd)-maxRnd/2.0);
//			pData.position = pos;
//			
//			// velocidad
//			// se da mas velocidad cuanto más lejos este del centro 
//			// (resize de pos bastaría)
//			float ddMax = 380;
//			float dd = pos.length();
//			pos.normalize();
//			pos*=-ofMap(dd,ddMax*0.25,ddMax, 10,18);
//			pData.velocity = pos;
//			
//			// tipo va a depender de la distancia o del angulo
//			//		pData.tpPartic = floor(ofRandom(3));
//			// tipos 0, 1, 2, 3, 4, 5
//			pData.tpPartic = floor(ofClamp(dd/(ddMax*0.75)*6,0,5)); 
//			
//			
//			if(false) {
//				// - - - devolver solo con pos, veloc y tipo - - - 
//				
//				// Segun el tipo asinar color
//				//		pData.nColor = floor(ofRandom(6));
//				pData.nColor = floor(dd/ddMax*6);
//				
//				
//				// que dependa del tipo!
//				pData.m = 1+ofRandom(5);
//				pData.q = floor(ofRandom(5))-2;
//			}
			
		
		
		
		
		
		
		
		
		
		
		
		
		
		
	}
	
	
	
	
	
	void addParticle(ParticleS _p) {
		// mete particulas dentro:
		//		suma momentos de las particulas
		//		y energía
		// analiza si vienen de varios emitters
		if(!bVariosEmitters) {
			// chequea solo si aun no se han detectado varios emisores
			for(int i=0; i<particsInside.size(); i++) {
				if(_p.idEmitter != particsInside[i].idEmitter) {
					bVariosEmitters = true;
					break;
				}
			}
		}
		particsInside.push_back(_p);
		
		pTot += _p.getMomentoP(); // m*_p.velocity;
		energy += _p.getEnergyK();
		
		contadorPartics++;
		
		pMean = pTot/contadorPartics;
		
	}
	
	
	void draw() {
		if(contadorPartics>0) bOcupado=true;
		
		ofPushStyle();
		ofEnableSmoothing();
		if(bOcupado) {
			float alpha = ofMap(contadorPartics, 0, 8, 35, 255);
			alpha = ofWrap(alpha,0,255);
			circuloPath.setFillColor(ofColor(255,224,23, alpha));
			circuloPath.setFilled(true);
//			ofSetColor(255,224,23, ofMap(contadorPartics, 0, 6, 35, 255));
			circuloPath.draw();
		}
//		ofSetColor(200);
//		ofNoFill();
//		ofSetLineWidth(6);
//		circulo.draw();
		
		ofSetColor(0,255,0, 70);
		ofSetLineWidth(4);
		float modPTot = pTot.length();
		float angPTot = atan2(pTot.y, pTot.x);
		if(modPTot>30) {
//			ofLine(pos.x, pos.y, pos.x+50*cos(angPTot), pos.y+50*sin(angPTot));
			ofLine(pos.x, pos.y, pos.x+pTot.x, pos.y+pTot.y);
		}
		
		ofSetColor(255,255,0, 70);
		float modPMeanT = pMeanT.length();
		float angPMeanT = atan2(pMeanT.y, pMeanT.x);
		ofLine(pos.x, pos.y, pos.x+pMeanT.x, pos.y+pMeanT.y);
		
		
		ofDrawBitmapString("pTot: " + ofToString(modPTot), pos.x+50, pos.y+50);
		ofDrawBitmapString("E: " + ofToString(energy), pos.x+50, pos.y+70);
		
		ofDisableSmoothing();
		ofPopStyle();
		
		bOcupado = false;
	}
	
};