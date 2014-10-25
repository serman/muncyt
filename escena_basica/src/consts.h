//
//  consts.h
//  example
//
//  Created by Sergio Galan on 7/10/14.
//
//
#include <ofMain.h>
#ifndef example_consts_h
#define example_consts_h

#define W_WIDTH 768
#define W_HEIGHT 768
#define TUIORATE
#define DEBUGOSC 1

#define MAX_TIME_ESCENA 30*1000 //tiempo maximo por defecto que está una escena en pantalla
enum scenes{ EM, DEBIL, FUERTE, GRAVEDAD,MENU};

enum Scenes_ofxScene{ SCENE_EM = 0, SCENE_NUC_D, SCENE_NUC_F, SCENE_GRAVEDAD, SCENE_MENU };

enum tipoOSCData {
	ESCENA,
	TOQUE_ON,
	TOQUE_OFF,
	EXPLOSION
	
};

typedef struct oscEventData {
	int		tipoOSCDato;
	int		ipar1;
	int		ipar2;
	float	fpar1;
	float	fpar2;
	
} oscData;




float X_(float X);



#endif
