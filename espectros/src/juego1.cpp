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
	ofDisableArbTex();
    box2d.init();

	box2d.setGravity(0, gravityY);
	box2d.createBounds(0,0,SCREEN_W,SCREEN_H);
    //box2d.createGround(0,SCREEN_H,SCREEN_W,SCREEN_H);
  	box2d.setFPS(30.0);
	box2d.registerGrabbing();
    
    goal.setPhysics(0, 0.53, 0.8);
    goal.setup(box2d.getWorld(),SCREEN_W-20, SCREEN_H-100, 10,goalHeight);
    datoObjeto *porteria=new datoObjeto;
    porteria->tipo=GOAL;
    goal.setData(porteria);
    moveGoal();
    
    datoObjeto *pelota= new datoObjeto;
    pelota->tipo=BALL;

    ball.setPhysics(ballDensity, ballBounce, ballFriction);
    ball.setup(box2d.getWorld(),100, SCREEN_H-100, 10); //
    
//    ball.setData(new string("ball"));
    ball.setData(pelota);
    
    throwDirection.set(100,-100);
    throwForce=4;
    amountForce=50;
    isGoal=false;
    
    fbo.allocate(SCREEN_W, SCREEN_H, GL_RGBA);
    
    reset();
    
    ofBackground(0,0,0);
    filter=new SobelEdgeDetectionFilter(VIDEO_W, VIDEO_H);
    addObstacle(ofPoint(ofGetMouseX(), ofGetMouseY()), -100,0.05,0.05);;
    
    setupUI();
    //eff.setup();
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
    //fbo.readToPixels(remoteBlobImgPxl);
    //feedImg.setFromPixels(remoteBlobImgPxl);
       // ofxCv::convertColor(remoteBlobImgPxl,gray , CV_RGB2GRAY);
       // ofxCv::Canny(gray, edge, ofGetMouseX(), ofGetMouseY(), 3);
       // edge.update();
  //  ofBackground(0,0,0);
//    for(int i=0; i<circles.size(); i++) {
//		ofFill();
//		ofSetColor(255, 100, 0);
//        datoObjeto *mdata=(datoObjeto *)circles[i]->getData();
//        if(mdata->isHidden==false){
//        //    circles[i].get()->draw();
//        }
//	}
    
   
 
    ofPushMatrix();
    ofTranslate(100,100); //PINTO EN LA ZONA D ELA PANTALLA QUE QUIERO
    //ofRect(-2,-2,SCREEN_W+5,SCREEN_H+5);
    ofSetColor(255);
    //  edge.draw(0,0);
    filter->begin() ;
        fbo.draw(0, 0);
    
    filter->end() ;
    if(appStatuses["game_status"]==PLAYING){
            drawControls();
         eff.draw();
    }
    
    else if(appStatuses["game_status"]==WIN){
        //ofSetColor(0,0,0);
        ofDrawBitmapString("GOAL",100,100);
        ofDrawBitmapString("LEVEL:" + ofToString(appStatuses["level"]),100,150);
         eff.draw();
        if(ofGetElapsedTimeMillis()-appStatuses["win_timer"]>2000){
            appStatuses["game_status"]=PLAYING;
            setLevel();
        }
    }
    
    else if(appStatuses["game_status"]==LOSE){
        //ofSetColor(0,0,0);
        ofDrawBitmapString("LOSER, TRY AGAIN!",100,100);
        eff.draw();
        if(ofGetElapsedTimeMillis()-appStatuses["win_timer"]>2000){
            reset();
        }
    }
    else if(appStatuses["game_status"]==WINWIN){
        //ofSetColor(0,0,0);
        ofDrawBitmapString("WELL DONE! HAS GANADO",200,100);
        eff.draw();
        if(ofGetElapsedTimeMillis()-appStatuses["win_timer"]>2000){
            reset();
        }
    }
    ofPopMatrix();
    
    showDebug();
    

    
}
void juego1::setupUI(){
    gui2 = new ofxUICanvas(0,500, 295,285);
    
    gui2->addSlider("ball Density", 0.0f, 1.0f, &ballDensity);
    gui2->addSlider("ballBounce", 0.0f, 3.0f, &ballBounce);
    gui2->addSlider("ballFriction", 0.0f, 1.0f, &ballFriction);
    
    gui2->addSlider("enemyDensity", 0.0f, 1.0f, &enemyDensity);
    gui2->addSlider("enemyBounce", 0.0f, 3.0f, &enemyBounce);
    gui2->addSlider("enemyFriction", 0.0f, 1.0f, &enemyFriction);
    gui2->addSlider("gravityY", 0.0f, 10.0f, &gravityY);
    
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
        int dif= ofGetElapsedTimeMillis()-appStatuses["lastTimeRead"] ;
        appStatuses["time"]-=dif;
        appStatuses["lastTimeRead"]= ofGetElapsedTimeMillis();
        //chequeo si el tiempo que tenia la pelota para lanzar ya ha terminado
        if( (ofGetElapsedTimeMillis()-appStatuses["counterThisLife"] >4000 ) &&  appStatuses["game_sub_status"]==BOUNCING)
        {
            //NUEVO INTENTO: HAS TIRADO MAL Y PIERDES UNA VIDA
            appStatuses["vidas"]-=1;
            appStatuses["game_sub_status"]=THROW;
            datoObjeto *mdata=(datoObjeto *)ball.getData();
            ball.destroy();
            ball.setPhysics(ballDensity, ballBounce, ballFriction);
            ball.setup(box2d.getWorld(),100, SCREEN_H-100, 10); //
            ball.setData(mdata);
            
        }
        if(appStatuses["vidas"]<0) {
            appStatuses["win_timer"]=ofGetElapsedTimeMillis();
            appStatuses["game_status"]=LOSE;
        }
        
        //Si ha habido colisión
        if(isGoal==true){
            appStatuses["win_timer"]=ofGetElapsedTimeMillis();
            isGoal=false;
            appStatuses["game_status"]=WIN;
            appStatuses["level"]+=1;
        }
        if(appStatuses["level"]>=1){ //nivel 1 y 2
            moveGoal();
        }
        
        if (appStatuses["level"]==3){//
            appStatuses["game_status"]=WINWIN;
            
        }
    }
    eff.update();
    updateObstacle(ofPoint(ofGetMouseX()+20, ofGetMouseY()+20), -100);
}

