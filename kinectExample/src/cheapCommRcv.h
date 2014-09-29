//
//  cheamComm.h
//  panel1_tracking
//
//  Created by Sergio Galan on 5/7/14.
//
//

#ifndef __panel1_tracking__cheapComm__
#define __panel1_tracking__cheapComm__

#include <iostream>
#include "ofxOsc.h"
#include "osc_match.h"
#include "testApp.h"

class testApp;
class cheapCommRcv {
	public:
	    enum  {none,position, explosion};

	    void setup();
    	void update();
        cheapCommRcv();
	    ofxOscReceiver myosc;
//	    float remotePosition=0;
//    	float remoteSpeed=0;
		float remotePosition;
		float remoteSpeed;
    
    private:

    

};

#endif /* defined(__panel1_tracking__cheapComm__) */