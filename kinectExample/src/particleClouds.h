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

#ifndef kinectExample_particleClouds_h
#define kinectExample_particleClouds_h
#define SAMPLING 2
enum	{FUERTE, DEBIL, EM, GRAVEDAD};
  enum	{NUBE, ESPEJO};
class particleClouds{
public:
// http://stackoverflow.com/questions/1701416/initialization-of-reference-member-requires-a-temporary-variable-c
// there is no default constructor
   // particleClouds(ofxOpenNI2Grabber &g, extendedDepthSource &s ) : oniCamGrabber(g),depthGenerator(s)  {
    particleClouds(){
        sampling=SAMPLING;
        particleMode=ESPEJO;
        mode=EM;
    }
    
   /* particleClouds(ofxOpenNI2Grabber &_oniCamGrabber, extendedDepthSource _depthGenerator){
        oniCamGrabber=_oniCamGrabber;
        depthGenerator= _depthGenerator;
    }*/
   
    //only call once but have to call it

    void setup(int _w, int _h, float _zMin, float _zMax, ofxOpenNI2Grabber *_oniCamGrabber, extendedDepthSource *_depthGenerator){
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
        setupParticles();
    }
    void setupParticles(){
        //int w= oniSettings.width;
        //int h= oniSettings.height;

        particles.clear();
        int    sampling=2;
        //Loop through all the rows
        numParticles = 0;
        //Loop through all the columns
        // Distribuir las particulas por la escena
        for ( int y = 0 ; y < h ; y+=sampling ){
            for ( int x = 0 ; x < w ; x+=sampling ){
                float ang1 = ofRandom(PI);
                float ang2 = ofRandom(TWO_PI);
                float rr = 100;
                float rrho = rr*sin(ang1);
                particles.push_back(Particle(ofVec3f(rrho*cos(ang2),rrho*sin(ang2),-rr*cos(ang1)-1500) ,ofColor(255,255,255) ,x,y));
                numParticles++ ;
            }
        }
        ofLogVerbose() <<"particulas"<<numParticles;
    }
    
    void updateParticles(){
        meshParticles.clear();
        ofVec3f mdestPoint;
        ofVec3f diff ;
        int pp=0;
        std::vector<Particle>::iterator p ;
        
        for ( p = particles.begin() ; p != particles.end() ; p++ )
        {
            if(mode==DEBIL){
                if(ofRandomf()>0.5)
                    p->color.set(0);
                else
                    p->color.set(255);
                p->color=ofColor(255,0,0,255);
                meshParticles.addVertex(p->position);
                meshParticles.addColor(p->color);
                
            }else if(mode==EM){
                int distance=depthGenerator->currentRawPixels->getPixels()[p->_y * depthGenerator->width + p->_x];
                if(distance> zMin && distance < zMax) {
                    pp++;
                    p->recentlyUsed=5;
                    if(particleMode==ESPEJO){
                        mdestPoint=  oniCamGrabber->convertDepthToWorld(p->_x, p->_y);
                        p->steer(mdestPoint, true, speed, stopUmbral );
                        p->update();
                        
                    }
                    else if(particleMode==NUBE) {
                        p->sandDown(acceleration,-200);
                        p->update();
                        // p->position+=ofNoise( p->position.x,p->position.y,p->position.z)*30;
                    }                    
//                    if (bRealColors){
//                        p->color= rgbGenerator.currentPixels->getColor(p->_x , p->_y) ;
//                    }
                    p->color=ofColor(255,255,255,alphaParticles);
                    meshParticles.addVertex(p->position);
                    meshParticles.addColor(p->color);
                }else{ // si las particulas están mas lejos
                    // meshParticles.addVertex(p->position);
                    if(!p->recentlyUsed>0){
                        p->color=ofColor(255,0,0,40);
                    }else{
                        p->color=ofColor(255,0,0,255);
                        meshParticles.addVertex(p->position);
                        meshParticles.addColor(p->color);
                        p->recentlyUsed--;
                    }
                }//else modo ruido NO
            }
        }//end for all points within vector
    }
    
    void drawParticles(){
        meshParticles.setMode(OF_PRIMITIVE_POINTS);
        glPointSize(2);
        glEnable(GL_POINT_SMOOTH);	// Para que sean puntos redondos
        ofEnableDepthTest();
        meshParticles.draw();
        ofDisableDepthTest();
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
    
    void setUI(ofxUITabBar *guiTabBar ){
        gui = new ofxUICanvas(0,100,400,800);
        gui->setName("Particles" );
        gui->addSlider("speed", 0.0f, 200,  &(speed));
        gui->addIntSlider("stopUmbral", 1, 300,  &(stopUmbral)) ;

        gui->addSlider("Acceleration", 1.05f, 3,  &(acceleration));
        vector<string> names;
        names.push_back("NUBE");
        names.push_back("ESPEJO");
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
            particleMode = wr->getValue();
            //		gui1->loadSettings("./config/gui/gui_kinect.xml");
        }

    }
    
    int		particleMode;
    float zMin, zMax;
    float speed;
    float acceleration;
    int stopUmbral;
    int alphaParticles;
    bool bRealColors;

private:
    int numParticles;
    int mode;
    int w;
    int h;
    int sampling;
    vector<Particle> particles ;
    ofMesh meshParticles;
    ofxUICanvas *gui;

    ofxOpenNI2Grabber *oniCamGrabber;
    extendedDepthSource *depthGenerator;

};


#endif
