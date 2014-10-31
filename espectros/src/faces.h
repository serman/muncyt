//
//  faces.h
//  mainScreen
//
//  Created by Sergio Galan on 7/28/14.
//
//

#ifndef __mainScreen__faces__
#define __mainScreen__faces__

#include <iostream>
#include <iostream>
#include "ofxUI.h"
#include "ofMain.h"
#include "ofxSyphon.h"
#include "ofxCv.h"
#include "mosaicPlayer.h"
#include "ofxTuio.h"
#include <ofMath.h>
#include "ofxScene.h"
#include "Cara.h"
#include "consts.h"
#define NCARAS 8
class faces : public ofxScene{
    
public:
    void setup();
    void update(float );
    void draw();
    
    ofxSyphonClient *mSyphonClient;
    ofxSyphonClient *mSyphonClient2;
    
    ofFbo fbo; // with alpha
    ofPixels cameraPixels;
    ofImage cameraImg;
    
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
	void keyPressed(int key);
    
    void cargaCaras();
	
	bool	swEnLinea;
	bool	swWireframe;
	
    Cara	cara;
    
    vector<Cara> caras;
    void drawMemoryLoading();
private:
    ofImage backgroundImg;
    bool swTexture;
    enum statuses{LOADINGFACE, FACE_FINISHED, PREPARE_NEW_FACE, ALL_FACES_LOADED};
    int status;
    int currentFace;
    int timeStatusChanged;
    ofTrueTypeFont fuente;
    long time_init_scene;
    bool end_of_scene_sent;
    void showDebug();
    bool bshowDebug;
    
};
#define LOADINGFACE_TIME 5000
#define LOADEDFACE_TIME 2500
#define ALL_FACES_LOADED_TIME 8000

#endif /* defined(__mainScreen__faces__) */
