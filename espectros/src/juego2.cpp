//
//  juego2.cpp
//  mainScreen
//
//  Created by Sergio Galan on 7/28/14.
//
//
#include "juego2.h"
void juego2::setup(){
    
	ofSetLogLevel(OF_LOG_NOTICE);
	

    
    box2d.init();
    ofSetFrameRate(40);
	box2d.setGravity(0, 5);
	box2d.createBounds(0,0,SCREEN_W,SCREEN_H);
    //box2d.createGround(0,SCREEN_H,SCREEN_W,SCREEN_H);
  	box2d.setFPS(30.0);
	box2d.registerGrabbing();
    

    datoObjeto *jugador= new datoObjeto;
    jugador->tipo=BALL;
    
    player.setPhysics(0, 0.53, 0.8);
    player.setup(box2d.getWorld(),0, SCREEN_H, 1);
    //    ball.setData(new string("ball"));
    player.setData(jugador);
    fbo.allocate(SCREEN_W, SCREEN_H, GL_RGBA);
    fboGeneral.allocate(640, 480, GL_RGBA);
    appStatuses["game_status"]=PLAYING;
    appStatuses["win_timer"]=0;
    appStatuses["hold_key"]=false;
    appStatuses["isOnContour"]=true;
    appStatuses["lastMovement"]=NONE;
    

    mask1.allocate(SCREEN_W, SCREEN_H,OF_IMAGE_COLOR);
    //maskFbo.allocate(SCREEN_W, SCREEN_H,GL_LUMINANCE);
    maskFbo.allocate(SCREEN_W, SCREEN_H,GL_RGB);
    maskFbo.begin();
        ofClear(0, 0, 0);
        ofSetColor(255);
        ofNoFill();
        ofSetLineWidth(1);
        ofDisableSmoothing();
        ofRect(1,1,SCREEN_W-1, SCREEN_H-1);
        ofRect(1,1,SCREEN_W-1, SCREEN_H-1);
        //ofRect(0,0,SCREEN_W, SCREEN_H);
    maskFbo.end();
    
    maskFbo.readToPixels(mask1.getPixelsRef());
    mask1.update();
    
    contourFinder.setMinAreaRadius(1);
	contourFinder.setMaxAreaRadius(17000);
    contourFinder.setFindHoles(true);
   // contourFinder.setUseTargetColor(false);
    contourFinder.setTargetColor(ofColor(0), ofxCv::TRACK_COLOR_RGB);
    threshold=254;
    contourFinder.setThreshold(threshold);
    contourFinder.findContours(mask1);
}


void juego2::update(float f){

    if(    appStatuses["game_status"]==PLAYING) {
    	box2d.update();
        move(currentKey);
               
        if(prepareReset) {
            //explode();
            reset();
        prepareReset=false;
        }
       // threshold = ofMap(ofGetMouseX(), 0, ofGetWidth(), 0, 255);
        //if(startProcessing);
            //contourFinder.findContours(mask1); // no hace falta pasar el contour finder todo el rato solo cuando se cierra
    }
    
    
}
void juego2::draw(){
  
  /*** CALCULO REGION QUE SE MUESTRA (CUANDO TOCA )  ****/
    drawBgRegion();
    
    drawNewArea();
    /*** DIBUJADO REAL */

    
    
    ofPushMatrix();
        ofTranslate(200,200); //PINTO EN LA ZONA DE LA PANTALLA QUE QUIERO
       // mask1.draw(0,0);   
        ofSetColor(255,255,255);
        ofFill();
        imgRecortada.draw(0,0);
        ofSetLineWidth(2);
        poli.draw();
        ofNoFill();
        player.draw();
        ofPushMatrix();
            ofTranslate(player.getPosition().x, player.getPosition().y);
            ofRotate(45);
            ofSetRectMode(OF_RECTMODE_CENTER);
            ofRect(0,0, 5, 5);
            ofSetRectMode(OF_RECTMODE_CORNER);
        ofPopMatrix();
    
        drawControls();
        //dibujo enemigos
    
        drawEnemies();

        if(appStatuses["game_status"]==WIN){
            ofRect(-2,-2,SCREEN_W+5,SCREEN_H+5);
            ofSetColor(0,0,0);
            ofDrawBitmapString("GOAL",600,600);
            if(ofGetElapsedTimeMillis()-appStatuses["win_timer"]>2000){
                appStatuses["game_status"]=PLAYING;
            }
        }
        if(drawExplodeb)
            drawExplode();
    ofPopMatrix();


    showDebug();
       //     imgRecortada.draw(900,0);
    
}