void juego1::reset(){
    appStatuses["game_status"]=PLAYING;
    appStatuses["game_sub_status"]=THROW; //THROWING WAITING
    appStatuses["win_timer"]=0;
    appStatuses["hold_key"]=false;
    appStatuses["timeleft"]=10000;
    appStatuses["lastTimeRead"];
    appStatuses["level"]=0;
    appStatuses["vidas"]=3;
    appStatuses["counterThisLife"]=0;
    
    //reset ball
    datoObjeto *mdata=(datoObjeto *)ball.getData();
    ball.destroy();
    ball.setPhysics(ballDensity, ballBounce, ballFriction);
    ball.setup(box2d.getWorld(),200, SCREEN_H-100, 10); //
    ball.setData(mdata);
    
    goalHeight=80;
    //reset goal
    mdata=(datoObjeto *)goal.getData();
    goal.destroy();
    goal.setPhysics(0, 0.53, 0.8);
    goal.setup(box2d.getWorld(),SCREEN_W-20, SCREEN_H-100, 10,goalHeight);
    goal.setData(mdata);

}

void juego1::setLevel(){
    appStatuses["lifes"]=3;
    appStatuses["game_sub_status"]=THROW;
    
    datoObjeto *mdata=(datoObjeto *)ball.getData();
    ball.destroy();
    ball.setPhysics(ballDensity, ballBounce, ballFriction);
    ball.setup(box2d.getWorld(),100, SCREEN_H-100, 10); //
    ball.setData(mdata);
    
    if(appStatuses["level"]==2){
        goalHeight=50;
        datoObjeto *mdata=(datoObjeto *)goal.getData();
        goal.destroy();
        goal.setPhysics(0, 0.53, 0.8);
        goal.setup(box2d.getWorld(),SCREEN_W-20, SCREEN_H-100, 10,goalHeight);
        goal.setData(mdata);

    }
}


void juego1::showDebug(){
    ofDrawBitmapString("Framerate " + ofToString(ofGetFrameRate()), 20, 40);
    ofDrawBitmapString("Level " + ofToString(appStatuses["level"]), 20, 55);
    ofDrawBitmapString("Lifes " + ofToString(appStatuses["vidas"]), 20, 70);
}


void juego1::randomMoveGoal(){
    goal.setPosition(     SCREEN_W-20,
    ofRandom(40,SCREEN_H-50)  );
}

