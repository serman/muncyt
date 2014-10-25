//
//  tangiblesHandler.h
//  example
//
//  Created by Sergio Galan on 7/8/14.
//
//
#include <ofMain.h>
#include "tangibleObject.h"
#ifndef example_tangiblesHandler_h
#define example_tangiblesHandler_h

class tangiblesHandler{
public:
    long id_counter;
    vector<tangibleObject *> objectsCol;
    
    tangiblesHandler(){
        //objectsCol=vector<tangibleObject> ();
        id_counter=1;
    }    

    void reset(){
        for (int i=0; i<objectsCol.size(); i++){
            delete objectsCol[i];
        }
        objectsCol.clear();
        
    }

    
    void addObject(tangibleObject &mobject){
        mobject.m_id=id_counter++;
        objectsCol.push_back(&mobject);
    }
    
    void removeObject(long obj_id){
        
        for (std::vector<tangibleObject *>::iterator it = objectsCol.begin() ; it != objectsCol.end(); ++it){
            if((*it)->m_id==obj_id)
                objectsCol.erase(it);
        }
    }
    
    void removeObjectByTuioID(long obj_id){
        
        for (std::vector<tangibleObject *>::iterator it = objectsCol.begin() ; it != objectsCol.end(); ++it){
            if((*it)->cursor_id==obj_id){
                objectsCol.erase(it);
             ofLog(OF_LOG_VERBOSE) << " deleted Object";
                break;
            }
        }
    }
    
    void filter(){
        
        
    }

    void draw(){
//		ofLogNotice("hands_draw()");
        for(int i=0; i<objectsCol.size();i++){
            objectsCol[i]->draw();
        }
    }
    
    void update(){
        for(int i=0; i<objectsCol.size();i++){
            objectsCol[i]->update();
        }
    }
    
    void notifyTouch(float x, float y, int s_id){
        for(int i=0; i<objectsCol.size();i++){
            objectsCol[i]->touch(x,y,s_id);
           
        }
        
    }
    
    void notifySlide(float x, float y, int s_id, int acc){
        for(int i=0; i<objectsCol.size();i++){
            objectsCol[i]->slide(x,y,s_id,acc);
        }
    }
    
    void notifyTouchUp( int s_id){
        for(int i=0; i<objectsCol.size();i++){
            objectsCol[i]->touchUp(s_id);
        }
    }
    void notifyTouchUpAll( ){
        for(int i=0; i<objectsCol.size();i++){
            objectsCol[i]->touchUpAll();
        }
    }
    
};

#endif
