#include "testApp.h"
#define OFX_UI_NO_XML
int main() {
    ofSetupOpenGL(1920, 1080, OF_FULLSCREEN);
	ofRunApp(new testApp());
}
