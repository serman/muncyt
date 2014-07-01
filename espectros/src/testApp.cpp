#include "testApp.h"

//--------------------------------------------------------------

using namespace ofxCv;
using namespace cv;

void testApp::setup(){
    ofSetFrameRate(30);
        setupStatus();
    myOSCcomm.setup();
    //Syphon client
    mSyphonClient.setup();
    
    mSyphonClient.set("CameraOutput","");
    
    mSyphonClient2.setup();
    mSyphonClient2.set("onlyBlobs","");

    fbo.allocate(640, 480, GL_RGB);
    fboGeneral.allocate(640, 480, GL_RGB);
    remoteBlobImgPxl.allocate(640,480,OF_PIXELS_RGB);
   // cameraImg.allocate(640, 480,OF_IMAGE_COLOR );
    //mPlayer.setup();
    tuioclient.start();

    moveandrecord.setup();
        //feedImg.allocate(640,480,OF_IMAGE_COLOR_ALPHA);
    testImg.loadImage("test.jpg");
    mRecorder.setup();
    for(int i=0; i<20;i++){
        sequences.push_back(ofxImageSequence());
    }
    setupSequences();

}

void testApp::setupSequences(){
    
    int ii=0;
    for (std::vector<ofxImageSequence>::iterator it = sequences.begin() ;
         it != sequences.end(); ++it){

    	//it->loadSequence(ofToDataPath("recordings/video_"+ofToString(ii)+"/frame" , "png", 0, 75, 4);
        it->loadSequence(ofToDataPath("recordings/video_"+ofToString(ii)));
        it->setFrameRate(25);
        it->preloadAllFrames();
        ii++;
        break;
    }
    
}

void testApp::setupStatus(){
    appStatuses["syphonEnabled"]=true;
    appStatuses["mode"]=MOSAIC;
    appStatuses["debug"]=true;
    
}

//--------------------------------------------------------------
void testApp::update(){
  //  mPlayer.update();
    tuioclient.getMessage();
    
    if(appStatuses["mode"]==CAPTURE){
        ofxTuioCursor* blobToRecord=moveandrecord.detectBlobinSquare(tuioclient.getTuioCursors());
        if(blobToRecord!=NULL && mRecorder.isRecording==false){
        //if(moveandrecord.detectMouseinSquare(mouseX, mouseY)){
            mRecorder.current_video=moveandrecord.currentRect;
            mRecorder.start(2000, 25, remoteBlobImgPxl,
                            80,100,
                           blobToRecord->getX()*640,blobToRecord->getY()*320
                            );
            RecordingBlobId=blobToRecord->getSessionId();
        }
        if( mRecorder.isRecording ){
            bool blobStillExists=false;
            list<ofxTuioCursor*>::iterator tobj;
            list<ofxTuioCursor*> objectList = tuioclient.getTuioCursors();
            for (tobj=objectList.begin(); tobj != objectList.end(); tobj++) {
                ofxTuioCursor *blob = (*tobj);
                if(blob->getSessionId() == RecordingBlobId){
                    if(mRecorder.update(blob->getX()*640, blob->getY()*480)==true) RecordingBlobId=-1;
                    blobStillExists=true;
                    break;
                }
            }
            if(blobStillExists==false){
                mRecorder.cancel();
                RecordingBlobId=-1;
                cout << "grabacion cancelada";
            }
        }
    }
//    mRecorder.update();
    
}

//--------------------------------------------------------------
void testApp::draw(){
    
    //
        ofBackground(0);
    
    
    if(appStatuses["mode"]==CAPTURE){
		ofSetColor(255);
        mSyphonClient.draw(0, 0,640,480);
        list<ofxTuioCursor*>::iterator tobj;
        list<ofxTuioCursor*> objectList = tuioclient.getTuioCursors();
        for (tobj=objectList.begin(); tobj != objectList.end(); tobj++) {
            ofxTuioCursor *blob = (*tobj);
            ofEllipse( blob->getX()*640, blob->getY()*480,9,9);
            //cout << "blob size" << blobTracker.trackedBlobs.size() << "\n";
        }
        
        fbo.begin();
        mSyphonClient.draw(0,0,640,480);
        //mSyphonClient.draw(0, 0,640,480);
        fbo.end();
        //fbo.draw(640,480,320,240);
        fbo.readToPixels(remoteBlobImgPxl);
        
        feedImg.setFromPixels(remoteBlobImgPxl);
        feedImg.update();
        feedImg.draw(640,480,320,240);
        
        //remoteBlobImgPxl=feedImg.getPixelsRef();
        //cameraImg.setFromPixels(cameraPixels);
        //cv::Mat imgMat = ofxCv::toCv(cameraPixels);
        //mPlayer.drawAll(0,0);
        
        ofPushMatrix();
        ofScale( 0.5, 1/1.5 );
        //tuioclient.drawCursors();
        ofPopMatrix();
        
        moveandrecord.draw();
    }else  if(appStatuses["mode"]==MOSAIC){
		ofPushMatrix();
        //ofEnableBlendMode(OF_BLENDMODE_ALPHA);
        mSyphonClient.draw(0,0,640,480);
        ofSetColor(255);
    	//ofEnableBlendMode(OF_BLENDMODE_ADD);
  
        float tt = ofGetElapsedTimef();
        
        int ii=0;
        for (std::vector<ofxImageSequence>::iterator it = sequences.begin() ;
                                                it != sequences.end(); ++it){
            int index= it->getCurrentFrame();


			it->getFrame(index)->draw(100,100);//draw(RECTANGLESIZE*(ii%5),RECTANGLESIZE*floor(ii/5));
            index++;
            if(index> it->getTotalFrames() ) index=0;
            it->setFrame(index);
//			it->getFrameForTime(tt+ii*10)->draw(140*((ii+9)%7),140*floor((9+ii)/7));
//			it->getFrameForTime(tt+ii*10)->draw(140*((ii+19)%7),140*floor((19+ii)/7));
            ii++;

            break;

        }
        
		//get the frame based on the current time and draw it
        // ofDisableBlendMode();
        ofPopMatrix();

    }
  //
	if(appStatuses["debug"]) showDebug();
}

void testApp::showDebug(){
    ofDrawBitmapString("Framerate " + ofToString(ofGetFrameRate()), 20, 20);
    if(appStatuses["mode"]==CAPTURE)
    ofDrawBitmapString("mode: CAPTURE" , 20, 40);
    else     ofDrawBitmapString("mode: DEBUG" , 20, 40);
    // ofDrawBitmapString("NonZero " + ofToString(nonZero), 20, 40);
}
/*
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
}*/

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    if(key=='r'){
        mRecorder.start(3000, 25, remoteBlobImgPxl,
                        moveandrecord.triggerRectangle.width,moveandrecord.triggerRectangle.height,
                        moveandrecord.triggerRectangle.x,moveandrecord.triggerRectangle.y);
    }
    else if(key=='m'){
        if(appStatuses["mode"]==CAPTURE)
            appStatuses["mode"]=MOSAIC;
        else appStatuses["mode"]=CAPTURE;
    }

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
