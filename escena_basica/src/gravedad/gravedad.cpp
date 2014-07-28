//
//  gravedad::.cpp
//  escena_basica
//
//  Created by Sergio Galan on 7/11/14.
//
//

#include "gravedad.h"


void gravedad::setup(){
    
    
}

void gravedad::init_Escena() {
	
	
//	ofPoint centro = ofPoint(W_WIDTH/2.0, W_HEIGHT/2.0);
//	
//	isKeyPressed = false;
//	isMousePressed = false;
	
	// Crear las particulas
	
	
	// addListeners
	// Aqui no hacemos nada (por ahora) con las colisiones y otros eventos
	//    box2d.enableEvents();
	//	ofAddListener(box2d.contactStartEvents, this, &gravedad::contactStart);
	//	ofAddListener(box2d.contactEndEvents, this, &gravedad::contactEnd);
	
	// Listeners para detectar que se esta tocando en una zona u otra
//    ofAddListener(button1.buttonEvent ,this, &gravedad::onButtonPressed);
//    ofAddListener(button2.buttonEvent ,this, &gravedad::onButtonPressed);
//    ofAddListener(button3.buttonEvent ,this, &gravedad::onButtonPressed);
//    ofAddListener(button4.buttonEvent ,this, &gravedad::onButtonPressed);
	
}

void gravedad::exit_Escena() {
	
	// borrar objetos
	
	
	// quitar listeners
	//    box2d.disableEvents();
	//	ofRemoveListener(box2d.contactStartEvents, this, &gravedad::contactStart);
	//	ofRemoveListener(box2d.contactEndEvents, this, &gravedad::contactEnd);
	
//    ofRemoveListener(button1.buttonEvent ,this, &gravedad::onButtonPressed);
//    ofRemoveListener(button2.buttonEvent ,this, &gravedad::onButtonPressed);
//    ofRemoveListener(button3.buttonEvent ,this, &gravedad::onButtonPressed);
//    ofRemoveListener(button4.buttonEvent ,this, &gravedad::onButtonPressed);
	
	
}


void gravedad::update(float d1){
    
}

void gravedad::draw(){
    
}

void gravedad::mousePressed( int x, int y, int button ){}


//scene notifications
void gravedad::sceneWillAppear( ofxScene * fromScreen ){  // reset our scene when we appear
    //gui1->enable();
	/* ofAddListener(tuioClient.cursorAdded,this,&gravedad::tuioAdded);
	 ofAddListener(tuioClient.cursorRemoved,this,&gravedad::tuioRemoved);
	 ofAddListener(tuioClient.cursorUpdated,this,&gravedad::tuioUpdated);
	 */
    init_Escena();
};


//scene notifications
void gravedad::sceneWillDisappear( ofxScene * toScreen ){
	// gui1->disable();
	/*  ofRemoveListener(tuioClient.cursorAdded,this,&gravedad::tuioAdded);
	 ofRemoveListener(tuioClient.cursorRemoved,this,&gravedad::tuioRemoved);
	 ofRemoveListener(tuioClient.cursorUpdated,this,&gravedad::tuioUpdated);
	 */
	exit_Escena();
};



