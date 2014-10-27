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



#include "ofMain.h"
#include "ofxXmlSettings.h"

#ifndef IMAGES
	#include "ofxVideoSaver.h"
#else
	#include "ofxImageSequenceRecorder.h"
#endif
#define MAX_VIDEOS 4
#define VIDEOSIZE_X 100
#define VIDEOSIZE_Y 140

static ofEvent<int> finishedRecordingEvent;
//static ofEvent<int> recordingAborted;

class mosaicRecorder {
public:

    int current_video=0; //identificador del video que se está grabando
    vector<ofPoint> points;
    ofxImageSequenceRecorder videoSaver;
    
    void setup(){
        for (int i=0; i<MAX_VIDEOS;i++){
            ofDirectory dir((ofToDataPath("recordings/"+filename1+"_"+ ofToString(i))));
            if(!dir.exists() ){
               dir.create(true);
            }
        }
        isRecording=false;
    }

    int x0,y0,w,h;
    mosaicRecorder(){
        croppedImg.allocate(VIDEOSIZE_X,VIDEOSIZE_Y,OF_PIXELS_RGB);
    }



    void start(int _time, int _FPS,  ofPixels  &srcImage, int _w, int _h , int _x0=0, int _y0=0  ){ //graba en la carpeta video1

		ofLog() << "startRecorder " << current_video << "\n";
        FPS=_FPS;
        time=_time;
        currentImage=&srcImage;
        isRecording=true;
        x0=_x0; y0=_y0;w=_w;h=_h;
        
        //borramos el contenido del directorio si ya existía.
        ofDirectory dir(ofToDataPath("recordings/video_"+ ofToString(current_video) + "_tmp"));
        if(dir.exists() ){
            dir.remove(true);
        }
        dir.create(true);
        
        videoSaver.setPrefix(ofToDataPath("recordings/video_"+ ofToString(current_video)+"_tmp/frame"));
        	// this directory must already exist
        	videoSaver.setFormat("png");
              //  videoSaver.setup(filename+"_"+ ofToString(current_video)+".mov", _w, _h,FPS );
        videoSaver.startThread();
		points.clear();

    }
    
    void start (int _time, int _FPS,  ofImage  &srcImage,int _w=0, int _h=0, int _x0=0, int _y0=0  ){
        start(_time,_FPS,srcImage.getPixelsRef(),_x0,_y0,_w,_h);
        
        if(_w==0 || _h==0){
            _w=srcImage.getWidth();
            _h=srcImage.getHeight();
        }
    }
    
    /*Returns true when recording hast just finished in this call. False otherwise*/
    bool update(  int x1=-1, int y1=-1 ,int _w=0, int _h=0){
        if(x1==-1 && y1 == -1){
        	x1=x0;
            y1=y0;
        }
        if(isRecording==true){
            if(1000*(++frameCount)/FPS>time){ //FIN. ha terminado de grabar una secuencia   si la grabación ha terminado correcta se graba en otra carpeta
                finishVRecording();
                return true;
            }
            else{
                points.push_back(ofPoint(x1,y1));
                currentImage->cropTo( croppedImg ,x1,y1,_w,_h);
                videoSaver.addFrame(croppedImg);
                //cout << "added Frame";
            }
        }else{
            if(videoSaver.q.size()==0){
              //  ofLog() << "La cola es 0. Cerrando thread \n";
                if(videoSaver.isThreadRunning() ){
                    videoSaver.stopThread();
                }
            }
        }
        return false;
    }
    
    
    void finishVRecording(){
        if(videoSaver.q.size()>0) return;
    //CHANGE CLASS STATUES
        ofLog() << "Recording finished. Success. stopRecorder\n";
        frameCount=0;
        videoSaver.counter=0;
        isRecording=false;

     //MOVE VIDEO TO ITS FINAL LOCATION
        
        
        ofDirectory dir(ofToDataPath("recordings/video_"+ ofToString(current_video)));
        if(dir.exists() ){
            dir.remove(true);
        }
        ofDirectory dir2(ofToDataPath("recordings/video_"+ ofToString(current_video)+"_tmp"));
        if(dir2.exists() ){
            dir2.renameTo(ofToDataPath("recordings/video_"+ ofToString(current_video)));
        }
     //SAVEXMLPOSITIONS
        ofxXmlSettings positions;
        positions.addTag("positions");
        positions.pushTag("positions");
        for(int i = 0; i < points.size(); i++){
            //each position tag represents one point
            positions.addTag("position");
            positions.pushTag("position",i);
            //so set the three values in the file
            positions.addValue("X", points[i].x);
            positions.addValue("Y", points[i].y);
            positions.popTag();//pop position
        }
                points.empty();
        positions.saveFile(ofToDataPath("recordings/positions"+ofToString(current_video)+".xml"));
        ofNotifyEvent(finishedRecordingEvent, current_video);
        if(++current_video>=MAX_VIDEOS) current_video=0;
        
        return true;

    }
    
    void abort(){//cancela la grabación
        frameCount=0;
        videoSaver.counter=0;
        isRecording=false;
        
    }
    bool isRecording=false;
private:
    ofPixels croppedImg;
	ofPixels *currentImage;
    ofVideoPlayer recordedVideoPlayback;

    string filename1="video";

    

    int FPS=25;
    int time=3000;
    int frameCount=0;

    
};

#endif /* defined(__mainScreen__mosaicRecorder__) */