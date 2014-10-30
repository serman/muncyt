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
    fuente.loadFont("Helvetica-Bold.otf", 11);
}

void faces::draw(){
    ofRestoreTextureWrap();
    ofRestoreMinMagFilters();
    ofBackground(0);
    backgroundImg.draw(0,0,1280,720);

    ofPushMatrix();
    ofTranslate(100,100);
        ofFill();
        ofSetColor(85,98,112);
        ofSetRectMode(OF_RECTMODE_CORNER);
        ofRect(0,0,SCREEN_W,SCREEN_H);
    ofDisableAlphaBlending();

    ofDisableDepthTest();
	float scaleX = ofMap(ofGetMouseX(), 0,ofGetWidth(), 0.0, 10.0);
	float scaleY = ofMap(ofGetMouseY(), 0,ofGetHeight(), 0.0, 10.0);
	float scaleZ = 2.0;
	
	int nCaras = caras.size();
	float zz = -600;
    for(int i=0; i<=currentFace; i++){
    ofPushMatrix();
//             ofTranslate(ofGetMouseX(),ofGetMouseY(),0);
        ofTranslate(141,83);
        // ofRotateY(360.0/nCaras*i);
        ofTranslate( (SCREEN_W/4.5) * (i%4), floor(i/4)*(SCREEN_H/2), 50);
        
        ofScale(2.4,2.4,1.0);//scaleY);//1);
        ofSetColor(255);
        ofNoFill();
        /*if(swTexture) {
            caras[i].img.bind();
            caras[i].meanObjMesh.drawFaces();
            caras[i].img.unbind();
        }*/
        if(i==currentFace && status==LOADINGFACE){
            ofRotateY(ofMap(ofGetElapsedTimeMillis()-timeStatusChanged , 0,LOADINGFACE_TIME, 0, 3*360.0));
                        glLineWidth(2);
            int time1=ofClamp(ofGetElapsedTimeMillis()-timeStatusChanged, 0, LOADINGFACE_TIME);
            caras[i].objVbo.draw(GL_LINE_STRIP,
                                 0, ofMap(time1,0,LOADINGFACE_TIME,
                                          0,caras[i].objVbo.getNumVertices()
                                          )
                                 );
        } else if(i==currentFace && status==FACE_FINISHED) {
            glLineWidth(2);
            ofRotateY(ofMap(ofGetElapsedTimeMillis()-timeStatusChanged , 0,LOADEDFACE_TIME, 0, 1*360));
            caras[i].img.bind();
            caras[i].meanObjMesh.drawFaces();
            caras[i].img.unbind();
            //caras[i].objMesh.drawWireframe();
            caras[i].objVbo.draw(GL_LINE_STRIP,0,caras[i].objVbo.getNumVertices());
        }
        else if(i!=currentFace || status==ALL_FACES_LOADED){
            ofPushMatrix();
            
            glLineWidth(2);
            if(i%2==0){
                ofRotateX( ((ofGetElapsedTimeMillis()-timeStatusChanged)/(30+i*3)) %15);
            }else{
                ofRotateZ( ((ofGetElapsedTimeMillis()-timeStatusChanged)/(30+i*3)) %15);
            }
            caras[i].img.bind();
            caras[i].meanObjMesh.drawFaces();
            caras[i].img.unbind();
            //caras[i].objMesh.drawWireframe();
            caras[i].objVbo.draw(GL_LINE_STRIP,0,caras[i].objVbo.getNumVertices());
            ofPopMatrix();
        }
        ofPopMatrix();
        fuente.drawString("visitante #" + ofToString( (time_init_scene/1000) %1000 +i*7) , (SCREEN_W/4) * (i%4)+(SCREEN_W/16), -30+(1+floor(i/4))*(SCREEN_H/2));
        
    }//fin for que dibuja las caras
    if(currentFace<4) {
        ofPushMatrix();
        ofTranslate( (SCREEN_W/4),
                    3*(SCREEN_H/4) );
        ofRect(0,0,SCREEN_W/2,20);
        ofFill();
        ofSetColor(251,184,41);
        ofRect(1,
               1,
               ofMap(ofGetElapsedTimeMillis()-time_init_scene, 0, (LOADINGFACE_TIME+LOADEDFACE_TIME)*4, 0, SCREEN_W/2-2),
               18 );
                ofSetColor(255);
        fuente.drawString("Cargando patrones desde memoria", 40,15);
        ofPopMatrix();
    }


    
	
	ofSetColor(255);
    ofPopMatrix();//fin del translate al punto de pantalla

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
    switch(status){
        case LOADINGFACE:
            if(ofGetElapsedTimeMillis()-timeStatusChanged>LOADINGFACE_TIME){
                status = FACE_FINISHED;
                timeStatusChanged=ofGetElapsedTimeMillis();
            }
            break;
        case FACE_FINISHED:
            if(ofGetElapsedTimeMillis()-timeStatusChanged>LOADEDFACE_TIME){
                status = PREPARE_NEW_FACE;
                if(currentFace<NCARAS-1) {
                    currentFace++;
                }
                else{
                    status=ALL_FACES_LOADED;
                }
                timeStatusChanged=ofGetElapsedTimeMillis();
            }
        break;
        case PREPARE_NEW_FACE:
            status = LOADINGFACE;
            break;
        case ALL_FACES_LOADED:
            if(ofGetElapsedTimeMillis()-timeStatusChanged>ALL_FACES_LOADED_TIME){
                if(end_of_scene_sent==false){
                    ofSendMessage("endOfScene");
                    end_of_scene_sent=true;
                }
            }
            break;
            
    }
    
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
    currentFace=0;
    timeStatusChanged=ofGetElapsedTimeMillis();
    time_init_scene=ofGetElapsedTimeMillis();
    status=LOADINGFACE;
    end_of_scene_sent=false;

}

void faces::exit_Escena(){
    
}

void faces::keyPressed(int key){
	if(key=='m') swEnLinea=!swEnLinea;
	else if(key=='w') swWireframe=!swWireframe;
	else if(key=='t') swTexture=!swTexture;
}