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




class nuclear_fuerte : public ofxScene {

public:
    enum colores{ROJOS=0, AZULES,VERDES,FLUOR};
	void setup();
	
	void update(float dt);
	void draw();
    
	void mousePressed( int x, int y, int button );
    
    void	tuioAdded(ofxTuioCursor & tuioCursor);
	void	tuioRemoved(ofxTuioCursor & tuioCursor);
	void	tuioUpdated(ofxTuioCursor & tuioCursor);
    
    void exchangeColors(int );
    int colorp1; //red
    int colorp2; //blue
    int colorp3;//green
    
};

#endif /* defined(__escena_basica__electromagnetica__) */
