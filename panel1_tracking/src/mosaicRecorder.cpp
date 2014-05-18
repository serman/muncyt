//
//  mosaicRecorder.cpp
//  mainScreen
//
//  Created by Sergio Galan on 5/13/14.
//
//

#include "mosaicRecorder.h"
#include <string>     // std::string, std::to_string
#include <iostream>   // std::cout

void mosaicRecorder::setup(){
    //
    //videoSaver.setup(ofRectangle(0,0,320,240),OF_IMAGE_COLOR);
}

 mosaicRecorder::mosaicRecorder(){

}

void mosaicRecorder::start(int _time, int _FPS,  ofImage *srcImage,int _w, int _h){
    if(_w==0 || _h==0){
        _w=srcImage->getWidth();
        _h=srcImage->getHeight();
    }
    cout << "startRecorder";
	FPS=_FPS;
    time=_time;
    currentImage=srcImage;
    isRecording=true;
	videoSaver.setup(filename+"_"+ ofToString(current_video)+".mov", _w, _h,FPS );
}

void mosaicRecorder::update(){
    if(isRecording==true){
	    videoSaver.addFrame(*currentImage);
        //cout << frameCount;
	    if(1000*(++frameCount)/FPS>time){
            cout << "stopRecorder";
    	    frameCount=0;
        	isRecording=false;
            //videoSaver.saveToDisk(filename+"_"+ ofToString(current_video) );
            //videoSaver.reset();
            videoSaver.close();
            if(++current_video>=MAX_VIDEOS)current_video=0;
	    }
    }

}

