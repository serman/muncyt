#ifndef _EMITTER
#define _EMITTER


#include "ofMain.h"
#include "Particle2.h"
#include <list>

#define psize 64

class ParticleEmitter {
typedef list<Particle2 *> particleList;
	private:
	particleList particles;
	int centerX;
	int centerY;
	float rotation;
	int current_size;
	public:
	
	void setup(int, int);
	void update();
	void draw();
	bool isEmpty();
	//Particle* p[size];
};

#endif
