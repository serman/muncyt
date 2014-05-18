#include "ofxVideoSaver.h"

ofxVideoSaver :: ofxVideoSaver ()
{

}

ofxVideoSaver :: ~ofxVideoSaver ()
{

}

void ofxVideoSaver :: setup ( const ofRectangle& rect, int type )
{
        imageSize.x                     = rect.x;
        imageSize.y                     = rect.y;
        imageSize.width         = rect.width;
        imageSize.height        = rect.height;
        
        imageType                       = type;
        
        switch ( imageType )
        {
                case OF_IMAGE_GRAYSCALE         : imageDepth = 1;       break;
                case OF_IMAGE_COLOR             : imageDepth = 3;       break;
                case OF_IMAGE_COLOR_ALPHA       : imageDepth = 4;       break;
        }
        
        frameIndex              = 0;
        frameSize               = imageSize.width * imageSize.height * imageDepth;
        framePage               = 0;
        framesPerPage   = OFX_VIDEO_SAVER_FRAME_BUFFER_INC_SIZE;
        
        pixels.push_back( new unsigned char[ frameSize * framesPerPage ] );
        
        setPause( true );
}

void ofxVideoSaver :: reset ()
{
        frameIndex = 0;
        
        for( int i=0; i<pixels.size(); i++ )
        {
                delete [] pixels[ i ];
        }
        pixels.clear();
}

/////////////////////////////////////////////////
//      PAUSE.
/////////////////////////////////////////////////

void ofxVideoSaver :: setPause ( bool value )
{
        paused = value;
}

void ofxVideoSaver :: togglePause ()
{
        paused = !paused;
}

bool ofxVideoSaver :: isPaused ()
{
        return paused;
}

/////////////////////////////////////////////////
//      SAVE.
/////////////////////////////////////////////////

void ofxVideoSaver :: addFrame  ( ofVideoGrabber& camera )
{
        addFrame( camera.getPixels() );
}

void ofxVideoSaver :: addFrame  ( ofImage& image )
{
        addFrame( image.getPixels() );
}

void ofxVideoSaver :: addFrame  ( const unsigned char* pixelsToCopy )
{
        if( paused )
                return;
        
        memcpy
        (
                pixels[ pixels.size() - 1 ] + frameIndex * frameSize,           // destination.
                pixelsToCopy,                                                                                           // source
                frameSize * sizeof( unsigned char )                                                     // num of bytes to copy.
        );
        
        if( ++frameIndex == framesPerPage )
        {
                cout << "ofxVideoSaver :: num of frames saved in memory = " << pixels.size() * framesPerPage << endl;
                
                frameIndex = 0;
                
                pixels.push_back( new unsigned char[ frameSize * framesPerPage ] );
        }
}

/////////////////////////////////////////////////
//      SAVE TO DISK.
/////////////////////////////////////////////////

void ofxVideoSaver :: saveToDisk ( string path )
{
        imagePath = ofToDataPath( path );
        ofImage image;
        image.allocate( imageSize.width, imageSize.height, (ofImageType)imageType );
        
        int frameCount = 0;
        
        for( int i=0; i<pixels.size(); i++ )
        {
                int t;
                t = ( i == pixels.size() - 1 ) ? frameIndex : framesPerPage;
                
                for( int j=0; j<t; j++ )
                {
                        unsigned char* pixelsCopy;
                        pixelsCopy = new unsigned char[ frameSize ];
                        
                        memcpy
                        (
                                pixelsCopy,                                                             // destination.
                                pixels[ i ] + j * frameSize,                    // source.
                                frameSize * sizeof( unsigned char )             // num of bytes to copy.
                        );

                        image.setFromPixels( pixelsCopy, imageSize.width, imageSize.height, (ofImageType)imageType, true);
                        
                        string imagePathNameStr = imagePath;
                        char imagePathName[ 255 ];
                        sprintf( imagePathName, "frame_%02d%02d%02d_%05d.png", ofGetYear() % 1000, ofGetMonth(), ofGetDay(), frameCount++ );
                        imagePathNameStr.insert( imagePathNameStr.size(), imagePathName );
                        
                        image.saveImage( imagePathNameStr );
                        
                        delete [] pixelsCopy;
                }
        }
        
        image.clear();
}