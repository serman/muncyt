//
//  particleClouds.h
//  kinectExample
//
//  Created by Sergio Galan on 8/29/14.
//
//
#include "ofMain.h"
#include "Particle.h"
#include "ofxOpenNI2Grabber.h"
#include "ofxOpenNI2Recorder.h"
#include "extendedDepthSource.h"
#include "ofxUI.h"
#include "consts.h"
#ifndef kinectExample_particleClouds_h
#define kinectExample_particleClouds_h
#define SAMPLING 3

class particleClouds{
public:
    enum	{RUIDO=0, ESPEJO,DESAPARECE};
    ofCamera *cam;
    ofFbo fbo_rects;
// http://stackoverflow.com/questions/1701416/initialization-of-reference-member-requires-a-temporary-variable-c
// there is no default constructor
   // particleClouds(ofxOpenNI2Grabber &g, extendedDepthSource &s ) : oniCamGrabber(g),depthGenerator(s)  {
    particleClouds(){
        sampling=SAMPLING;
        cloudState=ESPEJO;
        mode=EM;
    }
    
   /* particleClouds(ofxOpenNI2Grabber &_oniCamGrabber, extendedDepthSource _depthGenerator){
        oniCamGrabber=_oniCamGrabber;
        depthGenerator= _depthGenerator;
    }*/
   
    //only call once but have to call it
    ofImage palete;
    void setup(int _w, int _h, float *_zMin, float *_zMax, ofxOpenNI2Grabber *_oniCamGrabber, extendedDepthSource *_depthGenerator, ofCamera *_cam){
        w=_w;
        h=_h;
        oniCamGrabber=_oniCamGrabber;
        depthGenerator= _depthGenerator;
        zMin=_zMin;
        zMax=_zMax;
        setupParticles();
        speed = 1.0;
        stopUmbral = 10000;
        alphaParticles = 255;
        cam=_cam;
        setupParticles();
        palete.loadImage("palete1.png");

    }
    void setupParticles(){
        //int w= oniSettings.width;
        //int h= oniSettings.height;

        particles.clear();
        int    sampling=SAMPLING;
        //Loop through all the rows
        numParticles = 0;
        //Loop through all the columns
        // Distribuir las particulas por la escena
        for ( int y = 0 ; y < h ; y+=sampling ){
            for ( int x = w ; x > 0 ; x-=sampling ){
              /*  float ang1 = ofRandom(PI);
                float ang2 = ofRandom(TWO_PI);
                float rr = 100;
                float rrho = rr*sin(ang1);
                particles.push_back(Particle(ofVec3f(rrho*cos(ang2),rrho*sin(ang2),-rr*cos(ang1)-1500) ,ofColor(255,255,255) ,x,y));*/
              //  ofVec3f p2=cam->screenToWorld(ofVec3f(x,y,0));
                ofVec3f v=ofVec3f(x,y,0);
#ifdef TESTMODE
                v.rotate(rotateX,ofVec3f(1,0,0));
                v.rotate(rotateZ,ofVec3f(0,0,1));
                v.x+=tranX;
                v.y+=tranY;
                v.z+=tranZ;
#else
                v.rotate(-1.82,ofVec3f(0,0,1));
                v.x+=-450;
                v.y+=-229;
                v.z+=757;
#endif
                particles.push_back(Particle(v , ofVec3f(0,ofRandom(-400,0),ofRandom(-400)), ofColor(255,255,255) ,x,y));
                numParticles++ ;
            }
        }
        ofLogVerbose() <<"particulas"<<numParticles;
    }
    
    void updateParticles(){
#ifdef TESTMODE
//        setupParticles();
#endif
            if(mode==NUCLEAR_DEBIL){
                updateDEBIL();
                
            }else if(mode==EM) updateEM();
    }
    
