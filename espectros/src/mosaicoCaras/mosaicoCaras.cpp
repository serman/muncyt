//
//  glitch.cpp
//  mainScreen
//
//  Created by Sergio Galan on 7/28/14.
//
//

#include "mosaicoCaras.h"
#include "consts.h"


//using namespace ofxCv;
//using namespace cv;


void MosaicoCaras::setup(){
	
	// Ubicación
	rectPantalla = ofRectangle(100,100, SCREEN_W, SCREEN_H);
	rectMosaicoL = ofRectangle(rectPantalla.x, rectPantalla.y, LADO_CARA, LADO_CARA);
	rectMosaicoR = ofRectangle(rectPantalla.x+LADO_CARA, rectPantalla.y, LADO_CARA, LADO_CARA);
	
	cargarFiles();
	
	setupFinders();
	
	setupBandas();
	
	swDrawLineasBandas = false;
	swDrawPartido = false;

    backgroundImg.loadImage("fondos/Mosaico.png");
    
}

void MosaicoCaras::cargarFiles() {

    if(ofToString(getenv("USER"))=="instalaciones" || ofToString(getenv("USER"))=="panel" ){
        ofLogToFile( ofToString (getenv("HOME") ) + "/machine_v_espejo_log.txt", true);
    }
    
	string path = ofToString (getenv("HOME")) + "/fotosCarasMosaico/1x1/";
//    string path = "./images/imgs_faces/";
	ofDirectory dir;
	//only show jpg files
	//	dir.allowExt("jpg");
	//populate the directory object
	int nFiles = dir.listDir(path);
	
	ofLogVerbose("Listar directorio - "+dir.path());
	if(nFiles) {
		for(int i = 0; i < dir.numFiles(); i++){
			string fn = dir.getPath(i);
			ofLogNotice(ofToString(i)+": "+ fn);
			fileNames.push_back(fn);
			ofImage imgTmp;
			imgTmp.loadImage(fn);
			images.push_back(imgTmp);
		}
	}
	ofLogVerbose("Listar directorio - FIN");
	
	// Suponemos que solo llegan imágenes procesadas y cortadas LxL
	// Por si acaso se hace un resize a LADO_CARA
	isFacesFormat_OK = true;
	
	// Pasar imágenes a facesImgs
	if(isFacesFormat_OK) {
		for(int i=0; i<images.size(); i++) {
			
			if(images[i].width!=LADO_CARA ||  images[i].height!=LADO_CARA) {
				images[i].resize(LADO_CARA, LADO_CARA);
			}
			images_faces.push_back(images[i]);
			
		}
	}
	else {
		// analizar images, recortaralas y meterlas en images_faces
	}
}

void MosaicoCaras::setupFinders() {
	// Prepara para análisis y corte de imágenes
//	finder_ofxcv.setup("./haar/haarcascade_frontalface_default.xml");
//	//	finder_ofxcv.setPreset(ofxCv::ObjectFinder::Fast);
//	finder_ofxcv.setPreset(ofxCv::ObjectFinder::Accurate); // Accurate Sensitive
//	finder_ofxcv.setMultiScaleFactor(1.09);
	
	modoDetect = MODO_OFXCV; //MODO_FCTRACKER;
	
	// Face Tracker
//	tracker.setup();
//	tracker.setRescale(1.0);
	
	isAnalisisDone = false;
	
	//
	// FCTRACKER
	//
	// Esto por si hay que analizar 
	escAreaUp  = 0.0;//0.2/10.0;
	escAreaDwn = 2.2/10.0;
	escArea = (escAreaUp + escAreaDwn);
	
	
}

void MosaicoCaras::setupBandas() {
	
	
	// Lineas de corte de las bandas
	// medidas para análisis con FT y escAreaDwn=2.2
	float props[4] = {0.18, 0.27, 0.12, 0.43};
	
	vector<float> alturas;
	for(int i=0; i<4; i++) {
		alturas.push_back(props[i]);
	}
	// setupMosaicos();
	mosaicoL.setup(CASILLAS_4, LADO_CARA);
	mosaicoL.setAlturas(alturas);
	
	mosaicoR.setup(CASILLAS_4, LADO_CARA);
	mosaicoR.setAlturas(alturas);
	
	
	iFaceAct = 0;	// idx de la imagen capturada que se representa.
	
	
}





void MosaicoCaras::update(float f){
    
    tuioclient->getMessage();

    
	mosaicoL.update(images_faces.size());
	mosaicoR.update(images_faces.size());
	
	// Interaccion conTUIOS
	
	//
	//	ANALISIS DE IMAGEN PARA ANIM FT: COPIAR DEL FINAL DEL DOC ORIGINAL
	// 

	
}

void MosaicoCaras::draw(){
	ofBackground(ofColor::black);
    backgroundImg.draw(0,0,1280,720);
    
	ofPushStyle();
	ofNoFill();
	ofSetColor(0,0,255);
	
	// DIBUJAR MEZCLA DE IMAGENES
	if(images_faces.size()>0) {
		ofPushMatrix();
		ofTranslate(rectMosaicoL.x, rectMosaicoL.y, 0);
		ofSetColor(255);
		drawBandas(mosaicoL);		
		ofSetColor(255);		
		ofPopMatrix();
		
		ofPushMatrix();
		ofTranslate(rectMosaicoR.x, rectMosaicoR.y, 0);
		ofSetColor(255);
		drawBandas(mosaicoR);		
		ofSetColor(255);		
		ofPopMatrix();
	}
	//
	
	ofPopStyle();
	
	ofDrawBitmapString("fr: " + ofToString(ofGetFrameRate()), ofPoint(10,ofGetHeight()-30));
	
    //
//    list<ofxTuioCursor*>::iterator tobj;
//    list<ofxTuioCursor*> objectList = tuioclient->getTuioCursors();
//	for (tobj=objectList.begin(); tobj != objectList.end(); tobj++) {
//		ofxTuioCursor *blob = (*tobj);
//		int x1=blob->getX()*VIDEO_W;
//		int y1=blob->getY()*VIDEO_H;
//	   // drawBigPixels(x1,y1,(blob->width)*640,(blob->height)*480);
//	}
    
}

