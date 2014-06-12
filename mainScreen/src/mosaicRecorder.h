//
//  mosaicRecorder.h
//  mainScreen
//
//  Created by Sergio Galan on 5/13/14.
//
//

#ifndef __mainScreen__mosaicRecorder__
#define __mainScreen__mosaicRecorder__

#include <iostream>
#define IMAGES 1

#endif /* defined(__mainScreen__mosaicRecorder__) */

#include "ofMain.h"


#ifndef IMAGES
	#include "ofxVideoSaver.h"
#else
	#include "ofxImageSequenceRecorder.h"
#endif
#define MAX_VIDEOS 20
#define VIDEOSIZE 128

class mosaicRecorder {
public:
    int current_video=0;
    
    void setup(){
        for (int i=0; i<MAX_VIDEOS;i++){
            ofDirectory dir((ofToDataPath("recordings/"+filename+"_"+ ofToString(i))));
            if(!dir.exists() ){
               dir.create(true);
            }
            
        }
    }

    int x0,y0,w,h;
    mosaicRecorder(){
        croppedImg.allocate(VIDEOSIZE,VIDEOSIZE,OF_PIXELS_RGB);
    }
    	ofxImageSequenceRecorder videoSaver;


    void start(int _time, int _FPS,  ofPixels  &srcImage, int _w, int _h , int _x0=0, int _y0=0  ){
		ofLog() << "startRecorder " << current_video << "\n";
        FPS=_FPS;
        time=_time;
        currentImage=&srcImage;
        isRecording=true;
        x0=_x0; y0=_y0;w=_w;h=_h;
        
        //borramos el contenido del directorio si ya existía.
        ofDirectory dir(ofToDataPath("recordings/"+filename+"_"+ ofToString(current_video)));
        if(dir.exists() ){
            dir.remove(true);
        }
        dir.create(true);
        
        videoSaver.setPrefix(ofToDataPath("recordings/"+filename+"_"+ ofToString(current_video)+"/frame"));
        	// this directory must already exist
        	videoSaver.setFormat("jpg");
        
              //  videoSaver.setup(filename+"_"+ ofToString(current_video)+".mov", _w, _h,FPS );
        videoSaver.startThread();
       
    }
    
    void start (int _time, int _FPS,  ofImage  &srcImage,int _w=0, int _h=0, int _x0=0, int _y0=0  ){
        start(_time,_FPS,srcImage.getPixelsRef(),_x0,_y0,_w,_h);
        
        if(_w==0 || _h==0){
            _w=srcImage.getWidth();
            _h=srcImage.getHeight();
        }
    }
    void update(){
        if(isRecording==true){
            currentImage->cropTo( croppedImg ,x0,y0,128,128);
            videoSaver.addFrame(croppedImg);
            //cout << "added Frame";
            if(1000*(++frameCount)/FPS>time){ //ha terminado de grabar una secuencia
                
                cout << "stopRecorder\n";
                frameCount=0;
                videoSaver.counter=0;
                isRecording=false;
             //   if(++current_video>=MAX_VIDEOS) current_video=0;
            }
        }else{
            if(videoSaver.q.size()==0)
                videoSaver.stopThread();
        }
        
    }
    
private:
    ofPixels croppedImg;
	ofPixels *currentImage;
    ofVideoPlayer recordedVideoPlayback;

    string filename="video";

    bool isRecording=false;

    int FPS=25;
    int time=3000;
    int frameCount=0;

    
};