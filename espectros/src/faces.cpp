//
//  faces.cpp
//  mainScreen
//
//  Created by Sergio Galan on 7/28/14.
//
//

#include "faces.h"
void faces::setup(){
    swEnLinea = true;
	swWireframe = false;	
	// cargar caras
	cargaCaras();
    backgroundImg.loadImage("fondos/juego1.png");
}

void faces::cargaCaras() {
//	string caraNombre = "capturas/imagen_0";
//	cara.setup(caraNombre);
//	caras.push_back(cara);
    
	for(int i=0; i<NCARAS; i++) {
		string caraNombre = "capturas/imagen_"+ofToString(i%6);
		Cara caraTmp;
		caraTmp.setup(caraNombre);
		caras.push_back(caraTmp);

	}
    swWireframe=true;
    swEnLinea=true;
}

void faces::draw(){
    ofBackground(0);
    backgroundImg.draw(0,0,1280,720);

    ofPushMatrix();
    ofTranslate(100,100);
        ofFill();
        ofSetColor(0,0,0,255);
        ofSetRectMode(OF_RECTMODE_CORNER);
        ofRect(0,0,SCREEN_W,SCREEN_H);
    ofDisableAlphaBlending();

    ofDisableDepthTest();
	float scaleX = ofMap(ofGetMouseX(), 0,ofGetWidth(), 0.0, 10.0);
	float scaleY = ofMap(ofGetMouseY(), 0,ofGetHeight(), 0.0, 10.0);
	float scaleZ = 2.0;
	
	int nCaras = caras.size();
	
	float zz = -600;
	
	if(swEnLinea) {
        // EN LINEA
		for(int i=0; i<NCARAS; i++){
        ofPushMatrix();
//             ofTranslate(ofGetMouseX(),ofGetMouseY(),0);
            ofTranslate(141,83);
            // ofRotateY(360.0/nCaras*i);
			ofTranslate( (SCREEN_W/4.5) * (i%4), floor(i/4)*(SCREEN_H/2), 50);
            //ofScale(scaleX,scaleY,1);
           // cout << floor(i/4);
            
			ofRotateY(ofMap(ofGetMouseX(), 0,ofGetWidth(), -180, 180.0)*0.5 + 180);

			//	ofRotateX(ofMap(ofGetMouseY(), 0,ofGetHeight(), -180, 180.0)*0.5);
			ofScale(2.4,2.4,1.0);//scaleY);//1);
			ofSetColor(255);
            ofNoFill();
            
            if(swTexture) {
				caras[i].img.bind();
				caras[i].meanObjMesh.drawFaces();
				caras[i].img.unbind();

			}
			if(swWireframe){
//                caras[i].objVbo.draw(GL_LINES, ofGetFrameNum()%caras[i].objVbo.getNumVertices(), 30);
                glLineWidth(2);
            caras[i].objMesh.drawWireframe();
            }
        ofPopMatrix();
		}
	}
		
	
	ofSetColor(255);
	ofLine(0,ofGetHeight()/2, ofGetWidth(),ofGetHeight()/2);
	ofLine(0,ofGetHeight()*5/6, ofGetWidth(),ofGetHeight()*5/6);
	ofLine(ofGetWidth()/2,0,ofGetWidth()/2,ofGetHeight());
	ofLine(ofGetWidth()/2,0,ofGetWidth()/2,ofGetHeight());
    
    ofPopMatrix();
	ofDrawBitmapString("'m' para cambiar modo de presentación", 20, 20);
	ofDrawBitmapString("'w' para dibujar wireframe", 20, 35);
	ofDrawBitmapString("'t' para dibujar texturas", 20, 50);
    
    ofDrawBitmapString(ofToString(ofGetMouseX()) +" Y: " + ofToString(ofGetMouseY()) , 20, 55);
    /*for(int i=0; i<nCaras; i++){
        caras[i].img.draw(i*300,100);
    }*/
}

void faces::drawMemoryLoading(){
    
    
}

void faces::update(float f){
    
    
}

//scene notifications
void faces::sceneWillAppear( ofxScene * fromScreen ){  // reset our scene when we appear
    init_Escena();
};


//scene notifications
void faces::sceneWillDisappear( ofxScene * toScreen ){
	exit_Escena();
};

void faces::init_Escena(){
}

void faces::exit_Escena(){
    
}

void faces::keyPressed(int key){
	if(key=='m') swEnLinea=!swEnLinea;
	else if(key=='w') swWireframe=!swWireframe;
	else if(key=='t') swTexture=!swTexture;
}