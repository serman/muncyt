#include "ofMain.h"
#include "testApp.h"
#include "ofAppGlutWindow.h"
#include "ofAppGLFWWindow.h"
int main() {

    
    
//    ofAppGlutWindow window;
    ofPtr<ofAppGLFWWindow> win = ofPtr<ofAppGLFWWindow>(new ofAppGLFWWindow());
    win->setStencilBits(8);
//	ofSetupOpenGL(&window, 700, 700, OF_WINDOW);
    if(ofToString(getenv("USER"))=="escaner")
        ofSetupOpenGL(win, 1024,768, OF_FULLSCREEN);			// <-------- setup the GL context
    else
        ofSetupOpenGL(win, 1024,768, OF_WINDOW);
//	ofSetupOpenGL(win, 1024,768, OF_FULLSCREEN); //OF_FULLSCREEN);
  //   ofSetWindowPosition(1440,0);
	ofRunApp(new testApp());
	
}
