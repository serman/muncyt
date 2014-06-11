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
    
	
}

int cheapCommRcv::update(){
    int positionReponse=none;
	while(myosc.hasWaitingMessages()){
		// get the next message
		ofxOscMessage m;
		myosc.getNextMessage(&m);
		// check for mouse moved message
		if(m.getAddress() == "/position"){
            positionReponse=position;
			remotePosition = m.getArgAsFloat(0);
			remoteSpeed = m.getArgAsFloat(1);
		}
        if(m.getAddress() == "/explosion"){
                    cout << "explosiooooooon"<< endl;
			//TODO trigger explosion event
            positionReponse=explosion;
		}
    }
    return positionReponse;
}