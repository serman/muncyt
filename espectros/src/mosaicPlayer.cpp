//
//  mosaicRecorder.cpp
//  mainScreen
//
//  Created by Sergio Galan on 5/13/14.
//
//

#include "mosaicPlayer.h"
#include <math.h>
mosaicPlayer::mosaicPlayer(){
    
    
}


void mosaicPlayer::setup(){
    for(int i=0; i<MAXVIDEOS; i++){
        ofQTKitDecodeMode decodeMode = OF_QTKIT_DECODE_PIXELS_AND_TEXTURE;
        videos[i].setPixelFormat(OF_PIXELS_RGBA);
        videos[i].loadMovie(path+filename+ofToString(i)+ext ,decodeMode);
        videos[i].setLoopState(OF_LOOP_NORMAL);
        if(videos[i].isLoaded())
	        videos[i].play(); 
    }    
}


void mosaicPlayer::drawAll(int x0, int y0){
    int nRows=3;
    //cout << ofGetWidth();
    int x_size=ofGetWidth()/(nRows);//ancho por video
    float ratio=x_size/videos[0].getWidth(); //factor de escala >1 aumento <1 diminuyo
    int y_size=videos[0].getHeight()*ratio;
    //int perRow=(int)ofGetWidth()/x_size;
    for(int i=0; i<6; i++){
        if(videos[i].isLoaded()){
	        videos[i].draw(x0+(i%nRows)*x_size,y0+ floor(i/nRows) * y_size, x_size, y_size);    
        }
    }
    
}

void mosaicPlayer::draw(int index){
    
}

void mosaicPlayer::update(){
    for(int i=0; i<MAXVIDEOS; i++){
        if(videos[i].isLoaded())
	        videos[i].update();
    }
    
}

void mosaicPlayer::replaceVideo(int index){
    
    
}