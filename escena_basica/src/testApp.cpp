#include "testApp.h"
#include "consts.h"
//static cheapComm myComm;

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
	_defaultRenderer = ofGetCurrentRenderer();
    _shivaVGRenderer = ofPtr<ofxShivaVGRenderer>(new ofxShivaVGRenderer);
   // ofSetCurrentRenderer(_shivaVGRenderer);
    
    _shivaVGRenderer->setLineJoinStyle(VG_JOIN_ROUND);
    _shivaVGRenderer->setLineCapStyle(VG_CAP_ROUND);

    
    
    
	sceneManager = ofxSceneManager::instance();
    cheapComm *myComm;
    myComm=cheapComm::getInstance();
    myComm->setup();
    
    menu *m1=new menu();
    m1->setTuio(&tuioClient);
	sceneManager->addScene( m1, SCENE_MENU);
    
    electromagnetica *electrom=new electromagnetica();
   // electrom->setComm(&myComm);
	sceneManager->addScene( electrom, SCENE_EM);
    
    nuclear_debil *nuclear_d = new nuclear_debil();
  //  nuclear_d->setComm(&myComm);
	sceneManager->addScene( nuclear_d, SCENE_NUC_D);

    nuclear_fuerte *nuclear_f = new nuclear_fuerte();
  //  nuclear_d->setComm(&myComm);
	sceneManager->addScene( nuclear_f , SCENE_NUC_F);
    
    gravedad *grave=new gravedad();
 //   grave->setComm(&myComm);
    sceneManager->addScene( grave, SCENE_MENU);
	sceneManager->setDrawDebug(false);
	sceneManager->setCurtainDropTime(1);
	sceneManager->setCurtainStayTime(0.6);
	sceneManager->setCurtainRiseTime(1);
	sceneManager->setOverlapUpdate(true);
    
    sceneManager->goToScene(SCENE_NUC_D);
    timeToEndScene=-1;
	// OSC
	//ofAddListener(eventoOSC, this, &testApp::eventoOSC_Recibido  );
}




void testApp::update(){
    
	tuioClient.getMessage();
	float dt = 0.016666666;
	sceneManager->update( dt );

	//
	// sceneManager->goToScene(SCENE_1, true); /* true >> regardless of curtain state (so u can change state while curtain is moving)*/	
	//
	if(timeToEndScene !=-1 && timeToEndScene<ofGetElapsedTimeMillis()){
        ofSendMessage("changeScene" +ofToString(SCENE_MENU));
    }
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


//if you require mouseMoved events in your scenes, fforward them to the SceneManager. Same for any other events
void testApp::mousePressed( int x, int y, int button ){
    /*if (ofGetCurrentRenderer() == _defaultRenderer)
    {
        ofSetCurrentRenderer(_shivaVGRenderer);
        cout << "VG RENDER";
    }
    else
    {
        ofSetCurrentRenderer(_defaultRenderer);
                cout << "DEFAULT RENDER";
    }*/
	sceneManager->mousePressed(x, y, button);
}
void testApp::mouseReleased( int x, int y, int button ){
	sceneManager->mouseReleased(x, y, button);
}

void testApp::keyPressed(int key){	
	if (key == 'c'){ sceneManager->goToScene(SCENE_EM);     setTimeToEndScene(SCENE_EM);}
    else if (key == 'v'){ sceneManager->goToScene(SCENE_NUC_D);  setTimeToEndScene(SCENE_NUC_D);}
    else if (key == 'b'){ sceneManager->goToScene(SCENE_NUC_F);  setTimeToEndScene(SCENE_NUC_F);}
    else if (key == 'n') {sceneManager->goToScene(SCENE_GRAVEDAD);setTimeToEndScene(SCENE_GRAVEDAD);}
    else if (key == 'm') {sceneManager->goToScene(SCENE_MENU);   setTimeToEndScene(SCENE_MENU);}
	
	else{
    sceneManager->keyPressed(key);
    }
	
	
}

void testApp::gotMessage(ofMessage m){
    cout << "message event";
    cout << m.message << endl;
    
    if(m.message=="changeScene1"){
        sceneManager->goToScene(SCENE_NUC_D, true);
        setTimeToEndScene(SCENE_NUC_D);
    }
    else if(m.message=="changeScene2"){
        sceneManager->goToScene(SCENE_NUC_F, true);
        setTimeToEndScene(SCENE_NUC_F);
    }
    else if(m.message=="changeScene3"){
        sceneManager->goToScene(SCENE_GRAVEDAD, true);
        setTimeToEndScene(SCENE_GRAVEDAD);
    }
    else if(m.message=="changeScene4"){
        sceneManager->goToScene(SCENE_MENU, true);
        setTimeToEndScene(SCENE_MENU);
    }
    else if(m.message=="changeScene0"){
        sceneManager->goToScene(SCENE_EM, true);
        setTimeToEndScene(SCENE_EM);
    }
}

// ajusta tiempo máximo de una escena en pantalla
void testApp::setTimeToEndScene(int scene_num){
    if(scene_num!=SCENE_MENU){
        timeToEndScene=ofGetElapsedTimeMillis()+MAX_TIME_ESCENA;
    }
    else{
        timeToEndScene=-1;
    }
}


void testApp::windowResized(int w, int h){
	sceneManager->windowResized(w,h); // in case your screens need to know, will forward to all of them
}


void testApp::tuioAdded(ofxTuioCursor &tuioCursor){
    sceneManager->getCurrentScene()->tuioAdded(tuioCursor);
}

void testApp::tuioUpdated(ofxTuioCursor &tuioCursor){
    sceneManager->getCurrentScene()->tuioUpdated(tuioCursor);
}

void testApp::tuioRemoved(ofxTuioCursor &tuioCursor){
    sceneManager->getCurrentScene()->tuioRemoved(tuioCursor);
    
}



