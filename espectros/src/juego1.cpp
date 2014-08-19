//
//  juego1.cpp
//  mainScreen
//
//  Created by Sergio Galan on 7/28/14.
//
//
#include "juego1.h"
void juego1::setup(){
    
	ofSetLogLevel(OF_LOG_NOTICE);
	
    box2d.init();

	box2d.setGravity(0, 8);
	box2d.createBounds(0,0,SCREEN_W,SCREEN_H);
    //box2d.createGround(0,SCREEN_H,SCREEN_W,SCREEN_H);
  	box2d.setFPS(30.0);
	box2d.registerGrabbing();
    
    goal.setPhysics(0, 0.53, 0.8);
    goal.setup(box2d.getWorld(),SCREEN_W-20, SCREEN_H-100, 20,100);
    datoObjeto *porteria=new datoObjeto;
    porteria->tipo=GOAL;
    goal.setData(porteria);
    moveGoal();
    
    datoObjeto *pelota= new datoObjeto;
    pelota->tipo=BALL;

    ball.setPhysics(0.2, 0.53, 0.8);
    ball.setup(box2d.getWorld(),0, SCREEN_H-100, 20);
//    ball.setData(new string("ball"));
    ball.setData(pelota);
    
    throwDirection.set(100,-100);
    throwForce=4;
    amountForce=50;
    isGoal=false;
    
    fbo.allocate(SCREEN_W, SCREEN_H, GL_RGBA);
    fboGeneral.allocate(640, 480, GL_RGBA);
    appStatuses["game_status"]=PLAYING;
    appStatuses["win_timer"]=0;
    appStatuses["hold_key"]=false;

}
/*
 void drawSubsection(float x, float y, float w, float h, float sx, float sy, float sw, float sh);
 void drawSubsection(float x, float y, float sx, float sy, float sw, float sh);
 */
void juego1::draw(){
    ofSetColor(255);
    fbo.begin();
        ofClear(0, 0, 0, 0);
        //  IMPORTANTE ESTOY DIBUJANDO SOBRE UN FBO DE TAMAÑO DEFINIDO CON LO QUE AL PINTAR FUERA NO SE VE
        mSyphonClient2->drawSubsection(0,0,SCREEN_W,SCREEN_H,0,VIDEO_offset,640,SCREEN_H);
    fbo.end();
    /*convertColor(cam, gray, CV_RGB2GRAY);
    Canny(gray, edge, mouseX, mouseY, 3);
    edge.update();*/
    fbo.readToPixels(remoteBlobImgPxl);
    //feedImg.setFromPixels(remoteBlobImgPxl);
    ofxCv::convertColor(remoteBlobImgPxl,gray , CV_RGB2GRAY);
    ofxCv::Canny(gray, edge, ofGetMouseX(), ofGetMouseY(), 3);
    edge.update();
    ofFill();
    ofBackground(0,0,0);

    ofPushMatrix();

    ofTranslate(200,200); //PINTO EN LA ZONA D ELA PANTALLA QUE QUIERO
    ofRect(-2,-2,SCREEN_W+5,SCREEN_H+5);
    ofSetColor(0,184, 0);
    edge.draw(0,0);
    /*ofPushMatrix();
    ofTranslate(0,-VIDEO_offset);
    ofPopMatrix();*/
    
    for(int i=0; i<circles.size(); i++) {
		ofFill();
		ofSetColor(255, 100, 0);
        datoObjeto *mdata=(datoObjeto *)circles[i]->getData();
        if(mdata->isHidden==false){
        //    circles[i].get()->draw();
        }
	}
    
    ofSetColor(255, 255, 255);
    ofNoFill();
    ball.draw();
    ofSetColor(248, 88, 152);
    box2d.drawGround();
    goal.draw();

    drawControls();
    
    if(isGoal==true){
        appStatuses["win_timer"]=ofGetElapsedTimeMillis();
        isGoal=false;
        appStatuses["game_status"]=WIN;
        
    }
/*    list<ofxTuioCursor*>::iterator tobj;
    list<ofxTuioCursor*> objectList = tuioclient->getTuioCursors();
    for (tobj=objectList.begin(); tobj != objectList.end(); tobj++) {
        ofxTuioCursor *blob = (*tobj);
        ofSetColor(255,0,0);
       // ofEllipse( blob->getX()*SCREEN_W, blob->getY()*480*VIDEO_scale-VIDEO_offset,9,9);
        //cout << "blob size" << blobTracker.trackedBlobs.size() << "\n";
    }
  */
    ofPopMatrix();
    showDebug();
 
    //tuioclient->drawCursors();
    ofPushMatrix();
    ofTranslate(200,200); //PINTO EN LA ZONA D ELA PANTALLA QUE QUIERO
    if(appStatuses["game_status"]==WIN){
       ofRect(-2,-2,SCREEN_W+5,SCREEN_H+5);
        ofSetColor(0,0,0);
        ofDrawBitmapString("GOAL",600,600);
        if(ofGetElapsedTimeMillis()-appStatuses["win_timer"]>2000){
            appStatuses["game_status"]=PLAYING;
        }
    }
    ofPopMatrix();
}


void juego1::update(float f){
    if(    appStatuses["game_status"]==PLAYING) {
    	box2d.update();
        ofRemove(circles, ofxBox2dBaseShape::shouldRemoveOffScreen);
        if(appStatuses["hold_key"]==true){
            if(forceDecrease==true){
                amountForce-=4;
            }
            else amountForce+=4;
            if(amountForce<0 || amountForce>200)forceDecrease=!forceDecrease;
        }
        else amountForce=0;
    }

}
void juego1::showDebug(){
    ofDrawBitmapString("Framerate " + ofToString(ofGetFrameRate()), 20, 40);
}


