/*
 *  menu_interaccion.cpp
 *  escena_basica
 *
 *  Created by guillermo casado on 24/07/14.
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */
#include "menu.h"

//--------------------------------------------------------------

void menu::interaccion_point(ofVec2f ptF, bool isTipoA) {
	float minDis = (isTipoA)? 300 : 200;

	float fFuerza = 5.0;
	if(ptF.distance(centro)<distConf) {
		if(isTipoA) {
			for(int i=0; i<circles.size(); i++) {
				float dis = ptF.distance(circles[i].get()->getPosition());
				if(dis < minDis) circles[i].get()->addRepulsionForce(ptF, 0.2f*fFuerza/dis);//3, 9);
					else circles[i].get()->addAttractionPoint(ptF, 4.2*fFuerza/dis);//4.0);
						}
			for(int i=0; i<boxes.size(); i++) {
				float dis = ptF.distance(boxes[i].get()->getPosition());
				if(dis < minDis) boxes[i].get()->addRepulsionForce(ptF, 1.8*fFuerza/dis);
					else boxes[i].get()->addRepulsionForce(ptF, 1.9*fFuerza/dis);//4.0);
						}
		}
		else {
			for(int i=0; i<circles.size(); i++) {
				float dis = ptF.distance(circles[i].get()->getPosition());
				if(dis < minDis) circles[i].get()->addAttractionPoint(ptF, 1.5*fFuerza/dis);//3, 9);
					else circles[i].get()->addRepulsionForce(ptF, 0.9*fFuerza/dis);//4.0);
						}
			for(int i=0; i<boxes.size(); i++) {
				float dis = ptF.distance(boxes[i].get()->getPosition());
				if(dis < minDis) boxes[i].get()->addAttractionPoint(ptF, 1.8*fFuerza/dis);
					else boxes[i].get()->addAttractionPoint(ptF, 1.9*fFuerza/dis);//4.0);
						}
		}
	}

}

//--------------------------------------------------------------
void menu::keyPressed(int key) {
	
	if(key == 'c') {
		//addCircle(ofPoint(mouseX, mouseY));
		bAddCircle=true;
	}
	else if(key == 'b') {
		//addBox(ofPoint(mouseX, mouseY));
		bAddBox=true;
	}
	else if(key == 'q') {
		//addCircle(ofPoint(mouseX, mouseY));
		removeParticles();
	}
	else if(key == '4') bDraw4Forces = !bDraw4Forces;
	else if(key=='r') fRed=!fRed;
	
	isKeyPressed = true;
}

//--------------------------------------------------------------
void menu::keyReleased(int key) {
	isKeyPressed = false;
}

//--------------------------------------------------------------
void menu::mouseMoved(int x, int y ) {
	
}

//--------------------------------------------------------------
void menu::mouseDragged(int x, int y, int button) {
}

//--------------------------------------------------------------
void menu::mousePressed(int x, int y, int button) {
	isMousePressed = true;
	ofLogNotice("menu - mousePressed");
}

//--------------------------------------------------------------
void menu::mouseReleased(int x, int y, int button) {
	isMousePressed = false;
}

//--------------------------------------------------------------
void menu::resized(int w, int h){
	borde.clear();
	ofColor ctmp = ofColor::black;
	borde.setFillColor(ctmp);
	//http://www.openframeworks.cc/documentation/graphics/ofPath.html#show_setPolyWindingMode
	borde.setPolyWindingMode(OF_POLY_WINDING_ODD);
	// rectangulo 
	borde.rectangle(0,0,ofGetWidth(),ofGetHeight());
	borde.setCircleResolution(60);
	borde.circle(ofGetWidth()/2,ofGetHeight()/2,ofGetHeight()/2*0.95);	
	
	centro = ofVec2f(ofGetWidth()/2.0, ofGetHeight()/2.0);
	distConf = ofGetHeight()/2.0*0.9;
  	
}


// - - - - - TUIOs - - - - - 

void menu::tuioAdded(ofxTuioCursor &tuioCursor){
	ofPoint loc = ofPoint(tuioCursor.getX()*W_WIDTH,tuioCursor.getY()*W_HEIGHT);
	//cout << "Point n" << tuioCursor.getSessionId() << " add at " << loc << endl;
    
    handShadow *h1 = new handShadow();
    h1->setup();
    hands.addObject(*h1);
    hands.notifyTouch(loc.x, loc.y,tuioCursor.getSessionId());
    touchElements.notifyTouch(loc.x, loc.y,tuioCursor.getSessionId());
}

void menu::tuioUpdated(ofxTuioCursor &tuioCursor){
    int mx = W_WIDTH*tuioCursor.getX();
    int my = W_HEIGHT*tuioCursor.getY();
	ofPoint loc = ofPoint(mx,my);
	//cout << "Point n" << tuioCursor.getSessionId() << " updated at " << loc << endl;
    
    hands.notifySlide(loc.x, loc.y,tuioCursor.getSessionId(),tuioCursor.getMotionAccel());
    touchElements.notifySlide(loc.x, loc.y,tuioCursor.getSessionId(),tuioCursor.getMotionAccel());
}

void menu::tuioRemoved(ofxTuioCursor &tuioCursor){
	ofPoint loc = ofPoint(tuioCursor.getX()*W_WIDTH,tuioCursor.getY()*W_HEIGHT);
    //cout << "Point n" << tuioCursor.getSessionId() << " remove at " << loc << endl;
    /*
     if(id_slider1==tuioCursor.getSessionId()) id_slider1=NULL;
     if(id_jumpingRect==tuioCursor.getSessionId()) id_jumpingRect=NULL;
     if(id_moverect==tuioCursor.getSessionId()) id_moverect=NULL;
	 */
    
	//    
    hands.removeObjectByTuioID(tuioCursor.getSessionId() );
    touchElements.notifyTouchUp( tuioCursor.getSessionId() );
    
}



void menu::onButtonPressed(BUTTON_TYPE1& mtype){
//    cout <<" buttonpressed " << endl;
//    if(mtype==TYPE_ACC){
//        anillo.acelera(-anillo.dAcc);
//        cout <<" acelera " << endl;
//    }
//    else if(mtype==TYPE_CRASH){
//        cout <<" crash " << endl;
//		// Lanzarla con velocidad que tenga el anillo
//		
//		float vvVal = anillo.getVelocNeutronLanz();
//		float vvAng = HALF_PI;
//		
//		addNeutron(W_WIDTH/2.0,W_HEIGHT/2.0-radioInt*0.9,vvVal,vvAng);
//	}
//	
}