void juego2::drawBgRegion(){
    fbo.begin();
    ofClear(0, 0, 0, 0);
    //  IMPORTANTE ESTOY DIBUJANDO SOBRE UN FBO DE TAMAÑO DEFINIDO CON LO QUE AL PINTAR FUERA NO SE VE
    ofSetColor(255);
    mSyphonClient->drawSubsection(0,0,SCREEN_W,SCREEN_H,0,VIDEO_offset,640,SCREEN_H);
    fbo.end();
    fbo.readToPixels(remoteBlobImgPxl);
    //  remoteBlobImgPxl.update();
    
    //Paso las imagenes originales a openCV
    cv::Mat fullimageCV;
    fullimageCV=ofxCv::toCv(remoteBlobImgPxl);
    
    cv::Mat mask1grayCV;
    ofxCv::copyGray(mask1, mask1grayCV);
    
    //creo la matriz donde se va a guardar la imagen de la mascara en blanco y negro. Negro donde no hay blob blanco donde lo hay
    cv::Mat contourMaskCV;//(640,480,CV_8U);
    cv::threshold( mask1grayCV,contourMaskCV,128, 255, cv::THRESH_BINARY); // esto sólo lo tengo que hacer para convertir la imagen a tipo 8 bits
    
    //2º aplico la mascara
    cv::Mat maskedImage; //Matriz donde almacenar la imagen de destino
    //hago una copia de fullImageCV a maskedImg pero aplicandole la mascara previamente calculada.
    fullimageCV.copyTo(maskedImage, contourMaskCV);
    
	ofxCv::toOf(maskedImage, imgRecortada);
	imgRecortada.update();

}

void juego2::drawNewArea(){
    /*** CALCULO REGION RECORTADA (CUANDO TOCA )  ****/
    // 1º draw contour
    maskFbo.begin();
    if(fillthis==true){
        ofDisableSmoothing();
        ofSetColor(255);
        ofNoFill();
        //ofRect(player.getPosition(), 1, 1);
        ofSetLineWidth(2);
        poli.draw();
        //path.draw();
        poli.clear();
    }
    ofFill();
    maskFbo.end();
    maskFbo.readToPixels(mask1.getPixelsRef());
    mask1.update();
    
    // 2º flood fill
    if(fillthis==true){
        ofxCv::copyGray(mask1, mask1gray);
        mask1gray.update();
        contourFinder.findContours(mask1);
        cv::Scalar colorfill = ofxCv::toCv(ofColor(255)); //CV_RGB(120,120,120);
        cv::Mat colorImage=  ofxCv::toCv(mask1gray);
        //  cv::Mat colorImage= colorImage2.clone();
        ofPoint p2= findRegionToPaint();
        if(p2.x>0 && p2.y>0){
            cv::Point po= cv::Point(p2.x, p2.y);
            cv::floodFill(colorImage,po , colorfill);
            fillthis=false;
            ofxCv::toOf(colorImage, mask1);
            mask1.update();
            ofSetColor(255);
            
            //actualizo la mascara fbo con lo que acabo de rellenar
            maskFbo.begin();
            ofClear(0, 0, 0,0);
            ofSetColor(255);
            mask1.draw(0,0);
            maskFbo.end();
            contourFinder.findContours(mask1); //dejo el contour finder actualizado con los nuevos contornos
        }
        else {
            ofLog()<< " NO POINT FOUND" << endl;
            fillthis=false;
        }
    }
    

}

void juego2::drawEnemies(){
    ofFill();
    ofEnableBlendMode(OF_BLENDMODE_SCREEN);
    for(int i=0; i<circles.size(); i++) {
		ofFill();
		ofSetColor(255, 100, 0);
        datoObjeto *mdata=(datoObjeto *)circles[i]->getData();
        if(mdata->isHidden==false){
            if( contourFinder.size() >0 &&contourFinder.getPolyline(0).inside(circles[i].get()->getPosition().x,circles[i].get()->getPosition().y)==true){
                enemy *menemy=(enemy *)circles[i]->getData();
                //circles[i]->draw();
                menemy->draw(circles[i].get()->getPosition().x,circles[i].get()->getPosition().y);
            }
        }
	}
    ofDisableBlendMode();
}

void juego2::showDebug(){
    ofDrawBitmapString("Framerate " + ofToString(ofGetFrameRate()) + "/"+ofToString(ofGetTargetFrameRate()), 20, 40);
    if(appStatuses["isOnContour"]==false) ofDrawBitmapString ("OUT OF CONTOUR",800,50);
    ofDrawBitmapString("contornos " + ofToString(contourFinder.size()),800,100);
    if(fillthis) ofDrawBitmapString ("FILL THIS",800,150);
    ofDrawBitmapString("vertices poli " + ofToString(poli.size() ), 800,170);
    // contourFinder.draw();
   /* for (int i=0; i<contourFinder.size(); i++){
        ofPath path;
        ofPolyline l= contourFinder.getPolyline(i);
        ofSetColor(20+20*i, 50+20*i, 20*i);
        l.draw();
    }*/
}


void juego2::addObstacle(){
    float r = ofRandom(4, 20);		// a random radius 4px - 20px
    circles.push_back(ofPtr<ofxBox2dCircle>(new ofxBox2dCircle));
    circles.back().get()->setPhysics(0.1, 1, 0.7);
    circles.back().get()->setup(box2d.getWorld(), ofRandom(SCREEN_W), ofRandom(SCREEN_H), 10);
    enemy *obstacle;
    obstacle= new enemy();
    obstacle->tipo=OBSTACLE;
    obstacle->id=1;
    obstacle->create();
    circles.back().get()->setData(obstacle);
}

