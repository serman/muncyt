#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofSetLogLevel(OF_LOG_VERBOSE);
	
    cam.initGrabber(640, 480);
    recording = false;
	
    recorder.setPrefix(ofToDataPath("recording1/frame_")); // this directory must already exist
    recorder.setFormat("jpg"); // png is really slow but high res, bmp is fast but big, jpg is just right
	
	periodoSaveFrame = 0.5;	// Graba cada 500 ms
	initZonasSave();
	
	activaThreads();
	
}

void	testApp::initZonasSave() {
	
	
	int nZonas = 5;
	float aspectRatio = (float)cam.width/cam.height;	// ancho/alto
	ofLogNotice("ratio: " + ofToString(aspectRatio));
	for(int  i=0; i<nZonas; i++) {
		float ww = ofRandom(0.2,0.5)*cam.width;
		float hh = ww/aspectRatio;
		float xx = ofRandom(0,cam.width-ww);
		float yy = ofRandom(0,cam.height-hh);
		
		zonasSave.push_back(ofRectangle(xx,yy,ww,hh));

		ofLogNotice(ofToString(zonasSave[i].x)+","+
					ofToString(zonasSave[i].y)+","+
					ofToString(zonasSave[i].width)+","+
					ofToString(zonasSave[i].height));
		
		ofPtr<ofxImageSequenceRecorder> rec(new ofxImageSequenceRecorder());
		rec->setPrefix(ofToDataPath("recordings/item_"+ofToString(i,2,'0')+"_frame_"));
		rec->setFormat("jpg");
		recordersPtr.push_back(rec);		
	}
	
	
	
}

//--------------------------------------------------------------
void testApp::update(){
//    cam.grabFrame();	// OF07x
	cam.update();		// OF08
	
	if (cam.isFrameNew() && recording && (tLastSave < ofGetElapsedTimef()-periodoSaveFrame) ){
		tLastSave = ofGetElapsedTimef();
        recorder.addFrame(cam);
		ofImage imgTmp;
		imgTmp.setFromPixels(cam.getPixels(), cam.getWidth(), cam.getHeight(), OF_IMAGE_COLOR);
		for(int i=0; i<recordersPtr.size(); i++) {
			ofImage imgTmpCrop;
			imgTmpCrop.cropFrom(imgTmp,zonasSave[i].x, zonasSave[i].y, zonasSave[i].width, zonasSave[i].height);
			recordersPtr.at(i)->addFrame(imgTmpCrop);
		}
    }
	
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackground(125);
    cam.draw(0,0);

    
	ofNoFill();
	for(int i=0; i<zonasSave.size(); i++) {
		ofSetColor(ofColor::fromHsb(360/zonasSave.size()*i,255,255));
//		ofSetColor(0,255,0);
		ofRect(zonasSave[i]);
		
	}
	
	
    ofSetColor(255);
    
    stringstream c;
    c << "Recording: " << recording << "\nThread running: " << recorder.isThreadRunning();
	c <<  "\nQueue Size: " << recorder.q.size() << "\n\nPress 'r' to toggle recording.\nPress 't' to toggle worker thread." << endl;
	c << "Frecuencia save en ms: " + ofToString(periodoSaveFrame) << endl;
	c << "fr: " + ofToString(ofGetFrameRate());
    
    ofDrawBitmapString(c.str(), 650, 10);
}

void testApp::exit(){
    recorder.waitForThread();
	for(int i=0; i<recordersPtr.size(); i++) {
		recordersPtr.at(i)->waitForThread();
	}
	
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    if(key == 'r'){
        recording = !recording;
		tLastSave = ofGetElapsedTimef();
    }
    
    if(key == 't'){
		toggleThreads();
    }
}

void testApp::toggleThreads() {
	if(recorder.isThreadRunning()){
		desactivaThreads();
	} else {
		activaThreads();
	}
}

void testApp::activaThreads() {
	recorder.startThread(false, true);   
	for(int i=0; i<recordersPtr.size(); i++) {
		recordersPtr.at(i)->startThread(false,true);
	}		
}

void testApp::desactivaThreads() {
	recorder.stopThread();
	for(int i=0; i<recordersPtr.size(); i++) {
		recordersPtr.at(i)->stopThread();
	}
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){

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