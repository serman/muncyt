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
    string dst="localhost";
	myosc.setup(serverRecvPort);
}

void cheapComm::oscRcvUpdate(){
	while(myosc.hasWaitingMessages()){
        if( ((testApp*)ofGetAppPtr())->appStatuses["forceCamera"]==true) {
            cout << "salimos forzados";
         return;
            
        }
		// get the next message
		ofxOscMessage m;
		myosc.getNextMessage(&m);
		// check for mouse moved message
		if(m.getAddress() == "/video/front"){
            ((testApp*)ofGetAppPtr())->setFrontCamera(); //TODO aqui hayuq eponer setFrontCamera cuando esté la segunda camara
		}
        if(m.getAddress() == "/video/ceil"){
            ((testApp*)ofGetAppPtr())->setCeilCamera();
		}
    }
}
