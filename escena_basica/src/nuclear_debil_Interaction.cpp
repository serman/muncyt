//
//  testAppInteraction.cpp
//  escena_basica
//
//  Created by Sergio Galan on 7/10/14.
//
//

#include "nuclear_debil.h"

//--------------------------------------------------------------
void nuclear_debil::keyPressed(int key) {
	ofLogVerbose("keyPressed");
	
	if(key == 'c') {
        //		addNucleo(mouseX, mouseY, rNucleo);
		addNucleo();
	}
	
	if(key == 'n') {
		addNeutron(ofGetAppPtr()->mouseX, ofGetAppPtr()->mouseY);
	}
	
	if(key=='q') {
		// Quitar particulas
		borrar_nucleos();
		borrar_neutrones();
//		for(int i=0; i<nucleos.size(); i++) {
//			nucleos[i].get()->destroy();
//		}
//		nucleos.clear();
//		for(int i=0; i<neutrones.size(); i++) {
//			neutrones[i].get()->destroy();
//		}
//		neutrones.clear();
	}
	
	if(key=='f') swFuerza = !swFuerza;
	
    if(key=='x') {
		ball_inside_event();
	}
    if(key=='e'){
        cheapComm::getInstance()->sendAudio0("/audio/weak_nuclear/chain_reaction_event");
    }
	
	if(key=='t') {
		fpsAct = ofMap(ofGetAppPtr()->mouseX,0,W_WIDTH, 20.0, 600.0);
		box2d.setFPS(fpsAct);
	}
    if(key=='a'){
        anillo.acelera(-anillo.dAcc/2);
    }
	
//	if(key == 's') ofToggleFullscreen();
	
	if(key=='b') swBlendModeADD=!swBlendModeADD;
	
	if(key=='y'){
		// cambiar particulas a modo PARTIC
		setModoParticulas(MODO_PARTIC);
	}
	else if(key=='u') {
		// cambiar particulas a modo PATH
		setModoParticulas(MODO_PATH);
	}
	else if(key=='i') {
		// cambiar particulas a modo BOX2D
		setModoParticulas(MODO_BOX2D);
	}
	
	else if(key=='j') {
		swDrawTRAILS = !swDrawTRAILS;
	}
    else if(key=='l'){
        
        addSprite(ofRandom(50,500),ofRandom(50,500));
    }
}

