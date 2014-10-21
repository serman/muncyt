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
#include "particleSourceHandler.h"
#include "consts.h"
#include "ofxPostProcessing.h"

enum states{ROTATING, OFF, FADING_IN};
class Anillo  {
public:

	
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
	
    int status;
    
	ofRectangle anilloUI_L, anilloUI_R;	
	ParticleSourceHandler mparticles;
    ofxPostProcessing post;
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
		status=FADING_IN;
            post.init(ofGetWidth(), ofGetHeight(),true);
            post.createPass<BloomPass>()->setEnabled(true);
	}
	
	void reset_wAng() {
		wAng = -0.1*TWO_PI/(60.0);
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
    
    void setFadeInMode(){
        reset();
        status=FADING_IN;
        mparticles.addEmitter(0,0);
        time_transition_start=ofGetElapsedTimeMillis();
    }
	
	void update() {
        if(status==ROTATING){
            if(abs(accAng)<0.01) accAng=0.0;
            accAng*=viscAcc;
            wAng += accAng;
            if(abs(wAng)>wAngMax ) {
                if(wAng<0) 			wAng=-wAngMax;
                else				wAng=wAngMax;
            }
            angT += wAng;
        }
        else if(status==FADING_IN){
            angT=2*PI/3;
            mparticles.update();
            if( ofGetElapsedTimeMillis() - time_transition_start > 2000 ){
                status=ROTATING;
            }
        }
	}
	
	void draw() {
               // post.begin();
		// por ahora dibujo una bola
		ofPushMatrix();
		ofPushStyle();
			ofEnableAlphaBlending();
			ofTranslate(pos.x, pos.y,0);
			// particula
			ofSetColor(color);
			ofPushMatrix();
			ofTranslate(radioMed*cos(angT), radioMed*sin(angT));
            if(status==ROTATING){
                if(swTexture) {
                    // IMAGEN!
                    tex.draw(-radioPart,-radioPart,2*radioPart,2*radioPart);
                }
                else {
                    ofEllipse(0,0, 2*radioPart, 2*radioPart);//+ofMap(wAng,-PI,PI,-140,140)) );
                }
            }
            else if(status==FADING_IN){
                mparticles.draw();
                ofEnableAlphaBlending();
                ofFill();
                ofSetColor(255,30);
                tex.draw(-radioPart,-radioPart,2*radioPart,2*radioPart);
                ofDisableAlphaBlending();
            }
			ofPopMatrix();
		
		ofPopStyle();
		ofPopMatrix();

        drawDisplay();
     //   post.end();
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

    void reset(){
        fase=0;
		angT = 0.0;
		wAng = -0.1*TWO_PI/(60.0);
		accAng = 0.0;
		wAngMax = 0.16;
		dAcc = 0.03;//0.03;
		viscAcc = 0.30;

    }
    void drawDisplay(){
        ofPushMatrix();
        ofPushStyle();
		ofEnableAlphaBlending();
		ofFill();
        ofTranslate(W_WIDTH/2, W_HEIGHT/2);
        int r=740/2;
        ofSetColor(ofColor::fromHsb(ofMap(abs(wAng),0,wAngMax,150,250), 200, 200,150+70*sin(ofGetElapsedTimef()/ofMap(abs(wAng),0,wAngMax,4.0,1.0)*TWO_PI)));
       // ofSetColor(27,117,187,200+50*sin(ofGetElapsedTimef()/2.0*TWO_PI));
        for(int i=0; i<ofMap(abs(wAng),0,wAngMax,0,360); i+=3){
            ofRotateZ(3);
            if( (i>=0 && i<10) || (i>73 && i<100) || (i>166 && i<190) || (i>342 && i<360)){
                
            }else
                ofRect(-r,0,ancho-14,10);
        }
        ofPopStyle();
        ofPopMatrix();
    }
    
private:
    ofTexture tex;
	bool	swTexture;
	// geometria
	float radioInt;
	float radioExt;
	
	float radioMed;
	float ancho;
	
	ofVec2f pos;
	ofPath contorno;
	
	ofColor	colorFondo;
    int time_transition_start;

};