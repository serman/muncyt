/*
 *  escenas.cpp
 *  scaner_faces
 *
 *  Created by guillermo casado on 23/06/14.
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */
#include "scanner_faces.h"

using namespace ofxCv;
using namespace cv;

void scanner_faces::setupEscenas() {
	
	contCaras = 0;
	CONT_CARAS_LIM = 40;
	
	numErrores = 0;
	NUMERRORESMAX = 10;
	
	setupMensajes();
	
}

void scanner_faces::setupMensajes() {

	vector<string>	titles;	
	vector<string>	mensajes_1;
	vector<string>	mensajes_peque;
	vector<int>	tiempos;

	//	focusPts.push_back(ofPoint(ofGetWidth()/2, ofGetHeight()/2));
	//	focusPts.push_back(ofPoint(ofGetWidth()*1/5, ofGetHeight()/2));
	//	focusPts.push_back(ofPoint(ofGetWidth()*4/5, ofGetHeight()/2));	
	
	titles.push_back("init");
	mensajes.push_back(" ");
	mensajes_1.push_back(" ");
	mensajes_peque.push_back(" ");
	tiempos.push_back(500);
	
	titles.push_back("analizador_0");
	mensajes.push_back("ANALIZADOR");
	mensajes_1.push_back("DE ROSTROS");
	mensajes_peque.push_back("Su rostro va \na ser analizado");
	tiempos.push_back(3000);

	titles.push_back("analizador_1");
	mensajes.push_back("ANALIZADOR");
	mensajes_1.push_back("DE ROSTROS");
	mensajes_peque.push_back("Si no lo desea \nretIrese de la cAmara");
	tiempos.push_back(3000);

	titles.push_back("analizador_1.5");
	mensajes.push_back("ANALIZADOR");
	mensajes_1.push_back("DE ROSTROS");
	mensajes_peque.push_back(" ");
	tiempos.push_back(1000);
	
	titles.push_back("analizador_2");
	mensajes.push_back("ANALISIS");
	mensajes_1.push_back("- - - -");
	mensajes_peque.push_back("SitUe su rostro \nen el marco");
	tiempos.push_back(2000);

	titles.push_back("analizador_3");
	mensajes.push_back("ANALISIS");
	mensajes_1.push_back("- - - -");
	mensajes_peque.push_back("  ");
	tiempos.push_back(1500);

	titles.push_back("mostrar_face");
	mensajes.push_back(" ");	// Mostrar FaceGuides
	mensajes_1.push_back("Cara");
	mensajes_peque.push_back("          XXX            ");
	tiempos.push_back(1500);

	titles.push_back("mostrar_eyes");
	mensajes.push_back(" ");	// Mostrar EyeGuides
	mensajes_1.push_back("Ojos");
	mensajes_peque.push_back("XXX                      ");
	tiempos.push_back(1800);

	titles.push_back("mostrar_boca");
	mensajes.push_back(" ");	// Mostrar BocaGuides
	mensajes_1.push_back("Boca");
	mensajes_peque.push_back("                      XXX");
	tiempos.push_back(1800);
	
	// . . . más posibles órdenes . . . 
	
	titles.push_back("final");
	mensajes.push_back(" ");	// Tiempo para mostar análisis
	mensajes_1.push_back(" ");
	mensajes_peque.push_back(" ");
	tiempos.push_back(5000);

	// Esto pasa a escena de escaneo:
//	mensajes.push_back("ESCANEO");	// Luego quitare el texto
//	mensajes_1.push_back(" ");
//	mensajes_peque.push_back(" ");
//	tiempos.push_back(3000);
//
	// Esto pasa a escena de final:
//	mensajes.push_back("OOXOO");	// 
//	mensajes_1.push_back(" ");
//	mensajes_peque.push_back(" ");
//	tiempos.push_back(10000);

	idMensajeAct = 0;
	
	int dtPaso = 2000;
	
	
	fontMensajes.loadFont("fonts/atari_full.ttf", 36);//45);
	fontMensajes_peque.loadFont("fonts/atari_full.ttf", 18);
	fontData.loadFont("data-latin", 10);

//	fontX_peq.setup("fonts/atari_full.ttf");

	for(int i=0; i<mensajes.size(); i++) {	
		MensajeTime mt;
		mt.title = titles[i];
		mt.mensaje = mensajes[i];
		mt.mensaje_1 = mensajes_1[i];
		mt.mensaje_peque = mensajes_peque[i];
		ofRectangle rTmp = fontMensajes.getStringBoundingBox(mt.mensaje, ofGetWidth()/2, ofGetHeight()/2);
		mt.posMsg = ofPoint(rTmp.x-rTmp.width/2, rTmp.y);
		mt.tiempo = tiempos[i];
		mensajesT.push_back(mt);
	}
}

// 

