#include "testApp.h"
//#include "consts.h"


void testApp::setup(){
    
	ofSetFrameRate(60);
	//ofEnableAlphaBlending();
	ofSetVerticalSync(true);
	ofBackground(32, 32, 32);
	//ofEnableSmoothing();
    
    tuioClient.start(3333);
    ofAddListener(tuioClient.cursorAdded,this,   &testApp::tuioAdded);
	ofAddListener(tuioClient.cursorRemoved,this, &testApp::tuioRemoved);
	ofAddListener(tuioClient.cursorUpdated,this, &testApp::tuioUpdated);
	///////////////////////////////////////

	
	sceneManager = ofxSceneManager::instance();

    mSyphonClient.setup();
    mSyphonClient.set("CameraOutput","");
   
    mSyphonClient2.setup();
    mSyphonClient2.set("onlyBlobs","");
    

    
    
//	sceneManager->addScene( new electromagnetica(), SCENE_2);
    fantasmas *mfantasmas=new fantasmas();
     mfantasmas->setTuioClient(&tuioClient);
    mfantasmas->setSyphonClients(&mSyphonClient, &mSyphonClient2);
	
    glitch *mglitch=new glitch();
    mglitch->setTuioClient(&tuioClient);
    mglitch->setSyphonClients(&mSyphonClient, &mSyphonClient2);
/*
    
   faces *mfaces=new faces();
    mfaces->setTuioClient(&tuioClient);
    mfaces->setSyphonClients(&mSyphonClient, &mSyphonClient2);
  */
    juego1 *mjuego=new juego1();
    mjuego->setTuioClient(&tuioClient);
    mjuego->setSyphonClients(&mSyphonClient, &mSyphonClient2);
    
    juego2 *mjuego2=new juego2();
    mjuego2->setTuioClient(&tuioClient);
    mjuego2->setSyphonClients(&mSyphonClient, &mSyphonClient2);
  
    sceneManager->addScene( mjuego2, SCENE_3);
	sceneManager->addScene( mglitch, SCENE_1);
    sceneManager->addScene( mjuego, SCENE_2);
    sceneManager->addScene( mfantasmas, SCENE_0);
//	sceneManager->addScene( mfaces, SCENE_3);


    
    //sceneManager->goToScene(SCENE_4);
	sceneManager->setDrawDebug(true);
	sceneManager->setCurtainDropTime(0.2);
	sceneManager->setCurtainStayTime(0.0);
	sceneManager->setCurtainRiseTime(0.2);
	sceneManager->setOverlapUpdate(true);

    
}

void testApp::update(){
	tuioClient.getMessage();
	sceneManager->update(  0.016666666 );
}


void testApp::draw(){
    
	sceneManager->draw();
	ofPushStyle();
	ofSetColor(255,0,0);
	ofDrawBitmapString( "press 1, 2, 3 or 4 to change scene", ofGetWidth() - 290, ofGetHeight() - 10);
	ofPopStyle();

}


//if you require mouseMoved events in your scenes, forward them to the SceneManager. Same for any other events
void testApp::mousePressed( int x, int y, int button ){
	sceneManager->mousePressed(x, y, button);
}

void testApp::keyPressed(int key){
    if (key == '0') sceneManager->goToScene(SCENE_0);
	if (key == '1') sceneManager->goToScene(SCENE_1); /* true >> regardless of curtain state (so u can change state while curtain is moving)*/
	if (key == '2') sceneManager->goToScene(SCENE_2);
	if (key == '3') sceneManager->goToScene(SCENE_3);
//	if (key == '4') sceneManager->goToScene(SCENE_4);
    sceneManager->keyPressed(key);
}
void testApp::keyReleased(int key){
    sceneManager->keyReleased(key);
}

void testApp::windowResized(int w, int h){
	sceneManager->windowResized(w,h); // in case your screens need to know, will forward to all of them
}

void testApp::exit(){

}


void testApp::tuioAdded(ofxTuioCursor &tuioCursor){
  //  cout << "tuio added";
    sceneManager->getCurrentScene()->tuioAdded(tuioCursor);
}

void testApp::tuioUpdated(ofxTuioCursor &tuioCursor){
    sceneManager->getCurrentScene()->tuioUpdated(tuioCursor);
}

void testApp::tuioRemoved(ofxTuioCursor &tuioCursor){
    sceneManager->getCurrentScene()->tuioRemoved(tuioCursor);
    
}



