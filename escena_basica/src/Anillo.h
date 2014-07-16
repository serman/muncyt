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
#include "tangibleObject.h"
class Anillo  {
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
	float accAng;
	float dAcc;	// variacion de aceleracion
	float viscAcc; // frenado de Aceleracion
	
	float velocNeutronLanz;
	float wAngMax;
	
	float radioPart;
	ofColor color;
	
	ofTexture tex;
	bool	swTexture;


    
	ofRectangle anilloUI_L, anilloUI_R;	
	
	Anillo() {}
    void setup(){}
	Anillo(ofVec2f _pos, float rInt, float rExt) {
		pos = _pos;
		radioInt = rInt;
		radioExt = rExt;
		
		radioMed = (radioInt + radioExt) / 2.0;
		ancho = radioExt-radioInt;	
		
		radioPart = ancho*0.5/2.0;
		ofLogVerbose("radioPart: " + ofToString(radioPart));
		fase=0;
		angT = 0.0;
		wAng = -0.1*TWO_PI/(60.0);
		accAng = 0.0;
		
		wAngMax = 0.16;
		
		dAcc = 0.03;//0.03;
		viscAcc = 0.30;
		
		color = ofColor(ofColor::fromHsb(30, 200, 200) );
		colorFondo = ofColor(ofColor::fromHsb(120, 200, 100, 40));

		swTexture = false;
		
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
	
	void setUI(ofRectangle ctrl_decc, ofRectangle ctrl_acc) {
		anilloUI_L = ctrl_decc;
		anilloUI_R = ctrl_acc;
	}

	void setAceleracion(float _accAng) {
		accAng = _accAng;
	}
	
	
	void acelera(float _accAng) {
		accAng += _accAng;
	}
	
	void setTexture(ofTexture _tex) {
		tex = _tex;
		swTexture = true;
	}
	
	void update() {
		if(abs(accAng)<0.01) accAng=0.0;
		accAng*=viscAcc;
		wAng += accAng;
		if(abs(wAng)>wAngMax ) {
			if(wAng<0) 			wAng=-wAngMax;
			else				wAng=wAngMax;
		}
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
			//ofFill();
			//ofSetColor(colorFondo);
			//contorno.draw();
            
            // fondo crece segun la velocidad
            //wAngMax = 0.14;
//            float angCtrlVelo = ofMap(abs(wAng), wAngMax, )
            
            

			// particula
			ofSetColor(color);
			ofPushMatrix();
			ofTranslate(radioMed*cos(angT), radioMed*sin(angT));
//			ofRotate(PI+ofRadToDeg(angT));
			if(swTexture) {
				// IMAGEN!
				tex.draw(-radioPart,-radioPart,2*radioPart,2*radioPart);
			}
			else {
				ofEllipse(0,0, 2*radioPart, 2*radioPart);//+ofMap(wAng,-PI,PI,-140,140)) );
			}
			ofPopMatrix();
		}
		ofPopStyle();
		ofPopMatrix();
	}
    ofPoint getParticlePosition(){
        return ofPoint(pos.x+radioMed*cos(angT), pos.y+radioMed*sin(angT));
    }

	void drawControls() {		
		ofPushStyle();
		ofEnableAlphaBlending();
		ofFill();
		//	ofSetColor(200);
		ofSetColor(0,0,0,100);
		ofRect(anilloUI_L);
		ofRect(anilloUI_R);
		ofPopStyle();
		
		//ofPushMatrix();
		//ofPushStyle();
	}
	
	float getVelocNeutronLanz() {
		float velocNeutronLim = 80.0;
		float velocNeutronDestroy = velocNeutronLim*0.7;
		velocNeutronLanz = ofMap(abs(wAng), 0,wAngMax, velocNeutronDestroy, 2*velocNeutronLim );
		ofLogNotice("lanzar neutron. wAng: " + ofToString(abs(wAng)) + "  vel: " + ofToString(velocNeutronLanz));
		return velocNeutronLanz;
	}


	/*bool interaccionAnillo(ofPoint pt) {
		return interaccionAnillo(pt.x, pt.y);
	}
	
	bool interaccionAnillo(float x, float y) {
		
		if(anilloUI_L.inside(x,y)) {
			acelera(dAcc);
			ofLogVerbose("Acc --: " + ofToString(accAng));
			return true;
		}
		if(anilloUI_R.inside(x,y)) {
			acelera(-dAcc);
			ofLogVerbose("Acc ++: " + ofToString(accAng));
			return false;
		}	
		return false;
	}*/
};