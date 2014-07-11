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
    //shaders
    ofShader shader;
    void	setupShader();
   	void	drawNoise();
    bool	boolDrawNoise;
    void setupParticles();
    void resetParticles();
    void showDebug();
    float alpha;
    
    void		setupGUI();
   	ofxUICanvas *gui1;
	void		grabarScreen();
    waves mwave;
};

#endif /* defined(__escena_basica__electromagnetica__) */
