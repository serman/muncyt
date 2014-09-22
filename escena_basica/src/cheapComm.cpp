//
//  cheapComm.cpp
//  panel1_tracking
//
//  Created by Sergio Galan on 5/7/14.
//
//

#include "cheapComm.h"

cheapComm::cheapComm(){

}

void cheapComm::setup(){
    int serverRecvPort = 9000;
    string dst="192.168.0.255";
	myosc.setup(dst, serverRecvPort);
	int maxServerMessages = 38;

	ofLogNotice("> >> >>> >> > >> >>> >> > >> >>> >> > >> cheapComm::setup()");
	
	// Add Listeners de eventos
//	ofAddListener(eventoOSC, this, &cheapComm::recibirEventoOSC  );
	
}

//void cheapComm::recibirEventoOSC( oscData &mydata ) {
//	
//	ofLogNotice("Evento Enviar-OSC recibido.");
//	
//	if(mydata.tipoOSCDato == EXPLOSION) {
//		sendExplosion();
//	}
//	
//}


void cheapComm::recibirEventoOSC( oscData &mydata ) {
	
	ofLogNotice("Evento Enviar-OSC recibido.");
	
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