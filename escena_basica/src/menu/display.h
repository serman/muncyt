//
//  display.h
//  escena_basica
//
//  Created by Sergio Galan on 9/23/14.
//
//

#ifndef escena_basica_display_h
#define escena_basica_display_h
#include "consts.h"
#include <math.h>
#include <ofMain.h>
#include "cheapComm.h"
#define RADIO 130
#define LOADINGTIME 6

/*
 DISPLAY CENTRAL 
 Estado OFF DESACTIVADO
 Estado PREVIOUS : SE HA ACTIVADO PERO HAY AÚN UN DELAY PARA EMPEZAR LA CUENTA ATRAS
 estado COUNTING: se muestra el display con cuenta atrás
 
 */
class display{
public:
        int scene;
    enum statuses{PREVIOUS, COUNTING, OFF};
    
    display(){
        status=OFF;
         myfont.loadFont("akzidenz.ttf", 80);
        ofTrueTypeFont::setGlobalDpi(72);
    }
    void init(int sc){
        if(status==OFF){
            time0=ofGetElapsedTimeMillis();
            scene=sc;
            status=PREVIOUS;
            if(sc==EM)
                textEm.loadImage("em.png");
            if(sc==FUERTE)
                textEm.loadImage("fuerte.png");
            if(sc==DEBIL)
                textEm.loadImage("debil.png");
            if(sc==GRAVEDAD)
                textEm.loadImage("gravedad.png");
            
        }
    }
    
    void interrupt(int sc ){
        if(scene==sc){
            status=OFF;
            cheapComm::getInstance()->sendSync0("/sync/menu/hand_off_event");
        }
    }
    
    void draw(){
        
        if(status==COUNTING) a<255?a+=10:a; //velocidad del fade
        if(status==OFF) return;
        if(status==PREVIOUS){
            if (ofGetElapsedTimeMillis()-time0 >2000){
                init_time=ofGetElapsedTimeMillis();                
                rotation_angle=0;
                a=0;
                status=COUNTING;
                cheapComm::getInstance()->sendSync1("/sync/menu/hand_on_event",scene);
            }
            return;
        }

        
        
        int tim=ofGetElapsedTimeMillis()-init_time;
        int timeleft=LOADINGTIME*1000-tim;
        if(timeleft<0) {
            init_time=ofGetElapsedTimeMillis(); a=0; ofSendMessage("changeScene" +ofToString(scene)); status==OFF;
            return;}
        int timeleft_sec=ceil(timeleft/1000);
        
        string timeleft_str=ofToString(timeleft_sec);
        ofPushMatrix();
            ofPushStyle();
            ofTranslate((ofGetWidth()-W_WIDTH)/2, 0);
            //ofSetRectMode(OF_RECTMODE_CENTER);
            ofSetCircleResolution(60);
            ofEnableAlphaBlending();
            ofSetColor(0,0,0,min(a,200));

            ofCircle(W_WIDTH/2, W_HEIGHT/2, RADIO);
            ofSetColor(255,a);
            ofPushMatrix();
                ofTranslate(W_WIDTH/2, W_HEIGHT/2);
                ofRotateZ(rotation_angle);
                textEm.draw(-textEm.width/2,-textEm.height/2);
            ofPopMatrix();
        
            ofPushMatrix();
                ofTranslate(W_WIDTH/2, W_HEIGHT/2);
               // ofRotateZ(rotation_angle);
                int wt= myfont.stringWidth(timeleft_str);
                int ht= myfont.stringHeight(timeleft_str);
                myfont.drawString(timeleft_str,-wt/2,+ht/2);
            ofPopMatrix();
        

            ofPushMatrix();
            ofTranslate(W_WIDTH/2, W_HEIGHT/2);
            int r=RADIO-6;
            ofSetColor(27,117,187,a);
            for(int i=0; i<ofMap(tim,0,LOADINGTIME*1000,0,360); i+=6){
//                int x1=r*sin(ofDegToRad(i))
//                int y1=r*cos(ofDegToRad(i))
                ofRotateZ(6);
                ofRect(-r,0,6,4);
            }
            ofPopMatrix();
        
            ofDisableAlphaBlending();
            ofPopStyle();
        
        ofPopMatrix();
        rotation_angle+=1;
        
    }
private:
    int init_time, time0; //time0 es el momento en el que se pulsa el boton, pero hasta que no pasan 2 segundos no se guarda init_time que es el momento de inicio de la cuenta atras
    float rotation_angle;
    ofImage textEm;
    ofTrueTypeFont myfont;
    int a;
    int status;

    
};


#endif
