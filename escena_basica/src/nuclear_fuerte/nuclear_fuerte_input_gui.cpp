//
//  nuclear_fuerte_input_gui.cpp
//  escena_basica
//
//  Created by Sergio Galan on 10/17/14.
//
//

#include "nuclear_fuerte.h"

//--------------------------------------------------------------
void nuclear_fuerte::keyPressed(int key){
    
	if(key=='d') swDifraccion=!swDifraccion;
	else if(key=='b') swMagnetField=!swMagnetField;
	else if(key=='q') ratePartic++;
	else if(key=='a') ratePartic = (ratePartic>2)? ratePartic-1 : 1;
	else if(key=='p') {
		particulas.clear();
	}
	else if(key=='o') {
		clearEMitters = true;
        //		emitters.clear();
	}
	else if(key=='m') bDrawingMode=!bDrawingMode;
	else if(key=='e') bDrawCaminos=!bDrawCaminos;
	else if(key=='r') {
		bDrawPtosChoque=!bDrawPtosChoque;
		if(!bDrawPtosChoque) ptsChoque.clear();
	}
	else if(key=='t') bTiltCamino=!bTiltCamino;
    //else if(key==)
}


//--------------------------------------------------------------
void nuclear_fuerte::mouseDragged(int x, int y, int button){
	if(button==0) {
		if(bDrawingMode) {
		}
	}
	else if(button==2) {
		// mover el último emitter si esta en la zona de accion
		ofVec2f posTmp(x,y);
		posTmp -= zentro;
		if(posTmp.length()<=radioEscena) {
            //		float rhoTmp = posTmp.length();
            //		float angTmp = atan2(posTmp.y, posTmp.x);
            //		emitters[emitters.size()-1].setPos_Radial(rhoTmp, angTmp);
			emitters[emitters.size()-1].setPos_XY(ofGetMouseX()-zentro.x, ofGetMouseY()-zentro.y);
		}
	}
}

//--------------------------------------------------------------
void nuclear_fuerte::mousePressed(int x, int y, int button){
	
    //	if(camino.getVertices().size()>4) camino.curveTo(x,y);
    //	else camino.addVertex(x,y);
	
	if(button==0) {
		if(bDrawingMode) {
			ofVec2f posTmp(x,y);
			posTmp -= zentro;
			if(posTmp.length()<=radioEscena) {
			}
		}
		else emitter.bActivo = true;
	}
	else if(button==2) {
		// add nuevo emitter
		//
		ofVec2f posTmp(x,y);
		posTmp -= zentro;
		if(posTmp.length()<=radioEscena) {
			addEmisor(posTmp);
		}
	}
}

//--------------------------------------------------------------
void nuclear_fuerte::mouseReleased(int x, int y, int button){
    
	if(button==0) {
	}
	
	emitter.bActivo = false;
}


void nuclear_fuerte::tuioAdded(ofxTuioCursor &tuioCursor){
	//ofPoint loc = ofPoint(tuioCursor.getX()*W_WIDTH,tuioCursor.getY()*W_HEIGHT);
    ofPoint loc = transf_PosTUIO(tuioCursor);
    ofVec2f posTmp(loc.x,loc.y);
    posTmp -= zentro;
    if(posTmp.length()<=radioEscena) {
        Emisor * e=addEmisor(posTmp,tuioCursor.getSessionId());
        cheapComm::getInstance()->sendAudio0("/audio/strong_nuclear/hand_on_event");
        
    }
    
    
    int tipo=(int)ofRandom(0,4);
    exchangeColors(tipo);
    if(tipo==1){
        cheapComm::getInstance()->sendSync4("/sync/strong_nuclear/hand_on_event",1.0,tipo,colorp1,(float)colorp2);
        cheapComm::getInstance()->sendSync2("/sync/strong_nuclear/setColor",1.0,colorp1);
        cheapComm::getInstance()->sendSync2("/sync/strong_nuclear/setColor",2.0,colorp2);
    }
    else if(tipo==2){
        cheapComm::getInstance()->sendSync4("/sync/strong_nuclear/hand_on_event",1.0,tipo,colorp2,(float)colorp3);
        cheapComm::getInstance()->sendSync2("/sync/strong_nuclear/setColor",2.0,colorp2);
        cheapComm::getInstance()->sendSync2("/sync/strong_nuclear/setColor",3.0,colorp3);
    }
    else if(tipo==3){
        cheapComm::getInstance()->sendSync4("/sync/strong_nuclear/hand_on_event",1.0,tipo,(float)colorp3,(float)colorp1);
        cheapComm::getInstance()->sendSync2("/sync/strong_nuclear/setColor",3.0,colorp3);
        cheapComm::getInstance()->sendSync2("/sync/strong_nuclear/setColor",1.0,colorp1);
    }
}
void nuclear_fuerte::tuioRemoved(ofxTuioCursor &tuioCursor){
    //hands.removeObjectByTuioID(tuioCursor.getSessionId() );
    for(int i=0; i<emitters.size(); i++){
        if(emitters[i].tuio_id==tuioCursor.getSessionId()) {
            //elimino emisor
            emitters.erase(emitters.begin() + i);
            cheapComm::getInstance()->sendAudio0("/audio/strong_nuclear/hand_off_event");
        }
    }
}

