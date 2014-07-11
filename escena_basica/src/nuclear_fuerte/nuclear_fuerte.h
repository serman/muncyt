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

#include "ofxScene.h"
#include "ofxAnimatableFloat.h"
// -------------------------------------------------




class nuclear_fuerte : public ofxScene {

public:
    
	void setup();
	
	void update(float dt);
	void draw();
    
	void mousePressed( int x, int y, int button );

    
};

#endif /* defined(__escena_basica__electromagnetica__) */
