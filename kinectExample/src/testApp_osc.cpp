//
//  testApp_osc.cpp
//  kinectExample
//
//  Created by Sergio Galan on 9/29/14.
//
//

#include <testApp.h>
#include "osc_match.h"
#include "consts.h"
void testApp::parseStrongOsc(string s,ofxOscMessage &m){
    
    if(s=="/setColor"){
        int target= round(m.getArgAsFloat(0));
        int color= round(m.getArgAsFloat(1));
        cout << "parse strong nuclear: " << s << " "<<target <<" "<< color <<" " << SCREEN_ID << endl;
        if(SCREEN_ID==target){
            mPuntosFormas.setColor(color);
        }
    }
    
}

void testApp::parseWeakOsc(string s,  ofxOscMessage &m){
    if(s=="/ball"){
        float angle= m.getArgAsFloat(0);
        float speed= m.getArgAsFloat(1);
        mrayoSil.triggerParticles(angle);
        
    }
    else if(s=="/colisiones"){
        mcontour.bFill=true;
    }
    else if(s=="/nocolisiones"){
        mcontour.bFill=false;
    }
    else if(s=="/chain_reaction_event"){
        
    }
    else if(s=="/acceleration_event"){
        
    }
    
    
}

void testApp::parseEmOsc(string s,   ofxOscMessage &m){
    if(s=="/number_waves"){
        if(m.getArgAsFloat(0)>0){
           
            particleCloud.setMode(particleClouds::ESPEJO);
        }
        else{
             particleCloud.setMode(particleClouds::RUIDO);
        }
    }
    else if(s=="/wave_length"){
        int id= m.getArgAsFloat(0);
        int length= m.getArgAsFloat(1);
    }
}


void testApp::parseGravityOsc(string s,   ofxOscMessage &m){
    //cout << "parse gravity: " << s << endl;
    if(s=="/collapse_proximity"){
      
        float collapseProximity=m.getArgAsFloat(0);
        mgrid.elongation( ofMap(collapseProximity, 0, 1, 1, 0.6 ) );
        cout << "collapse proximity " << collapseProximity <<endl;
    }
    else if(s=="/sun_collision_event"){
        mgrid.setVibration();
    }
    
    else if(s=="/collapse_start_event"){
        mgrid.initBlackHole();
    }
    
    else if(s=="/collapse_end_event"){
        mgrid.endBlackHole();
    }

    

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

//AQUI HAY QUE COLOCAR PARA CADA CAMBIO DE ESCENA LAS FUNCIONES A LAS QUE HAYA QUE LLAMAR PARA RESETEAR LA ENTRADA EN LA ESCENA ANTES DE EMPEZARLA O PONERLA EN MODO "ENTRADA"

void testApp::toScene(int scn){
//    Antes de Entrar en la escena se reinicia la camara
    cameraMoves::getInstance()->reset();
    appStatuses["escena"]=scn;
    cout << "toScene:  " << scn <<endl;
    if(appStatuses["escena"]==EM) {
        particleCloud.setMode(particleClouds::RUIDO);
        particleCloud.resetParticles();    
    }
    else if(appStatuses["escena"]==NUCLEAR_DEBIL) {
        mrayoSil.reset();
        mcontour.reset();
        mcontour.modoFill=0;
        mcontour.bFill=false;
    }
    
    else if(appStatuses["escena"]==NUCLEAR_FUERTE) {
        mcontour.reset();
    }
    
    else if(appStatuses["escena"]==GRAVEDAD){
        mgrid.fadeIn();
    }
    
    else if(appStatuses["escena"]==MENU) {
        mmenu.reset();
    }
}

//SE EJECUTA CUANDO COMIENZA LA TRANSICION PARA SALIR DE LA ESCENA
void testApp::endScene(int scn){
    if(appStatuses["escena"]==EM) {
     //   particleCloud.setMode(particleClouds::DESAPARECE);
        particleCloud.setMode(particleClouds::DESAPARECE);
        cout << "particle cloud desaparece";
    }
}
