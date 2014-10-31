#include "ofMain.h"
#include "testApp.h"

//========================================================================
int main( ){
    if(ofToString(getenv("USER"))=="instalaciones")
        ofSetupOpenGL(1280,720,OF_FULLSCREEN);			// <-------- setup the GL context
    else
        ofSetupOpenGL(1280,720,OF_WINDOW);

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
    if(ofToString(getenv("USER"))=="instalaciones"){
        ofSetWindowPosition(1280, 0);
    }
	ofRunApp(new testApp());

}
