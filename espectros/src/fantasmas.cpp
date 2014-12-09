#include "fantasmas.h"

//--------------------------------------------------------------

using namespace ofxCv;
using namespace cv;



bool
idExist(ofxTuioClient *tuioclient,  int mId  ){
    list<ofxTuioCursor*>::iterator tobj;
    list<ofxTuioCursor*> objectList = tuioclient->getTuioCursors();
    for (tobj=objectList.begin(); tobj != objectList.end(); tobj++) {
        ofxTuioCursor *blob = (*tobj);
        if(blob->getSessionId() == mId){
            return true;
        }
    }
    return false;
    
}


ofxTuioCursor* recordSelectedBlob(list<ofxTuioCursor*> objectList,int mId){
    list<ofxTuioCursor*>::iterator tobj;
    for (tobj=objectList.begin(); tobj != objectList.end(); tobj++) {
        ofxTuioCursor *blob = (*tobj);
        if(blob->getSessionId()==mId){
            return blob;
        }
    }
}


void fantasmas::setup(){

    setupStatus();
    //Syphon client


    fbo.allocate(SCREEN_W, SCREEN_H, GL_RGBA);
    //fboGeneral.allocate(640, 480, GL_RGB);
    remoteBlobImgPxl.allocate(SCREEN_W,SCREEN_H,OF_PIXELS_RGBA);
    
   // cameraImg.allocate(640, 480,OF_IMAGE_COLOR );
   //mPlayer.setup();
    

  //  moveandrecord.setup();
        //feedImg.allocate(640,480,OF_IMAGE_COLOR_ALPHA);
    testImg.loadImage("test.jpg");
    mRecorder.setup();
    setupSequences();
    
    gui2 = new ofxUICanvas(100,500, 200,200);
    gui2->disable();
    vector<string> vnames; vnames.push_back("0"); vnames.push_back("1"); vnames.push_back("2");
    gui2->addLabel("VERTICAL RADIO", OFX_UI_FONT_MEDIUM);
    ofxUIRadio *radio = gui2->addRadio("VR", vnames, OFX_UI_ORIENTATION_HORIZONTAL);
    radio->activateToggle("0");
    maxFrame=16;
    gui2->addIntSlider("maxFrame", 0, 49, &maxFrame);
    gui2->addLabel("GRABACION", OFX_UI_FONT_MEDIUM);
    gui2->addToggle("grabacion", false);
    videoCounter=0;
    for(int i=0; i< MAX_SEQUENCES; i++){
        videoQueue.push(i);
    }
}

void fantasmas::setupSequences(){
    ofxXmlSettings settings;
    for(int i=0; i<MAX_SEQUENCES;i++){
        sequences.push_back(ofxImageSequence());
        //points[i]= vector<ofPoint>();
    }
    int ii=0;
    for (std::vector<ofxImageSequence>::iterator it = sequences.begin() ;
         it != sequences.end(); ++it){
    	//it->loadSequence(ofToDataPath("recordings/video_"+ofToString(ii)+"/frame" , "png", 0, 75, 4);
        it->loadSequence(ofToDataPath("recordings/video_"+ofToString(ii)));
        it->setFrameRate(30);
        it->preloadAllFrames();
        
    //LOAD POINTS
        vector<ofPoint> mpoints;
        if(settings.loadFile("recordings/positions"+ofToString(ii)+".xml")){
            settings.pushTag("positions");
            int numberOfSavedPoints = settings.getNumTags("position");
            for(int i = 0; i < numberOfSavedPoints; i++){
                settings.pushTag("position", i);
                ofPoint p;
                p.x = settings.getValue("X", 0);
                p.y = settings.getValue("Y", 0);
                mpoints.push_back(p);
                settings.popTag();
            }
            settings.popTag(); //pop position
            points[ii]=mpoints;
        }
        else{
            ofLogError("Position file did not load!");
            ofLogError("recordings/positions"+ofToString(ii)+".xml");
        }
        ii++;
    }
    backgroundImg.loadImage("fondos/fantasmas.png");
    
}

