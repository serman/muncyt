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
enum scenes{MENU, EM, DEBIL, FUERTE, GRAVEDAD};

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
