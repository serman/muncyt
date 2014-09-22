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
#include "consts.h"

static ofEvent	<oscData>	eventoOSC;

class cheapComm {
	public:
        cheapComm();    
	    void setup();

		void recibirEventoOSC( oscData &mydata );
	
		void sendPosition( float sector, float speed );
    	void sendExplosion();

	    ofxOscSender myosc;
    	
    private:
	
};

#endif /* defined(__panel1_tracking__cheapComm__) */
