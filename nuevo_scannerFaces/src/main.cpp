#include "scanner_faces.h"
#include "ofAppGlutWindow.h"

int main() {
	ofAppGlutWindow window;
	ofSetupOpenGL(&window, 1280, 720, OF_FULLSCREEN);
	ofRunApp(new scanner_faces());
}