void nuclear_debil::setModoParticulas(int _modo) {
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
void nuclear_debil::keyReleased(int key) {
	ofLogVerbose("keyReleased");
    
}

//--------------------------------------------------------------
void nuclear_debil::mouseMoved(int x, int y ) {
	
}

//--------------------------------------------------------------
void nuclear_debil::mouseDragged(int x, int y, int button) {
	if(marco.inside(X_(x),y)) {
        fuerzaWAng = ofDegToRad( ofMap(x-marco.x, 0, marco.width, 30, 360*6) );
        fuerzaVal = ( ofMap(y-marco.y, 0, marco.height, 0, 200) );
	}
	
}

//--------------------------------------------------------------
void nuclear_debil::mousePressed(int x, int y, int button) {
	if(marco.inside(X_(x),y)) {
		fuerzaWAng = ofDegToRad( ofMap(X_(x)-marco.x, 0, marco.width, 30, 360*6) );
		fuerzaVal = ( ofMap(y-marco.y, 0, marco.height, 0, 200) );
	}
    
	//anillo.interaccionAnillo(X_(x),y);
	cout << X_(x) << " " << y << endl;
}

//--------------------------------------------------------------
void nuclear_debil::mouseReleased(int x, int y, int button) {
	if(!swFuerza) {
		chispa.pos = ofPoint(X_(x),y);
		chispa.start();
		
		addDestello(X_(x),y);
	}
}

void nuclear_debil::tuioAdded(ofxTuioCursor &tuioCursor){
	ofPoint loc = ofPoint(tuioCursor.getX()*W_WIDTH,tuioCursor.getY()*W_HEIGHT);
	//cout << "Point n" << tuioCursor.getSessionId() << " add at " << loc << endl;
    
    // o1.touch(loc.x, loc.y,tuioCursor.getSessionId());
    // h1.touch(loc.x, loc.y,tuioCursor.getSessionId());
    
    handShadow *h1 = new handShadow();
    h1->setup();
    hands.addObject(*h1);
    hands.notifyTouch(loc.x, loc.y,tuioCursor.getSessionId());
    touchElements.notifyTouch(loc.x, loc.y,tuioCursor.getSessionId());
            cheapComm::getInstance()->sendAudio0("/audio/weak_nuclear/hand_on_event");
    
}

void nuclear_debil::tuioUpdated(ofxTuioCursor &tuioCursor){
    int mx = W_WIDTH*tuioCursor.getX();
    int my =  W_HEIGHT*tuioCursor.getY();
	ofPoint loc = ofPoint(tuioCursor.getX()*W_WIDTH,tuioCursor.getY()*W_HEIGHT);
	//cout << "Point n" << tuioCursor.getSessionId() << " updated at " << loc << endl;
    
    //o1.slide(loc.x, loc.y,tuioCursor.getSessionId(),tuioCursor.getMotionAccel());
    // h1.slide(loc.x, loc.y,tuioCursor.getSessionId(),tuioCursor.getMotionAccel());
    touchElements.notifySlide(loc.x, loc.y,tuioCursor.getSessionId(),tuioCursor.getMotionAccel());
    hands.notifySlide(loc.x, loc.y,tuioCursor.getSessionId(),tuioCursor.getMotionAccel());
}

void nuclear_debil::tuioRemoved(ofxTuioCursor &tuioCursor){
	ofPoint loc = ofPoint(tuioCursor.getX()*W_WIDTH,tuioCursor.getY()*W_HEIGHT);
    //cout << "Point n" << tuioCursor.getSessionId() << " remove at " << loc << endl;
    /*
     if(id_slider1==tuioCursor.getSessionId()) id_slider1=NULL;
     if(id_jumpingRect==tuioCursor.getSessionId()) id_jumpingRect=NULL;
     if(id_moverect==tuioCursor.getSessionId()) id_moverect=NULL;*/
    
    
    hands.removeObjectByTuioID(tuioCursor.getSessionId() );
    touchElements.notifyTouchUp( tuioCursor.getSessionId() );
    cheapComm::getInstance()->sendAudio0("/audio/weak_nuclear/hand_off_event");
    
}

void nuclear_debil::onButtonPressed(BUTTON_TYPE& mtype){
    cout <<" buttonpressed " << endl;
    if(mtype==TYPE_ACC){
        anillo.acelera(-anillo.dAcc/2);
        cout <<" acelera " << endl;
        cheapComm::getInstance()->sendAudio0("/audio/weak_nuclear/acceleration_event");
    }
    else if(mtype==TYPE_CRASH){
		// Lanzarla con velocidad que tenga el anillo
		ball_inside_event();
	}
	
}

void nuclear_debil::ball_inside_event(){
    float vvVal = anillo.getVelocNeutronLanz();
    float vvAng = HALF_PI;
    
    if(abs(anillo.wAng)>=anillo.wAngMax*0.95){
        status=nuclear_debil::EXPLOSION;
        init_explosion_time=ofGetElapsedTimeMillis();
        ofLogNotice() << "MODO explosion" <<endl;
    }
    
    addNeutron(W_WIDTH/2.0,W_HEIGHT/2.0-radioInt*0.9,vvVal,vvAng);
    cheapComm::getInstance()->sendAudio0("/audio/weak_nuclear/ball_inside_event");

    anillo.setFadeInMode();
    //TODO EVENTO EXPLOSION SI LAS VELOCIDAD ES DEL 100%

}



