
#ifndef __escena_basica__menu__
#define __escena_basica__menu__

#include <iostream>

#pragma once
#include "ofMain.h"
#include "consts.h"

#include "ofxScene.h"
#include "ofxAnimatableFloat.h"

#include "ofxTuio.h"
#include "tangiblesHandler.h"
#include "handShadow.h"

#include "menuButton.h"

// Esto ya es parte de la escena en si:
#include "ofxBox2d.h"
#include "cheapComm.h"
#include "display.h"


// -------------------------------------------------

class menu : public ofxScene { //ofBaseApp {
	
public:
	
	void setup();
	void update(float d1);
	void draw();
	
	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void resized(int w, int h);
	cheapComm *myComm;
    void setComm(cheapComm *m){ myComm=m;}
	//
	// Cosas para incorporacion a Prog_PPal, a TUIO...
	//
	void init_Escena();
	void exit_Escena();
	void sceneWillAppear( ofxScene * fromScreen );
    void sceneWillDisappear( ofxScene * toScreen );
	
	tangiblesHandler hands;			// hands: representacion de los cursores TUIO
	tangiblesHandler touchElements;	// 
	//menuButton button1, button2, button3, button4;	// Habrá 4 botones touchElements
    vector<menuButton> buttons;
    
    void	tuioAdded(ofxTuioCursor & tuioCursor);
	void	tuioRemoved(ofxTuioCursor & tuioCursor);
	void	tuioUpdated(ofxTuioCursor & tuioCursor);
	
	ofPoint transf_PosTUIO(ofxTuioCursor & tuioCursor);
	
    
    // TUIOs
    ofxTuioClient *tuioclient;
    void setTuio(ofxTuioClient *_client){
        tuioclient=_client;
    }
    
    void    onButtonPressed(BUTTON_TYPE1 &mtype);
	
	
	// End add cosas - - - 

	ofVec2f centroScreen;	
	
	// - - - Menu 4 Forces (4ces) - - -
	void	draw4Forces();	
	bool	bDraw4Forces;
	void	drawBola4(ofVec2f pos, float radius, float rot=0);
	void	drawBola4_fbos(ofVec2f pos, float radius, float rot=0);
	void	draw4Forces_fbos();	

	void	draw2Colors();	
	
	ofxBox2d                                box2d;			  //	the box2d world
	vector		<ofPtr<ofxBox2dCircle> >	circles;		  //	default box2d circles
	vector		<ofPtr<ofxBox2dRect> >		boxes;			  //	defalut box2d rects
	bool	bAddCircle, bAddBox;
	void	initParticles();
	void	removeParticles();	
	void	addCircle(ofPoint _pos);
	void	addBox(ofPoint _pos);
	
	ofVec2f ptoMedio(vector <ofPtr<ofxBox2dCircle> > shapes);
	ofVec2f ptoMedio(vector <ofPtr<ofxBox2dRect> > shapes);
	ofVec2f ptoMed_circles, ptoMed_boxes;
	
	bool	swFuerzaDensidad;
	
	bool	isKeyPressed;
	bool	isMousePressed;
	
	ofImage bola;
	float ladoPart1, ladoPart2;
	
	vector<ofColor> coloresBN;
	
	vector<ofPolyline>	sectores;
	void setupSectores();
	void interactionUpdate();
	ofPath borde;
    char currentButton;
	ofVec2f centro;
	float distConf;
	
	float minDisInt;

	// Interaccion Mouse y TUIO
	void interaccion_point(ofVec2f ptF, bool isNeg);
	
	bool fRed;	// para representar uno de los secotroes como partics rojas o partics RGB

	// fbos
	ofFbo	fbo1, fbo2, fbo3, fbo4;
	bool	bDrawFbos;
	
	// rangos angulares
	float rangosAngDeg [4];
    display mdisplay;
    bool bshowdebug;
	
};

#endif /* defined(__escena_basica__menu__) */
