//
//  cheapComm.cpp
//  panel1_tracking
//
//  Created by Sergio Galan on 5/7/14.
//
//

#include "cheapComm.h"
cheapComm* cheapComm::m_pInstance = NULL;

cheapComm::cheapComm(){

}

void cheapComm::setup(){
//    int serverRecvPort = 9000;
    int serverRecvPort = 7777;
    string dst="192.168.1.8";
	myosc.setup(dst, serverRecvPort);
	int maxServerMessages = 38;

	ofLogNotice("> >> >>> >> > >> >>> >> > >> >>> >> > >> cheapComm::setup()");
	
	// Add Listeners de eventos
	//ofAddListener(eventoOSC, this, &cheapComm::recibirEventoOSC  );
	
}

void cheapComm::eventoOSC( oscData &mydata ) {
	
	ofLogNotice("Evento Enviar-OSC recibido.");
//	
	if(mydata.tipoOSCDato == EXPLOSION) {
		sendExplosion();
	}
//	
}


//void cheapComm::recibirEventoOSC( oscData &mydata ) {
	
//	ofLogNotice("Evento Enviar-OSC recibido.");
	
//}

void cheapComm::sendAudio0(string pattern){
    ofxOscMessage m;
    m.setAddress(pattern);
    myosc.sendMessage(m);
    
}

void cheapComm::sendAudio1(string pattern, float i0){
    ofxOscMessage m;
    m.setAddress(pattern);
    m.addFloatArg(i0);
    myosc.sendMessage(m);
    
}

void cheapComm::sendAudio2(string pattern, float i0, float i1){
    ofxOscMessage m;
    m.setAddress(pattern);
    m.addFloatArg(i0);
    m.addFloatArg(i1);
    myosc.sendMessage(m);
}

void cheapComm::sendAudio3(string pattern, float i0, float i1, float i2){
    ofxOscMessage m;
    m.setAddress(pattern);
    m.addFloatArg(i0);
    m.addFloatArg(i1);
    m.addFloatArg(i2);
    myosc.sendMessage(m);
}

void cheapComm::sendPosition( float sector, float speed ){
        ofxOscMessage m;
		m.setAddress("/position");
		m.addFloatArg(sector);
        m.addFloatArg(speed);
		myosc.sendMessage(m);
}

void cheapComm::sendExplosion(){
    ofxOscMessage m;
    m.setAddress("/explosion");
    myosc.sendMessage(m);  

}