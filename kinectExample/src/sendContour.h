//
//  sendContour.h
//  kinectExample
//
//  Created by Sergio Galan on 7/24/14.
//
//
#include "ofxNetwork.h"
#include "msgpack.hpp"

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
        udpConnection.Create();
        udpConnection.Connect("127.0.0.1",11999);
        udpConnection.SetNonBlocking(true);
    }
    
    
    
    
};


#endif
