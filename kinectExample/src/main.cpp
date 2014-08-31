#include "testApp.h"

#define OFX_UI_NO_XML
int main() {
  //  ofSetCurrentRenderer(ofGLProgrammableRenderer::TYPE);
//    ofSetupOpenGL(1920, 1080, OF_FULLSCREEN);
    ofSetupOpenGL(1280, 720, OF_FULLSCREEN);
	ofRunApp(new testApp());
}