void scanner_faces::setEscena(int idScena) {
	if(idScena==SCN_WAITING) {
		nombreEscena = "WAITING";
		scnAct = SCN_WAITING;
		contCaras = 0;
		
		numErrores = 0;
		
		bFaceDetected = false;
				
		doHaarFace = true;
		doDrawHaarFace = false;
		doFaceTracker = false;
		doDrawFaceTracker = false;
		
		guidesFace = false;
		guidesEyes = false;
		guidesBoca = false;
		guidesSkin = false;
		
		show_HaarRect = false;
		
//		enum { FT_LINES, FT_WIREFRAME, FT_TEXTURE};
		modoDrawFT = FT_LINES;
		
		// apagar luces
		setLight(false);
		
		// eliminar cosas y timers, eventos
		ofRemoveListener(timerMsg.TIMER_REACHED, this, &scanner_faces::tiempo_Msg);	
		ofRemoveListener(timerFin.TIMER_REACHED, this, &scanner_faces::tiempo_Fin);	
		
		
		// lanzar animaciones: timers y demás 
		
	} 
	else if(idScena==SCN_PRESCAN) {
		nombreEscena = "PRESCAN";
		scnAct = SCN_PRESCAN;
		
		doHaarFace = true;
		doDrawHaarFace = true;
		doFaceTracker = true;
		doDrawFaceTracker = false;
		
		guidesFace = false;
		guidesEyes = false;
		guidesBoca = false;
		guidesSkin = false;		
		
		numErrores = 0;
		
		caraOut = true;
		
		show_HaarRect = false;
		
		nPersonaAct++;
		nZonaAct = 0;
		
		// encender luces
		setLight(true);
		
		// eliminar cosas y timers, eventos
		
		// lanzar animaciones: timers y demás 
		idMensajeAct = 0;
		timerMsg.setup(mensajesT[idMensajeAct].tiempo, false);
		timerMsg.startTimer();
		ofAddListener(timerMsg.TIMER_REACHED, this, &scanner_faces::tiempo_Msg);	
		
	} 
	else if(idScena==SCN_SCAN) {
		nombreEscena = "SCAN";
		scnAct = SCN_SCAN;

		doHaarFace = false;
		doDrawHaarFace = false;
		doFaceTracker = true;
		doDrawFaceTracker = true;

		numErrores = 0;
		
		show_HaarRect = false;
		
		// encender luces (por si accedemos directos a la escena)
		setLight(true);
		
		// eliminar cosas y timers, eventos
		ofRemoveListener(timerMsg.TIMER_REACHED, this, &scanner_faces::tiempo_Msg);	
		ofRemoveListener(timerFin.TIMER_REACHED, this, &scanner_faces::tiempo_Fin);	
		
		// lanzar animaciones: timers y demás 
//		timerScan.setup(TIME_SCAN, false);
		timerScan.startTimer();
		ofAddListener(timerScan.TIMER_REACHED, this, &scanner_faces::tiempo_Scan);	
		
	} 
	else if(idScena==SCN_FIN) {
		nombreEscena = "END";
		scnAct = SCN_FIN;
		
		doHaarFace = false;
		doDrawHaarFace = false;
		doFaceTracker = true;
		doDrawFaceTracker = true;

		numErrores = 0;

		show_HaarRect = false;
		
		// encender luces (por si accedemos directos a la escena)
		setLight(true);
		
		// eliminar cosas y timers, eventos
		ofRemoveListener(timerScan.TIMER_REACHED, this, &scanner_faces::tiempo_Scan);	
		
		// lanzar animaciones: timers y demás 
		timerFin.startTimer();
		ofAddListener(timerFin.TIMER_REACHED, this, &scanner_faces::tiempo_Fin);	
		
	}
	
}

void scanner_faces::nextEscena() {
	scnAct++;
	scnAct%=4;
	setEscena(scnAct);	
}

void scanner_faces::updateEscena() {
	// Update una cosa u otra según el modo
	
	
	// draw fbos
	
	
	// preparar/aplicar shaders
	
	
	if(scnAct==SCN_WAITING) {
		update_ScnWait();
		if(bFaceDetected) {
			contCaras++;
			if(contCaras>=CONT_CARAS_LIM) {
				setEscena(SCN_PRESCAN);
			}
		}
	}
	else if(scnAct==SCN_PRESCAN) {
		// dependiendo de que punto de mensaje este, hacer una u otra cosa
		
	}
	else if(scnAct==SCN_SCAN) {
		// si se dan las condiciones necesarias)
		if (tracker.getFound()) {
			// generar el fbo con el mesh 
			// (ya se genero anteriormente en update_faceTracker())
			
			// generar el fbo con la transicion de scaneo
			
		}
		
	}
	else if(scnAct==SCN_FIN) {
	}
	
	
}

void scanner_faces::update_ScnWait() {
	float r = ofRandom(30,80);
	float xx = ofRandom(r, ofGetWidth()-r);
	float yy = ofRandom(r, ofGetHeight()-r);
	
	ofPath c;
	c.setCircleResolution(60);
	c.setFillColor(ofColor(colores.at(floor(ofRandom(colores.size()))), 100));
	c.circle(xx, yy, r);
	c.close();
	shapes_Wait.push_back(c);
	
	if(shapes_Wait.size()>40) {
		shapes_Wait.erase(shapes_Wait.begin(), shapes_Wait.begin()+3);
	}
	
}