//carga la  secuencia "seq" En la lista
void fantasmas::setupSequence(int seq){
        ofLog() <<" Entering setupSequence " << seq;
        sequences[seq].loadSequence(ofToDataPath("recordings/video_"+ofToString(seq)));
        sequences[seq].setFrameRate(30);
        sequences[seq].preloadAllFrames();
        ofxXmlSettings settings;
        vector<ofPoint> mpoints;
        if(settings.loadFile("recordings/positions"+ofToString(seq)+".xml")){
            settings.pushTag("positions");
            int numberOfSavedPoints = settings.getNumTags("position");
            for(int i = 0; i < numberOfSavedPoints; i++){
                settings.pushTag("position", i);
                ofPoint p;
                p.x = settings.getValue("X", 0);
                p.y = settings.getValue("Y", 0);
                mpoints.push_back(p);
                settings.popTag();
            }
            settings.popTag(); //pop position
            points[seq]=mpoints;
        }
        else{
            ofLogError("Position file did not load!");
            ofLogError("recordings/positions"+ofToString(seq)+".xml");
        }
}

void fantasmas::setupStatus(){
    appStatuses["syphonEnabled"]=true;
    appStatuses["mode"]=CAPTURE;
    appStatuses["debug"]=false;
    
}

//--------------------------------------------------------------
void fantasmas::update(float d){
  //  mPlayer.update();

 //   tuioclient->getMessage();
//Cuando se est� capturando un fragmento y no reproduciendo se entra en este modo
    if(appStatuses["mode"]==CAPTURE){
        
        
        if(!idExist(tuioclient,selectedBlobId) ){
            selectedBlobId=-1;
            recordThisBlob=false;
        }
        
   //si el usuario ha seleccionado que hay que grabar:
        if(recordThisBlob==true && selectedBlobId!=-1){
            selectedBlob=recordSelectedBlob(tuioclient->getTuioCursors(),selectedBlobId);
            if(selectedBlob!=NULL && mRecorder.status==mosaicRecorder::NORECORDING){
               // mRecorder.current_video=moveandrecord.currentRect;
                mRecorder.current_video=videoCounter;
                cout<< "width" << selectedBlob->width<< "height"<< selectedBlob->height;
                ofPoint p1 = convertPoint2(selectedBlob->getX(), selectedBlob->getY());
                
                mRecorder.start(2000, 30, remoteBlobImgPxl,
                                selectedBlob->width*VIDEO_W*VIDEO_scale,selectedBlob->height*VIDEO_H*VIDEO_scale,
                                p1.x, p1.y
                                );
                RecordingBlobId=selectedBlob->getSessionId();
                recordThisBlob=false;
            }else{ //error raro, no se ha encontrado blob
                
                
            }
            
        }

        
        if( mRecorder.status==mosaicRecorder::RECORDING ){
            bool blobStillExists=false;
            list<ofxTuioCursor*>::iterator tobj;
            list<ofxTuioCursor*> objectList = tuioclient->getTuioCursors();
            
            //Buscamos el blob que estamos grabando ( el que tiene la id=RecordingBlobId
            for (tobj=objectList.begin(); tobj != objectList.end(); tobj++) {
                ofxTuioCursor *blob = (*tobj);
                if(blob->getSessionId() == RecordingBlobId){
                    ofPoint p2 = convertPoint2(blob->getX(), blob->getY());
                    //mandamos el nuevo frame a grabar
                    if(mRecorder.update(p2.x-10 , p2.y-10,
                                        blob->width*VIDEO_scale*VIDEO_W+20,
                                        blob->height*VIDEO_scale*VIDEO_H+20)==true) RecordingBlobId=-1;
                    blobStillExists=true;
                    break;
                }
            }
            if(blobStillExists==false){
                mRecorder.abort();
                RecordingBlobId=-1;
                selectedBlobId=-1;
                cout << "grabacion cancelada";
                showAbortRecordUntil=ofGetElapsedTimeMillis()+5000;
                recordThisBlob=false;

                
            }
        }else{
            mRecorder.update();
        }
        if(ofGetFrameNum()%3==0){
            if(checkPosibleCollision()==true)
                selectNextPhantom();
        }
    } // END STATUSES CAPTURE
}


