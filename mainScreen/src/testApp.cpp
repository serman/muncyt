#include "testApp.h"

//--------------------------------------------------------------

using namespace ofxCv;
using namespace cv;

void testApp::setup(){
    myOSCcomm.setup();
    //Syphon client
    mSyphonClient.setup();
    
    mSyphonClient.set("CameraOutput","");

    fbo.allocate(640, 480, GL_RGB);
    cameraPixels.allocate(640,480,OF_PIXELS_RGB);
    cameraImg.allocate(640, 480,OF_IMAGE_COLOR );
    mPlayer.setup();
}

//--------------------------------------------------------------
void testApp::update(){
    mPlayer.update();
}

//--------------------------------------------------------------
void testApp::draw(){
    //fbo.begin();
    mSyphonClient.draw(0, 0,640,480);
    //fbo.end();
    //fbo.readToPixels(cameraPixels);
    //cameraImg.setFromPixels(cameraPixels);
    //cv::Mat imgMat = ofxCv::toCv(cameraPixels);
    mPlayer.drawAll(0,0);
	
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}