void juego1::addObstacle(){
    float r = ofRandom(4, 20);		// a random radius 4px - 20px
    circles.push_back(ofPtr<ofxBox2dRect>(new ofxBox2dRect));
    circles.back().get()->setPhysics(0.0, 0.53, 0.7);
    circles.back().get()->setup(box2d.getWorld(), ofGetMouseX(), ofGetMouseY(), 40, 10);
    
}

void juego1::addObstacle(ofPoint p1, int m_id){
    float r = ofRandom(5, 6);		// a random radius 4px - 20px
    circles.push_back(ofPtr<ofxBox2dRect>(new ofxBox2dRect) );
    circles.back().get()->setPhysics(0.0, 0.53, 0.7);
    circles.back().get()->setup(box2d.getWorld(), p1.x+15, p1.y+50, 20, 70);
    
    
    datoObjeto *obstacle;
    obstacle = new datoObjeto;
    obstacle->tipo=BALL;
    obstacle->id=m_id;
    
    circles.back().get()->setData(obstacle);
}

void juego1::updateObstacle(ofPoint p1, int m_id){
   	for(int i=0; i<circles.size(); i++) {
        if( ( (datoObjeto *)circles[i].get()->getData())->id  == m_id){
            circles[i].get()->setPosition(p1.x+15, p1.y+50);
            datoObjeto *mdata=(datoObjeto *)circles[i]->getData();
            mdata->isHidden=false;
        }
    }
}

void juego1::hideObstacle( int m_id){
   	for(int i=0; i<circles.size(); i++) {
        if( ( (datoObjeto *)circles[i].get()->getData())->id  == m_id){
            datoObjeto *mdata=(datoObjeto *)circles[i]->getData();
            mdata->isHidden=true;
        }
    }
}


void juego1::removeObstacle(int m_id){
    
    vector<ofPtr<ofxBox2dRect> >::iterator tobj;

    for ( tobj =  circles.begin();
          tobj != circles.end();
          tobj++ ) {
          datoObjeto *mdata=(datoObjeto *)tobj->get()->getData();
          if(  mdata->id== m_id){
              delete mdata;
            circles.erase(tobj);
              break;
          }
        
    }
}


void juego1::moveGoal(){
    goal.setPosition(     SCREEN_W-20,
                          ofRandom(40,SCREEN_H-50)  );
    
}

void juego1::throwBall(){

    ball.setPosition(100, SCREEN_H-100);
//    ball.addForce(ofVec2f(0.8,-0.9), 60);
    ofVec2f dirNorm=throwDirection.getNormalized();
    dirNorm=dirNorm*amountForce;
    ball.body->ApplyForceToCenter(b2Vec2(dirNorm.x, dirNorm.y), true);

 /*   ball.addImpulseForce(ofVec2f(
                                 ball.body->GetWorldCenter().x*OFX_BOX2D_SCALE,
                                 ball.body->GetWorldCenter().y*OFX_BOX2D_SCALE
                                 ),
                         ofVec2f(4,-20));
  
*/
    
  }

//--------------------------------------------------------------
void juego1::contactStart(ofxBox2dContactArgs &e) {

	if(e.a != NULL && e.b != NULL) {
        datoObjeto *s1=(datoObjeto *)e.a->GetBody()->GetUserData();
        datoObjeto *s2=(datoObjeto *)e.b->GetBody()->GetUserData();
		if(s1!=NULL && s2!=NULL){
            cout << "asdta astas " << endl;
        }

		if(s1!=NULL && s2!=NULL &&
        (   (s1->tipo==BALL && s2->tipo==GOAL) ||
            (s2->tipo==BALL && s1->tipo==GOAL)
        )
           ) {
                isGoal=true;
                cout << "choque" <<endl;
            
		}
	}
}


//scene notifications
void juego1::sceneWillAppear( ofxScene * fromScreen ){  // reset our scene when we appear
    init_Escena();
};


//scene notifications
void juego1::sceneWillDisappear( ofxScene * toScreen ){
	exit_Escena();
};

void juego1::init_Escena(){
 //   ofAddListener(box2d.contactStartEvents, this, &nuclear_debil::contactStart);
//	ofAddListener(box2d.contactEndEvents, this, &nuclear_debil::contactEnd);
	ofSetFrameRate(60);
        box2d.enableEvents();   // <-- turn on the event listener
    ofAddListener(box2d.contactStartEvents, this, &juego1::contactStart);
}

void juego1::exit_Escena(){
    box2d.disableEvents();
    ofRemoveListener(box2d.contactStartEvents, this, &juego1::contactStart);
}



void juego1::drawControls(){
    
    ofPushMatrix();
    ofTranslate(0,SCREEN_H);
   // ofSetColor(0,136,136);
    ofSetColor(0,184, 0);
    ofSetColor(183,248, 26);
    ofLine(0,0, throwDirection.x, throwDirection.y);
    ofPopMatrix();

    
    ofNoFill();
    ofRect(0,0,SCREEN_W,SCREEN_H);
    ofRect(0,0,200,20);
    ofFill();
    ofRect(0,0,amountForce,20);
    
    
    
}


void juego1::keyPressed(int key){
    if(key=='o')
        addObstacle();
    else if(key=='t')
        appStatuses["hold_key"]=true;
    else if(key=='a'){
        throwDirection.rotate(-1);
    }
    else if(key=='d'){
        throwDirection.rotate(1);
    }
    else if(key=='m'){
        moveGoal();
    }
    
}

void juego1::keyReleased(int key){
    if(key=='t'){
        throwBall();
        appStatuses["hold_key"]=false;
    }

}