//--------------------------------------------------------------
void fantasmas::draw(){

    ofBackground(0);
	ofSetColor(255);
    if(appStatuses["mode"]==CAPTURE){
            backgroundImg.draw(0,0,1280,720);
         //ofDisableBlendMode();
        //mSyphonClient->draw(0, 0,640,480);
        ofPushMatrix();
        ofTranslate(100,100);
//dibujo la imagen normal que se ve en pantalla
        syphonFullImage->drawSubsection(0,0,SCREEN_W,SCREEN_H,0,130,VIDEO_W,SCREEN_H);
        //DIBUJO LOS
        list<ofxTuioCursor*>::iterator tobj;
        list<ofxTuioCursor*> objectList = tuioclient->getTuioCursors();
        for (tobj=objectList.begin(); tobj != objectList.end(); tobj++) {
            ofxTuioCursor *blob = (*tobj);
            ofSetColor(255,0,0);
            ofPoint p1 = convertPoint2(blob->getX(), blob->getY());
           // ofEllipse( p1.x, p1.y,9,9);
            //cout << "blob size" << blobTracker.trackedBlobs.size() << "\n";
            if(blob->getSessionId() == selectedBlobId){
                ofSetColor(255,0,0);
                ofNoFill();
                ofSetLineWidth(2);
                ofRect(p1.x , p1.y, blob->width*VIDEO_scale*VIDEO_W, blob->height*VIDEO_scale*VIDEO_H);
                ofFill();
                //dibuja cirulo o circulo parpadeate en funcion del estado
                if(mRecorder.status==mosaicRecorder::RECORDING){
                    if(ofGetElapsedTimeMillis()%1000 <500)
                        ofEllipse(p1.x-7 , p1.y-7,6,6);
                }else{
                    ofEllipse(p1.x-6 , p1.y-6,4,4);
                }
            }
            else{
                ofSetColor(0,100,0);
                ofNoFill();
                ofRect(p1.x , p1.y, blob->width*VIDEO_scale*VIDEO_W, blob->height*VIDEO_scale*VIDEO_H);
            }
        }
        ofSetColor(255);


        if(secuence_to_play!=-1){
            playImage(secuence_to_play);
        }
  /**************** FIN TRANSLATE 100 100 ********/
        ofPopMatrix();
        drawDisplays();
        
//codigo para tener actualizados los pixels de:remoteBlobImgPxl que es donde se graban
        
        ofSetColor(255);
        fbo.begin();
        ofClear(0, 0, 0, 0);
//      mSyphonClient2->draw(0,0,640,480);
        syphonOnlyBlobs->drawSubsection(0,0,SCREEN_W,SCREEN_H,0,130,VIDEO_W,SCREEN_H);
        fbo.end();
        fbo.readToPixels(remoteBlobImgPxl);
        //feedImg.setFromPixels(remoteBlobImgPxl);
        //feedImg.update();


    }else  if(appStatuses["mode"]==REPLAYING){ //este metodo al final casi que n ose usa mas que para test
        backgroundImg.draw(0,0,1280,720);
		ofPushMatrix();
        ofTranslate(100,100);
//        mSyphonClient->draw(0,0,640,480);
        syphonFullImage->drawSubsection(0,0,SCREEN_W,SCREEN_H,0,0,VIDEO_W,SCREEN_H);
        ofSetColor(255,255,255);

        float tt = ofGetElapsedTimef();
        
        int ii=0;
        /*for (std::vector<ofxImageSequence>::iterator it = sequences.begin() ;
                                                it != sequences.end(); ++it){
            int index= it->getCurrentFrame();
			it->getFrame(index)->draw(points[ii][index].x,points[ii][index].y);//draw(RECTANGLESIZE*(ii%5),RECTANGLESIZE*floor(ii/5));
            index++;
            if(index> it->getTotalFrames() ) index=0;
            it->setFrame(index);
            ii++;
            break;
        } */
        
		//get the frame based on the current time and draw it
         //ofDisableBlendMode();
       // glDisable (GL_BLEND);
        playImage(currentImg);
        ofPopMatrix();
    }
  //
	if(appStatuses["debug"]) showDebug();
    
    
}

void fantasmas::drawDisplays(){
    ofPushMatrix();
    ofTranslate(976,425);
    ofSetColor(20);
    if(ofGetElapsedTimeMillis()< showAbortRecordUntil){
        courierFont.drawString("El ordenador ha perdido la pista \n al paseante o se ha mezclado \n con otro. Prueba de nuevo"  , 0, 0);
    }
    if(mRecorder.status==mosaicRecorder::RECORDING){
        courierFont.drawString("Grabando!"  , 0, 0);
    }
    
    
    if(ofGetElapsedTimeMillis()< showSuccessRecordUntil){
        courierFont.drawString("Grabacion realizada con exito"  , 0,0);
        //TODO y se muestra aqu�
    }
    ofPopMatrix();
   
}

void fantasmas::playImage(int i ){
    int index = sequences[i].getCurrentFrame();
    sequences[i].getFrame(index)->draw(points[i][index].x,points[i][index].y);
    index++;
    if(index> maxFrame/*sequences[i].getTotalFrames()*/ ) index=0;
    sequences[i].setFrame(index);
}

