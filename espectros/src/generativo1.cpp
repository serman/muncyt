//
//  generativo1.cpp
//  mainScreen
//
//  Created by Sergio Galan on 7/28/14.
//
//

#include "generativo1.h"
void generativo1::setup(){
    fbo.allocate(SCREEN_W, SCREEN_H, GL_RGBA);
    //fboGeneral.allocate(640, 480, GL_RGB);
    cameraPixels.allocate(SCREEN_W,SCREEN_H,OF_PIXELS_RGBA);
    
}

void generativo1::keyPressed(int k){
    if(k=='t'){
        rotateAng+=2;
    }
}

void generativo1::update(float f){
    if(rotateAng!=0) rotateAng+=4;
    if(rotateAng==180) rotateAng=0;
    
}

void generativo1::draw(){
    fbo.begin();
    
    ofClear(0, 0, 0, 0);
        ofSetColor(255);
    mSyphonClient->drawSubsection(0,0,SCREEN_W,SCREEN_H,
                                  0,VIDEO_offset,VIDEO_W,SCREEN_H);
    fbo.end();
    fbo.readToPixels(cameraPixels);
    cameraImg.setFromPixels(cameraPixels);
    cameraImg.update();
    int blockSize=32;
    int blockSizeScaled=32/VIDEO_scale;
    ofSetColor(255);
    
    ofTranslate(100+blockSize/2,100+blockSize/2);

        for(int i=0; i< 24; i++){
            for (int j=0; j<12; j++){
//                fbo.draw(i*blockSize,j*blockSize,blockSize,blockSize);
/*        mSyphonClient->drawSubsection(i*blockSize,j*blockSize,
                                      blockSize,blockSize,
                                              i*blockSizeScaled,
                                      VIDEO_offset+j*blockSizeScaled,
                                      blockSizeScaled,
                                      blockSizeScaled);*/
                    ofPushMatrix();
                ofTranslate(i*blockSize,j*blockSize);
                if(i%4==0)
                    ofRotateX(rotateAng);
                cameraImg.drawSubsection(-blockSize/2,-blockSize/2,blockSize, blockSize, i*blockSize, j*blockSize,blockSize,blockSize);
               // if(i%4==0)                ofRotateX(-rotateAng);
                    ofPopMatrix();
            }
        }

    
}




//scene notifications
void generativo1::sceneWillAppear( ofxScene * fromScreen ){  // reset our scene when we appear
    init_Escena();
};


//scene notifications
void generativo1::sceneWillDisappear( ofxScene * toScreen ){
	exit_Escena();
};

void generativo1::init_Escena(){
}

void generativo1::exit_Escena(){
    
}

void generativo1::tuioAdded(ofxTuioCursor &tuioCursor){
    ofPoint p= convertPoint(tuioCursor.getX(), tuioCursor.getY());
    if(p.x == -1 && p.y == -1){
        return;
    }else{
      //  addObstacle(p , tuioCursor.getSessionId(),tuioCursor.width,
               //     tuioCursor.height );
    }
    //cout << ofToString(tuioCursor.height*VIDEO_H*VIDEO_scale) <<endl;
    // if (tuioCursor.getSessionId()%3==0)
    //    cout << tuioCursor.getSessionId() << " : " << tuioCursor.height*480  <<endl;
}

void generativo1::tuioUpdated(ofxTuioCursor &tuioCursor){
    ofPoint p= convertPoint(tuioCursor.getX(), tuioCursor.getY());
    if(p.x == -1 && p.y == -1){
      //  hideObstacle(tuioCursor.getSessionId() );
    }else{
        //updateObstacle( p , tuioCursor.getSessionId(),tuioCursor.width,
               //        tuioCursor.height );
    }
    // if (tuioCursor.getSessionId() %3==0)
    //  cout << tuioCursor.getSessionId() << " : " << tuioCursor.height*480  <<endl;
    //cout << ofToString(tuioCursor.height*VIDEO_H*VIDEO_scale) <<endl;;
	
}

void generativo1::tuioRemoved(ofxTuioCursor &tuioCursor){
	//removeObstacle(tuioCursor.getSessionId());
}
