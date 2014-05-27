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
#endif /* defined(__panel1_tracking__cheapComm__) */


class cheapComm {
	public:
        cheapComm();    
	    void setup();
		void sendPosition( float sector, float speed );
    	void sendExplosion();

	    ofxOscSender myosc;
    	
    private:
        
    

};