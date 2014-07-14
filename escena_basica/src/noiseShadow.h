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

class noiseShadow : public tangibleObject{
public:
    ofMesh mesh;
    int alpha;
    long age; //time of life
    ofPath mshape;
    ofColor mcolor;
    ofVec3f rotVect;//=ofVec3f(0,0,1);
    void setup(){
		rotVect=ofVec3f(0,0,1);
        cursor_id=-1;
        //tangibleObject::setup();
        for(int i=0; i<10; i++)
            mesh.addVertex(ofVec3f( ofRandom(20),ofRandom(20) ));
        alpha=0;
        age=0;
        mshape.lineTo(0,0,0);
        mshape.lineTo(30,5,0);
        mshape.lineTo(55,2,0);
        mshape.lineTo(54,23,0);
        mshape.lineTo(60,53,0);
        mshape.lineTo(45,50,0);
        mshape.lineTo(20,48,0);
        mshape.lineTo(10,30,0);
        mshape.lineTo(0,0,0);
        mshape.setColor(ofColor(0,0,0,alpha));
       // mshape.setClosed(true);
        
        
    }
    void setColor(ofColor _color){
        mcolor=_color;
        mshape.setFillColor(mcolor);
    }
    
    void update(){
        if(cursor_id!=-1){
            if(alpha<255) alpha+=20;
            else alpha=255;
            
        }else{
            if(alpha>0) alpha-=10; else alpha=0;
        }
        
        int size=20;
        
        if(age >0 ){
            
            size=MIN(70,size+((ofGetElapsedTimeMillis()-age)/300));
        }
        /* mesh.clear();
         for(int i=0; i<6; i++)
         mesh.addVertex(ofVec3f( ofRandom(size),ofRandom(size) ));*/
       // mshape.rotate(1,rotVect);
        mshape.setColor(ofColor(0,0,0,alpha));
    }
    
    void draw(){
        ofEnableAlphaBlending();
        ofFill();
        ofSetColor (255,0,0,alpha);
        ofPushMatrix();
        ofTranslate(x,y);
        scale(3);
        mshape.draw();
//        ofEllipse( x, y,40, 40);
        ofPopMatrix();
        //movidillas
        ofDisableAlphaBlending();
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
        
    }
    void touchUp(int s_id){
        if(s_id==cursor_id){
            cursor_id=-1;
        }
        age=0;
        
    }
    
};




#endif