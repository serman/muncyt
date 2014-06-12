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
	myosc.setup(dst, serverRecvPort);
	int maxServerMessages = 38;


}


/**
void cheapComm::sendBlobs( vector<ofxBlob> blobList){
	int i;
    for(i=0; i<blobList.size(); i++){
        ofxOscMessage m;
		m.setAddress("/blob");
		m.addIntArg(blobList[i].angleBoundingRect.x);
        m.addIntArg(blobList[i].angleBoundingRect.y);
        m.addIntArg(blobList[i].angleBoundingRect.width);
		myosc.sendMessage(m);
    }
}**/