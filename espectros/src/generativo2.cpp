//
//  generativo2.cpp
//  mainScreen
//
//  Created by Sergio Galan on 7/28/14.
//
//

#include "generativo2.h"
void generativo2::setup(){
    fbo.allocate(SCREEN_W, SCREEN_H, GL_RGB);
    //fboGeneral.allocate(640, 480, GL_RGB);
    cameraPixels.allocate(SCREEN_W,SCREEN_H,OF_PIXELS_RGBA);
    
   // left = null;
   // right = null;
    
    //lines = new float[NLINES][lineW+1];
    
    //Create filter
   // filterr = new float[lineW];
    for (int i = 0; i < lineW; i++){
        filterr[i]= pow( 1 - (abs(i-lineW/2) / 390.00) , 4)*2;
    }
    filter=new SobelEdgeDetectionFilter(VIDEO_W, VIDEO_H);
    
}

void generativo2::keyPressed(int k){
    if(k=='t'){
        rotateAng+=2;
    }
}

void generativo2::update(float f){
    people.clear();
    list<ofxTuioCursor*>::iterator tobj;
    list<ofxTuioCursor*> objectList = tuioclient->getTuioCursors();
    for (tobj=objectList.begin(); tobj != objectList.end(); tobj++) {
        ofxTuioCursor *blob = (*tobj);
        ofPoint p1 = convertPoint(blob->getX(), blob->getY());
        if(p1.x != -1 && p1.y!=-1){
            people.push_back(p1);
        }
    }
    

    
    
}

void generativo2::draw(){
    ofSetColor(255);
   /* fbo.begin();
    ofClear(0, 0, 0, 0);
    //  IMPORTANTE ESTOY DIBUJANDO SOBRE UN FBO DE TAMAÑO DEFINIDO CON LO QUE AL PINTAR FUERA NO SE VE
        mSyphonClient2->drawSubsection(0,0,SCREEN_W,SCREEN_H,0,VIDEO_offset,640,SCREEN_H);
    fbo.end();
    */
    ofPushMatrix();
    ofTranslate(100,100); //PINTO EN LA ZONA D ELA PANTALLA QUE QUIERO
    //ofRect(-2,-2,SCREEN_W+5,SCREEN_H+5);
    ofSetColor(255);
    //  edge.draw(0,0);
    filter->begin() ;
    mSyphonClient2->drawSubsection(0,0,SCREEN_W,SCREEN_H,0,VIDEO_offset,640,SCREEN_H);
   // fbo.end();
    filter->end() ;
    
    //fin procesado imagen video que llega
    
    ofSetColor(0,100,0);
    ofNoFill();
    ofSetLineWidth(1);
    ofRect(0,0,SCREEN_W,SCREEN_H);
    p.clear();
    
    int lineP = 0; //posicion altura
    for(int i = 0; i< NLINES-1; i++){
        ofFill();
        ofSetColor(0);
        for(int j = 0; j< lineW; j++){
            ofLine(j,lineP,j, lineP-abs(lines[i][j])*filterr[j]*lineS);
        }
        
        ofNoFill();
        ofSetColor(255);
        ofSetLineWidth(2);
        //beginShape();
        for(int j = 0; j< lineW; j++){
            p.addVertex(j, lineP-abs(lines[i][j])*filterr[j]*lineS-0.5);
        }
        //endShape();
        p.draw();
        p.clear();
        lineP = lineP + lineDistance;
    }
    
    ofSetColor(250,0,0);
    ofSetLineWidth(2);
  /* dibujar puntos px1 =x[0];
    py1 = y[0];
    px2 =x[1];
    py2 = y[1];
    rect(px1,py1,7,7);
    rect(px2,py2,7,7);*/
    
   /* px1 =ofNoise(tx1) *600;
    py1 = ofNoise(ty1) *300  ;
    tx1 +=0.01;
    ty1 += 0.01;
    
    px2 = ofNoise(tx2) *500;//min(mouseX,screenWidth);//
    py2 = ofNoise(ty2) *600  ;// min(mouseY,screenHeight);//
    tx2 +=0.01;
    ty2 += 0.01;*/
    
    /****** calculos ******/
    
    //Difunde lineas
    if(ofGetFrameNum()%3==0){
        //Spread
        for (int i = 0; i < NLINES-1; i++){
            for(int j = 0; j < lineW; j++){
                if(lines[i][j]>0)    lines[i][j] = lines[i][j]-0.05;
                else if(lines[i][j]<0.1)  lines[i][j]=ofRandom(-0.05,0.05);
            }
        }
    }//framecount
    
    

    for (int j=0; j< people.size(); j++){
        
        int px1=people[j].x;
        int py1=people[j].y;
        int line1=(int)round(py1/lineDistance);
        for (int i=0; i<lineW; i++){
    //Si los puntos están junto a un blob los dibujo mayores
            if(i>(px1-20) && i<(px1+20)){
                if(i<20 || i>lineW-20) continue;
                else
                    lines[line1][i]=cos(ofDegToRad(px1-i)*5)/1.5+ofRandom(-0.05,0.05);
            }
        }
        ofSetColor(255, 100, 0);
      //  ofRect(px1,py1,7,7);
    }
    ofPopMatrix();
    
}




//scene notifications
void generativo2::sceneWillAppear( ofxScene * fromScreen ){  // reset our scene when we appear
    init_Escena();
};


//scene notifications
void generativo2::sceneWillDisappear( ofxScene * toScreen ){
	exit_Escena();
};

void generativo2::init_Escena(){
    // tx1 = ofRandom(0,9);
    //ty1 = ofRandom(0,9);
}

void generativo2::exit_Escena(){
    
}

void generativo2::tuioAdded(ofxTuioCursor &tuioCursor){
    ofPoint p= convertPoint(tuioCursor.getX(), tuioCursor.getY());
    if(p.x == -1 && p.y == -1){
        return;
    }else{
        //  addObstacle(p , tuioCursor.getSessionId(),tuioCursor.width,
        //     tuioCursor.height );
    }
    //cout << ofToString(tuioCursor.height*VIDEO_H*VIDEO_scale) <<endl;
    // if (tuioCursor.getSessionId()%3==0)
    //    cout << tuioCursor.getSessionId() << " : " << tuioCursor.height*480  <<endl;
}

void generativo2::tuioUpdated(ofxTuioCursor &tuioCursor){
    ofPoint p= convertPoint(tuioCursor.getX(), tuioCursor.getY());
    if(p.x == -1 && p.y == -1){
        //  hideObstacle(tuioCursor.getSessionId() );
    }else{
        //updateObstacle( p , tuioCursor.getSessionId(),tuioCursor.width,
        //        tuioCursor.height );
    }
    // if (tuioCursor.getSessionId() %3==0)
    //  cout << tuioCursor.getSessionId() << " : " << tuioCursor.height*480  <<endl;
    //cout << ofToString(tuioCursor.height*VIDEO_H*VIDEO_scale) <<endl;;
	
}

void generativo2::tuioRemoved(ofxTuioCursor &tuioCursor){
	//removeObstacle(tuioCursor.getSessionId());
}
