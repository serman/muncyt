//
//  waves.h
//  escena_basica
//
//  Created by Sergio Galan on 7/11/14.
//
//

#ifndef __escena_basica__waves__
#define __escena_basica__waves__

#include <iostream>
#include <ofMain.h>

#include "consts.h"

class waves{
    
public:
    vector<ofVec3f> points;
    vector<ofVec3f> AbsPoints;
    

    ofPolyline curvas,curvas2;

    ofVec2f p1; bool p1set;
    ofVec2f p2; bool p2set;
    ofVec2f pInit;
    ofVec2f pEnd;
    ofVec2f xvector;
    float theta;// = 0.05;
    float dx;// = (2*PI / 500.0); // si t es mas pequeño hay mas ondas
    float a;//amplitud
    float n_ciclos;
    int npuntos;
    int rotation;
    float incr;//=-0.11; //esto define la frecuencia
    //gestion de punteros
    long p1_id;
    long p2_id;
    int waveID;
    
    bool isCompleted(){
        if(p1set&& p2set)
            return true;
        else return false;
    }
    
    bool isHalf(){
        if((p1set || p2set) && (p1set && p2set)==false  )
            return true;
        else return false;
    }
    
    bool contains(int s_id){
        if(p1_id==s_id){
            return true;
        }
        if(p2_id==s_id){
            return true;
        }
        return false;
    }
    
    void remove_id(int _id){
        if(p1_id==_id){
            p1_id=-1;
            p1set=false;
        }
        if(p2_id==_id){
            p2_id=-1;
            p2set=false;

        }
    }
    void clear(){
        p1_id=-1;
        p1set=false;
        p2_id=-1;
        p2set=false;
    }
    
    ofPoint getSinglePoint(){
        if(p1set==true){
            return p1;
        }else return p2;
    }
    int getSinglePointID(){
        if(p1set==true){
            return p1_id;
        }else return p2_id;
    }
    
    
    waves(){
        p1set=false;
        p2set=false;
        p1_id=-1;
        p2_id=-1;
       // isCompleted=false;
		theta = 0.05;
        n_ciclos=2;
		dx = n_ciclos*(2*PI / 500.0); // si t es mas pequeño hay mas ondas
		incr=-0.11; //esto define la frecuencia
        a=50;
    }
    waves(int mid){
        waveID=mid;
        waves();
    }
    
    void update(){
        curvas.clear();
        points.clear();
        curvas2.clear();
        AbsPoints.clear();
        float x1=theta;
        for (int i=0; i < npuntos; i+=1){//cos (i/(npuntos*PI))
            ofVec3f vector=ofVec3f(i,  a*cos ( x1 ),3);
            points.push_back(vector);
            curvas.addVertex(vector);
            x1+=dx;
            ofVec3f vectorT=vector.getRotated(0,0,rotation);
            vectorT=vectorT+pInit;
            AbsPoints.push_back(vectorT);
            curvas2.addVertex(vectorT);
        }
        theta += incr;
    }
    
    void setup(){
  
        /*for (int i=0; i < npuntos; i++){
            ofVec3f vector=ofVec3f(i,100*sin( (ofGetFrameNum()*freq +2*PI*i*1 /freq2) /( npuntos) ),3);
            //points.push_back(vector);
            AbsPoints.push_back(vector);
            curvas.addVertex(vector);
        }*/
        

    }
    
    void draw(){
ofSetColor(255,255,0);
        curvas2.draw();
        ofSetColor(0,255,0);
        ofCircle(pInit.x,pInit.y,4,4);
        ofSetColor(0,0,255);
        ofCircle(pEnd.x,pEnd.y,4,4);
        ofPushMatrix();
        ofTranslate(pInit.x, pInit.y);        
        ofSetColor(255,255,255);
        //ofLine(ofVec3f(0,0), ofVec3f(npuntos,0));
        ofRotateZ(rotation);
        
        ofLine(ofVec3f(0,0), ofVec3f(npuntos,0));
        
        curvas.draw();
        ofPopMatrix();
    }
    
    void addPoint(int _x, int _y,int _id){
        if(p1set==false){
            p1=ofVec2f(_x,_y);
            p1set=true;
            p1_id=_id;
        }
        else if(p1set==true && p2set==false){
            p2=ofVec2f(_x,_y);
            p2set=true;
            p2_id=_id;
        }
        else if(p1set==false && p2set==true){
            p1=ofVec2f(_x,_y);
            p1set=true;
            p1_id=_id;
        }
/*        else{ //ambos están puestos
            p1=ofVec2f(_x,_y);
            p1set=true;
            p2set=false;
        } */
        
        calcWave();
        if(p1set && p2set ) update();
    }
    void setFreq(float _f){
        incr=_f;
    }
    void setAmplitude(float _amp){
        a=_amp;
        
    }
    void calcWave(){
        if(p1set && p2set ){
            npuntos=int(p1.distance(p2));
            if(p1.distance(ofVec2f(0,0)) < p2.distance(ofVec2f(0,0) ) ){
                pInit=p1;
                pEnd=p2;
            }
            else{
                pEnd=p1;
                pInit=p2;
            }
            xvector=pEnd-pInit;
            ofVec2f ejeX0=ofVec3f(1,0);
            rotation=ejeX0.angle(xvector);
            // cout << pInit.x << " " << pInit.y << " -angu " << rotation << endl;
            
        
            dx = n_ciclos*(2*PI / npuntos);
        }
    }
    void setCiclos(float _n){
        n_ciclos= _n;
        dx = n_ciclos*(2*PI / npuntos);
    }
    bool updateCursor(int s_id, int _x, int _y){
        if(p1_id==s_id){
            p1.set(_x,_y);
            calcWave();
            return true;
        }
        if(p2_id==s_id){
            p2.set(_x,_y);
            calcWave();
            return true;
        }
        return false;
        
    }
    
    
};
#endif /* defined(__escena_basica__waves__) */