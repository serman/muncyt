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


#endif
#define RECORDINGTIME 3000
#define RECTANGLESIZE 80
#define TUIOMODE
class moveAndRecord {
    public:
    ofRectangle triggerRectangle;
   // bool blobInSquare=false;
	long timeLastDetection=0;
    long timeHidden=0;
    enum statesDrawing{blobInSquare,blobOutSquare,hidden};
    int state=blobOutSquare;
    //este es el "update"
#ifndef TUIOMODE
    bool detectBlobinSquare(ofxBlobTracker &blobTracker){
        if(state==blobInSquare){
            if(ofGetElapsedTimeMillis()-timeLastDetection > RECORDINGTIME){
                triggerRectangle.setPosition( ofRandom(0,640-RECTANGLESIZE), ofRandom(0,480-RECTANGLESIZE) );
                state=hidden;
                timeHidden=ofGetElapsedTimeMillis();
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
#else
    bool detectBlobinSquare(list<ofxTuioObject*> objectList){
        if(state==blobInSquare){
            if(ofGetElapsedTimeMillis()-timeLastDetection > RECORDINGTIME){
                triggerRectangle.setPosition( ofRandom(0,640-RECTANGLESIZE), ofRandom(0,480-RECTANGLESIZE) );
                state=hidden;
                timeHidden=ofGetElapsedTimeMillis();
            }
            return false;
        }
        else if (state == hidden){
            if(ofGetElapsedTimeMillis()- timeHidden >10000){
                state=blobOutSquare;
            }
        }
        
        list<ofxTuioObject*>::iterator tobj;
        for (tobj=objectList.begin(); tobj != objectList.end(); tobj++) {
			ofxTuioObject *blob = (*tobj);
            //cout << "blob size" << blobTracker.trackedBlobs.size() << "\n";
            if ( triggerRectangle.inside( blob->getX()*640, blob->getY()*480) ){
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

        ofTranslate(194,139);

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
        triggerRectangle=ofRectangle(100,100,RECTANGLESIZE,RECTANGLESIZE);
    };
    
    
	private:


};