void juego1::moveGoal(){
    ofPoint p =goal.getPosition();
    if(goalDown==true) p.y+=4;
    else p.y-=4;
    if(p.y>SCREEN_H-goalHeight/2) goalDown=false;
    else if(p.y<0) goalDown=true;
    goal.setPosition(    p);
}

void juego1::throwBall(){

    ball.setPosition(100, SCREEN_H-100);
//    ball.addForce(ofVec2f(0.8,-0.9), 60);
    ofVec2f dirNorm=throwDirection.getNormalized();
    dirNorm=dirNorm*amountForce;
    ball.body->ApplyForceToCenter(b2Vec2(dirNorm.x, dirNorm.y), true);
    
    appStatuses["counterThisLife"]=ofGetElapsedTimeMillis();
    appStatuses["game_sub_status"]=BOUNCING;
    
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
            if(   (s1->tipo==BALL && s2->tipo==GOAL) ||
            (s2->tipo==BALL && s1->tipo==GOAL)     )
            {
                isGoal=true;
                cout << "choque" <<endl;
            
            }
            else if ((s1->tipo==BALL && s2->tipo==OBSTACLE) ||
                     (s2->tipo==BALL && s1->tipo==OBSTACLE) ){
                cout << "colision" <<endl;
                ofVec2f p= ball.getPosition();
             //   b2Vec2 p=e.b->GetBody()->GetPosition();
                ofPtr<effects> exx =  eff.addEffect(COLL);
                ofPtr<explosion> ex1;//= exx;
                ex1 = std::static_pointer_cast<explosion>(exx);
                
                ex1->setPosition(p.x,p.y);

            }
        
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
    ofAddListener(gui2->newGUIEvent,this,&juego1::gui2Event);
    gui2->enable();
}

void juego1::exit_Escena(){
    box2d.disableEvents();
    ofRemoveListener(box2d.contactStartEvents, this, &juego1::contactStart);
    gui2->disable();
}

void juego1::gui2Event(ofxUIEventArgs &e)
{
    cout << "event";
	string name = e.widget->getName();
    datoObjeto *mdata=(datoObjeto *)ball.getData();
    ball.destroy();
    ball.setPhysics(ballDensity, ballBounce, ballFriction);
    ball.setup(box2d.getWorld(),100, SCREEN_H-100, 10); //
    ball.setData(mdata);
    
    box2d.setGravity(0, gravityY);
	int kind = e.widget->getKind();
    cout << "name" << name << "\n";
    cout << "kind" << kind << "\n";

    //if(kind==OFX_UI_WIDGET_TOGGLE && name=="grabacion"){
    //    if(appStatuses["mode"]==CAPTURE)
    //        appStatuses["mode"]=MOSAIC;
    //    else appStatuses["mode"]=CAPTURE;
    //}
}




void juego1::drawControls(){
    
    ofPushMatrix();
    ofTranslate(0,SCREEN_H);
   // ofSetColor(0,136,136);
    ofSetColor(124, 124, 124);
    ofLine(0,0, throwDirection.x, throwDirection.y);
    float ang= atan(throwDirection.y/throwDirection.y);
    ofLine(throwDirection.x-10, throwDirection.y, throwDirection.x, throwDirection.y);
    ofLine(throwDirection.x, throwDirection.y+10, throwDirection.x, throwDirection.y);
    ofPopMatrix();

    
    ofNoFill();
    ofRect(0,0,SCREEN_W,SCREEN_H);
    ofRect(0,0,200,20);
    ofFill();
    ofRect(0,0,amountForce,20);
    
    ofNoFill();
    ball.draw();
    box2d.drawGround();
    goal.draw();
    ofSetColor(124, 124, 124);
    for(int i=0; i<circles.size(); i++) {
        
                datoObjeto *mdata=(datoObjeto *)circles[i]->getData();
              //  if(mdata->id==-100){
                    circles[i].get()->draw();
              //  }
        	}
}


void juego1::keyPressed(int key){
    if(key=='o')
        addObstacle();
    else if(key=='e')
        appStatuses["hold_key"]=true;
    else if(key=='w'){
        throwDirection.rotate(-1);
    }
    else if(key=='s'){
        throwDirection.rotate(1);
    }
    else if(key=='t'){
        moveGoal();
    }
    
}

void juego1::keyReleased(int key){
    if(key=='e'){
        if(appStatuses["game_sub_status"]==THROW)
            throwBall();
        appStatuses["hold_key"]=false;
    }

}