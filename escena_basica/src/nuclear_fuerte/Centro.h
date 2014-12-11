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
	
	vector<int> particsCreadas;
	
	int sumaTipos;
	
	vector<ParticleData> newPartics;
	
	
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
		
		newPartics.clear();
		
		sumaTipos = 0;
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
		contadorPartics++;		
		
		pTot += _p.getMomentoP(); // m*_p.velocity;
		energy += _p.getEnergyK();
		
		sumaTipos += _p.tipoPart;
		
	}
	
	bool updateEnd(int rate) {
		// Para llamar al final del update

		bool bNuevasPart = false;
		
		pMean = pTot/contadorPartics;
		
		
		pMeans.push_back(pTot);
		energies.push_back(energy);
		if(pMeans.size()>nMuestrasT) {
			pMeans.erase(pMeans.begin(), pMeans.begin()+1);
			energies.erase(energies.begin(), energies.begin()+1);
			particsCreadas.erase(particsCreadas.begin(), particsCreadas.begin()+1);
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
		newPartics.clear();
		
		// partics a crear
		// Dependerá de si hay particulas y la energia acumulada
		// y debe seguir el rate de creación
		int nPartics = 0;
		float pMin = 20.0;
		bool bRadial = false;
		if(ofGetFrameNum()%rate == 0 && contadorPartics>1) {
			nPartics += floor((float)energyMeanT/100)*2;
			
			// Si ademas el pTot es bajo y la energ alta (>600), generar muchas más partículas
			// y distribuirlas radialmente
			if(energyMeanT>300 && pMeanT.length()<10) {
				nPartics += 120;
				bRadial = true;
			}
			
			//			ofLogNotice() << "ENERGY: " << energy << "    nPartics: " << nPartics;
			
		}
		
		// Dispersión angular: depende del momento neto total y de la energia acumulada
		
		
		// Direccion principal: la de pMean
		
		
		// Si p < pMin:		
		// Si la energia es peque, emitir partics lentas en dir media
		// Si la energia es grande, emitir partics de todo tipo en todas direcciones
		// para energias medias, distribucion experimental: 
		// hay un minEnerg y maxEnerg: funcion sin(angulo) para asignar direccion de salida (o similar)
		
		
		for(int i=0; i<nPartics; i++) {
			
			ParticleData pData;
			
			// tipo: combinacion de los tipos de las particulas incidentes
			// particsInside
			int ntipo = (sumaTipos+i)%6;			
			
			float maxRnd = 1;
			// posicion
			ofVec2f pos = ofVec2f(0,0);
			pData.position = pos;
	  		
			// velocidad
			// direccion: la del pTot (o pMean)
			ofVec2f vel = pTot;			
			float ddMax = 380;
			float dd = vel.length();
			vel.normalize();
			
			//
			// ****
			//
			// Dispersión, depende del valor de pTot: Si es bajo, más dispersión
			// vel.rotate....
			
			vel*=25+ntipo+ofRandom(3); //-ofMap(dd,ddMax*0.25,ddMax, 10,18);
			
			if(bRadial) {
				// distribuye radialmente
				float angulo = (float) 360/nPartics*i;
				angulo += (0.5*(1-ofRandom(1.0))) * 10.1;
				vel.rotate(angulo);		// DEGREES
			}
			pData.velocity = vel;
			
			// setTipo
			//			pData.tpPartic = ntipo; //floor(ofClamp(dd/(ddMax*0.75)*6,0,5)); 
			pData.setTipoPart(ntipo); //floor(ofClamp(dd/(ddMax*0.75)*6,0,5)); 
			
			newPartics.push_back(pData);
			
			// En algunas partículas, poner dos 
			// pero con +q y -q
			// y ang(vel,pTot) = -ang(vel,pTot)
			if(ofRandom(1.0)<0.01) {
				pData.q=-pData.q;
				
				float angulo = pData.velocity.angle(pTot);
//				pData.velocity.rotate(-2*angulo, ofVec3f(0,0,1));
				pData.velocity.rotate(180.0, ofVec3f(0,0,1));
				
				newPartics.push_back(pData);	
			}
			
			
			bNuevasPart = true;
		}
		//		ofLogNotice("partics creadas: " +ofToString(newPartics.size()));
		particsCreadas.push_back(nPartics);
		
		return bNuevasPart;		
		
		
		
	}
	
	
	
	void draw() {
		if(contadorPartics>0) bOcupado=true;
		
		ofPushStyle();
		ofEnableSmoothing();
		ofEnableAlphaBlending();
		if(bOcupado) {
			float alpha = ofMap(contadorPartics, 0, 8, 35, 255);
			alpha = ofWrap(alpha,0,100);
			circuloPath.setFillColor(ofColor(255,224,23, alpha));
			circuloPath.setFilled(true);
			//			ofSetColor(255,224,23, ofMap(contadorPartics, 0, 6, 35, 255));
			circuloPath.draw();
		}
		ofDisableAlphaBlending();

//		ofSetColor(200);
//		ofNoFill();
//		ofSetLineWidth(6);
//		circulo.draw();

		//
		// Direccion ---
		//
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
		
		// Animacion con valores modPTot y energy
		ofPushStyle();
		ofEnableBlendMode(OF_BLENDMODE_ADD);
		int nBalls = floor(ofRandom(energy));
		for(int i=0; i<nBalls; i++) {
			int nc = floor(ofRandom(3));
			ofColor cc;
			if(nc==0) cc = ofColor(255,0,0);
			else if(nc==1) cc = ofColor(0,255,0);	
			else if(nc==2) cc = ofColor(0,0,255);
			ofSetColor(cc);
			float radTmp = ofRandom(modPMeanT)/2.0;
			float angTmp = ofRandom(TWO_PI);
//			ofCircle(pos.x+pMeanT.x, pos.y+pMeanT.y, 3);
//			ofCircle(pos.x+pMeanT.x+radTmp*cos(angTmp), pos.y+pMeanT.y+radTmp*sin(angTmp), 3);
			ofCircle(pos.x+radTmp*cos(angTmp), pos.y+radTmp*sin(angTmp), 3);
		}
		ofDisableBlendMode();
		ofPopStyle();
		
		
		
		
		ofDrawBitmapString("pTot: " + ofToString(modPTot), pos.x+50, pos.y+50);
		ofDrawBitmapString("E: " + ofToString(energy), pos.x+50, pos.y+70);
		
		ofDisableSmoothing();
		ofPopStyle();
		
		bOcupado = false;
	}
	
	
	
	void drawStats(ofRectangle cuad) {
		ofPushMatrix();
		ofPushStyle();
		{
			ofTranslate(cuad.x, cuad.y, 0);
			ofNoFill();
			ofSetColor(200);
			ofSetLineWidth(1.0);
			ofRect(0,0, cuad.width, cuad.height);
			ofRect(0,cuad.height, cuad.width, cuad.height);
			ofRect(0,2*cuad.height, cuad.width, cuad.height);
			
			if(pMeans.size() > 0) {
				ofMesh linP, linE, linN;
				linP.setMode(OF_PRIMITIVE_LINE_STRIP);
				linE.setMode(OF_PRIMITIVE_LINE_STRIP);
				linN.setMode(OF_PRIMITIVE_LINE_STRIP);
				float di = cuad.width / pMeans.size();
				for(int i=0; i<pMeans.size(); i++) {
					linP.addVertex(ofPoint(di*i, cuad.height-ofMap(pMeans[i].length(), 0, 50, 0, cuad.height), 0));
					linE.addVertex(ofPoint(di*i, 2*cuad.height-ofMap(energies[i], 0, 1000, 0,cuad.height), 0));
					linN.addVertex(ofPoint(di*i, 3*cuad.height-ofMap(particsCreadas[i], 0, 15, 0,cuad.height), 0));
				}
				
				ofNoFill();
				float h_pMeanT = cuad.height-ofMap(pMeanT.length(), 0, 50, 0, cuad.height);
				float h_eMeanT = cuad.height-ofMap(energyMeanT, 0, 1000, 0, cuad.height);
				
				ofSetColor(ofColor::blueSteel);
				linP.draw();
				ofSetColor(ofColor::blue);
				ofLine(0, h_pMeanT, cuad.width, h_pMeanT);
				ofDrawBitmapString("momento", 10,cuad.height+15);
				
				ofSetColor(ofColor::red);
				linE.draw();
				ofSetColor(ofColor::orange);
				ofLine(0, cuad.height+h_eMeanT, cuad.width, cuad.height+h_eMeanT);
				ofDrawBitmapString("energia", 10,cuad.height+30);
				
				ofSetColor(ofColor::orange);
				linN.draw();
				
				
				
			}
		}
		ofPopStyle();
		ofPopMatrix();
		
	}
	
	
};