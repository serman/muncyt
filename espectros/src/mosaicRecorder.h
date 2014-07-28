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
#include "ofxXmlSettings.h";

#ifndef IMAGES
	#include "ofxVideoSaver.h"
#else
	#include "ofxImageSequenceRecorder.h"
#endif
#define MAX_VIDEOS 4
#define VIDEOSIZE_X 100
#define VIDEOSIZE_Y 140

static ofEvent<int> finishedRecordingEvent;

class mosaicRecorder {
public:

    int current_video=0;
    vector<ofPoint> points;
    
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
        croppedImg.allocate(VIDEOSIZE_X,VIDEOSIZE_Y,OF_PIXELS_RGB);
    }
    	ofxImageSequenceRecorder videoSaver;


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
    bool update(int x1=-1, int y1=-1 ){
        if(x1==-1 && y1 == -1){
        	x1=x0;
            y1=y0;
        }
        
    	cout << "x: "<< x1 << "  ___ y: "<< y1<<"\n";
        if(isRecording==true){
            if(1000*(++frameCount)/FPS>time){ //FIN. ha terminado de grabar una secuencia   si la grabación ha terminado correcta se graba en otra carpeta
                finishVRecording();
                return;
            }
            else{
                points.push_back(ofPoint(x1,y1));
                currentImage->cropTo( croppedImg ,x1,y1,80,100);
                videoSaver.addFrame(croppedImg);
                //cout << "added Frame";
            }

        }else{
            if(videoSaver.q.size()==0){
                videoSaver.stopThread();
                
            }
        }
        return false;
        
    }
    
    void finishVRecording(){
        if(videoSaver.q.size()>0) return;
    //CHANGE CLASS STATUES
        cout << "stopRecorder\n";
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
    
    void cancel(){//cancela la grabación
        frameCount=0;
        videoSaver.counter=0;
        isRecording=false;
        
    }
    bool isRecording=false;
private:
    ofPixels croppedImg;
	ofPixels *currentImage;
    ofVideoPlayer recordedVideoPlayback;

    string filename="video";

    

    int FPS=25;
    int time=3000;
    int frameCount=0;

    
};

#endif /* defined(__mainScreen__mosaicRecorder__) */