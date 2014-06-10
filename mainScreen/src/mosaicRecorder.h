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
//#define IMAGES 1

#endif /* defined(__mainScreen__mosaicRecorder__) */

#include "ofMain.h"

#ifndef IMAGES
	#include "ofxVideoSaver.h"
#else
	#include "ofxVideoRecorder.h"
#endif



class mosaicRecorder {
public:
    void setup(){}
    ofImage croppedImg;
    int x0,y0,w,h;
    mosaicRecorder(){}
    #ifndef IMAGES
    	ofxVideoSaver videoSaver;
    #else
    	ofxVideoRecorder videoSaver;
    #endif

    void start(int _time, int _FPS,  ofImage *srcImage,int _x0=0, int _y0=0, int _w=0, int _h=0 ){
        if(_w==0 || _h==0){
            _w=srcImage->getWidth();
            _h=srcImage->getHeight();
        }
        cout << "startRecorder";
        FPS=_FPS;
        time=_time;
        currentImage=srcImage;
        isRecording=true;
        x0=_x0; y0=_y0;w=_w;h=_h;
        #ifndef IMAGES
        	videoSaver.setup(ofRectangle(_x0,_y0,_w,_h),OF_IMAGE_COLOR);
	        videoSaver.setPause(false);
        #else
                videoSaver.setup(filename+"_"+ ofToString(current_video)+".mov", _w, _h,FPS );
        #endif
       
    }
    void update(){
        if(isRecording==true){
            croppedImg.cropFrom( *currentImage,x0,y0,80,80);
            videoSaver.addFrame(croppedImg);
            if(1000*(++frameCount)/FPS>time){
                cout << "stopRecorder";
                frameCount=0;
                isRecording=false;

                #ifndef IMAGES
                //1st borramos el directorio si ya existe
                ofDirectory dir(filename+"_"+ ofToString(current_video));
                if(dir.exists() ){
					dir.remove(true);
                }
                    
                videoSaver.setPause(true);
                //se guardan las imgs
                videoSaver.saveToDisk(filename+"_"+ ofToString(current_video)+"/" );
                //videoSaver.reset();
				#else
					videoSaver.close();
				#endif
                if(++current_video>=MAX_VIDEOS) current_video=0;
            }
        }        
    }
    
private:
    ofVideoPlayer recordedVideoPlayback;
    int MAX_VIDEOS=10;
    string filename="videoRecorded";
    int current_video=0;
    bool isRecording=false;
    ofImage *currentImage;
    int FPS=25;
    int time=3000;
    int frameCount=0;

    
};