void scanner_faces::draw_ScnWait() {
	ofPushStyle();
	ofSetColor(ofColor::beige); // beige
	ofFill();
	ofRect(0,0,ofGetWidth(),ofGetHeight());
	
	for(int i=0; i<shapes_Wait.size(); i++) {
		shapes_Wait[i].draw();
	}
	
	ofPopStyle();
}




void scanner_faces::setupTimers() {
	
	TIME_HOLA = 2000;
	TIME_MSG = 5000;	// tiempo duracion de los mensajes
	//	TIME_ENCUADRE;
	
	TIME_SCAN = 2000;	// tiempo de línea de escaneo
	TIME_FIN = 10000;	// tiempo de recreo antes de acabar la sesión
	
	TIME_FOTOS = 500;	// intervalo de toma de fotos
	NFOTOSMAX = 3;
	nFotoAct = 0;
	
	timerMsg.setup(TIME_MSG, false);
	timerScan.setup(TIME_SCAN, false);
	timerFin.setup(TIME_FIN, false);

	stopTimers();
}

void scanner_faces::stopTimers() {
	timerMsg.stopTimer();
	timerScan.stopTimer();
	timerFin.stopTimer();	
}


void scanner_faces::tiempo_Msg(ofEventArgs &e) {
	// cada vez que se cumple este tiempo se suma 1 al mensaje actual
	// y se vuelve a lanzar si el num_mensaje es el último
	// Luego ya se hará lo que se deba.
	ofLogNotice("tiempo_Msg REACHED del msg #"+ofToString(idMensajeAct));
	
	
	// comprobaciones para ver si hay que hacer algo
	// y puede continuar o no
	if(!bFaceDetected || !tracker.getFound()) {
		// SI hay error en la detección:
		numErrores++;
        ofLogNotice() << "tiempo_Msg EVENT. Error. Num errores " << numErrores;
		if(numErrores>=NUMERRORESMAX) {
			// Salir del proceso
			setEscena(0);
		}
		else {
			// relanzar este paso again
			timerMsg.setup(mensajesT[idMensajeAct].tiempo, false);
			timerMsg.startTimer();			
		}
		
	}
	else {
		// Hacer foto y guardar mesh asociado
		
		
		
	
		// Pasar al siguiente paso o escena
		
		idMensajeAct++;
		// lanzar nuevo timer o pasar de escena
		if(idMensajeAct<mensajesT.size()) {
			
			
			if(mensajesT[idMensajeAct].title=="analizador_2") {
				show_HaarRect = true;
			}
			if(mensajesT[idMensajeAct].title=="mostrar_face") {
				guidesFace=true;
				// capturar para haarviz
				hacerFoto_haarViz();
			}
			else if(mensajesT[idMensajeAct].title=="mostrar_eyes") {
				guidesEyes=true;				
				// foto 1
				hacerFoto(); // Ya suma nZonaAct++ si se hace la foto
			}
			else if(mensajesT[idMensajeAct].title=="mostrar_boca") {
				guidesBoca=true;
				// foto 2
				hacerFoto(); // Ya suma nZonaAct++ si se hace la foto				
			}
			
			timerMsg.setup(mensajesT[idMensajeAct].tiempo, false);
			timerMsg.startTimer();
			
		} else {
			// pasar a la siguiente escena
			// Ojo con esto porque podria dar error
			// porque esto es un evento y puede 'colarse' en el flujo del programa
			nextEscena();
		}
		
	   
	}
	   
	
}


void scanner_faces::tiempo_Scan(ofEventArgs &e) {
	ofLogNotice("tiempo_Scan");
	ofRemoveListener(timerScan.TIMER_REACHED, this, &scanner_faces::tiempo_Scan);		
	// Cuando se cumple este tiempo es porque ha acabado el proceso de escaneo
	// Pasar a la siguiente escena
	// Ojo con esto porque podria dar error
	timerScan.stopTimer();
	nextEscena();
}

void scanner_faces::tiempo_Fin(ofEventArgs &e) {
	ofLogNotice("tiempo_Fin");
	ofRemoveListener(timerFin.TIMER_REACHED, this, &scanner_faces::tiempo_Fin);		
	// Cuando se cumple este tiempo es el final de la sesion
	// Pasar al comienzo del proceso
	// Ojo con esto porque podria dar error
	timerFin.stopTimer();
	scnAct=0;
	setEscena(scnAct);	
	
}

void scanner_faces::tiempo_Foto(ofEventArgs &e) {
	// si hay rostro en la camara y esta bien situado ==> hacer foto
	if(hayPersona() && posicionOK()) {
		
		
	}
	// si ya no hay rostro ==> quitar listener a tiempo_Foto y volver al inicio del proceso
	
	// si el nFotoAct es menor que el maximo ==> lanzar timer.
	
	// si no ==> quitar listener a tiempo_Foto
	
	
}



