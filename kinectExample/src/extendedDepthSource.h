//
//  extendedDepthSource.h
//  kinectExample
//
//  Created by Sergio Galan on 6/9/14.
//
//

#ifndef kinectExample_extendedDepthSource_h
#define kinectExample_extendedDepthSource_h

#include "DepthSource.h"




class extendedDepthSource: public DepthSource{
public:
	    Boolean isUpdated=false;

    

private:
    
    void onNewFrame(VideoStream& stream){
        isUpdated=true;
        DepthSource::onNewFrame(stream);
    }
    
};

#endif