void fantasmas::showDebug(){
    ofPushMatrix();
    ofDrawBitmapString("Framerate " + ofToString(ofGetFrameRate()), 20, 20);
    if(appStatuses["mode"]==CAPTURE)
    ofDrawBitmapString("mode: CAPTURE" , 20, 40);
    else     ofDrawBitmapString("mode: DEBUG" , 20, 40);
    // ofDrawBitmapString("NonZero " + ofToString(nonZero), 20, 40);
    ofDrawBitmapString("selected id" + ofToString(selectedBlobId) , 300, 20);
    courierFont.drawString("Cola"+ ofToString( mRecorder.videoSaver.q.size() ), 480, 20);
    ofDrawBitmapString("Reproduciendo" + ofToString(secuence_to_play) , 300, 40);
    ofDrawBitmapString("Grabar en el slot" + ofToString(videoCounter) , 420, 40);
    
    ofPopMatrix();
}
/*
void fantasmas::tuioAdded(ofxTuioCursor &tuioCursor){
	ofPoint loc = ofPoint(tuioCursor.getX()*ofGetWidth(),tuioCursor.getY()*ofGetHeight());
	//cout << "Point n" << tuioCursor.getSessionId() << " add at " << loc << endl;
}

void fantasmas::tuioUpdated(ofxTuioCursor &tuioCursor){
	ofPoint loc = ofPoint(tuioCursor.getX()*ofGetWidth(),tuioCursor.getY()*ofGetHeight());
	//cout << "Point n" << tuioCursor.getSessionId() << " updated at " << loc << endl;
}

void fantasmas::tuioRemoved(ofxTuioCursor &tuioCursor){
	ofPoint loc = ofPoint(tuioCursor.getX()*ofGetWidth(),tuioCursor.getY()*ofGetHeight());
	//cout << "Point n" << tuioCursor.getSessionId() << " remove at " << loc << endl;
}*/

//--------------------------------------------------------------
void fantasmas::keyPressed(int key){

}

void fantasmas::onRecordingFinished(int &num){
    cout << "onRecordingFinished:: grabacion finalizada "<< endl;
    selectedBlobId=-1;
    recordThisBlob==false;
    setupSequence(num);
    selectedBlob=NULL;
    if(++videoCounter>=MAX_SEQUENCES){
        videoCounter=0;
    }
    showSuccessRecordUntil=ofGetElapsedTimeMillis()+4000;
   
}



//--------------------------------------------------------------
void fantasmas::keyReleased(int key){
    if(key=='e'){
        recordThisBlob=true;
    }
    
    else if(key=='m'){
        if(appStatuses["mode"]==CAPTURE)
            appStatuses["mode"]=REPLAYING;
        else appStatuses["mode"]=CAPTURE;
    }
    
    else if(key=='g'){
        if(gui2->isEnabled()) {
            appStatuses["debug"]=false;
            gui2->disable();
        }
        else {
            appStatuses["debug"]=true;
            gui2->enable();
        }
    }
    
    else if(key=='d' || key=='a'){
        // si se esta grabando no se hace nada
        if(recordThisBlob==true) return;
        //se pilla el primer blob disponible
        list<ofxTuioCursor*>::iterator tobj;
        list<ofxTuioCursor*> objectList = tuioclient->getTuioCursors();
        
        if(selectedBlobId==-1){ //Si no hay blob seleccionado seleccioamos el primero
         //   cout << "selected blog = -1"<<endl;
            if(objectList.size()>0) {
                selectedBlobId=(*objectList.begin())->getSessionId();
            }
            return; //si no hay ning�n blob selectedblob seguir� siendo -1 y al volver lo seleccionara
        }
        else{ //recorremos todos los blobs
            for (tobj=objectList.begin(); tobj != objectList.end(); tobj++) {
                ofxTuioCursor *blob = (*tobj);
                if(blob->getSessionId() == selectedBlobId){
            //  Cuanco encontramos el blob actual seleccionamos el siguiente
                    ofxTuioCursor* nextb=getNextBlob(tobj,objectList);
                    if(nextb!=NULL){
                        selectedBlobId=nextb->getSessionId();
                    }
                    else{
                        selectedBlobId =-1;
                        ofLogError()<<"next blog was null" <<endl;
                    }
                    return;
                }
            }
            selectedBlobId=-1; // si se llega a este punto algo ha ido raro asi que lo ponemos a -1
        }
    }    
}




//--------------------------------------------------------------
void fantasmas::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void fantasmas::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void fantasmas::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void fantasmas::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void fantasmas::windowResized(int w, int h){

}

//--------------------------------------------------------------
void fantasmas::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void fantasmas::dragEvent(ofDragInfo dragInfo){ 

}


