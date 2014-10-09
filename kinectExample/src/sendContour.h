//
//  sendContour.h
//  kinectExample
//
//  Created by Sergio Galan on 7/24/14.
//
//
#include "ofxNetwork.h"
#include "msgpack.hpp"
#include "consts.h"

#ifndef kinectExample_sendContour_h
#define kinectExample_sendContour_h

typedef struct some_struct {
    int x;
    int y;
} punto;


class sendContour{
public:
    ofxUDPManager udpConnection;
    vector<int> ptos;
    msgpack::sbuffer sbuf;
    void serializeContour(ofPolyline &mline){
        ptos.clear();
        for(int i=0; i<mline.size(); i++){
            ptos.push_back(mline[i].x);
            ptos.push_back(mline[i].y);
        }
        
        sbuf.clear();
        msgpack::pack(sbuf, ptos);
        
    }
    
    void send(ofPolyline &mline){
//        string message="";
     //   for(unsigned int i=0; i<stroke.size(); i++){
     //       message+=ofToString(stroke[i].x)+"|"+ofToString(stroke[i].y)+"[/p]";
      //  }
        if(mline.size()>0){
            serializeContour(mline);
            udpConnection.Send(sbuf.data(),sbuf.size());
        }
     //   udpConnection.S
    }
    
    
    void setup(){
        cout << "screen id que se ve en sendcontour" << SCREEN_ID;
            string ip_espejo_1="127.0.0.1";
            string ip_espejo_2="192.168.98.22";
            string ip_espejo_3="192.168.98.101";
        udpConnection.Create();
//            SCREEN1 SENDS TO SCREEN 2 2->3 3->1
        
        if(SCREEN_ID==3){

            udpConnection.Connect(ip_espejo_1.c_str() ,11999);
        }
        
        if(SCREEN_ID==1){
            udpConnection.Connect(ip_espejo_1.c_str(),11999);
            cout <<"ENTRANDO SCREEN ID 1" <<endl;
        }

            udpConnection.SetNonBlocking(true);
    }
    
    
    
    
};


#endif
