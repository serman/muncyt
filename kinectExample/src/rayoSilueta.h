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
class rayoSilueta{
public:
    void setup();
    void update();
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

    ofPath	testPath;

    bool	bDrawCaminos;
    bool	bDrawPtosChoque;
    ofxUICanvas *gui;
    void	setUI(ofxUITabBar *guiTabBar);
    void guiEvent(ofxUIEventArgs &e);
    
    void setSilueta(ofPolyline p);

};

#endif /* defined(__kinectExample__rayoSilueta__) */
