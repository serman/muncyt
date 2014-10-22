//
//  rayoSilueta.h
//  kinectExample
//
//  Created by Sergio Galan on 9/16/14.
//
//

#ifndef __kinectExample__rayoSilueta__
#define __kinectExample__rayoSilueta__

#include <iostream>
#include <ofMain.h>
#include "ParticleX.h"
#include "ofxUI.h"
#include "particleSourceHandler.h"
#include "consts.h"
#include "ofxPostProcessing.h"
class contours;
class rayoSilueta{
public:
    void setup();
    void update(contours &cont);
    void draw();

    ofVec3f zentro;

    ofPolyline	camino, camino1;
    void		tiltCamino();
    bool		bTiltCamino;

    vector<ParticleX>	particulas;
    void				addParticleLateral();
    int					ratePartic;

    ofVboMesh			particMesh;

    vector<ofVec2f>		ptsChoque;
    vector<ofVec2f>		tangChoque;
    bool				swDifraccion;
    float vel;
    void reset();
    ofPath	testPath;

    bool	bDrawCaminos;
    bool	bDrawPtosChoque;
    bool    bDrawTraza;
    ofxUICanvas *gui;
    void	setUI(ofxUITabBar *guiTabBar);
    void guiEvent(ofxUIEventArgs &e);
    
    void setSilueta(ofPolyline p);
    void addParticlesFor(int );
    ParticleSourceHandler mExplosionEfect;
    bool bAddParticles;
    long stopParticlesAt;
    void triggerParticles(float ang);
    ofShader glowShader;
    ofFbo   fboGeneral, fboRayos;
        bool guiAddParticles;
    ofxPostProcessing postp1;
};

#endif /* defined(__kinectExample__rayoSilueta__) */
