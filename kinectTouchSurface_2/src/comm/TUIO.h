/*
*  TUIO.h
*  
*
*  Created on 2/2/09.
*  Copyright 2009 NUI Group. All rights reserved.
*
*/

#ifndef TUIO_H
#define TUIO_H

#include "ofxOsc.h"
#include "BlobX.h"


class TUIO {
public:		
	TUIO();
	~TUIO();
	
	//methods
	void setup(const char* host, int port);
	void sendTUIO(vector<BlobX> blobs);
	
	ofxOscSender	TUIOSocket; 
	const char*		localHost;
	int				TUIOPort;	
	bool 			bHeightWidth;
	
	bool			bBlobs;
	
private:
	int				frameseq;
	bool			send(string message);
	string			partialPrevMsg;
};


#endif