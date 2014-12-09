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
#define VIDEO_W 720
#define VIDEO_H 576
#define ABS_MAX_TIME_SCENE 60*2*1000
//En el caso de bloqueo 5 minutos

extern ofTrueTypeFont courierFont;

//la imagen que se recibe hay que escalarla por este ratio para que se convierta en pantalla completa
static float VIDEO_scale=SCREEN_W/VIDEO_W;

//Como el ratio de imagen en la pantalla es de 2:1 y no de 4:3 como el vídeo, tomamos la franja central de la imagen a partir de un offset:
//VIDEO_scale*VIDEO_H es el alto del video que se obtiene tras escalar

static  float VIDEO_offset=((VIDEO_scale*VIDEO_H)-SCREEN_H)/2;

//devuelve (-1, -1) si el punto está fuera de la pantalla
// Recibe un valor entre 0 y 1 sin escalar

static ofPoint convertPoint(float x1, float y1){
    float y1px=y1*VIDEO_H*VIDEO_scale;
    
    if(y1px >(VIDEO_offset) /* el punto está dentro de la franja que se muetra ( que comienza en VIDEO_offset*/
       && y1px < (VIDEO_offset + SCREEN_H)
       /* Y termina en video_offset+ screen_h*/
       ){
        ofPoint p1=ofPoint((float)x1*SCREEN_W, (float)y1px-VIDEO_offset);
        return p1;
    }
    else return ofPoint(-1,-1);
}

static ofPoint convertPoint2(float x1, float y1){
    int offset1=SCREEN_H/2-130;
    float y1px=y1*VIDEO_H*VIDEO_scale;
    
    if(y1px >(offset1) /* el punto está dentro de la franja que se muetra ( que comienza en VIDEO_offset*/
       && y1px < (offset1 + SCREEN_H)
       /* Y termina en video_offset+ screen_h*/
       ){
        ofPoint p1=ofPoint((float)x1*SCREEN_W, (float)y1px-offset1);
        return p1;
    }
    else return ofPoint(-1,-1);
}


#endif
