//
//  videoMask.h
//  kinectExample
//
//  Created by Sergio Galan on 10/12/14.
//
//

#ifndef kinectExample_videoMask_h
#define kinectExample_videoMask_h
#include "consts.h"
#include "ofQTKitPlayer.h"

class videoMask{
private:
    ofQTKitPlayer galaxyMovie;
    ofShader maskShader;
    ofFbo fboMascara;
    
public:
    void setup(){
        galaxyMovie.setPixelFormat(OF_PIXELS_RGBA);
        ofQTKitDecodeMode decodeMode = OF_QTKIT_DECODE_TEXTURE_ONLY;
        galaxyMovie.loadMovie("movies/Galaxy.mp4", decodeMode);
        fboMascara.allocate(ofGetWidth()/2, ofGetHeight()/2);
        maskShader.load("","shaders/maskVideo.frag");
    }
    

    void update(){ // sólo llamo a esta funcion al entrar en el modo agujero negro
        if(galaxyMovie.isPaused()) galaxyMovie.play() ;
            galaxyMovie.update();
      //  galaxyMovie.setPaused(true);
    }
    
    void draw(ofPolyline &mask){
        ofPath path;
        
        for( int i = 0; i < mask.getVertices().size(); i++) {
            if(i == 0) {
                path.newSubPath();
                path.moveTo(mask.getVertices()[i] );
            } else {
                path.lineTo( mask.getVertices()[i] );
            }
        }
        
        path.close();  
        path.simplify();
        ofColor c1= ofColor(255,0,0);
        path.setFillColor(c1);
        
        fboMascara.begin();
        ofFill();
        ofSetColor(0,0,0,255);
        ofRect(0,0,ofGetWidth()/2, ofGetHeight()/2);
        ofSetColor(255,0,0,255);
        path.draw();
        fboMascara.end();
        
        maskShader.begin();
        //ofTexture t=*galaxyMovie.getTexture();
        //maskShader.setUniformTexture( "texture2", t , 1 );
        maskShader.setUniformTexture( "texture1", fboMascara.getTextureReference(),  1 );
            galaxyMovie.draw(0,0,ofGetWidth()/2, ofGetHeight()/2);
        
        maskShader.end();
        
    }
    
};

#endif
