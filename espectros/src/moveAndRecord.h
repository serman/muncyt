//
//  moveAndRecord.h
//  panel1_tracking
//
//  Created by Sergio Galan on 5/27/14.
//
//

#ifndef panel1_tracking_moveAndRecord_h
#define panel1_tracking_moveAndRecord_h
#include "ofMain.h"
#include <math.h>

#endif
#define RECORDINGTIME 5000
#define RECTANGLESIZE 128

//en 1280x720 8 columnas x 4 filas = 32 cuadraditos
//640 x 480 son 5 x casi 4

#define TUIOMODE
class moveAndRecord {
    public:
    ofRectangle triggerRectangle;
   // bool blobInSquare=false;
	long timeLastDetection, timeHidden;
    enum statesDrawing{blobInSquare,blobOutSquare,hidden};
    int state;
    int currentRect;
    //este es el "update"
#ifndef TUIOMODE
    
    bool detectBlobinSquare(ofxBlobTracker &blobTracker){
        if(state==blobInSquare){
            if(ofGetElapsedTimeMillis()-timeLastDetection > RECORDINGTIME){
                //triggerRectangle.setPosition( ofRandom(0,640-RECTANGLESIZE), ofRandom(0,480-RECTANGLESIZE) );
                
				int newXPos = floor(currentRect%5)*RECTANGLESIZE;
                int newYPos = floor(currentRect/5)*RECTANGLESIZE;
                
                triggerRectangle.setPosition(newXPos, newYPos );
                
                state=hidden;
                timeHidden=ofGetElapsedTimeMillis();
                if( ++currentRect >20 currentRect=0);
            }
            return false;
        }
        else if (state == hidden){
            if(ofGetElapsedTimeMillis()- timeHidden >10000){
                state=blobOutSquare;
            }
        }
    
        for (int j=0; j<blobTracker.trackedBlobs.size(); j++){
            //cout << "blob size" << blobTracker.trackedBlobs.size() << "\n";
            if ( triggerRectangle.inside( blobTracker[j].lastCentroid.x*640, blobTracker[j].lastCentroid.y*480) ){
                state= blobInSquare;
                timeLastDetection=ofGetElapsedTimeMillis();
                return true;
            }
        }
        return false;
    };
#else //MODO TUIO
    ofxTuioCursor* detectBlobinSquare(list<ofxTuioCursor*> objectList){
        if(state==blobInSquare){
            if(ofGetElapsedTimeMillis()-timeLastDetection > RECORDINGTIME){
				int newXPos = floor(currentRect%5)*RECTANGLESIZE;
                int newYPos = floor(currentRect/5)*RECTANGLESIZE;
                
                triggerRectangle.setPosition(newXPos, newYPos );
                
                state=hidden;
                timeHidden=ofGetElapsedTimeMillis();
                if( ++currentRect >20 )currentRect=0;

            }
            return NULL;
        }
        else if (state == hidden){
            if(ofGetElapsedTimeMillis()- timeHidden >10000){
                state=blobOutSquare;
            }
        }else{
            list<ofxTuioCursor*>::iterator tobj;
            for (tobj=objectList.begin(); tobj != objectList.end(); tobj++) {
                ofxTuioCursor *blob = (*tobj);
                //cout << "blob size" << blobTracker.trackedBlobs.size() << "\n";
                if ( triggerRectangle.inside( blob->getX()*640, blob->getY()*480) ){
                    state= blobInSquare;
                    timeLastDetection=ofGetElapsedTimeMillis();
                    return blob;
                }
            }
        }
        return NULL;
    }
    
    bool detectMouseinSquare(int mousex, int mousey){
        //
        if(state==blobInSquare){
            if(ofGetElapsedTimeMillis()-timeLastDetection > RECORDINGTIME){
                if( ++currentRect >=20 )currentRect=0;
				int newXPos = (currentRect%5)*RECTANGLESIZE;
                cout <<  newXPos <<" " << currentRect;
                int newYPos = floor(currentRect/5)*RECTANGLESIZE;
                triggerRectangle.setPosition(newXPos, newYPos );
                state=hidden;
                timeHidden=ofGetElapsedTimeMillis();
                
                
            }
            return false;
        }
        else if (state == hidden){
            if(ofGetElapsedTimeMillis()- timeHidden >2000){
                state=blobOutSquare;
            }
        }else{ //blobOutSquare Comprobamos si está dentro            
                if ( triggerRectangle.inside( mousex, mousey) ){
                    state= blobInSquare;
                    timeLastDetection=ofGetElapsedTimeMillis();
                    return true;
                }
            
        }
        return false;
    }

    
 #endif
    void draw(){
        
        if(state==hidden){
        	return;
        }
        ofPushMatrix();
        ofPushStyle();

        //ofTranslate(194,139);

        if(state==blobInSquare){
        	ofSetColor(255, 0, 0);
            if(ofGetFrameNum()%8<4){
             ofEllipse(triggerRectangle.x,triggerRectangle.y-10,8,8);
            }
            glPointSize(1.0);
            ofNoFill();
        }
        else{
        	ofSetColor(0, 255, 0);
            ofNoFill();
        }
        
        ofRect(triggerRectangle);
        ofPopStyle();
        ofPopMatrix();
    };
    

    
    void setup(){
        timeLastDetection=0;
        timeHidden=0;
        state=blobOutSquare;
        currentRect=0;
        triggerRectangle=ofRectangle(140,140,RECTANGLESIZE,RECTANGLESIZE);
    };
    
    
	private:


};