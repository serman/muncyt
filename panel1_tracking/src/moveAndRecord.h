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
#include "ofxBlobTracker.h"

#endif
#define RECORDINGTIME 3000
#define RECTANGLESIZE 160
//en 1280x720 8 columnas 4 filas

class moveAndRecord {
    public:
    ofRectangle triggerRectangle;
   // bool blobInSquare=false;
	long timeLastDetection=0;
    long timeHidden=0;
    enum statesDrawing{blobInSquare,blobOutSquare,hidden};
    int state=blobOutSquare;
    int squareNumber=0;
    //este es el "update"
    bool detectBlobinSquare(ofxBlobTracker &blobTracker){
        if(state==blobInSquare){
            if(ofGetElapsedTimeMillis()-timeLastDetection > RECORDINGTIME){
                //triggerRectangle.setPosition( ofRandom(0,320-RECTANGLESIZE), ofRandom(0,240-RECTANGLESIZE) );
                triggerRectangle.setPosition( ofRandom(0,320-RECTANGLESIZE), ofRandom(0,240-RECTANGLESIZE) );
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
            if ( triggerRectangle.inside( blobTracker[j].lastCentroid.x*320, blobTracker[j].lastCentroid.y*240) ){
                state= blobInSquare;
                timeLastDetection=ofGetElapsedTimeMillis();
                return true;
            }
        }
        return false;
    };
    
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