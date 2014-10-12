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
    int serverRecvPortData = 9000;
    int serverRecvPortAudio = 7777;
    string dst="192.168.1.8";
	myOscAudio.setup(dst, serverRecvPortAudio);
    
    myOscData.setup("192.168.1.255",serverRecvPortData);
	int maxServerMessages = 38;
	ofLogNotice("> >> >>> >> > >> >>> >> > >> >>> >> > >> cheapComm::setup()");
	
		
}

/*void cheapComm::eventoOSC( oscData &mydata ) {
	
	ofLogNotice("Evento Enviar-OSC recibido.");
//	
	if(mydata.tipoOSCDato == EXPLOSION) {
		sendExplosion();
	}
//	
}*/


//void cheapComm::recibirEventoOSC( oscData &mydata ) {
	
//	ofLogNotice("Evento Enviar-OSC recibido.");
	
//}

void cheapComm::sendAudio0(string pattern){
    ofxOscMessage m;
    m.setAddress(pattern);
    myOscAudio.sendMessage(m);
    
}

void cheapComm::sendAudio1(string pattern, float i0){
    ofxOscMessage m;
    m.setAddress(pattern);
    m.addFloatArg(i0);
    myOscAudio.sendMessage(m);
    
}

void cheapComm::sendAudio2(string pattern, float i0, float i1){
    ofxOscMessage m;
    m.setAddress(pattern);
    m.addFloatArg(i0);
    m.addFloatArg(i1);
    myOscAudio.sendMessage(m);
}

void cheapComm::sendAudio3(string pattern, float i0, float i1, float i2){
    ofxOscMessage m;
    m.setAddress(pattern);
    m.addFloatArg(i0);
    m.addFloatArg(i1);
    m.addFloatArg(i2);
    myOscAudio.sendMessage(m);
}


void cheapComm::sendSync0(string pattern){
    ofxOscMessage m;
    m.setAddress(pattern);
    myOscData.sendMessage(m);
    
}

void cheapComm::sendSync1(string pattern, float i0){
    ofxOscMessage m;
    m.setAddress(pattern);
    m.addFloatArg(i0);
    myOscData.sendMessage(m);
    
}

void cheapComm::sendSync2(string pattern, float i0, float i1){
    ofxOscMessage m;
    m.setAddress(pattern);
    m.addFloatArg(i0);
    m.addFloatArg(i1);
    myOscData.sendMessage(m);
}

void cheapComm::sendSync3(string pattern, float i0, float i1, float i2){
    ofxOscMessage m;
    m.setAddress(pattern);
    m.addFloatArg(i0);
    m.addFloatArg(i1);
    m.addFloatArg(i2);
    myOscData.sendMessage(m);
}

void cheapComm::sendSync4(string pattern, float i0, float i1, float i2, float i3){
    ofxOscMessage m;
    m.setAddress(pattern);
    m.addFloatArg(i0);
    m.addFloatArg(i1);
    m.addFloatArg(i2);
        m.addFloatArg(i3);
    myOscData.sendMessage(m);
}



