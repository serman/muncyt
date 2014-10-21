#include "testApp.h"
#include "ofAppGlutWindow.h"
#define OFX_UI_NO_XML
int main() {
    ofAppGlutWindow window;
  //  ofSetCurrentRenderer(ofGLProgrammableRenderer::TYPE);
    ofSetupOpenGL(&window,1920, 1080, OF_FULLSCREEN);
  //  ofSetupOpenGL(1280, 720, OF_FULLSCREEN);
	ofRunApp(new testApp());
}
