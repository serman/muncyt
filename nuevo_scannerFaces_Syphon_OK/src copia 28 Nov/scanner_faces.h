#pragma once

#include "ofMain.h"
#include "ofxCv.h"
//#include "ofxFaceTracker.h"
#include "ofxFaceTrackerThreaded.h"
#include "ofxTimer.h"
#include "ofxUI.h"
#include "ofxSyphon.h"


#include "Marco.h"

#define SYPHON_CAM

struct MensajeTime {
	int		tiempo;
	string	title;
	string	mensaje;
	string	mensaje_1;
//	string	mensaje_peque;
	ofPoint	posMsg;
	ofRectangle	rectMsg;
	
	ofImage	imgMsg;

	void drawImage(ofRectangle rectPos) {
		ofPushMatrix();
		ofPushStyle();
		ofSetColor(255);
//		ofTranslate(rectPos.x, rectPos.y);
//		imgMsg.draw(0,0, rectPos.width, rectPos.height);
		imgMsg.draw(rectPos);
		ofPopStyle();
		ofPopMatrix();
	}
	
	void draw(ofTrueTypeFont font, ofTrueTypeFont fontPeque) {
		
		int yy = 50;
		
		ofPushMatrix();
//		ofTranslate((ofGetWidth()-rectMsg.width)/2, yy);
		ofTranslate(posMsg.x, posMsg.y);
		ofPushStyle();
		ofSetColor(255,0,0,180);
		ofFill();
//		ofRectangle rTmp = font.getStringBoundingBox(mensaje, ofGetWidth()/2, ofGetHeight()/6);
//		font.drawString(mensaje + ofToString(rTmp), rTmp.x-rTmp.width/2, rTmp.y);
//		ofRectangle rTmp = font.getStringBoundingBox(mensaje, 0,0);
		font.drawString(mensaje, 0,0);
		
//		ofLogNotice() << ofToString(rectMsg.x) <<","<< ofToString(rectMsg.y) 
//						<< "   wh: " << ofToString(rectMsg.width) <<","<< ofToString(rectMsg.height);
		
		
//		ofSetColor(200,0,0,160);
//		ofRectangle rTmp1 = font.getStringBoundingBox(mensaje_1, rTmp.x, rTmp.y+rTmp.height);
//		font.drawString(mensaje_1, rTmp1.x-rTmp1.width/2, rTmp1.y+30);
		
		ofRectangle rTmp1 = font.getStringBoundingBox(mensaje_1, 0,0);
//		font.drawString(mensaje_1, (ofGetWidth()-rTmp1.width)/2, 85);//145);
//		font.drawString(mensaje_1, -rTmp1.width/2.0, rectMsg.height);//145);
		font.drawString(mensaje_1, 0, rectMsg.height);//145);
		ofPopStyle();
		ofPopMatrix();

		ofPushStyle();
		ofNoFill();
		ofSetColor(ofColor::lime);
		ofRect(rectMsg);
		ofPopStyle();
		
//		ofRect(rTmp1);
		
//		ofSetColor(200,200,0,180);
////		ofRectangle rTmp2 = font.getStringBoundingBox(mensaje_peque, rTmp1.x, rTmp1.y+rTmp1.height);
////		fontPeque.drawString(mensaje_peque, rTmp2.x-rTmp2.width/2, rTmp2.y);//+rTmp.height+30);
//		ofRectangle rTmp2 = font.getStringBoundingBox(mensaje_peque, 0,0);
//		fontPeque.drawString(mensaje_peque, ofGetWidth()/2-200, 400);//+rTmp.height+30);
////		ofRect(rTmp2);
		
		
		
	}
};



class scanner_faces : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	
	void keyPressed(int key);
	
#ifdef SYPHON_CAM
    ofxSyphonClient camSyphon;
    ofFbo           fboSyphon;
#else
	ofVideoGrabber cam;
