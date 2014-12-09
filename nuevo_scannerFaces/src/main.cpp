#include "scanner_faces.h"
#include "ofAppGlutWindow.h"

int main() {
	ofAppGlutWindow window;
	ofSetupOpenGL(&window, 1280, 800, OF_WINDOW);//OF_FULLSCREEN);
	
	if(ofToString(getenv("USER"))=="instalaciones") {
		ofSetWindowPosition(1280,0);
	}
	
	ofRunApp(new scanner_faces());
}
