//
//  electromagnetica.h
//  escena_basica
//
//  Created by Sergio Galan on 7/11/14.
//
//

#ifndef __escena_basica__gravedad__
#define __escena_basica__gravedad__

#include <iostream>

#pragma once
#include "ofMain.h"
#include "consts.h"
#include "cheapComm.h"
#include "ofxScene.h"
#include "ofxAnimatableFloat.h"
#include "ofxXmlSettings.h"
#include "ofxUI.h"
#include "ParticleX.h"
#include "Sol.h"
#include "tangiblesHandler.h"
#include "handShadow.h"
// -------------------------------------------------


#define MAX_MASA_SOL 32000
#define INIT_MASA_SOL 7000
#define DURACION_BLACK_HOLE 15000
class gravedad : public ofxScene {

public:
    
	void setup();
	
	void update(float dt);
	void draw();
    
	void mousePressed( int x, int y, int button );
    void keyPressed  (int key);
	
	
	//
	// Cosas para incorporacion a Prog_PPal, a TUIO...
	//
	void init_Escena();
	void exit_Escena();
	void sceneWillAppear( ofxScene * fromScreen );
    void sceneWillDisappear( ofxScene * toScreen );
	
    
    ofTrueTypeFont font;
	
    ofVec2f		zentro;
	float		radioEscena;
	ofPath		borde;	
	
	ofMesh	superf, superfOrig;
	int		superfW, superfH;
	int		nH, nW;
	void	setupMeshSuperf();
	void	updateMeshSuperf();
	
	float nivelNoise;
	bool	noiseAuto;
	
	
	bool	swWeb;
	float gg;	// interact SOL
	float kk;	// interact user
	
	// helper functions
	void addFace(ofMesh& mesh, ofVec3f a, ofVec3f b, ofVec3f c);
	void addFace(ofMesh& mesh, ofVec3f a, ofVec3f b, ofVec3f c, ofVec3f d);
	void addTexCoords(ofMesh& mesh, ofVec2f a, ofVec2f b, ofVec2f c);
	void addTexCoords(ofMesh& mesh, ofVec2f a, ofVec2f b, ofVec2f c, ofVec2f d);
	ofVec3f getVertexFromImg(ofImage& img, int x, int y);
	
	void setNormals( ofMesh &mesh );
	
	
	ofEasyCam	cam;
	ofCamera	camera;
	
    ofLight		light;
	ofMaterial	mat1;
	ofColor		mat1Color;
	
	bool	swWireFrame;
	bool	swCool;
	bool	swLight;
	bool	swOrtho;
	
	
	bool	bUseTexture;
	ofImage	imgTex;
    
	ofFbo	imgMix;
	ofFbo	imgDyn;
	ofFbo	imgWeb;
	void	updateFBO(vector<ofVec2f> ptsInter);
	void	updateFBO();
	
	bool bshowdebug;
	// SOL
	Sol sol;
	
	// TUIOS
	float masaTUIO;
	
	void setupGUI();
	ofxUISuperCanvas *gui1;
	void guiEvent(ofxUIEventArgs &e);
	
    void exit();
	
	
	// camara-z
	float zCam;
	
	float lon, lat;
	
	
	// Particulas
	vector <ParticleX> particulas;
	void	addParticleOut();
	void	updateParticlesX();
	void	drawParticlesX();
	int		ratePartic;
	

    void	tuioAdded(ofxTuioCursor & tuioCursor);
	void	tuioRemoved(ofxTuioCursor & tuioCursor);
	void	tuioUpdated(ofxTuioCursor & tuioCursor);
    void sendSunParticlesOsc();
    enum states {GROWING, EXPLOSION};
    enum states state;
    int initExplosionTime;
    void    initExplosion();
    tangiblesHandler hands;
    
    ofShader blackHoleShader;
    
    bool status_sent_EOS_Sent;
    
};

#endif /* defined(__escena_basica__electromagnetica__) */
