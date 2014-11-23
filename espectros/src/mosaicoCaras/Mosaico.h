/*
 *  Mosaico.h
 *  mosaicoFaces
 *
 *  Created by guillermo casado on 18/11/14.
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _MOSAICO
#define _MOSAICO

#import "ofMain.h"



enum tipoMosaico { 
	CASILLAS_4, 
	CASILLAS_4X2 
};

class Mosaico {

public:
	int					tipo;
	float				lado, load_W, lado_H;
	vector<float>		alturas;
	vector<ofRectangle> bandas;
	vector<int>			ids;
	
	int lastTime;		// CTRL de tiempo
	
	Mosaico() {  };
	
	void setup(int tp, float _lado) {
		tipo = tp;
		lado = _lado;
		
		lastTime = ofGetElapsedTimeMillis();
	}
	
	void setAlturas(vector<float>& _alturas) {
		for(int i=0; i<_alturas.size(); i++) {
			alturas.push_back(_alturas[i]);
		}
		
		setupBandas();
	};
	
	void setupBandas() {
		float ladoBanda = (tipo==CASILLAS_4)? lado : lado/2.0;

		for(int i=0; i<alturas.size(); i++) {
			ofRectangle rT;
			if(i==0) {
				rT = ofRectangle(0,0, ladoBanda, lado*alturas[0]);
				bandas.push_back(rT);
				ids.push_back(0);
			}
			else {
				ofRectangle rAnt = bandas[bandas.size()-1];
				if(tipo==CASILLAS_4) {
					rT = ofRectangle(0,rAnt.y+rAnt.height, ladoBanda, lado*alturas[i]);
					bandas.push_back(rT);
					ids.push_back(0);
				}
				else {
					float px, py;
					px=0;
					py=rAnt.y+rAnt.height;
					rT = ofRectangle(px,py, ladoBanda, lado*alturas[i]);
					bandas.push_back(rT);
					ids.push_back(0);
					
					px=ladoBanda;
					rT = ofRectangle(px,py, ladoBanda, lado*alturas[i]);
					bandas.push_back(rT);
					ids.push_back(0);
				}
			}
			
			
		}
			
	};

	
	void update(int nImages) {
		
		if(ofGetElapsedTimeMillis()>(lastTime+500)) {
			
			for(int i=0; i<ids.size(); i++) {
				if(0.10>ofRandom(1.0)) ids[i] = floor(ofRandom(nImages));				
			}
			
			lastTime = ofGetElapsedTimeMillis();				
		}	
		
	}
	
	

};


#endif