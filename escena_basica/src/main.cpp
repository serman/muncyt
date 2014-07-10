#include "ofMain.h"
#include "testApp.h"
#include "ofAppGlutWindow.h"

int main() {
	
    ofAppGlutWindow window;
//	ofSetupOpenGL(&window, 700, 700, OF_WINDOW);
	ofSetupOpenGL(&window, 1024,768, OF_FULLSCREEN);
     ofSetWindowPosition(1440,0);
	ofRunApp(new testApp());
	
}
