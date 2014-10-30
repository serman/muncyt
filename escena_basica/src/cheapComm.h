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
#include "ofxXmlSettings.h"

//static ofEvent	<oscData> eventoOSC;



class cheapComm {
	public:
    
	    void setup();

	    ofxOscSender myOscAudio;
        ofxOscSender myOscData;
    
        static cheapComm *getInstance(){
            if (!m_pInstance)   // Only allow one instance of class to be generated.
                m_pInstance = new cheapComm;
            return m_pInstance;

        }
        void sendAudio0(string pattern);
        void sendAudio1(string, float );
        void sendAudio2(string, float , float);
        void sendAudio3(string, float , float, float);
    
        void sendSync0(string pattern);
        void sendSync1(string, float );
        void sendSync2(string, float , float);
        void sendSync3(string, float , float, float);
        void sendSync4(string, float , float, float,float);
    
    private:
            ofxXmlSettings XML;
            cheapComm();
            cheapComm(cheapComm const&);              // Don't Implement
            void operator=(cheapComm const&); // Don't implement
            static cheapComm* m_pInstance;

};

#endif /* defined(__panel1_tracking__cheapComm__) */
