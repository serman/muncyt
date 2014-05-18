#ifndef OFX_VIDEO_SAVER_H
#define OFX_VIDEO_SAVER_H

#include "ofMain.h"

#define OFX_VIDEO_SAVER_FRAME_BUFFER_INC_SIZE   100

class ofxVideoSaver
{

public :
        
         ofxVideoSaver();
        ~ofxVideoSaver();
        
        void setup                      ( const ofRectangle& rect, int imageType );
        void reset                      ( );
        
        void setPause           ( bool value );
        void togglePause        ( );
        bool isPaused           ( );
        
        void addFrame           ( ofVideoGrabber& camera );
        void addFrame           ( ofImage& image );
        void addFrame           ( const unsigned char* pixels );
        
        void saveToDisk         ( string path = "" );
    
        
private :
        
        bool                    paused;
        
        ofRectangle             imageSize;
        string                  imagePath;
        int                             imageType;
        int                             imageDepth;
        
        int                             frameIndex;
        int                             frameSize;
        int                             framePage;
        int                             framesPerPage;
        
        
        vector<unsigned char*> pixels;
};

#endif