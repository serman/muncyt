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
// -------------------------------------------------




class gravedad : public ofxScene {

public:
    
	void setup();
	
	void update(float dt);
	void draw();
    
	void mousePressed( int x, int y, int button );

	
	
	//
	// Cosas para incorporacion a Prog_PPal, a TUIO...
	//
	void init_Escena();
	void exit_Escena();
	void sceneWillAppear( ofxScene * fromScreen );
    void sceneWillDisappear( ofxScene * toScreen );
//	tangiblesHandler hands;			// hands
//	tangiblesHandler touchElements;	// Habrá 4 touchElements
	
    cheapComm *myComm;
    void setComm(cheapComm *m){ myComm=m;}
};

#endif /* defined(__escena_basica__electromagnetica__) */
