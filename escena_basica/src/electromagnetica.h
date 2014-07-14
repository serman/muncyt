//
//  electromagnetica.h
//  escena_basica
//
//  Created by Sergio Galan on 7/11/14.
//
//

#ifndef __escena_basica__electromagnetica__
#define __escena_basica__electromagnetica__

#include <iostream>

#pragma once
#include "ofMain.h"
#include "consts.h"

#include "ofxScene.h"
#include "ofxAnimatableFloat.h"
#include "Particle.h"
#include "ofxUI.h"
#include "waves.h"
#include "tangiblesHandler.h"
#include "noiseShadow.h"
#include "ofxTuio.h"
#include "wavesManager.h"

// -------------------------------------------------




class electromagnetica : public ofxScene {
public:
    vector<Particle> particles ;
    ofMesh meshParticles;
    int numParticles;
	void setup();
	
	void update(float dt);
	void draw();
    
	void mousePressed( int x, int y, int button );
    void keyPressed(int key);
    //scene notifications
	void sceneWillAppear( ofxScene * fromScreen );
	
	//scene notifications
	void sceneWillDisappear( ofxScene * toScreen );
    //shaders
    ofShader shader;
    void	setupShader();
   	void	drawNoise();
    bool	boolDrawNoise;
    void setupParticles();
    void resetParticles();
    void showDebug();
    float alpha;
    
    void setupGUI();
   	ofxUICanvas *gui1;
	void		grabarScreen();
    wavesManager wavesm;
    ofxTuioClient   tuioClientEm;
    bool noiseMode;
	tangiblesHandler hands;
    
    void	tuioAdded(ofxTuioCursor & tuioCursor);
	void	tuioRemoved(ofxTuioCursor & tuioCursor);
	void	tuioUpdated(ofxTuioCursor & tuioCursor);
    int  options_sampling;
    vector<ofColor> colours;
    bool drawlines;
        
};

#endif /* defined(__escena_basica__electromagnetica__) */
