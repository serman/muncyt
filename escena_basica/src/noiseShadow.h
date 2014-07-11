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
    
    void setup(){
        cursor_id=-1;
        //tangibleObject::setup();
        for(int i=0; i<10; i++)
            mesh.addVertex(ofVec3f( ofRandom(20),ofRandom(20) ));
        alpha=0;
        age=0;
        
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
    }
    
    void draw(){
        ofEnableAlphaBlending();
        ofSetColor (200,0,0,alpha);
        ofEllipse( x, y,40, 40);
        //movidillas
        ofPushMatrix();
        
        mcolor.set(0,200,0,alpha);
        ofSetColor(mcolor);
        ofTranslate(x-15, y-15);
        /*  mesh.setMode(OF_PRIMITIVE_LINES);
         mesh.draw();
         ofDisableAlphaBlending();*/
        ofPopMatrix();
        //  cout << alpha;
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
