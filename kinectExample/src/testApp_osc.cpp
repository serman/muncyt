//
//  testApp_osc.cpp
//  kinectExample
//
//  Created by Sergio Galan on 9/29/14.
//
//

#include <testApp.h>
#include "osc_match.h"

void testApp::parseStrongOsc(string s,ofxOscMessage &m){
    
}

void testApp::parseWeakOsc(string s,  ofxOscMessage &m){

}

void testApp::parseEmOsc(string s,   ofxOscMessage &m){

}


void testApp::parseGravityOsc(string s,   ofxOscMessage &m){
    

}

void testApp::parseMenuOsc(string s,   ofxOscMessage &m){
    cout << "parse menu osc;" << s;
    int pattrOffset = 0;
    int addrOffset  = 0;
    int matchResult1 = 0;
    char* addr = (char*)s.c_str();
    /*ir a escena */
    string pattern="/hand_on_event";
    matchResult1 = osc_match(pattern.c_str(), addr, &pattrOffset, &addrOffset);
    if(matchResult1==OSC_MATCH_ADDRESS_AND_PATTERN_COMPLETE){
             mmenu.start((int)m.getArgAsFloat(0));
        
        return;
    }
    
    pattern="/hand_off_event";
    matchResult1 = osc_match(pattern.c_str(), addr, &pattrOffset, &addrOffset);
    if(matchResult1==OSC_MATCH_ADDRESS_AND_PATTERN_COMPLETE){
        mmenu.start(MENU);
        return;
    }
    
}


void testApp::toScene(int scn){
    appStatuses["escena"]=scn;
    cout << "toScene" << scn <<endl;
    if(appStatuses["escena"]==EM) particleCloud.resetParticles();
}