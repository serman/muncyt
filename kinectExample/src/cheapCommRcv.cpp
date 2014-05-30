//
//  cheapComm.cpp
//  panel1_tracking
//
//  Created by Sergio Galan on 5/7/14.
//
//

#include "cheapCommRcv.h"

cheapCommRcv::cheapCommRcv(){

}
void cheapCommRcv::setup(){
    int serverRecvPort = 9000;
	myosc.setup(serverRecvPort);

	remotePosition=0;
	remoteSpeed=0;


}

bool cheapCommRcv::update(){
    bool positionReponse=false;
	while(myosc.hasWaitingMessages()){
		// get the next message
		ofxOscMessage m;
		myosc.getNextMessage(&m);
		// check for mouse moved message
		if(m.getAddress() == "/position"){
            positionReponse=true;
			remotePosition = m.getArgAsFloat(0);
			remoteSpeed = m.getArgAsFloat(1);
		}
        if(m.getAddress() == "/explosion"){
			//TODO trigger explosion event
		}
    }
    return positionReponse;
}