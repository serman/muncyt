//
//  consts.h
//  mainScreen
//
//  Created by Sergio Galan on 10/13/14.
//
//

#ifndef mainScreen_consts_h
#define mainScreen_consts_h
#include <ofMain.h>

#define SCREEN_W 768
#define SCREEN_H 384
#define VIDEO_W 640
#define VIDEO_H 480

static float VIDEO_scale=SCREEN_W/VIDEO_W;
static  float VIDEO_offset=((VIDEO_scale*VIDEO_H)-SCREEN_H)/2;

//devuelve (-1, -1) si el punto está fuera de la pantalla
static ofPoint convertPoint(float x1, float y1){
    if(y1*VIDEO_H*VIDEO_scale>(VIDEO_offset-30)
       && y1*VIDEO_H*VIDEO_scale>VIDEO_offset <  (VIDEO_offset + SCREEN_H+30) ){
        ofPoint p1=ofPoint((float)x1*SCREEN_W, (float)y1*VIDEO_H*VIDEO_scale-VIDEO_offset);
        return p1;
    }
    else return ofPoint(-1,-1);
}

#endif
