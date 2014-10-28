#include "testApp.h"
//#include "consts.h"

ofTrueTypeFont courierFont;
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
    
    j2key.setup();
    
    
//	sceneManager->addScene( new electromagnetica(), SCENE_2);
    fantasmas *mfantasmas=new fantasmas();
     mfantasmas->setTuioClient(&tuioClient);
    mfantasmas->setSyphonClients(&mSyphonClient, &mSyphonClient2);
	
    glitch *mglitch=new glitch();
    mglitch->setTuioClient(&tuioClient);
    mglitch->setSyphonClients(&mSyphonClient, &mSyphonClient2);

    
   faces *mfaces=new faces();
    mfaces->setTuioClient(&tuioClient);
    mfaces->setSyphonClients(&mSyphonClient, &mSyphonClient2);
 
    juego1 *mjuego=new juego1();
    mjuego->setTuioClient(&tuioClient);
    mjuego->setSyphonClients(&mSyphonClient, &mSyphonClient2);
    
    juego2 *mjuego2=new juego2();
    mjuego2->setTuioClient(&tuioClient);
    mjuego2->setSyphonClients(&mSyphonClient, &mSyphonClient2);
  
    generativo1 *mgenerativo1=new generativo1();
    mgenerativo1->setTuioClient(&tuioClient);
    mgenerativo1->setSyphonClients(&mSyphonClient, &mSyphonClient2);
    
    generativo2 *mgenerativo2=new generativo2();
    mgenerativo2->setTuioClient(&tuioClient);
    mgenerativo2->setSyphonClients(&mSyphonClient, &mSyphonClient2);
    
    sceneManager->addScene( mjuego, BOLA);
    sceneManager->addScene( mjuego2, COMEPANTALLA);
	sceneManager->addScene( mfaces, CARAS3D);
	//sceneManager->addScene( mglitch, SCENE_1);
    sceneManager->addScene( mfantasmas, FANTASMAS);
	sceneManager->addScene( mgenerativo1, SCENE_4);
    sceneManager->addScene( mgenerativo2, SCENE_5);

    
    sceneManager->goToScene(CARAS3D);
	sceneManager->setDrawDebug(false);
	sceneManager->setCurtainDropTime(0.2);
	sceneManager->setCurtainStayTime(0.0);
	sceneManager->setCurtainRiseTime(0.2);
	sceneManager->setOverlapUpdate(true);

    courierFont.loadFont("CourierNew.ttf", 11);
    
}

void testApp::update(){
    j2key.update();
	tuioClient.getMessage();
	sceneManager->update(  0.016666666 );
}


void testApp::draw(){
    ofPushStyle();
	sceneManager->draw();
    	ofPopStyle();
	ofPushStyle();
    ofNoFill();
	ofSetColor(200,200,200);
    ofRect(-1,-1,1282,722);
	ofPopStyle();
}


//if you require mouseMoved events in your scenes, forward them to the SceneManager. Same for any other events
void testApp::mousePressed( int x, int y, int button ){
	sceneManager->mousePressed(x, y, button);
}

void testApp::keyPressed(int key){
    if (key == '0') sceneManager->goToScene(FANTASMAS);
	if (key == '1') sceneManager->goToScene(BOLA); /* true >> regardless of curtain state (so u can change state while curtain is moving)*/
	if (key == '2') sceneManager->goToScene(COMEPANTALLA);
	if (key == '3') sceneManager->goToScene(SCENE_3);
	if (key == '4') sceneManager->goToScene(SCENE_4);
    if (key == '5') sceneManager->goToScene(SCENE_5);
        if (key == '6') sceneManager->goToScene(CARAS3D);
    sceneManager->getCurrentScene()->keyPressed(key);
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



