/*
 *  Anillo.h
 *  escena_basica
 *
 *  Created by guillermo casado on 10/05/14.
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

/*
 * 
 * Anillo exterior donde se 'aceleran' objetos (partículas)
 * 
 * 
 * 
 * 
*/

class Anillo {
public:
	// geometria
	float radioInt;
	float radioExt;
	
	float radioMed;
	float ancho;
	
	ofVec2f pos;
	ofPath contorno;
	
	ofColor	colorFondo;
	
	// particula que gira
	float fase;
	float angT;
	float wAng;
	
	float radioPart;
	ofColor color;
	
	float accAng;
	
	Anillo() {}
	Anillo(ofVec2f _pos, float rInt, float rExt) {
		pos = _pos;
		radioInt = rInt;
		radioExt = rExt;
		
		radioMed = (radioInt + radioExt) / 2.0;
		ancho = radioExt-radioInt;	
		
		radioPart = ancho*0.4/2.0;
		ofLogVerbose("radioPart: " + ofToString(radioPart));
		fase=0;
		angT = 0.0;
		wAng = -0.3*TWO_PI/(60.0);
		accAng = 0.0;
		
		color = ofColor(ofColor::fromHsb(30, 200, 200) );
		colorFondo = ofColor(ofColor::fromHsb(120, 200, 100, 40));
		
		// preparar contorno
		int resol = 360/5;
		
		contorno.setFilled(true);
		contorno.setColor(colorFondo);
		contorno.moveTo(radioInt,0);
		for (int i = 0; i <= resol; i++){
			float anglef = (i) * TWO_PI/resol;
			float x = radioInt * cos(anglef);
			float y = radioInt * sin(anglef); 
			contorno.lineTo(x,y);
		}
		for (int i = 0; i <= resol; i++){
			float anglef = -(i) * TWO_PI/resol;
			float x = radioExt * cos(anglef);
			float y = radioExt * sin(anglef); 
			contorno.lineTo(x,y);
		}
		contorno.close();
		
	}

	void setAceleracion(float _accAng) {
		accAng = _accAng;
	}
	
	
	void acelera(float _accAng) {
		accAng += _accAng;
	}
	
	void update() {
		if(abs(accAng)<0.05) accAng=0.0;
		accAng*=0.60;
		wAng += accAng;
		angT += wAng;
	}
	
	void draw() {
		// por ahora dibujo una bola
		ofPushMatrix();
		ofPushStyle();
		{
			ofEnableAlphaBlending();
			ofTranslate(pos.x, pos.y,0);
			// fondo
			ofFill();
			ofSetColor(colorFondo);
			contorno.draw();

			// particula
			ofSetColor(color);
			ofCircle(radioMed*cos(angT), radioMed*sin(angT), radioPart);
		}
		ofPopStyle();
		ofPopMatrix();
	}

};