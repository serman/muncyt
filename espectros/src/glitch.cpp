//
//  glitch.cpp
//  mainScreen
//
//  Created by Sergio Galan on 7/28/14.
//
//

#include "glitch.h"


void glitch::setup(){
    fbo.allocate(640, 480, GL_RGBA);
    fboGlitches.allocate(640, 480, GL_RGBA);
    //fboGeneral.allocate(640, 480, GL_RGB);
    remoteBlobImgPxl.allocate(640,480,OF_PIXELS_RGBA);
    fboGlitches.begin();
    ofClear(0,255);
    fboGlitches.end();
    
}

void glitch::update(float f){
    
    tuioclient->getMessage();
    
}

//bool ofPixels_::pasteInto(ofPixels_< PixelType > &dst, int x, int y)
//int ofPixels_::getPixelIndex(int x, int y)
void glitch::draw(){
    ofSetColor(255);
    fbo.begin();
    
    ofClear(0, 0, 0, 0);
    mSyphonClient->draw(0,0,640,480);
    fbo.end();
    fbo.readToPixels(remoteBlobImgPxl);
    cameraImg.setFromPixels(remoteBlobImgPxl);
    cameraImg.update();
    
    cameraImg.draw(0,0);

    //
    list<ofxTuioCursor*>::iterator tobj;
    list<ofxTuioCursor*> objectList = tuioclient->getTuioCursors();
    fboGlitches.begin();
    ofEnableAlphaBlending();
    ofSetColor(255,255,255,2);
    ofRect(0,0,640,480);
    ofSetColor(255,255,255,255);
        for (tobj=objectList.begin(); tobj != objectList.end(); tobj++) {
            ofxTuioCursor *blob = (*tobj);
            int x1=blob->getX()*640;
            int y1=blob->getY()*480;
           // drawBigPixels(x1,y1,(blob->width)*640,(blob->height)*480);
        }
        ofDisableAlphaBlending();
   
    ofPushStyle();
    ofPushMatrix();
    if(ofRandom(1.0)<0.5){
        ofSetColor(255,0,0);
    }
    else{
         ofSetColor(0,255,0);
    }
    ofRotate(ofRandom(-20,20));

    mSyphonClient2->draw(0,0,640,480);
        ofPopMatrix();
    ofPopStyle();
    fboGlitches.end();
    fboGlitches.draw(0, 0);
    drawSectionAlpha();
    drawBigPixels();
    
}

void glitch::drawBigPixels(){
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

void glitch::drawSectionAlpha(){
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

void glitch::drawBigPixels(int x1, int y1,int sizex,int sizey){
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

void drawMovement(){
    
    
}






//scene notifications
void glitch::sceneWillAppear( ofxScene * fromScreen ){  // reset our scene when we appear
    init_Escena();
};


//scene notifications
void glitch::sceneWillDisappear( ofxScene * toScreen ){
	exit_Escena();
};

void glitch::init_Escena(){
   }

void glitch::exit_Escena(){
      
}