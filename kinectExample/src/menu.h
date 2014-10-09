//
//  menu.h
//  kinectExample
//
//  Created by Sergio Galan on 9/29/14.
//
//

#ifndef kinectExample_menu_h
#define kinectExample_menu_h
#include "consts.h"
#include <ofMain.h>
#include <math.h>
#include "Particle.h"
class menu{
public:
    vector<Particle> particles;
    ofMesh mesh;
    ofImage *currentImg;
    vector<ofPoint> emImgWP,weakImgWP,strongImgWP,gravityImgWP,flabImgWP;
    vector<ofPoint> *currentvector;
    ofImage texture1;
    void setup(){
        emImg.loadImage("emImg.png");
        weakImg.loadImage("weakImg.png");
        strongImg.loadImage("strongImg.png");
        gravityImg.loadImage("gravityImg.png");
        flabImg.loadImage("flabImg.png");
        texture1.loadImage("text1.png");
        
        start(MENU);
        
        for ( int y = 0 ; y < ofGetHeight();  y+=12 ){
             for ( int x = 0 ; x < ofGetWidth(); x+=12 ){
                Particle mparticle=Particle(ofVec3f(x-1000,y-1000,0),
                                            ofVec3f(0,ofRandom(-400,0),ofRandom(-400)),
                                            ofColor(255,255,255) ,x,y);
                particles.push_back(mparticle);
             }
        }
        analyzeImg();
        currentvector=&emImgWP;
    }
    
    void start(int _state){
        initTime=ofGetElapsedTimeMillis();
        state=_state;
        
    }
    
    void analyzeImg( ){
        for ( int y = 0 ; y < ofGetHeight();  y+=4 ){
            for ( int x = 0 ; x < ofGetWidth(); x+=4 ){
                    if(emImg.getColor(x,y).getBrightness()>100)
                        emImgWP.push_back(ofPoint(x,y));
            }
        }
        
        for ( int y = 0 ; y < ofGetHeight();  y+=4 ){
            for ( int x = 0 ; x < ofGetWidth(); x+=4 ){
                if(weakImg.getColor(x,y).getBrightness()>100)
                    weakImgWP.push_back(ofPoint(x,y));
            }
        }
        
        for ( int y = 0 ; y < ofGetHeight();  y+=4 ){
            for ( int x = 0 ; x < ofGetWidth(); x+=4 ){
                if(strongImg.getColor(x,y).getBrightness()>100)
                    strongImgWP.push_back(ofPoint(x,y));
            }
        }
        
        for ( int y = 0 ; y < ofGetHeight();  y+=4 ){
            for ( int x = 0 ; x < ofGetWidth(); x+=4 ){
                if(gravityImg.getColor(x,y).getBrightness()>100)
                    gravityImgWP.push_back(ofPoint(x,y));
            }
        }
        
        for ( int y = 0 ; y < ofGetHeight();  y+=4 ){
            for ( int x = 0 ; x < ofGetWidth(); x+=4 ){
                if(flabImg.getColor(x,y).getBrightness()>100)
                    flabImgWP.push_back(ofPoint(x,y));
            }
        }
        
    }
    
    
    void draw(){
        ofPushStyle();
        ofEnableAlphaBlending();
        ofSetColor(255/*,min( (int)(ofGetElapsedTimeMillis()-initTime)/2,255) */) ;
        switch(state){
            case EM:
                //emImg.draw(0,0,ofGetWidth(),ofGetHeight());
                ofDrawBitmapString (ofToString(ofGetElapsedTimeMillis()-initTime),500,500);
                currentImg=&emImg;
                currentvector=&emImgWP;
                
            break;
            case NUCLEAR_DEBIL:
                //weakImg.draw(0,0,ofGetWidth(),ofGetHeight());
                currentImg=&weakImg;
                currentvector=&weakImgWP;
            break;
            case NUCLEAR_FUERTE:
               // strongImg.draw(0,0,ofGetWidth(),ofGetHeight());
                currentImg=&strongImg;
                currentvector=&strongImgWP;
                break;
                
            case GRAVEDAD:
                //gravityImg.draw(0,0,ofGetWidth(),ofGetHeight());
                currentImg=&gravityImg;
                currentvector=&gravityImgWP;
                break;

            case MENU:
                //flabImg.draw(0,0,ofGetWidth(),ofGetHeight());
                 currentImg=&flabImg;
                 currentvector=&flabImgWP;
            break;
        }
        //ofDisableAlphaBlending();
        ofPopStyle();
        
        drawImgParticles();
        
    }
    
    void drawImgParticles(){
        mesh.clear();
        
        for ( int i = 0 ; i < particles.size();  i+=1 ){
            int index=i%currentvector->size();
            ofVec3f p=ofVec3f( (*currentvector)[index] );
             particles[i].steer(p,true,4,4);
            particles[i].update();
            mesh.addVertex(particles[i].position);
            mesh.addTexCoord(ofVec2f(4, 4));
            //mesh.addColor(
            //currentImg->getColor(particles[i]._x,particles[i]._y)
            //              );
          //  mesh.addColor( ofColor(180 + 30*sin(  0.5*PI * (float) ofGetElapsedTimeMillis()/1000 ) ) ) ;
              mesh.addColor( ofColor(140 + 70* ofNoise( (float) ofGetElapsedTimeMillis()/1000 )) ) ;
//            mesh.addColor(ofColor(255, 255, 255));
        }
        mesh.setMode(OF_PRIMITIVE_POINTS);
        glPointSize(2);
        texture1.bind();
        mesh.draw();
        texture1.unbind();

        
        
    }

private:
    ofImage emImg, weakImg,strongImg,gravityImg,flabImg;
    long initTime;
    int state;
    
    
};

#endif