//	ofQTKitGrabber cam;
#endif
    
    // Dimensiones de la pantalla
    float wMonitor, hMonitor;
    
	float esc_HD2Mon;
	
	// Dimensiones de la imagen de caámra a tratar
    float wCam, hCam;
    
	ofxCv::ObjectFinder finder;
	ofxCv::ObjectFinder finderEyes;
	bool		swHaarEyes;
	void		setupObjFinders();
	void		update_haarFinder();
	void		draw_haarFinder();
	
	float finderSmoothRate;

	//	***************
	// Fondo
	ofImage	fondo;
	//	***************
	ofRectangle	rectCamera;
	ofRectangle rectMensajes;
	
	
	Marco marco;
	
	// fbo's
	ofFbo	fboHaar;
	ofFbo	fboFT;
	
	ofFbo	fboScan;
	
	bool	doProjectFT;
	
	bool	doHaarFace;
	bool	doFaceTracker;
	bool	doDrawHaarFace, doDrawFaceTracker;
	
	bool	bFlipH, bFlipV;
	bool	swFullScreen;
	
	// FaceTracker
	ofxFaceTrackerThreaded tracker;
	
	// ofImage para pasar a FT
	ofImage	camImage;
	
	void			update_faceTracker();	
	ofMesh			LEyeRect, REyeRect, mouthRect;
	ofVec2f			position;
	float			scale;
	ofMatrix4x4		rotationMatrix;
	void			drawFaceTracker();
	
	void drawFT();

	
	enum { FT_LINES, FT_WIREFRAME, FT_TEXTURE};
	int modoDrawFT;
		
	
	//
    void    setupVarios();

	vector<ofColor> colores;
	
	// Test Haar Models
	vector<string> modos_haar;
	int id_modo_haar_act;
	
	
	
	
	// ESCENA
	void setupScn();
	// modos escena
	enum {	SCN_WAITING, // sistema en reposo
		SCN_PRESCAN,	// detecta a alguien. Propone acción. 
		SCN_SCAN,	// Animacion de escaneado.
		SCN_FIN	// Proceso escaneado terminado OK y Se ve el resultado en pantalla
	};
	int scnAct;	
	string nombreEscena;
	
	void setupEscenas();
	void setEscena(int idScena);
	void nextEscena();
	void updateEscena();
	
	int numErrores;
	int NUMERRORESMAX;
	
	// caras detectadas
	int 	contCaras;
	int		CONT_CARAS_LIM;
	bool	bFaceDetected;
	bool	bFTDetected;
	
	bool	hayPersona();
	bool	posicionOK();
	
	// mensajes prescan
	vector<MensajeTime> mensajesT;
	vector<string> mensajes;
	void setupMensajes();
	int idMensajeAct;
	
	int idMsgShow;
	
	
	ofTrueTypeFont	fontMensajes, fontMensajes_peque, fontData;
//	ofxFontStash	fontX_peq;
	
	// Control cambio de escena
	bool bScnOK;	
	
	// Limites tiempo
	int tWaiting;
	int tPreScan;
	int tScan;
	int tFin;

	
	int			TIME_MSG;
	int			TIME_HOLA;
	int			TIME_ENCUADRE;
	int			TIME_FOTOS;
	int			TIME_FOTOS_FOCUSPOINT;
	int			TIME_SCAN;
	int			TIME_FIN;

	ofxTimer	timerMsg;	// timer para los mensajes informativos
	ofxTimer	timerScan;	// tiempo que se toma la animacion de escaneo: paso de Haar a FT
	ofxTimer	timerFin;	// Tiempo final para observar resultado
	
//	void		setup
	void		tiempo_Msg(ofEventArgs &e);
	void		setupTimers();
	void		stopTimers();
	
	void		tiempo_Hola(ofEventArgs &e);
	void		tiempo_Encuadre(ofEventArgs &e);
	void		tiempo_Foto(ofEventArgs &e);
	void		tiempo_Scan(ofEventArgs &e);
	void		tiempo_Fin(ofEventArgs &e);	
	
	
	int			ladoCaraMosaico;
	int			WSCAN, HSCAN;
	ofRectangle rectHaar, rectHaarAmpl;
	ofPolyline	leftEye, rightEye, featMouth;	// guarda las features detectadas

	
	// flags para mostrar animaciones de analisis
	bool guidesFace, guidesEyes, guidesBoca, guidesSkin;
	void draw_featEnMarco(ofPolyline feat, int gap, bool uu, bool rr);
	bool show_HaarRect;
	
	
	// Fase fotos
	vector<ofPoint>	focusPts;
	int		NFOTOSMAX;
	int		nFotoAct;
	void	setupFotos();
	
	void	hacerFoto();
	void	hacerFoto_haarViz();
	void	hacerFoto_CamMesh();
	int tLastCambio;

	
	
	// Animacion Escena Wait
	vector<ofPath>	shapes_Wait;
	void 		update_ScnWait();
	void 		draw_ScnWait();
	
	
	// - - - - - - - Shader escaneo
	
	
	
	// grabar imagenes
	void saveScanImg2HaarViz(ofImage &imgScan, int nPers);	
	
	void saveScanImg(ofImage &imgScan, int nPers, int nImg);
	// nImg es un numero de sequencia de imagen del rostro:
	// 0: centro
	// 1: UL, 2: UR, 3:DR, 4:DL 
	int nZonaAct;
	int nPersonaAct;
	ofImage faceImage, faceImageAmpl;
	ofRectangle faceRect, faceRectAmpl;
	ofRectangle faceRect_Tgt;	// este guarda el rect del Haar exacto
	
	
	bool caraOut;
	
	ofSoundPlayer	snd_click;
	
	// Enviar orden a Arduino para que encienda/apague la luz
	ofSerial serial;
    void     setupSerial();
    void     update_Serial();
	void     setLight(bool swlight);
	bool	 bSendSerialMessage;
	char	 charSend;
    

	// mostrar info
	bool	swShowInfo;
    
	
	// GUI
    ofxUICanvas *gui; 
	void setupGUI();
    void exit(); 
    void guiEvent(ofxUIEventArgs &e);
	
	
};
