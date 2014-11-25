//
//  glitch.h
//  mainScreen
//
//  Created by Sergio Galan on 7/28/14.
//
//

#ifndef __mainScreen__mosaico__
#define __mainScreen__mosaico__

#include <iostream>
#include "ofxUI.h"
#include "ofMain.h"
#include "ofxSyphon.h"
#include "ofxCv.h"
#include "mosaicPlayer.h"
#include "ofxTuio.h"
#include <ofMath.h>
#include "ofxScene.h"
#include "Mosaico.h"
//#include "ofxOpenCv.h"
//#include "ofxCv.h"
//#include "ofxFaceTracker.h"

#define LADO_CARA 384

enum modos_deteccion {
	MODO_OFXCV,
	MODO_FCTRACKer
};

class MosaicoCaras : public ofxScene{
    
public:
    void setup();
    void update(float );
    void draw();
	
    void keyPressed(int key);
	
    ofxSyphonClient *mSyphonClient;
    ofxSyphonClient *mSyphonClient2;
    
    ofxTuioClient *tuioclient;
    
    void setTuioClient(ofxTuioClient *mtuioclient){
        tuioclient=mtuioclient;
    }
    
    void setSyphonClients( ofxSyphonClient *mSyphonClient_,ofxSyphonClient *mSyphonClient2_ ){
        cout << "settings syphon client" <<endl;
        mSyphonClient=mSyphonClient_;
        mSyphonClient2=mSyphonClient2_;
    }
    
    /*    void	tuioAdded(ofxTuioCursor & tuioCursor);
     void	tuioRemoved(ofxTuioCursor & tuioCursor);
     void	tuioUpdated(ofxTuioCursor & tuioCursor);
     */



    void sceneWillAppear( ofxScene * fromScreen );
    void sceneWillDisappear( ofxScene * toScreen );
    void init_Escena();
    void exit_Escena();
    
//    void drawBigPixels();
//    void drawBigPixels(int x1, int y1, int sizex, int sizey);
//    void drawSectionAlpha();
    

	
	// - - - - - MosaicoCaras - - - - - - 
	//
	
	// Ubicacion de los mosaicos
	ofRectangle	rectPantalla;
	ofRectangle	rectMosaicoL, rectMosaicoR;
	
	// Carga de imagenes
	vector<string>		fileNames;
	vector<ofImage>		images;
	void cargarFiles();
	
	// indica si las caras ya llegan recortadas
	// si es true, directamente se pasan al vector de caras a mezclar
	// si es false, se recortan las caras antes
	bool	isFacesFormat_OK;	// true por defecto
	
	// Acumular de imagenes de caras
	vector<ofImage>		images_faces;
	// Por si quisiera mostralas secuencialmente para lo que sea:
	int					iFaceAct, iFacePrev; 	
	
	// Mosaicos
	Mosaico	mosaicoL, mosaicoR;
	void	drawBandas(Mosaico mosaico);
		
	// Si hay que hacer análisis y procesado de imagenes
	bool			isAnalisisDone;
	// FINDERS
	void setupFinders();
	int modoDetect;	
	// MODO OFXCV
//	ofxCv::ObjectFinder	finder_ofxcv;
	// MODO FACETRACKER
	//		ofxFaceTrackerThreaded tracker, trackerFT;
//	ofxFaceTracker tracker, trackerFT;
	
	//		void			update_faceTracker();
	//		ofVec2f			position;
	//		float			scale;
	//		ofMatrix4x4		rotationMatrix;
	//		void			drawFaceTracker();
	
	ofRectangle	rectCara, rectCara_X;	
	
	void	setupBandas();
	bool	swDrawLineasBandas;
	
	// Para redimensionar las bandas a escala mayor
	// Solo en caso de que haya que analizar y preprocesar
	float escAreaUp;
	float escAreaDwn;
	float escArea;
	
	
	
	//
	//
	
private:
    
};


#endif /* defined(__mainScreen__glitch__) */