    void updateEM(){
        meshParticles.clear();
        ofVec3f mdestPoint;
        ofVec3f diff ;
        std::vector<Particle>::iterator p ;
        for ( p = particles.begin() ; p != particles.end() ; p++ )
        {
            if(cloudState==RUIDO){
                p->steer(p->spawnPoint, false, speed, stopUmbral );
                p->update();
                    p->color=ofColor(255,255,255,ofRandom(0,200));
            }else if(cloudState==ESPEJO){
                int distance=depthGenerator->currentRawPixels->getPixels()[p->_y * depthGenerator->width + p->_x];
                if(distance> *zMin && distance < *zMax) {
                    p->recentlyUsed=5;
                        mdestPoint=  oniCamGrabber->convertDepthToWorld(p->_x, p->_y);
                        p->steer(mdestPoint, true, speed, stopUmbral );
                        p->update();
                        p->color=ofColor(255,255,255,255);
                }else{ // si las particulas están mas lejos
                    /*
                    if(!p->recentlyUsed>0){
                        p->color=ofColor(255,0,0,0);
                    }else{
                        p->color=ofColor(255,0,0,255);
                        p->recentlyUsed--;
                    }*/
                    p->steer(p->spawnPoint, true, speed, stopUmbral );
                    p->update();
                    p->color=ofColor(255,255,255,ofRandom(0,160));
                }//ESPEJO
            }else if(cloudState==DESAPARECE){
                p->sandDown(acceleration/3,-800);
                p->update();
                    //p->position+=ofNoise( p->position.x,p->position.y,p->position.z)*30;
                    //   p->color=ofColor(255,255,255,alphaParticles);
                
                p->color=ofColor(255,255,255,alphaParticles);
            }
            meshParticles.addVertex(p->position);
            meshParticles.addColor(p->color);
        }//end for all points within vector
       
    } //end updateEM
    
    
    void updateDEBIL(){
        /*meshParticles.clear();
        ofVec3f mdestPoint;
        ofVec3f diff ;
        std::vector<Particle>::iterator p ;
        for ( p = particles.begin() ; p != particles.end() ; p++ )
        {
                if(ofRandomf()>0.5)
                    p->color.set(0);
                else
                    p->color.set(255);
                p->color=ofColor(255,0,0,255);
                meshParticles.addVertex(p->position);
                meshParticles.addColor(p->color);
        }*/
        
        
        for ( int y = 0 ; y < h ; y+=15 ){
            for ( int x = w ; x > 0 ; x-=15 ){
                int distance=depthGenerator->currentRawPixels->getPixels()[y * depthGenerator->width + x];
                ofSetColor(ofColor::fromHsb(ofRandom(100,200), 200, 200));
                ofRect(x,y,distance,15,15);
            }
        }
    }
    
    void drawWithRectangles(){
        for ( int y = 0 ; y < h ; y+=5 ){
            for ( int x = w ; x > 0 ; x-=5 ){
                  int distance=depthGenerator->currentRawPixels->getPixels()[y * depthGenerator->width + x];
                 if(distance> *zMin && distance < *zMax) {
                      ofVec3f mdestPoint=  oniCamGrabber->convertDepthToWorld(x,y);
                     ofColor c= oniCamGrabber->rgbSource.currentPixels->getColor(x,y);
                    //ofSetColor(ofColor::fromHsb(150+ 100* ofNoise(0.4+ofGetElapsedTimeMillis()/100000), 200, 200));
                     ofNoFill();
                     ofSetColor(ofColor::white);
                     ofPushMatrix();
                     ofTranslate(mdestPoint.x,mdestPoint.y,mdestPoint.z);
                     if((x+y)%10==0){
                         ofRotateY(ofGetElapsedTimeMillis()/3);
                     }
                     ofRect(-4,-4,8,8);
                     ofFill();
//                     ofSetColor(c);
//                     ofSetColor(ofColor::fromHsb(50+ 100* ofNoise(0.4+ofGetElapsedTimeMillis()/1000+(x+y)), 200, 200));
                     ofSetColor(
                                palete.getColor(x+100*ofNoise( (float) ofGetElapsedTimeMillis()/1000 ),y+50*ofNoise( (float) ofGetElapsedTimeMillis()/1000 ))
                     );
                     ofRect(-4,-4,8,8);
                     ofPopMatrix();
                 }
            }
        }
    }

    
    void drawParticles(){
       // ofPushMatrix();

       /* ofRotateX(rotateX);
        ofRotateY(rotateY);
        ofRotateZ(rotateZ);
        ofTranslate(tranX,tranY,tranZ);*/
        meshParticles.setMode(OF_PRIMITIVE_POINTS);
        glPointSize(4);
        
        //glEnable(GL_POINT_SMOOTH);	// Para que sean puntos redondos

        ofEnableDepthTest();

            meshParticles.draw();
        ofDisableDepthTest();

//        ofPopMatrix();
    }
    
