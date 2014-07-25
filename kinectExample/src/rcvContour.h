//
//  rcvContour.h
//  kinectExample
//
//  Created by Sergio Galan on 7/25/14.
//
//

#ifndef kinectExample_rcvContour_h
#define kinectExample_rcvContour_h


#include "ofxNetwork.h"
#include "msgpack.hpp"

class rcvContour: public ofThread{
private:
    ofxUDPManager udpConnection;
    vector<int> ptos;
    msgpack::sbuffer sbuf;
    ofPolyline mline;
    int buffer_size;
    char udpMessage[10000];
    
    void rcv(){
        memset(udpMessage, 0, buffer_size);
        int msg_size;
        msg_size=udpConnection.Receive(udpMessage,buffer_size); //num de bits
        if(msg_size>2){
            //msgpack::unpacker unpac;
            //unpac.reserve_buffer(err);
            //memcpy(unpac.buffer(), udpMessage, err);
            //unpac.buffer_consumed(err);
            msgpack::unpacked result;
            msgpack::unpack(&result, udpMessage, msg_size);
            msgpack::object obj;
            obj=result.get();
            
            ptos.clear();
            obj.convert(&ptos);
            deSerializeContour();
            
            /*while(unpac.next(&result)) {
             cout << "ds: ";
             std::cout << result.get() << std::endl;
             msgpack::object obj;
             ptos.clear();
             obj.convert(&ptos);
             deSerializeContour();
             }*/
            //std::vector<int> rvec;
            //obj.convert(&rvec);
        }
    }
public:
    
    
    rcvContour(){
        buffer_size=10000;
    }
    void deSerializeContour(){
        lock();
        mline.clear();
        
        for(int i=0; i<ptos.size()-1; i+=2){
            mline.addVertex(ptos[i],ptos[i+1]);
        }
        unlock();
        
    }
    
    ofPolyline getRemoteContour(){
        lock();
        ofPolyline rtline=mline;
        unlock();
        return rtline;
    }
    
    void start(){
        startThread(true, false);   // blocking, verbose
    }
    
    void stop(){
        stopThread();
    }
    
    void finish(){
        waitForThread(true);
        udpConnection.Close();
        ofLog() << "end finish"<<endl;
    }
    //--------------------------
    void threadedFunction(){
        while( isThreadRunning() != 0 ){
            rcv();
            ofLog() << "msg rcved" <<endl;
            sleep(30);
        }
    }
    
   
    void setup(){
        udpConnection.Create();
        udpConnection.Bind(11999);
        udpConnection.SetNonBlocking(true);
        udpConnection.SetTimeoutReceive(1000);
    }
    
    
    
};


#endif
