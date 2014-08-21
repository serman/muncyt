//
//  juego1_red.cpp
//  mainScreen
//
//  Created by Sergio Galan on 8/14/14.
//
//

#include "juego1.h"
void juego1::tuioAdded(ofxTuioCursor &tuioCursor){
    ofPoint p= convertPoint(tuioCursor.getX(), tuioCursor.getY());
    if(p.x == -1 && p.y == -1){
        return;
    }else{
        addObstacle(p , tuioCursor.getSessionId() );
    }
}

void juego1::tuioUpdated(ofxTuioCursor &tuioCursor){
    ofPoint p= convertPoint(tuioCursor.getX(), tuioCursor.getY());
    if(p.x == -1 && p.y == -1){
        hideObstacle(tuioCursor.getSessionId() );
    }else{
        updateObstacle( p , tuioCursor.getSessionId() );
    }
	
}

void juego1::tuioRemoved(ofxTuioCursor &tuioCursor){
	removeObstacle(tuioCursor.getSessionId());
}

ofPoint juego1::convertPoint(float x1, float y1){
    if(y1*VIDEO_H*VIDEO_scale>(VIDEO_offset-30)
       && y1*VIDEO_H*VIDEO_scale>VIDEO_offset <  (VIDEO_offset + SCREEN_H+30) ){
        ofPoint p1=ofPoint((float)x1*SCREEN_W, (float)y1*VIDEO_H*VIDEO_scale-VIDEO_offset);
        return p1;
    }
    else return ofPoint(-1,-1);
}