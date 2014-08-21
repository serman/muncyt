#include "testApp.h"
#include "consts.h"

void testApp::setup(){

	ofSetFrameRate(60);
	ofEnableAlphaBlending();
	ofSetVerticalSync(true);
	ofBackground(32, 32, 32);
	ofEnableSmoothing();
    
    tuioClient.start(3333);
    ofAddListener(tuioClient.cursorAdded,this,&testApp::tuioAdded);
	ofAddListener(tuioClient.cursorRemoved,this,&testApp::tuioRemoved);
	ofAddListener(tuioClient.cursorUpdated,this,&testApp::tuioUpdated);
	///////////////////////////////////////
	
	sceneManager = ofxSceneManager::instance();
	sceneManager->addScene( new menu(), SCENE_0);
	sceneManager->addScene( new electromagnetica(), SCENE_2);
	sceneManager->addScene( new nuclear_debil(), SCENE_1);

	sceneManager->addScene( new nuclear_fuerte(), SCENE_3);
    sceneManager->addScene( new gravedad(), SCENE_4);
	
	sceneManager->setDrawDebug(true);
	sceneManager->setCurtainDropTime(1.0);
	sceneManager->setCurtainStayTime(0.0);
	sceneManager->setCurtainRiseTime(1.0);
	sceneManager->setOverlapUpdate(true);
    //sceneManager->goToScene(SCENE_2);

}

void testApp::update(){
	tuioClient.getMessage();
	float dt = 0.016666666;
	sceneManager->update( dt );

	
	//
	// sceneManager->goToScene(SCENE_1, true); /* true >> regardless of curtain state (so u can change state while curtain is moving)*/	
	//
	
}


void testApp::draw(){

	ofPushStyle();
	sceneManager->draw();
	ofPopStyle();
	
//	ofPushStyle();
//	ofSetColor(255,0,0);
//	ofDrawBitmapString( "press 0, 1, 2, or 3 to change scene.", ofGetWidth() - 290, ofGetHeight() - 10);
//	ofPopStyle();
//
//	ofPushStyle();
//    borde.clear();
//    borde.setFillColor(ofColor::black);
//    
//    borde.setPolyWindingMode(OF_POLY_WINDING_ODD);
//    borde.setMode(ofPath::POLYLINES);
//        borde.setCircleResolution(100);
//    ofPushMatrix(); //colocamos el canvas en su posicion centrada
//    ofTranslate((ofGetWidth()-W_WIDTH)/2, 0);
//    borde.rectangle(-100,0,W_WIDTH+200, W_HEIGHT+100);
//    borde.circle(W_WIDTH/2, W_HEIGHT/2, W_HEIGHT/2);
//
//    borde.draw();
//    ofPopMatrix();
//	ofPopStyle();
}


//if you require mouseMoved events in your scenes, forward them to the SceneManager. Same for any other events
void testApp::mousePressed( int x, int y, int button ){
	sceneManager->mousePressed(x, y, button);
}

void testApp::keyPressed(int key){	
	if (key == '0') sceneManager->goToScene(SCENE_0);
	if (key == '1') sceneManager->goToScene(SCENE_1, true); /* true >> regardless of curtain state (so u can change state while curtain is moving)*/
	if (key == '2') sceneManager->goToScene(SCENE_2);
	//if (key == '3') sceneManager->goToScene(SCENE_3);
	//if (key == '4') sceneManager->goToScene(SCENE_4);
    sceneManager->keyPressed(key);
}


void testApp::windowResized(int w, int h){
	sceneManager->windowResized(w,h); // in case your screens need to know, will forward to all of them
}


void testApp::tuioAdded(ofxTuioCursor &tuioCursor){
    cout << "tuio added";
    sceneManager->getCurrentScene()->tuioAdded(tuioCursor);
}

void testApp::tuioUpdated(ofxTuioCursor &tuioCursor){
    sceneManager->getCurrentScene()->tuioUpdated(tuioCursor);
}

void testApp::tuioRemoved(ofxTuioCursor &tuioCursor){
    sceneManager->getCurrentScene()->tuioRemoved(tuioCursor);
    
}



