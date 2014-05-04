#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    bg.loadImage("bg.png");
    ofSetBackgroundAuto(false);
    #ifdef _USE_LIVE_VIDEO
        //cam.setVerbose(true);
        cam.initGrabber(320, 240);
    #else
        vidPlayer.loadMovie("fingers.mov");
        vidPlayer.play();
    #endif
    consoleFont.loadFont("Menlo.ttc",17);
    
    ofAddListener(blobTracker.blobAdded, this, &testApp::blobAdded);
    ofAddListener(blobTracker.blobDeleted, this, &testApp::blobDeleted);
    
    grayImage.allocate(320,240);
    
    gui1 = new ofxUICanvas(186,627,336,218);
    textString = "";
    gui1->addSpacer();
    msgArea=gui1->addTextArea("textarea", textString, OFX_UI_FONT_LARGE);
    bLearnBackground=true;
    gui2 = new ofxUICanvas(1038,583, 295,285);
    gui2->addIntSlider("blob_threshold", 0, 100, &blobThreshold);
    
    individualTextureSyphonServer.setName("CameraOutput");
    tex.allocate(320, 240, GL_RGB);
    
}

//--------------------------------------------------------------
void testApp::update(){
    #ifdef _USE_LIVE_VIDEO
        cam.update();
        isNewFrame=cam.isFrameNew();
    #else
        vidPlayer.update();
        isNewFrame = vidPlayer.isFrameNew();
    #endif
    
    if (isNewFrame){
        #ifdef _USE_LIVE_VIDEO
                colorImg.setFromPixels(cam.getPixels(), 320,240);
        #else
                colorImg.setFromPixels(vidPlayer.getPixels(), 320,240);
        #endif
        backgroundAddon.update(colorImg);
        grayImage = colorImg;
        blobTracker.update(grayImage, blobThreshold);
//        void    update( ofxCvGrayscaleImage& input, int _threshold = -1,  int minArea = 20 ,int maxArea = 40*240)/3, int nConsidered = 10, double hullPress = 20, bool bFindHoles = false, bool bUseApproximation = true);
	}    
	if(bLearnBackground){
		backgroundAddon.startLearning();
		bLearnBackground = false;
	}
    
    
}

//--------------------------------------------------------------
void testApp::draw(){
    
    if(ofGetFrameNum() ==0 || (ofGetFrameNum() % 100 == 0)){
     bg.draw(0,0,1920,1080);
    }
    cleanBackgrounds();
    colorImg.draw(194,139);
    //backgroundAddon.draw(800,139);
    backgroundAddon.backgroundCodeBookConnectedComponents.draw(800,139);
    // or, instead we can draw each blob individually,
	// this is how to get access to them:
    blobTracker.draw(1391,139);
    //consoleFont.drawString("prueba de cadena", 189, 620);
    
    tex.loadData(cam.getPixels(), 320, 240, GL_RGB);
	individualTextureSyphonServer.publishTexture(&tex);
}

void testApp::blobAdded(ofxBlob &_blob){
    ofLog(OF_LOG_NOTICE, "Blob ID " + ofToString(_blob.id) + " added" );
    textString+= "Blob ID " + ofToString(_blob.id) + " added\n";
    msgArea->setTextString(textString);
}


void testApp::blobDeleted(ofxBlob &_blob){
    ofLog(OF_LOG_NOTICE, "Blob ID " + ofToString(_blob.id) + " deleted" );
    textString+="Blob ID " + ofToString(_blob.id) + " deleted\n";
    msgArea->setTextString(textString);
}

void testApp::cleanBackgrounds(){
    ofPushStyle();
    ofFill();
    ofSetColor(0);
    ofRect(194, 139, 320, 240);
    ofRect(800, 139, 320, 240);
    ofRect(1391, 139, 320, 240);
    ofRect(634, 586, 375, 280);
    ofRect(166, 586, 375, 280);
    ofRect(1363, 586, 375, 280);
    
    ofPopStyle();
    
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