void juego2::addObstacle(ofPoint p1, int m_id){
    float r = ofRandom(5, 6);		// a random radius 4px - 20px
    circles.push_back(ofPtr<ofxBox2dCircle>(new ofxBox2dCircle) );
    circles.back().get()->setPhysics(0.001, 0.53, 0.7);
    circles.back().get()->setup(box2d.getWorld(), p1.x+15, p1.y+15, 20);
    enemy *obstacle;
    obstacle= new enemy();
    obstacle->tipo=OBSTACLE;
    obstacle->id=m_id;
    obstacle->create();
    circles.back().get()->setData(obstacle);
}

void juego2::updateObstacle(ofPoint p1, int m_id){
   	for(int i=0; i<circles.size(); i++) {
        if( ( (enemy *)circles[i].get()->getData())->id  == m_id){
            circles[i].get()->setPosition(p1.x+15, p1.y+50);
            //datoObjeto *mdata=(datoObjeto *)circles[i]->getData();
            //mdata->isHidden=false;
        }
    }
}

void juego2::hideObstacle( int m_id){
   	for(int i=0; i<circles.size(); i++) {
        if( ( (datoObjeto *)circles[i].get()->getData())->id  == m_id){
            datoObjeto *mdata=(datoObjeto *)circles[i]->getData();
            mdata->isHidden=true;
        }
    }
}


void juego2::removeObstacle(int m_id){
    vector<ofPtr<ofxBox2dCircle> >::iterator tobj;
    for ( tobj =  circles.begin();
         tobj != circles.end();
         tobj++ ) {
         enemy *mdata=(enemy *)tobj->get()->getData();
         if(  mdata->id== m_id){
            delete mdata;
            circles.erase(tobj);
            break;
         }
        
    }
}





//--------------------------------------------------------------
void juego2::contactStart(ofxBox2dContactArgs &e) {
    
	if(e.a != NULL && e.b != NULL) {
        datoObjeto *s1=(datoObjeto *)e.a->GetBody()->GetUserData();
        datoObjeto *s2=(datoObjeto *)e.b->GetBody()->GetUserData();
/*		if(s1!=NULL && s2!=NULL){
            cout << "asdta astas " << endl;
        }*/
		if((s1!=NULL && s2!=NULL) &&
              ((s1->tipo==BALL && s2->tipo==OBSTACLE) ||
               
               (s2->tipo==BALL && s1->tipo==OBSTACLE))
            )
            {
                cout << "choque" <<endl;
                prepareReset=true;
            }
		}
}



//scene notifications
void juego2::sceneWillAppear( ofxScene * fromScreen ){  // reset our scene when we appear
    init_Escena();
};


//scene notifications
void juego2::sceneWillDisappear( ofxScene * toScreen ){
	exit_Escena();
};

void juego2::init_Escena(){
    //   ofAddListener(box2d.contactStartEvents, this, &nuclear_debil::contactStart);
    //	ofAddListener(box2d.contactEndEvents, this, &nuclear_debil::contactEnd);
	ofSetFrameRate(40);
    box2d.enableEvents();   // <-- turn on the event listener
    ofAddListener(box2d.contactStartEvents, this, &juego2::contactStart);
    //mSyphonClient->unbind();
    //mSyphonClient2->unbind();
}

void juego2::exit_Escena(){
    box2d.disableEvents();
    ofRemoveListener(box2d.contactStartEvents, this, &juego2::contactStart);
    //mSyphonClient->bind();
    //mSyphonClient2->bind();
}



void juego2::drawControls(){

}


void juego2::keyPressed(int key){
    startProcessing=true;
    if(key=='o')
        addObstacle();

    else if(key=='a'){
        previousKey=currentKey;
        currentKey=LEFT;

    }
    else if(key=='d'){
        previousKey=currentKey;
        currentKey=RIGHT;
    }
    else if(key=='w'){
        previousKey=currentKey;
        currentKey=UP;
    }
    else if(key=='s'){
        previousKey=currentKey;
        currentKey=DOWN;;
    }
    
    else if(key=='f')
        fillthis=true;
    else if(key=='r'){
        reset();
    }
    else if(key=='p')
        startProcessing=true;

    
}

void juego2::reset(){
    ofPoint d=ofPoint(ofRandom(0,SCREEN_W),SCREEN_H);
    explode(d);
    player.setPosition(d);
    poli.clear();
    prevPos=player.getPosition();
    fillthis=false;
    appStatuses["isOnContour"]=true;
    
    mask1.clear();
    maskFbo.begin();
        ofClear(0, 0, 0);
        ofSetColor(255);
        ofNoFill();
        ofSetLineWidth(1);
        ofRect(1,1,SCREEN_W-1, SCREEN_H-1);
        ofRect(1,1,SCREEN_W-1, SCREEN_H-1);
    maskFbo.end();
    maskFbo.readToPixels(mask1.getPixelsRef());
    mask1.update();
    ofLog()<< "reset" << endl;

    
}

void juego2::keyReleased(int key){
    currentKey=NONE;
    
    
}