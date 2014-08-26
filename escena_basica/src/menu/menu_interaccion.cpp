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

void menu::interaccion_point(ofVec2f ptF, bool isNeg) {
	float minDis = ofGetHeight()/3.0;// (isNeg)? 400 : 200;
	
	float minDis2 = minDis*minDis;
	
	float ff = 1.0;

	float fFuerza = 45.0;
	if(ptF.distance(centro)<distConf) {
//	if(ptF.squareDistance(centro)<distConf) {
		if(isNeg) {
			// Atrae circulos y repele boxes
			for(int i=0; i<circles.size(); i++) {
//				float dis = ptF.distance(circles[i].get()->getPosition());
				float dis2 = ptF.squareDistance(circles[i].get()->getPosition());
//				if(dis < minDis) 
//				if(dis2 < minDis2) 
//					circles[i].get()->addRepulsionForce(ptF, ff*1.4f*fFuerza/dis2);//3, 9);
//				else 
//					circles[i].get()->addAttractionPoint(ptF, ff*2.2*fFuerza/dis2);//4.0);
				
				circles[i].get()->addAttractionPoint(ptF, ff*2.2*fFuerza/dis2);//4.0);
				
			}
			for(int i=0; i<boxes.size(); i++) {
				float dis = ptF.distance(boxes[i].get()->getPosition());
				float dis2 = dis*dis;//ptF.squareDistance(boxes[i].get()->getPosition());
//				if(dis < minDis) 
//				if(dis2 < minDis2) 
//					boxes[i].get()->addAttractionPoint(ptF, ff*1.2*fFuerza/dis2);
//				else 
//					boxes[i].get()->addRepulsionForce(ptF, ff*0.8*fFuerza/dis2);//4.0);
				boxes[i].get()->addRepulsionForce(ptF, ff*0.9*fFuerza/dis2);//4.0);
			}
		}
		else {
			// Mouse Pressed
			// Atrae boxes y repele circulos
			for(int i=0; i<circles.size(); i++) {
//				float dis = ptF.distance(circles[i].get()->getPosition());
				float dis2 = ptF.squareDistance(circles[i].get()->getPosition());
//				if(dis < minDis) 
//				if(dis2 < minDis2) 
//					circles[i].get()->addAttractionPoint(ptF, ff*1.2*fFuerza/dis2);//3, 9);
//				else 
					circles[i].get()->addRepulsionForce(ptF, ff*0.8*fFuerza/dis2);//4.0);
			}
			for(int i=0; i<boxes.size(); i++) {
				float dis = ptF.distance(boxes[i].get()->getPosition());
				float dis2 = ptF.squareDistance(boxes[i].get()->getPosition());
//				if(dis < minDis) 
				if(dis2 < minDis2) 
//					boxes[i].get()->addRepulsionForce(ptF, ff*1.4*fFuerza/dis);
//				else 
					boxes[i].get()->addAttractionPoint(ptF, ff*2.2*fFuerza/dis2);//4.0);
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

	else if(key=='f') bDrawFbos=!bDrawFbos;
	else if(key=='d') swFuerzaDensidad=!swFuerzaDensidad;
	else if(key=='m') isMousePressed=!isMousePressed;
	
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
	ofLogNotice("menu - mouseReleased");
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

ofPoint menu::transf_PosTUIO(ofxTuioCursor & tuioCursor) {

    int mx = W_WIDTH*tuioCursor.getX() + (ofGetWidth()-W_WIDTH)/2.0;
    int my = W_HEIGHT*tuioCursor.getY();
//	ofPoint loc = ofPoint(mx,my);

	return ofPoint(mx,my);
}

void menu::tuioAdded(ofxTuioCursor &tuioCursor){
//    int mx = W_WIDTH*tuioCursor.getX();
////    int mx = W_WIDTH*(tuioCursor.getX()+0.5);
//    int my = W_HEIGHT*tuioCursor.getY();
//	ofPoint loc = ofPoint(mx,my);

	ofPoint loc = transf_PosTUIO(tuioCursor);

	
	cout << "Point n" << tuioCursor.getSessionId() << " add at " << loc << endl;
    
    handShadow *h1 = new handShadow();
    h1->setup();
    hands.addObject(*h1);
    hands.notifyTouch(loc.x, loc.y,tuioCursor.getSessionId());
    touchElements.notifyTouch(loc.x, loc.y,tuioCursor.getSessionId());
}

void menu::tuioUpdated(ofxTuioCursor &tuioCursor){
	//    int mx = W_WIDTH*tuioCursor.getX();
	////    int mx = W_WIDTH*(tuioCursor.getX()+0.5);
	//    int my = W_HEIGHT*tuioCursor.getY();
	//	ofPoint loc = ofPoint(mx,my);
	
	ofPoint loc = transf_PosTUIO(tuioCursor);
	cout << "Point n" << tuioCursor.getSessionId() << " updated at " << loc << endl;
    
    hands.notifySlide(loc.x, loc.y, tuioCursor.getSessionId(),tuioCursor.getMotionAccel());
    touchElements.notifySlide(loc.x, loc.y,tuioCursor.getSessionId(),tuioCursor.getMotionAccel());
}

void menu::tuioRemoved(ofxTuioCursor &tuioCursor){
	//    int mx = W_WIDTH*tuioCursor.getX();
	////    int mx = W_WIDTH*(tuioCursor.getX()+0.5);
	//    int my = W_HEIGHT*tuioCursor.getY();
	//	ofPoint loc = ofPoint(mx,my);
	
	ofPoint loc = transf_PosTUIO(tuioCursor);
    cout << "Point n" << tuioCursor.getSessionId() << " remove at " << loc << endl;
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


