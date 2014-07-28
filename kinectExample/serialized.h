//
//  serialized.h
//  kinectExample
//
//  Created by Sergio Galan on 7/24/14.
//
//

#ifndef kinectExample_serialized_h
#define kinectExample_serialized_h


#include <msgpack.hpp>
#include <ofMain.h>
class MsgPackMyClass
{
public:
    ofPolyline polyline;
    MsgPackMyClass(){}
    
    MsgPackMyClass( ofPolyline mpolyline )
    {
        polyline = mpolyline;
    }
    
    MSGPACK_DEFINE(polyline);
};


#endif



