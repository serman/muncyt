/*
*  TUIO.h
*  
*
*  Created on 2/2/09.
*  Copyright 2009 NUI Group. All rights reserved.
*
*/

#include "TUIO.h"

TUIO::TUIO(){
	
}

TUIO::~TUIO(){
	// this could be useful for whenever we get rid of an object
}

void TUIO::setup(const char* host, int port) {
	localHost = host;
	TUIOPort = port;
	frameseq = 0;
	
	//FOR OSC
	TUIOSocket.setup(localHost, TUIOPort);
}


void TUIO::sendTUIO(vector<BlobX> blobs) { 
	frameseq += 1;
	
	// if sending OSC (not TCP)
	ofxOscBundle b;
	ofxOscMessage alive;
	// Sends alive message - saying 'Hey, there's no alive blobs'
	alive.setAddress("/tuio/2Dcur");
	alive.addStringArg("alive");	// lista con los ids de blobs detectados
	
	// Send fseq message
	ofxOscMessage fseq;
	fseq.setAddress( "/tuio/2Dcur" );
	fseq.addStringArg( "fseq" );
	fseq.addIntArg(frameseq);
	
	if(blobs.size() == 0){
		// si no hay blobs:
		b.addMessage( alive );		// add message to bundle
		b.addMessage( fseq );		// add message to bundle
		TUIOSocket.sendBundle( b ); // send bundle
	} 
	else { 
		// actually send the blobs
//		for(int i=0; i<blobs.size(); i++) {
		vector<BlobX>::iterator blob;
		for(blob = blobs.begin(); blob != blobs.end(); blob++) {
			// omit point (0,0) since this means that we are outside of the range
			if(blob->pos.x == 0 && blob->pos.y == 0)
				continue;
				
			//Set Message
			ofxOscMessage set;
			set.setAddress( "/tuio/2Dcur" );
			set.addStringArg("set");
			set.addIntArg(blob->label);		// id
			set.addFloatArg(blob->pos.x);	// x
			set.addFloatArg(blob->pos.y);	// y
			set.addFloatArg(blob->vel.x);	// dX
			set.addFloatArg(blob->vel.y);	// dY
			set.addFloatArg(blob->maccel);	// vel
//			if(bHeightWidth){
//				set.addFloatArg(blob->boundingRect.x);	// wd
//				set.addFloatArg(blob->boundingRect.y);	// ht
//			}
			b.addMessage( set );							// add message to bundle
			alive.addIntArg(blob->label);				// add blob to list of ALL active IDs
		}
		
		b.addMessage( alive );	 //add message to bundle
		b.addMessage( fseq );	 //add message to bundle
		TUIOSocket.sendBundle( b ); //send bundle
	}
}