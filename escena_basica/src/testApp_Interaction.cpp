//
//  testAppInteraction.cpp
//  escena_basica
//
//  Created by Sergio Galan on 7/10/14.
//
//

#include "testApp.h"

//--------------------------------------------------------------
void testApp::keyPressed(int key) {
	ofLogVerbose("keyPressed");
	
	if(key == 'c') {
        //		addNucleo(mouseX, mouseY, rNucleo);
		addNucleo();
	}
	
	if(key == 'n') {
		addNeutron(mouseX, mouseY);
	}
	
	if(key=='q') {
		// Quitar particulas
		for(int i=0; i<nucleos.size(); i++) {
			nucleos[i].get()->destroy();
		}
		nucleos.clear();
		for(int i=0; i<neutrones.size(); i++) {
			neutrones[i].get()->destroy();
		}
		neutrones.clear();
	}
	
	if(key=='f') swFuerza = !swFuerza;
	
	if(key=='x') {
		myComm.sendExplosion();
	}
	
	if(key=='t') {
		fpsAct = ofMap(mouseX,0,W_WIDTH, 20.0, 600.0);
		box2d.setFPS(fpsAct);
	}
	
	if(key == 's') ofToggleFullscreen();
	
	if(key=='b') swBlendModeADD=!swBlendModeADD;
	
	if(key=='1'){
		// cambiar particulas a modo PARTIC
		setModoParticulas(MODO_PARTIC);
	}
	else if(key=='2') {
		// cambiar particulas a modo PATH
		setModoParticulas(MODO_PATH);
	}
	else if(key=='4') {
		swDrawTRAILS = !swDrawTRAILS;
	}
}

void testApp::setModoParticulas(int _modo) {
	modoDrawParticulas = _modo;
	ofLogVerbose("setModoParticulas modo Draw: " + ofToString(_modo));
	for(int i=0; i<neutrones.size(); i++) {
		ofLogVerbose("neutron: " + ofToString(i));
		neutrones[i].get()->setModoDraw(_modo);
	}
	for(int i=0; i<nucleos.size(); i++) {
		nucleos[i].get()->setModoDraw(_modo);
	}
}

//--------------------------------------------------------------
void testApp::keyReleased(int key) {
	ofLogVerbose("keyReleased");
    
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ) {
	
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button) {
	if(marco.inside(X_(x),y)) {
        fuerzaWAng = ofDegToRad( ofMap(x-marco.x, 0, marco.width, 30, 360*6) );
        fuerzaVal = ( ofMap(y-marco.y, 0, marco.height, 0, 200) );
	}
	
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button) {
	if(marco.inside(X_(x),y)) {
		fuerzaWAng = ofDegToRad( ofMap(X_(x)-marco.x, 0, marco.width, 30, 360*6) );
		fuerzaVal = ( ofMap(y-marco.y, 0, marco.height, 0, 200) );
	}
    
	//anillo.interaccionAnillo(X_(x),y);
	
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button) {
	if(!swFuerza) {
		chispa.pos = ofPoint(X_(x),y);
		chispa.start();
		
		addDestello(X_(x),y);
	}
}

void testApp::tuioAdded(ofxTuioCursor &tuioCursor){
	ofPoint loc = ofPoint(tuioCursor.getX()*W_WIDTH,tuioCursor.getY()*W_HEIGHT);
	//cout << "Point n" << tuioCursor.getSessionId() << " add at " << loc << endl;
    
    
    // o1.touch(loc.x, loc.y,tuioCursor.getSessionId());
    // h1.touch(loc.x, loc.y,tuioCursor.getSessionId());
    
    handShadow *h1 = new handShadow();
    h1->setup();
    hands.addObject(*h1);
    hands.notifyTouch(loc.x, loc.y,tuioCursor.getSessionId());
    touchElements.notifyTouch(loc.x, loc.y,tuioCursor.getSessionId());
}

void testApp::tuioUpdated(ofxTuioCursor &tuioCursor){
    int mx = W_WIDTH*tuioCursor.getX();
    int my =    W_HEIGHT*tuioCursor.getY();
	ofPoint loc = ofPoint(tuioCursor.getX()*W_WIDTH,tuioCursor.getY()*W_HEIGHT);
	//cout << "Point n" << tuioCursor.getSessionId() << " updated at " << loc << endl;
    
    
    //o1.slide(loc.x, loc.y,tuioCursor.getSessionId(),tuioCursor.getMotionAccel());
    // h1.slide(loc.x, loc.y,tuioCursor.getSessionId(),tuioCursor.getMotionAccel());
    touchElements.notifySlide(loc.x, loc.y,tuioCursor.getSessionId(),tuioCursor.getMotionAccel());
    hands.notifySlide(loc.x, loc.y,tuioCursor.getSessionId(),tuioCursor.getMotionAccel());
}

void testApp::tuioRemoved(ofxTuioCursor &tuioCursor){
	ofPoint loc = ofPoint(tuioCursor.getX()*W_WIDTH,tuioCursor.getY()*W_HEIGHT);
    //cout << "Point n" << tuioCursor.getSessionId() << " remove at " << loc << endl;
    /*
     if(id_slider1==tuioCursor.getSessionId()) id_slider1=NULL;
     if(id_jumpingRect==tuioCursor.getSessionId()) id_jumpingRect=NULL;
     if(id_moverect==tuioCursor.getSessionId()) id_moverect=NULL;*/
    
    
    hands.removeObjectByTuioID(tuioCursor.getSessionId() );
    
    touchElements.notifyTouchUp( tuioCursor.getSessionId() );
    
}

void testApp::onButtonPressed(BUTTON_TYPE& mtype){
    cout <<" buttonpressed " << endl;
    if(mtype==TYPE_ACC){
        anillo.acelera(-anillo.dAcc);
        cout <<" acelera " << endl;
    }
    else if(mtype==TYPE_CRASH){
        cout <<" crash " << endl;
    }
}


