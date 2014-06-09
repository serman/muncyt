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
    tuioclient.start();
    ofAddListener(tuioclient.cursorAdded,this,&testApp::tuioAdded);
	ofAddListener(tuioclient.cursorRemoved,this,&testApp::tuioRemoved);
	ofAddListener(tuioclient.cursorUpdated,this,&testApp::tuioUpdated);
    
    moveandrecord.setup();
    
}

//--------------------------------------------------------------
void testApp::update(){
    mPlayer.update();
    tuioclient.getMessage();
    
    if(moveandrecord.detectBlobinSquare(tuioclient.getTuioObjects())){
        //blobImgOF_min.cropFrom(maskedImageOF, moveandrecord.triggerRectangle.x, moveandrecord.triggerRectangle.y, moveandrecord.triggerRectangle.width, moveandrecord.triggerRectangle.height);
        // ESTE ES EL BUENO mRecorder.start(3000, 25, &blobImgOF_min,moveandrecord.triggerRectangle.x,moveandrecord.triggerRectangle.y,moveandrecord.triggerRectangle.width,moveandrecord.triggerRectangle.height);
    }
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackground(0);
    //fbo.begin();
    mSyphonClient.draw(0, 0,640,480);
    //fbo.end();
    //fbo.readToPixels(cameraPixels);
    //cameraImg.setFromPixels(cameraPixels);
    //cv::Mat imgMat = ofxCv::toCv(cameraPixels);
    mPlayer.drawAll(0,0);
    ofPushMatrix();
    ofScale( 0.5, 1/1.5 );
    tuioclient.drawCursors();
    ofPopMatrix();
    
    moveandrecord.draw();
	
}

void testApp::tuioAdded(ofxTuioCursor &tuioCursor){
	ofPoint loc = ofPoint(tuioCursor.getX()*ofGetWidth(),tuioCursor.getY()*ofGetHeight());
	//cout << "Point n" << tuioCursor.getSessionId() << " add at " << loc << endl;
}

void testApp::tuioUpdated(ofxTuioCursor &tuioCursor){
	ofPoint loc = ofPoint(tuioCursor.getX()*ofGetWidth(),tuioCursor.getY()*ofGetHeight());
	//cout << "Point n" << tuioCursor.getSessionId() << " updated at " << loc << endl;
}

void testApp::tuioRemoved(ofxTuioCursor &tuioCursor){
	ofPoint loc = ofPoint(tuioCursor.getX()*ofGetWidth(),tuioCursor.getY()*ofGetHeight());
	//cout << "Point n" << tuioCursor.getSessionId() << " remove at " << loc << endl;
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
