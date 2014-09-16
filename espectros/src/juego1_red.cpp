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
        addObstacle(p , tuioCursor.getSessionId(),tuioCursor.width,
                    tuioCursor.height );
    }
    //cout << ofToString(tuioCursor.height*VIDEO_H*VIDEO_scale) <<endl;
   // if (tuioCursor.getSessionId()%3==0)
    //    cout << tuioCursor.getSessionId() << " : " << tuioCursor.height*480  <<endl;
}

void juego1::tuioUpdated(ofxTuioCursor &tuioCursor){
    ofPoint p= convertPoint(tuioCursor.getX(), tuioCursor.getY());
    if(p.x == -1 && p.y == -1){
        hideObstacle(tuioCursor.getSessionId() );
    }else{
        updateObstacle( p , tuioCursor.getSessionId(),tuioCursor.width,
                       tuioCursor.height );
    }
   // if (tuioCursor.getSessionId() %3==0)
   //  cout << tuioCursor.getSessionId() << " : " << tuioCursor.height*480  <<endl;
    //cout << ofToString(tuioCursor.height*VIDEO_H*VIDEO_scale) <<endl;;
	
}

void juego1::tuioRemoved(ofxTuioCursor &tuioCursor){
	removeObstacle(tuioCursor.getSessionId());
}

//returns bloob position only if it is within the range
ofPoint juego1::convertPoint(float x1, float y1){
    if(y1*VIDEO_H*VIDEO_scale>(VIDEO_offset-30)
       && y1*VIDEO_H*VIDEO_scale>VIDEO_offset <  (VIDEO_offset + SCREEN_H+30) ){
        ofPoint p1=ofPoint((float)x1*SCREEN_W, (float)y1*VIDEO_H*VIDEO_scale-VIDEO_offset);
        return p1;
    }
    else return ofPoint(-1,-1);
}




void juego1::addObstacle(){
    float r = ofRandom(4, 20);		// a random radius 4px - 20px
    circles.push_back(ofPtr<ofxBox2dRect>(new ofxBox2dRect));
    circles.back().get()->setPhysics(enemyDensity, enemyBounce, enemyFriction);
    circles.back().get()->setup(box2d.getWorld(), ofGetMouseX(), ofGetMouseY(), 40, 10);
    datoObjeto *obstacle;
    obstacle = new datoObjeto;
    obstacle->tipo=BALL;
    obstacle->id=1;
    
    circles.back().get()->setData(obstacle);
}

void juego1::addObstacle(ofPoint p1, int m_id,float w, float h){
    float r = ofRandom(5, 6);		// a random radius 4px - 20px
    h=h*VIDEO_H*VIDEO_scale;
    w=w*VIDEO_W*VIDEO_scale;
    circles.push_back(ofPtr<ofxBox2dRect>(new ofxBox2dRect));
    circles.back().get()->setPhysics(enemyDensity, enemyBounce, enemyFriction);
    circles.back().get()->setup(box2d.getWorld(), p1.x+w/2, p1.y+h/2, w, h);
    
    datoObjeto *obstacle;
    obstacle = new datoObjeto;
    obstacle->tipo=OBSTACLE;
    obstacle->id=m_id;
    
    circles.back().get()->setData(obstacle);
}

void juego1::updateObstacle(ofPoint p1, int m_id){
   	for(int i=0; i<circles.size(); i++) {
        if( ( (datoObjeto *)circles[i].get()->getData())->id  == m_id){
            circles[i].get()->setPosition(p1.x+15, p1.y+50);
            datoObjeto *mdata=(datoObjeto *)circles[i]->getData();
            mdata->isHidden=false;
        }
    }
}

void juego1::updateObstacle(ofPoint p1, int m_id,float w, float h){
    // h=h*480;
    // w=w*640;
    int i=0;
   	for(i=0; i<circles.size(); i++) {
        if( ( (datoObjeto *)circles[i].get()->getData())->id  == m_id){
            if(ofGetFrameNum()%10==0){
                datoObjeto *mdata=(datoObjeto *) circles[i].get()->getData();
                delete mdata;
                circles.erase(circles.begin()+i);
                addObstacle(p1, m_id,w,h);
                //  cout << ofToString(w*640) << " " << ofToString(h *480);
            }else{
                int w= circles[i].get()->getWidth();
                int h=circles[i].get()->getHeight();
                
                circles[i].get()->setPosition(p1.x+w/2, p1.y+h/2);
                
                datoObjeto *mdata=(datoObjeto *)circles[i]->getData();
                mdata->isHidden=false;
            }
            break;
        }
    }
    //This only runs if the updated object wasn't present before because it started out of the screen
    if(i==circles.size())    addObstacle( p1,m_id,w,h);
}



void juego1::hideObstacle( int m_id){
   	for(int i=0; i<circles.size(); i++) {
        if( ( (datoObjeto *)circles[i].get()->getData())->id  == m_id){
            datoObjeto *mdata=(datoObjeto *)circles[i]->getData();
            mdata->isHidden=true;
        }
    }
}


void juego1::removeObstacle(int m_id){
    vector<ofPtr<ofxBox2dRect> >::iterator tobj;
    for ( tobj =  circles.begin();
         tobj != circles.end();
         tobj++ ) {
        datoObjeto *mdata=(datoObjeto *)tobj->get()->getData();
        if(  mdata->id== m_id){
            delete mdata;
            circles.erase(tobj);
            break;
        }
    }
}
