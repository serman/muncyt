//
//  cameraMoves.cpp
//  kinectExample
//
//  Created by Sergio Galan on 10/21/14.
//
//

#include "cameraMoves.h"

cameraMoves* cameraMoves::m_pInstance = NULL;
cameraMoves::cameraMoves(){}


    void cameraMoves::setup(ofCamera *_cam){
        zoomSpeed=20.0;
        shouldRestartCamera=false;
        cam=_cam;
    }
    void cameraMoves::zoomInAndCenter(){
        if(zoomInFirstTime){
            cam->dolly(-2000.0);
            //cam->tilt(-10.0);
            zoomInFirstTime=false;
        }else{
            if(zoomInCounter>0){
                cam->dolly(+zoomSpeed);
                zoomInCounter-=zoomSpeed;
                if(zoomInCounter<=0 && tiltCounter>=0){
                  //  cam->tilt(+0.1);
                   // tiltCounter-=0.1;
                }
            }
        }
    }
    
    void cameraMoves::reset(){
        ofLog()<< "RESET CAMERA MOVES"<<endl;
        shouldRestartCamera=true;
        zoomInFirstTime=true;
        zoomInCounter=2000;
        tiltCounter=10;
    }
