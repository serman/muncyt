//
//  noiseShadow.h
//  escena_basica
//
//  Created by Sergio Galan on 7/11/14.
//
//

#ifndef escena_basica_noiseShadow_h
#define escena_basica_noiseShadow_h


#include <ofMain.h>
#include <math.h>
#include "ofxSvg.h"
class noiseShadow : public tangibleObject{
public:
    ofMesh mesh;
    int alpha1;
    long age; //time of life
    ofPath mshape;
    ofColor mcolor;
    ofVec3f rotVect;//=ofVec3f(0,0,1);
    bool isSingle;
    ofxSVG magneticFieldSvg;
    vector<ofPolyline> fieldLines;
    void setup(){
		rotVect=ofVec3f(0,0,1);
        cursor_id=-1;
        //tangibleObject::setup();
        for(int i=0; i<10; i++)
            mesh.addVertex(ofVec3f( ofRandom(20),ofRandom(20) ));
        alpha1=0;
        age=0;
     /*   mshape.lineTo(0,0,0);
        mshape.lineTo(30,5,0);
        mshape.lineTo(55,2,0);
        mshape.lineTo(54,23,0);
        mshape.lineTo(60,53,0);
        mshape.lineTo(45,50,0);
        mshape.lineTo(20,48,0);
        mshape.lineTo(10,30,0);
        mshape.lineTo(0,0,0);
        mshape.setColor(ofColor(0,0,0,alpha));*/
       // mshape.setClosed(true);
        
        magneticFieldSvg.load("magneticfield.svg");
        
    }
    void setColor(ofColor _color){
        mcolor=_color;
        mshape.setFillColor(mcolor);
    }
    
    void update(){
        if(cursor_id!=-1){
            if(alpha1<180) alpha1+=10;
            else alpha1=180;
            
        }else{
            if(alpha1>0) alpha1-=10; else alpha1=0;
        }
        
        int size=20;
        
        if(age >0 ){
            
            size=MIN(70,size+((ofGetElapsedTimeMillis()-age)/300));
        }
        /* mesh.clear();
         for(int i=0; i<6; i++)
         mesh.addVertex(ofVec3f( ofRandom(size),ofRandom(size) ));*/
       // mshape.rotate(1,rotVect);
       // mshape.setColor(ofColor(0,0,0,alpha));
    }
    
    void draw(){
        ofPushMatrix();
        ofPushStyle();
        ofEnableAlphaBlending();
        ofFill();
        ofSetColor (50,250,50,alpha1);
       // ofTranslate(x,y);
//        scale(3);
        ofEllipse( x-10, y-10,50, 50);
        ofSetColor (255,0,0,alpha1);
        ofDrawBitmapString(ofToString(x) + "," + ofToString(y),x+20,y+20 );
        ofTranslate(x,y);
       // drawEM();

        ofDisableAlphaBlending();
        
        ofPopStyle();
        ofPopMatrix();

    };
    
    void slide(float mx, float my, int s_id, int acc){
        if(s_id==cursor_id){
            x=mx;
            y=my;
        }
        
        
    };
    void touch(float mx, float my, int s_id){
        if(cursor_id==-1){
            cursor_id=s_id;
            age=ofGetElapsedTimeMillis();
            x=mx;
            y=my;
        }
        fieldLines.clear();
        ofSetColor(0,255,0);
        ofNoFill();
        // ofRect(0,0,length,10);
        // ofEllipse(length/2,-50, 80,100);
        
        int tra_y=(magneticFieldSvg.getHeight()*0.6)/2;
        int tra_x=(magneticFieldSvg.getWidth()*0.6)/2;
        
        ofPoint pto=ofPoint(-tra_x,-tra_y);
        for(int i=0; i< magneticFieldSvg.getNumPath(); i++){
            ofPath path1=magneticFieldSvg.getPathAt(i);
            path1.scale(0.6,0.6);
            path1.translate(pto);
            fieldLines.push_back( path1.getOutline()[0].getResampledByCount(15));
            //magneticFieldSvg.getPathAt(i).setColor(ofColor::red);
        }

    }
    
    
    
    void touchUp(int s_id){
        if(s_id==cursor_id){
            cursor_id=-1;
        }
        age=0;
        
    }
    
    ofPoint getDstPoint(int x1,int y1){
      //  x1=x1-x;
      //  y1=x1-x;
        ofPoint particlePoint=ofPoint(x1-x,y1-y); //particle point translated to 0,0
        int minDist=1000000000;
        ofPoint returnPoint=ofPoint(0,0);
        ofPoint tmpPoint;
        for(int i=0; i<fieldLines.size();i++){
            tmpPoint=fieldLines[i].getClosestPoint(particlePoint);
            if(tmpPoint.distance(particlePoint)<minDist){
                minDist=tmpPoint.distance(particlePoint);
                returnPoint=tmpPoint;
            }
        }
        returnPoint.x+=x;
        returnPoint.y+=y;
        return returnPoint;
    }
    
    ofPoint getDstPoint2(int x1,int y1){
        //  x1=x1-x;
        //  y1=x1-x;
        ofPoint particlePoint=ofPoint(x1,y1); //particle point translated to 0,0
        int minDist=1000000000;
        ofPoint returnPoint=ofPoint(0,0);
        ofPoint tmpPoint;
        for(int i=0; i<fieldLines.size();i++){
            tmpPoint=fieldLines[i].getClosestPoint(particlePoint);
            if(tmpPoint.distance(particlePoint)<minDist){
                minDist=tmpPoint.distance(particlePoint);
                returnPoint=tmpPoint;
            }
        }
        returnPoint.x+=x;
        returnPoint.y+=y;
        return returnPoint;
    }
    
    
    void drawEM(){
        ofSetColor(0,255,0);
        for(int i=0; i<fieldLines.size();i++){
            fieldLines[i].draw();
        }
    }
};




#endif
