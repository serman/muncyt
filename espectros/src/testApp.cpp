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
    
    
    cheapComm *myComm;
    myComm=cheapComm::getInstance();
    myComm->setup();

    
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
 
	MosaicoCaras *mMosaicoCaras = new MosaicoCaras();
    mMosaicoCaras->setTuioClient(&tuioClient);
    mMosaicoCaras->setSyphonClients(&mSyphonClient, &mSyphonClient2);
    
    juego1 *mjuego=new juego1();
    mjuego->setTuioClient(&tuioClient);
    mjuego->setSyphonClients(&mSyphonClient, &mSyphonClient2);
    
    juego2 *mjuego2=new juego2();
    mjuego2->setTuioClient(&tuioClient);
    mjuego2->setSyphonClients(&mSyphonClient, &mSyphonClient2);
  
    /*generativo1 *mgenerativo1=new generativo1();
    mgenerativo1->setTuioClient(&tuioClient);
    mgenerativo1->setSyphonClients(&mSyphonClient, &mSyphonClient2);*/
    
    generativo2 *mgenerativo2=new generativo2();
    mgenerativo2->setTuioClient(&tuioClient);
    mgenerativo2->setSyphonClients(&mSyphonClient, &mSyphonClient2);
    
    sceneManager->addScene( mjuego, BOLA);
    sceneManager->addScene( mjuego2, COMEPANTALLA);
	sceneManager->addScene( mfaces, CARAS3D);
	sceneManager->addScene( mMosaicoCaras, MOSAICOCARAS);
	//sceneManager->addScene( mglitch, SCENE_1);
    sceneManager->addScene( mfantasmas, FANTASMAS);
//	sceneManager->addScene( mgenerativo1, SCENE_4);
    sceneManager->addScene( mgenerativo2, JOY_WAVES);

    
//    sceneManager->goToScene(COMEPANTALLA);
    timeInitScene=ofGetElapsedTimeMillis();
    timeToEndScene=ofGetElapsedTimeMillis()+DEFAULT_MAX_TIME_SCENE;
    
	sceneManager->setDrawDebug(false);
	sceneManager->setCurtainDropTime(0.8);
	sceneManager->setCurtainStayTime(0.0);
	sceneManager->setCurtainRiseTime(0.8);
	sceneManager->setOverlapUpdate(true);

    courierFont.loadFont("CourierNew.ttf", 11);
    
    if(ofToString(getenv("USER"))=="instalaciones" || ofToString(getenv("USER"))=="panel" ){
        ofLogToFile( ofToString (getenv("HOME") ) + "/machine_v_espejo_log.txt", true);
    }
    
}

void testApp::update(){
    j2key.update();
	tuioClient.getMessage();
	sceneManager->update(  0.016666666 );
    if(timeToEndScene !=-1 && timeToEndScene<ofGetElapsedTimeMillis()){
        ofSendMessage("endOfScene");
    }
    if(ofGetFrameNum()%25==0){
        updatetimeToEndScene();
    }
}

void testApp::updatetimeToEndScene(){
    //si se está dentro de un juego
    if(sceneManager->getCurrentSceneID()==BOLA || sceneManager->getCurrentSceneID()==COMEPANTALLA){
        if((ofGetElapsedTimef()- j2key.timeLastTouch )<2 && (ofGetElapsedTimeMillis()-timeInitScene)> DEFAULT_MIN_TIME_SCENE) { //Si hace menos de 2 segundoS que he tocado el joystick y ademas la escena ya lleva mas de  DEFAULT_MIN_TIME_SCENE rulando: aumento la duracion del juego.
            if((timeToEndScene-timeInitScene)< ABS_MAX_TIME_SCENE ) // Si aun no se ha llegado al maximo
               timeToEndScene= MAX(ofGetElapsedTimeMillis()+ 30*1000,timeToEndScene); // le sumamos 30 segundos de juego
        }
    }
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
    
    if (key == '0') {sceneManager->goToScene(FANTASMAS,false);
    timeToEndScene=ofGetElapsedTimeMillis()+DEFAULT_MAX_TIME_SCENE;
    }
	if (key == '1'){ sceneManager->goToScene(BOLA,false); timeToEndScene=ofGetElapsedTimeMillis()+DEFAULT_MAX_TIME_SCENE;
        timeInitScene=ofGetElapsedTimeMillis();
    }/* true >> regardless of curtain state (so u can change state while curtain is moving)*/
    if (key == '2'){ sceneManager->goToScene(COMEPANTALLA,false);timeToEndScene=ofGetElapsedTimeMillis()+DEFAULT_MAX_TIME_SCENE;
        timeInitScene=ofGetElapsedTimeMillis();
    }
    if (key == '3'){ sceneManager->goToScene(JOY_WAVES,false);timeToEndScene=ofGetElapsedTimeMillis()+DEFAULT_MAX_TIME_SCENE;
        timeInitScene=ofGetElapsedTimeMillis();
    }
    if (key == '4'){ sceneManager->goToScene(CARAS3D,false);timeToEndScene=ofGetElapsedTimeMillis()+DEFAULT_MAX_TIME_SCENE;
        timeInitScene=ofGetElapsedTimeMillis();
    }
    if (key == '5'){ sceneManager->goToScene(MOSAICOCARAS,false);timeToEndScene=ofGetElapsedTimeMillis()+DEFAULT_MAX_TIME_SCENE;
        timeInitScene=ofGetElapsedTimeMillis();
    }
    
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

void testApp::gotMessage(ofMessage m){
    cout << "message event";
    cout << m.message << endl;
    
    if(m.message=="endOfScene"){
        int nextS=chooseScene();
        timeToEndScene=ofGetElapsedTimeMillis()+DEFAULT_MAX_TIME_SCENE;
        sceneManager->goToScene(nextS, false);
        timeInitScene=ofGetElapsedTimeMillis();
    }
    
}


int testApp::chooseScene(){
    int id=sceneManager->getCurrentSceneID();
    id++;
    if(id>NUM_SCENES) // si se han acabado las escenas volver
        return 0;
    else return id;
}




