/*
 *  Particle.h
 *  openFrameworks
 *
 *  Created by Stephen Braitsch on 11/6/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef PARTICLE
#define PARTICLE

#include  "ofMain.h"
#define MAXTIME 65
#define MINTIME 20
class Particle2 {
	
	private:
	float x;		// current y position
	float y;		// current x position

	ofColor color;	
	int radius;
	int age;		// current age
	int maxAge;		// max age
	float speedX;
	float speedY;
	
	public:
	Particle2();
	
	void update();
	void draw();
	bool dead;
};


#endif