//
//  electromagnetica.h
//  escena_basica
//
//  Created by Sergio Galan on 7/11/14.
//
//

#ifndef __escena_basica__nuclear_fuerte__
#define __escena_basica__nuclear_fuerte__

#include <iostream>
#pragma once
#include "ofMain.h"
#include "consts.h"
#include "cheapComm.h"
#include "ofxScene.h"
#include "ofxAnimatableFloat.h"
// -------------------------------------------------

#include "ParticleS.h"
#include "Emisor.h"
#include "Centro.h"
#include "ParticleTipos.h"
#include "ofxXmlSettings.h"
#include "ofxUI.h"



class nuclear_fuerte : public ofxScene {

public:
    enum colores{ROJOS=0, VERDES,AZULES,FLUOR};
    
    
    void	tuioAdded(ofxTuioCursor & tuioCursor);
	void	tuioRemoved(ofxTuioCursor & tuioCursor);
	void	tuioUpdated(ofxTuioCursor & tuioCursor);
    
    void exchangeColors(int );
    	void                sendExchangeColors();
    float sendexchangeAt;
    float timeEndExplosion, timeEndBeam;
    int colorp1; //red
    int colorp2; //blue
    int colorp3;//green
    
    void setup();
    void update(float);
    void draw();
	
	void init_Scena();
	/*void exit_Scena();*/
    int getAvailableId();
    void keyPressed(int key);
  //  void keyReleased(int key);
  //  void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
   // void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
	
	bool	bSetBack;
	float	alpha_Back;
	
	ofVec3f		zentro;
	float		radioEscena;
	ofPath		borde;
	ofPolyline	circExt;
	float		radioZentro;
	float		r1, r2;

	ofVbo	anilloFondo1, anilloFondo2;
		
	ofPath		anilloExterior;
	ofPath		anilloExteriorLineas;
	ofMesh		ringExt;
	
	// colores
	void setupColores();
	ofColor				coloresAll[6];
	ofColor				colores[3];
	ofColor				antiColores[3];
	
	
	ofPolyline	camino;
	void		tiltCamino();
	bool		bTiltCamino;
	
	vector<ParticleS>	particulas, particulas_old;
	void				addParticleLateral();
	void				addParticleFromEmiter(Emisor &em);
	int					ratePartic;
	
	ofVboMesh			particMesh;
	
	vector<ofVec2f>		ptsChoque;
	vector<ofVec2f>		tangChoque;
	bool				swDifraccion;

	Emisor	emitter;
	vector <Emisor>		emitters;
	int		totEmitters;
	bool	clearEMitters;
	Emisor*	addEmisor(ofVec2f posTmp, int id_tuio=-1);
	
	Centro		centroLab;
	
	float	magnetField;
	bool	swMagnetField;
	bool	changeMagneticField;
	
	
	bool	bDrawingMode;
	bool	bDrawCaminos;
	bool	bDrawPtosChoque;
    
	
	void	setupGUI();
	ofxUISuperCanvas *gui1;
	void	guiEvent(ofxUIEventArgs &e);
	bool	bSave;
	
	ofImage	imgPunto;
	
	ParticleData newParticleData(int tipo);
	vector<ParticleS>	particulas_out, particulas_out_old;
	
    void exit();
    
    void sceneWillAppear( ofxScene * fromScreen );
    void sceneWillDisappear( ofxScene * fromScreen );
    ofPoint transf_PosTUIO(ofxTuioCursor & tuioCursor);
    bool bshowdebug;
};

#endif /* defined(__escena_basica__electromagnetica__) */
