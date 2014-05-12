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
		// get the next message
		ofxOscMessage m;
		myosc.getNextMessage(&m);
		// check for mouse moved message
		if(m.getAddress() == "/blob"){
			// both the arguments are int32's
			int blob_x = m.getArgAsInt32(0);
			int blob_y = m.getArgAsInt32(1);
            int blob_w = m.getArgAsInt32(2);
		}
    }
}