    void explosionParticles(){
        ofVec3f centroEscena = ofVec3f(0,0,2500);
        for(int i=0; i<particles.size(); i++) {
            ofVec3f dir = particles[i].position - centroEscena;
            dir.normalize();
            dir*=1000;
            particles[i].applyForce(dir);
        }
    }
    
    void resetParticles(){
        particles.clear();
        setupParticles();
    }
    
    void setMode(int m){
        cloudState=m;
    }
    
    void setUI(ofxUITabBar *guiTabBar ){
        gui = new ofxUICanvas(0,100,400,800);
        gui->setName("Particles" );
        gui->addSlider("speed", 0.0f, 30,  &(speed));
        gui->addIntSlider("stopUmbral", 1, 300,  &(stopUmbral)) ;

        gui->addSlider("Acceleration", 1.05f, 3,  &(acceleration));
        gui->addSlider("rotateY",-360.0f, 360.0f,  &(rotateY));
        gui->addSlider("rotateZ", -360.0f, 360.0f,  &(rotateZ));
                gui->addSlider("rotateX", -360.0f, 360.0f,  &(rotateX));
        gui->addSlider("tranX", -1800.0f, 1800.0f,  &(tranX));
                gui->addSlider("tranY", -1800.0f, 1800.0f,  &(tranY));
                gui->addSlider("tranZ", -2800.0f, 2800.0f,  &(tranZ));
        gui->addButton("fadeIn",&fadeIn_b );
        vector<string> names;
        names.push_back("RUIDO");
        names.push_back("ESPEJO");
        names.push_back("DESAPARECE");
        gui->addRadio("MODO_Partics", names, OFX_UI_ORIENTATION_HORIZONTAL);
        gui->addIntSlider("alpha Particles", 1, 255, &(alphaParticles)) ;
        ofAddListener(gui->newGUIEvent,this,&particleClouds::guiEvent);
        
        guiTabBar->addCanvas(gui);
        
    }
    
    void guiEvent(ofxUIEventArgs &e)
    {
        string name = e.widget->getName();
        int kind = e.widget->getKind();
        
        if(name == "MODO_Partics")
        {
            ofxUIRadio *  wr = (ofxUIRadio *) e.widget;
            ofLogNotice("MODO_Partics. " + wr ->getActiveName() + " = " + ofToString(wr->getValue()));
            cloudState = wr->getValue();
            //		gui1->loadSettings("./config/gui/gui_kinect.xml");
        }
        if(name == "fadeIn")
        {
            if(fadeIn_b==true)
             resetParticles();
        }

    }
    
    int		cloudState;
    float *zMin, *zMax;
    float speed;
    float acceleration;
    int stopUmbral;
    int alphaParticles;
    bool bRealColors;
    float rotateY=0; float rotateZ=0; float rotateX=0;
    float tranX=0;
        float tranY=0;
        float tranZ=0;

private:
    int numParticles;
    int mode;
    int w;
    int h;
    int sampling;
    vector<Particle> particles ;
    ofMesh meshParticles;
    ofxUICanvas *gui;
    bool fadeIn_b;
    ofxOpenNI2Grabber *oniCamGrabber;
    extendedDepthSource *depthGenerator;

};


#endif
