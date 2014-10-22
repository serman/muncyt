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

void cheapCommRcv::update(){
	while(myosc.hasWaitingMessages()){
		// get the next message
		ofxOscMessage m;
		myosc.getNextMessage(&m);
		// check for mouse moved message
        //cout << m.getAddress()<<endl;
        
        //((testApp*)ofGetAppPtr());
        
        int pattrOffset =  0;
        int addrOffset  =  0;
        int matchResult1 = 0;
        int matchResult2 = 0;
        char* addr = (char*)(m.getAddress()).c_str();

   /*ir a escena */
        string pattern="/sync/*/start_event";
        matchResult1 = osc_match(pattern.c_str(), addr, &pattrOffset, &addrOffset);
        //cout << "matchresult aa"<< matchResult1 << endl;
        if(matchResult1==3){
            if(m.getAddress()=="/sync/electromagnetism/start_event")
                ((testApp*)ofGetAppPtr())->toScene(EM);
            if(m.getAddress()=="/sync/gravity/start_event")
                ((testApp*)ofGetAppPtr())->toScene(GRAVEDAD);
            if(m.getAddress()=="/sync/weak_nuclear/start_event")
                ((testApp*)ofGetAppPtr())->toScene(NUCLEAR_DEBIL);
            if(m.getAddress()=="/sync/strong_nuclear/start_event")
                ((testApp*)ofGetAppPtr())->toScene(NUCLEAR_FUERTE);
            if(m.getAddress()=="/sync/menu/start_event")
                ((testApp*)ofGetAppPtr())->toScene(MENU);
            //return;
           /* pattrOffset =  0;            addrOffset  =  0;            matchResult1 = 0;            matchResult2 = 0;
             addr = (char*)(m.getAddress()).c_str();
            myosc.getNextMessage(&m);
            */
        }
        
        pattern="/sync/*/end_event";
        matchResult1 = osc_match(pattern.c_str(), addr, &pattrOffset, &addrOffset);
        //cout << "matchresult aa"<< matchResult1 << endl;
        if(matchResult1==3){
            if(m.getAddress()=="/sync/electromagnetism/end_event")
                ((testApp*)ofGetAppPtr())->endScene(EM);
            if(m.getAddress()=="/sync/gravity/end_event")
                ((testApp*)ofGetAppPtr())->endScene(GRAVEDAD);
            if(m.getAddress()=="/sync/weak_nuclear/end_event")
                ((testApp*)ofGetAppPtr())->endScene(NUCLEAR_DEBIL);
            if(m.getAddress()=="/sync/strong_nuclear/end_event")
                ((testApp*)ofGetAppPtr())->endScene(NUCLEAR_FUERTE);
            if(m.getAddress()=="/sync/menu/end_event")
                ((testApp*)ofGetAppPtr())->endScene(MENU);
            //return;
        }
        
        /** si no dentro de cada escena recibe lo suyo **/
        
         pattern="/sync/electromagnetism";
        matchResult1 = osc_match(pattern.c_str(), addr, &pattrOffset, &addrOffset);
        if(matchResult1==2) {
            ((testApp*)ofGetAppPtr())->parseEmOsc(m.getAddress().substr(addrOffset),m );
            //return;
        }
        pattern="/sync/weak_nuclear";
        matchResult1 = osc_match(pattern.c_str(), addr, &pattrOffset, &addrOffset);
        if(matchResult1==2){ ((testApp*)ofGetAppPtr())->parseWeakOsc(m.getAddress().substr(addrOffset),m);
            //return;
        }
        
        pattern="/sync/strong_nuclear";
        matchResult1 = osc_match(pattern.c_str(), addr, &pattrOffset, &addrOffset);
        if(matchResult1==2){
            ((testApp*)ofGetAppPtr())->parseStrongOsc(m.getAddress().substr(addrOffset),m);
            //return;
        }
        
        pattern="/sync/gravity";
        matchResult1 = osc_match(pattern.c_str(), addr, &pattrOffset, &addrOffset);
        if(matchResult1==2){
            ((testApp*)ofGetAppPtr())->parseGravityOsc(m.getAddress().substr(addrOffset),m);
            //return;
        }
        
        pattern="/sync/menu";
        matchResult1 = osc_match(pattern.c_str(), addr, &pattrOffset, &addrOffset);
            if(matchResult1==2){
                ((testApp*)ofGetAppPtr())->parseMenuOsc(m.getAddress().substr(addrOffset),m);
                //return;
            }
     /*   cout << "matchresult"<< matchResult1 << endl;
        cout << "addrOffset"<< addrOffset << "     ";
        cout << "pattrOffset"<< m.getAddress().substr(addrOffset) << endl;
*/
    }
}