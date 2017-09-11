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
#define LOADINGTIME 4.0
#define DISPLAY_INFO_TIME 7000
/*
 DISPLAY CENTRAL 
 Estado OFF DESACTIVADO
 Estado PREVIOUS : SE HA ACTIVADO PERO HAY AÚN UN DELAY PARA EMPEZAR LA CUENTA ATRAS
 estado COUNTING: se muestra el display con cuenta atrás
 
 */
class display{
public:
        int scene;
    enum statuses{PREVIOUS, COUNTING, OFF,DISPLAY_INFO};
    
    display(){
        status=OFF;
         myfont.loadFont("akzidenz.ttf", 80);
        ofTrueTypeFont::setGlobalDpi(72);
        display_info_time=0;
        alphaInfoText=0;
    }
    void init(int sc){
        if(status==OFF){
            time0=ofGetElapsedTimeMillis();
            scene=sc;
            status=PREVIOUS;
            if(sc==EM)
            { 
               textEm.loadImage("em.png");
                infoEm.loadImage("info_em.png");
            }
            if(sc==FUERTE)
            { 
               textEm.loadImage("fuerte.png");
                infoEm.loadImage("info_fuerte.png");
            }
            if(sc==DEBIL)
            { 
               textEm.loadImage("debil.png");
                infoEm.loadImage("info_debil.png");
            }
            if(sc==GRAVEDAD)
            {
                textEm.loadImage("gravedad.png");
                infoEm.loadImage("info_gravedad.png");
            }

        }
    }
    
    void interrupt(int sc ){
        if(status==DISPLAY_INFO) return;
        if(scene==sc){
            status=OFF;
            cheapComm::getInstance()->sendSync0("/sync/menu/hand_off_event");
        }
    }
    
    void changeScene(){
        init_time=ofGetElapsedTimeMillis(); 
            a=0;
            radioIncreasing=0;
            ofSendMessage("changeScene" +ofToString(scene)); 
            status=OFF;
            alphaInfoText=0;
    }
    void draw(){
        
        if(status==COUNTING) a<255?a+=10:a; //velocidad del fade
        if(status==OFF) return;
        if(status==PREVIOUS){
            if (ofGetElapsedTimeMillis()-time0 >100){
                init_time=ofGetElapsedTimeMillis();                
                rotation_angle=0;
                a=0;
                status=COUNTING;
                cheapComm::getInstance()->sendSync1("/sync/menu/hand_on_event",scene);
            }
            return;
        }
        //LO QUE VIENE AHORA SOLO SE EJECUTA CON status == COUNTING o status == DISPLAY_INFO
        
        int tim=ofGetElapsedTimeMillis()-init_time;
        int timeleft=LOADINGTIME*1000-tim;
        if(timeleft<800) {
            if(status!=DISPLAY_INFO) { 
                display_info_time=ofGetElapsedTimeMillis()+DISPLAY_INFO_TIME;
            }
            status=DISPLAY_INFO;
            if(ofGetElapsedTimeMillis()>display_info_time){
                changeScene();
                return;
            }
            
        }
        if(status==DISPLAY_INFO) radioIncreasing++;
        int timeleft_sec=ceil(timeleft/1000);
        
        string timeleft_str=ofToString(timeleft_sec);
        
        
        ofPushMatrix();
            ofPushStyle();
            ofTranslate((ofGetWidth()-W_WIDTH)/2, 0);
            //ofSetRectMode(OF_RECTMODE_CENTER);
            ofSetCircleResolution(60);
            ofEnableAlphaBlending();
            ofSetColor(0,0,0,min(a,200));

            ofCircle(W_WIDTH/2, W_HEIGHT/2, RADIO+radioIncreasing);
            ofSetColor(255,a);
            ofPushMatrix();
                ofTranslate(W_WIDTH/2, W_HEIGHT/2);
                ofRotateZ(rotation_angle);
                if(status==COUNTING){
                    textEm.draw(-textEm.width/2,-textEm.height/2);
                }
                else if (status==DISPLAY_INFO){
                    ofSetColor(255,alphaInfoText);
                    alphaInfoText=((++alphaInfoText)%255);
                    infoEm.draw(-RADIO,-RADIO,RADIO*2,RADIO*2);
                }
            ofPopMatrix();
        
            if(status==COUNTING){
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
            }
            ofDisableAlphaBlending();
            ofPopStyle();
        
        ofPopMatrix();
        rotation_angle+=1;
        
    }
private:
    int init_time, time0; //time0 es el momento en el que se pulsa el boton, pero hasta que no pasan 2 segundos no se guarda init_time que es el momento de inicio de la cuenta atras
    int display_info_time;
    float rotation_angle;
    ofImage textEm;
    ofImage infoEm;
    ofTrueTypeFont myfont;
    int a;
    int alphaInfoText;
    int status;
    int radioIncreasing;

    
};


#endif