void nuclear_fuerte::tuioUpdated(ofxTuioCursor &tuioCursor){
    ofPoint loc = transf_PosTUIO(tuioCursor);
    for(int i=0; i<emitters.size(); i++){
        if(emitters[i].tuio_id==tuioCursor.getSessionId()) {
            //actualizao posicion
            emitters[i].setPos_XY(loc.x-zentro.x, loc.y-zentro.y);
        }
    }
    
}



// - - - - GUI - - - -




void nuclear_fuerte::setupGUI() {
	
	gui1 = new ofxUISuperCanvas("STRONG NUCLEAR");
    gui1->addSpacer();
	//    gui1->addLabel("Press 'h' to Hide GUIs", OFX_UI_FONT_SMALL);
    
	//    gui1->addSpacer();
	gui1->addLabel("Magnetic Field");
	gui1->addToggle("(b) Magnetic Field ON/OFF", &swMagnetField );
	gui1->addSlider("Magnetic Force", -1.0, 1.0, &magnetField);
	
	gui1->addSpacer();
	gui1->addToggle("(m) Modo Dibujo ON/OFF", &bDrawingMode );
	gui1->addToggle("(d) Difracción ON/OFF", &swDifraccion );
	gui1->addToggle("(e) Draw Camino ON/OFF", &bDrawCaminos );
	gui1->addToggle("(r) Draw Colisiones ON/OFF", &bDrawPtosChoque );
	gui1->addToggle("(t) Tilt Camino ON/OFF", &bTiltCamino );
	gui1->addToggle("(o) Clear Emitters", &clearEMitters );
	gui1->addToggle("BackGround New", &bSetBack );
	gui1->addSlider("Alpha background", 0, 255, &alpha_Back);
	gui1->addSpacer();
	gui1->addButton("Save", bSave);
	gui1->addButton("Load", bSave);
	
    //	ofDrawBitmapString("'c' clear camino", 10,hLin); hLin+=dLin;
    //	ofDrawBitmapString("p clear partics", 10,hLin); hLin+=dLin;
	//
	gui1->addSpacer();
	gui1->addIntSlider("(q/a)_Rate_partics", 1,120, &ratePartic);
	gui1->addSpacer();
	
    gui1->autoSizeToFitWidgets();
	ofAddListener(gui1->newGUIEvent,this,&nuclear_fuerte::guiEvent);
}

void nuclear_fuerte::guiEvent(ofxUIEventArgs &e) {
	string name = e.getName();
	int kind = e.getKind();
	//	cout << "got event from: " << name << endl;
	if(name == "(r) Draw Colisiones ON/OFF")
	{
		ofxUIToggle *toggle = (ofxUIToggle *) e.getToggle();
		bDrawPtosChoque = toggle->getValue();
		if(!bDrawPtosChoque) ptsChoque.clear();
	}
	else if(name=="Save") {
		ofLogNotice("GRABAR");
		gui1->saveSettings("gui_nuclear_fuerte.xml");
	}
	else if(name=="Load") {
		ofLogNotice("CARGAR");
		gui1->loadSettings("gui_nuclear_fuerte.xml");
	}
	
}