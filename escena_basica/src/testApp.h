#pragma once
#include "ofMain.h"
#include "ofxBox2d.h"

#include "Efectos.h"
#include "particulas.h"
#include "Anillo.h"

#include "ofxXmlSettings.h"
#include "cheapComm.h"
// -------------------------------------------------

#define COLOR_VERDE 0x5dca31
const int tipoNucleo  = 0;
const int tipoNeutron = 1;


#define N_SOUNDS 8

// 

// Para computar y guardar datos y estad’sticas
struct EnergyData {
	float energTotal;
	int numCollisions;
	EnergyData() {energTotal=0; numCollisions=0;}
};

// Estructura para crear nuevas particulas
// Con esto se hace una lista de particulas a crear
// en el nuevo frame al salir de una colisi—n
//
struct CineticData {
	float x;
	float y;
	float vVal;
	float vAng;
	int tipoPart;
	CineticData (float _x, float _y, float _vVal, float _vAng, int _tipoPart) {
		x=_x;
		y=_y;
		vVal=_vVal;
		tipoPart=_tipoPart;
	}
};

class testApp : public ofBaseApp {
	
public:
	
	void setup();
	void update();
	void draw();
	
	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void resized(int w, int h);
	
	EnergyData	energData;
    
	ofxBox2d                            box2d;			  //	the box2d world

	// this is the function for contacts
	void contactStart(ofxBox2dContactArgs &e);
	void contactEnd(ofxBox2dContactArgs &e);

	void colisiona_N_n (Particula * pN, Particula * pn);
	
	// Borde circular
	ofxBox2dEdge			borde;
	ofPolyline              bordeLine;
	ofPolyline				circuloInt,circuloExt;
	float			radioInt, radioExt;
	
	
//	vector    <ofPtr<ofxBox2dCircle> >	circles;  //	default box2d circles
	vector    <ofPtr<Particula> >	nucleos;	//	
	vector    <ofPtr<Particula> >	neutrones;  //	
	vector	  <ofPtr<ofxBox2dRect> >	boxes;	//	defalut box2d rects

	void addNucleo();
	void addNucleo(int xx, int yy);
	void addNucleo(int xx, int yy, float r);
	void addBox(int xx, int yy);

	void addNeutron(int xx, int yy);
	void addNeutron(int xx, int yy, float vVal, float vAng);
	
	void setModoParticulas(int _modo);
	int modoDrawParticulas;
		
	vector <CineticData>	nuevasPartics;
	
	// Fuerza
	float		fuerzaVal;
	float		fuerzaAng;
	float		fuerzaWAng;
	ofPoint		fuerza;
	
	bool		swFuerza;
	void		drawFuerza(ofPoint p, ofPoint gravity, float esc = 1.0);
	void		drawFuerzaSelector();

	ofRectangle marco;
	// TUT en https://www.iforce2d.net/b2dtut/fixtures
	
	// Textura de particulas
	ofTexture	texPartic;

	// Color Radioactivo
	// #5dca31 = RGB(93,202,49)  = HSB(103,76,79)
	// Ver mas en: http://rgb.to/hex/5dca31
	ofColor colorVerde;
	
	float rNucleo;
	float rNeutron;
	
	float velocNeutronLim, velocNeutronDestroy, velocNeutronLanz;
	
	// when the ball hits we play this sound
	ofSoundPlayer  sounds[N_SOUNDS];
	void cargaSounds();
	
	// Control de TimeStep
	float fpsAct;
	
	// Efectos: explosi—n
	Destello	chispa;
	vector<Destello>	chispas;
	
	vector <ofPtr<Destello> >	destellos;	//	
	void addDestello(float px, float py);
	
	
	// Anillo de aceleraci—n
	Anillo	anillo;
	
	bool swBlendModeADD;
	
	float frecFondo;	// frecuencia de cambio del 
	
	
	// XML
	void GrabaConfig();
	void CargaConfig();
    
    
    //Comunicacin
    cheapComm myComm;
};

