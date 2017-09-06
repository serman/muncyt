//
//  Header.h
//  escena_basica
//
//  Created by Sergio Galan on 7/10/14.
//
//

#ifndef escena_basica_anilloButton_h
#define escena_basica_anilloButton_h
#define INTERACTIONTIMEOUT
#include <ofMain.h>
#include <math.h>
enum BUTTON_TYPE {TYPE_ACC,TYPE_CRASH};
enum helpStatuses{ALL, HIDE_ACE, BLINK_EXPLOSION, HIDE_EXPLOSION};

class anilloButton : public tangibleObject{
    
public:
    ofImage imgPlus;
    ofImage imgPlusON;
    ofImage imgExp, imgExpON, imgExpDis;
    bool status_handIsOn;
    bool status_active;
    int status_highlight;
    BUTTON_TYPE btype;
    long lastCollision;
    ofEvent<BUTTON_TYPE> buttonEvent;
    ofColor mcolor;
    //ofNotifyEvent(onVolumeChange, 10.0);
    anilloButton(){
        status_handIsOn=false;
        status_active=true;
        btype = TYPE_ACC;
        imgPlus.loadImage("botono_anillo_mas_nuevo.png");
        imgPlusON.loadImage("botono_anillo_mas_nuevo_on.png");
        imgExp.loadImage("botono_explosion_nuevo.png");
        imgExpON.loadImage("botono_explosion_nuevo_on.png");
        imgExpDis.loadImage("botono_explosion_nuevo_disabled.png");
        status_highlight=ALL;
        mcolor=ofColor::white;
    }
    
    
    void setup(){
        if(x<100 && y>100){//izquierda
            imgPlus.rotate90(2);
            imgPlusON.rotate90(2);
        }
        else if (y<100 && x<500){ //arriba
            imgExp.rotate90(3);
            imgExpON.rotate90(3);
            imgExpDis.rotate90(3);
        }
        
    }
    
    void draw(){
        ofPushStyle();
            if(status_handIsOn ==true){
                mcolor=ofColor::white;
                ofSetColor(mcolor);
                //ofFill();
                //ofRect(x,y,width,height);
                
            }else{ //active bug hand is not on
                mcolor=ofColor(255,190+65*sin(ofGetElapsedTimef()/3.0*TWO_PI));
                //ofFill();
               // ofRect(x,y,width,height);
            }

        
        if( (status_highlight==HIDE_ACE ||  status_highlight==BLINK_EXPLOSION ) && btype==TYPE_ACC){
            mcolor=ofColor(128);
            mcolor.a=40;
        }
        
        if(status_highlight==BLINK_EXPLOSION && btype==TYPE_CRASH){
            mcolor=ofColor(255);
            if(ofGetElapsedTimeMillis()%600>350){
                mcolor.a=0;
            }
            else
                mcolor.a=255;
        }
        if(status_highlight==HIDE_EXPLOSION && btype==TYPE_CRASH){
            mcolor=ofColor(128);
            mcolor.a=60;
        }
            ofSetColor(mcolor);
//POSICION
            //ñapa para tres botones CAMBIAR!!
            if(x<100 && y>100){//izquierda
                if(status_handIsOn ==true)
                    imgPlusON.draw(5, 305);
                else
                    imgPlus.draw(10, 314);
            }else if (y<100 && x<500){ //arriba
                //ofRect(x,y,width,height);
                if(status_handIsOn ==true) imgExpON.draw(302,8);
                else imgExp.draw(309,13);
            }
            else{ //derecha
                if(status_handIsOn ==true)
                    imgPlusON.draw(670,305);
                else
                    imgPlus.draw(683,314);
            }
        ofPopStyle();
        //
        
    }
    void update(){}

    void update_prev(float speed, float num_particles){ //dato normalizado de la velocidad de la bola sobre la velocidad a la que ahy explosion y el numero de particulas
        if( btype==TYPE_ACC){
            if(speed>=1){
                status_highlight=HIDE_ACE;
            }
            else{
                status_highlight=ALL;
            }
        }
        
        if(btype==TYPE_CRASH){
            if(speed>=0.98 && num_particles>=0.9)
                status_highlight=BLINK_EXPLOSION;
            else
                    status_highlight=HIDE_EXPLOSION;
        }
        //TEST Lo dejamos siempre activo:
        
        status_active=true;
        return;
     /*   if(p.distance(ofPoint(x,y)) <200){
            status_active=true;            
        }
        else{
            status_active=false;
        }
        */
    }
    
    
    
    
    void touch(float x, float y, int s_id){
        if(inside(x, y)){
            status_handIsOn=true;
            if(status_active){
                cout << "notify event"<<endl;
                ofNotifyEvent(buttonEvent, btype);
            }
        }else
            status_handIsOn=false;
        
    }
    
    void slide(float x, float y, int s_id, int acc){
        if(canInteract()==false) return;
        if(inside(x, y)){
            status_handIsOn=true;
            if(status_active){
                    cout << "notify event"<<endl;
                ofNotifyEvent(buttonEvent, btype);
            }
        }else
            status_handIsOn=false;
        
    }
    void touchUp(int s_id){
            status_handIsOn=false;
    }
    
    void touchUpAll(){
        status_handIsOn=false;
    }
    
    bool canInteract(){
        if(ofGetElapsedTimeMillis()-lastCollision>400){
            lastCollision=ofGetElapsedTimeMillis();
            return true;
        }
        else return false;
    }
    
};

#endif