void MosaicoCaras::drawBandas(Mosaico mm) {
	for(int idBanda=0; idBanda<mm.bandas.size(); idBanda++) {
		images_faces[mm.ids[idBanda]].drawSubsection(mm.bandas[idBanda].x,
													 mm.bandas[idBanda].y,
													 mm.bandas[idBanda].width, 
													 mm.bandas[idBanda].height, 
													 mm.bandas[idBanda].x,
													 mm.bandas[idBanda].y);
		
		if(swDrawLineasBandas){
			ofPushStyle();
			
			ofNoFill();
			ofSetColor(255);
			ofRect(mm.bandas[idBanda]);
			ofPopStyle();
		}
	}
}


void MosaicoCaras::keyPressed(int key){
    if(key=='l') {
		swDrawLineasBandas=!swDrawLineasBandas;
	}
	else if(key=='p') {
		swDrawPartido=!swDrawPartido;
		if(swDrawPartido) {
			mosaicoL.tipo=CASILLAS_4X2;
			mosaicoR.tipo=CASILLAS_4X2;
		}
		else {
			mosaicoL.tipo=CASILLAS_4;
			mosaicoR.tipo=CASILLAS_4;
		}
	}
}	

//scene notifications
void MosaicoCaras::sceneWillAppear( ofxScene * fromScreen ){  // reset our scene when we appear
    init_Escena();
};


//scene notifications
void MosaicoCaras::sceneWillDisappear( ofxScene * toScreen ){
	exit_Escena();
};

void MosaicoCaras::init_Escena(){
    
   }

void MosaicoCaras::exit_Escena(){
      
}




// - - - - - - - - 

/*
 
void MosaicoCaras::drawBigPixels(){
    for (int i=0;i<10; i++){
        int x1=ofRandom(620);
        int y1=ofRandom(100);
        //long randn= remoteBlobImgPxl.getPixelIndex(,);
        ofColor c1= remoteBlobImgPxl.getColor(x1+10,y1);
        ofPushStyle();
        ofSetColor(c1);
        ofRect(x1,y1,20,20);
        ofPopStyle();
        //pasteInto
        // for(int j=0; j<20;j++)
        //     remoteBlobImgPxl[randn+j]=ofRandom(65000*4);
        
    }
}

void MosaicoCaras::drawSectionAlpha(){
    ofPushStyle();
    ofEnableAlphaBlending();
    ofSetColor(255,0,0,130);
    cameraImg.drawSubsection(0, 400, 600, 80, 300, 100);
    ofDisableAlphaBlending();
    ofSetColor(0,255,0,130);
    cameraImg.drawSubsection(400, 380, 240, 70, 600, 100);
    ofDisableAlphaBlending();
	
    ofPopStyle();
}

void MosaicoCaras::drawBigPixels(int x1, int y1,int sizex,int sizey){
	//long randn= remoteBlobImgPxl.getPixelIndex(,);
	ofColor c1= remoteBlobImgPxl.getColor(x1,y1);
	ofPushStyle();
	ofSetColor(c1);
	ofRect(x1,y1,sizex,sizey);
	ofSetColor(255,0,0,10);
	ofRect(x1,y1,sizex,sizey);
	ofPopStyle();
	//pasteInto
	// for(int j=0; j<20;j++)
	//     remoteBlobImgPxl[randn+j]=ofRandom(65000*4);
	
}
 
 void MosaicoCaras::draw(){
 
 //
 list<ofxTuioCursor*>::iterator tobj;
 list<ofxTuioCursor*> objectList = tuioclient->getTuioCursors();
 //    fboGlitches.begin();
 //    ofEnableAlphaBlending();
 //    ofSetColor(255,255,255,2);
 //    ofRect(0,0,VIDEO_W,VIDEO_H);
 //    ofSetColor(255,255,255,255);
 //        for (tobj=objectList.begin(); tobj != objectList.end(); tobj++) {
 //            ofxTuioCursor *blob = (*tobj);
 //            int x1=blob->getX()*VIDEO_W;
 //            int y1=blob->getY()*VIDEO_H;
 //           // drawBigPixels(x1,y1,(blob->width)*640,(blob->height)*480);
 //        }
 //        ofDisableAlphaBlending();
 //   
 //    ofPushStyle();
 //    ofPushMatrix();
 //    if(ofRandom(1.0)<0.5){
 //        ofSetColor(255,0,0);
 //    }
 //    else{
 //         ofSetColor(0,255,0);
 //    }
 //    ofRotate(ofRandom(-20,20));
 //
 //    mSyphonClient2->draw(0,0,VIDEO_W,VIDEO_H);
 //        ofPopMatrix();
 //    ofPopStyle();
 //    fboGlitches.end();
 //    fboGlitches.draw(0, 0);
 //    drawSectionAlpha();
 //    drawBigPixels();
 
 }
 


*/

