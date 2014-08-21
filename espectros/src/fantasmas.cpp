#include "fantasmas.h"

//--------------------------------------------------------------

using namespace ofxCv;
using namespace cv;

void fantasmas::setup(){

    setupStatus();
    myOSCcomm.setup();
    //Syphon client


    fbo.allocate(640, 480, GL_RGBA);
    //fboGeneral.allocate(640, 480, GL_RGB);
    remoteBlobImgPxl.allocate(640,480,OF_PIXELS_RGBA);
    
   // cameraImg.allocate(640, 480,OF_IMAGE_COLOR );
   //mPlayer.setup();
    

    moveandrecord.setup();
        //feedImg.allocate(640,480,OF_IMAGE_COLOR_ALPHA);
    testImg.loadImage("test.jpg");
    mRecorder.setup();
        setupSequences();
    
    

    
     gui2 = new ofxUICanvas(1038,583, 295,285);
    vector<string> vnames; vnames.push_back("0"); vnames.push_back("1"); vnames.push_back("2");
    gui2->addLabel("VERTICAL RADIO", OFX_UI_FONT_MEDIUM);
    ofxUIRadio *radio = gui2->addRadio("VR", vnames, OFX_UI_ORIENTATION_HORIZONTAL);
    radio->activateToggle("0");

    gui2->addIntSlider("maxFrame", 0, 49, &maxFrame);
    gui2->addLabel("GRABACION", OFX_UI_FONT_MEDIUM);
    gui2->addToggle("grabacion", false);
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
        it->setFrameRate(25);
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
    
}

void fantasmas::setupSequence(int seq){
        sequences[seq].loadSequence(ofToDataPath("recordings/video_"+ofToString(seq)));
        sequences[seq].setFrameRate(25);
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
    appStatuses["mode"]=MOSAIC;
    appStatuses["debug"]=true;
    
}

//--------------------------------------------------------------
void fantasmas::update(float d){
  //  mPlayer.update();

 //   tuioclient->getMessage();

    if(appStatuses["mode"]==CAPTURE){
        ofxTuioCursor* blobToRecord=moveandrecord.detectBlobinMouse(tuioclient->getTuioCursors(),ofGetMouseX(),ofGetMouseY());
        if(blobToRecord!=NULL && mRecorder.isRecording==false){
        //if(moveandrecord.detectMouseinSquare(mouseX, mouseY)){
            mRecorder.current_video=moveandrecord.currentRect;
            cout<< "width" << blobToRecord->width<< "height"<< blobToRecord->height;
            mRecorder.start(2000, 25, remoteBlobImgPxl,
                            blobToRecord->width*640,blobToRecord->height*320,
                           blobToRecord->getX()*640,blobToRecord->getY()*320
                            );
            RecordingBlobId=blobToRecord->getSessionId();
        }
        if( mRecorder.isRecording ){
            bool blobStillExists=false;
            list<ofxTuioCursor*>::iterator tobj;
            list<ofxTuioCursor*> objectList = tuioclient->getTuioCursors();
            for (tobj=objectList.begin(); tobj != objectList.end(); tobj++) {
                ofxTuioCursor *blob = (*tobj);
                if(blob->getSessionId() == RecordingBlobId){
                    if(mRecorder.update(blob->getX()*640-10 , blob->getY()*480-10)==true) RecordingBlobId=-1;
                    blobStillExists=true;
                    break;
                }
            }
            if(blobStillExists==false){
                mRecorder.cancel();
                RecordingBlobId=-1;
                cout << "grabacion cancelada";
            }
        }
    }
//    mRecorder.update();
    
}

//--------------------------------------------------------------
void fantasmas::draw(){
    
    //
    ofBackground(0);
	ofSetColor(255);
    if(appStatuses["mode"]==CAPTURE){
         //ofDisableBlendMode();

        mSyphonClient->draw(0, 0,640,480);
        list<ofxTuioCursor*>::iterator tobj;
        list<ofxTuioCursor*> objectList = tuioclient->getTuioCursors();
        for (tobj=objectList.begin(); tobj != objectList.end(); tobj++) {
            ofxTuioCursor *blob = (*tobj);
            		ofSetColor(255,0,0);
            ofEllipse( blob->getX()*640, blob->getY()*480,9,9);
            //cout << "blob size" << blobTracker.trackedBlobs.size() << "\n";
        }
                    		ofSetColor(255);
        fbo.begin();
        ofClear(0, 0, 0, 0);
        mSyphonClient2->draw(0,0,640,480);
        //mSyphonClient.draw(0, 0,640,480);
        fbo.end();
        //fbo.draw(640,480,320,240);
        fbo.readToPixels(remoteBlobImgPxl);
        
        feedImg.setFromPixels(remoteBlobImgPxl);
        feedImg.update();
        //feedImg.draw(640,480,320,240);
        
        //remoteBlobImgPxl=feedImg.getPixelsRef();
        //cameraImg.setFromPixels(cameraPixels);
        //cv::Mat imgMat = ofxCv::toCv(cameraPixels);
        //mPlayer.drawAll(0,0);
        
        ofPushMatrix();
        ofScale( 0.5, 1/1.5 );
        //tuioclient.drawCursors();
        ofPopMatrix();
        
        moveandrecord.draw();
    }else  if(appStatuses["mode"]==MOSAIC){
		ofPushMatrix();
        mSyphonClient->draw(0,0,640,480);
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

void fantasmas::playImage(int i ){
    int index = sequences[i].getCurrentFrame();
    sequences[i].getFrame(index)->draw(points[i][index].x,points[i][index].y);
    index++;
    if(index> maxFrame/*sequences[i].getTotalFrames()*/ ) index=0;
    sequences[i].setFrame(index);
}

void fantasmas::showDebug(){
    ofDrawBitmapString("Framerate " + ofToString(ofGetFrameRate()), 20, 20);
    if(appStatuses["mode"]==CAPTURE)
    ofDrawBitmapString("mode: CAPTURE" , 20, 40);
    else     ofDrawBitmapString("mode: DEBUG" , 20, 40);
    // ofDrawBitmapString("NonZero " + ofToString(nonZero), 20, 40);
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
    setupSequence(num);

}

//--------------------------------------------------------------
void fantasmas::keyReleased(int key){
    if(key=='r'){
        mRecorder.start(3000, 25, remoteBlobImgPxl,
                        moveandrecord.triggerRectangle.width,moveandrecord.triggerRectangle.height,
                        moveandrecord.triggerRectangle.x,moveandrecord.triggerRectangle.y);
    }
    else if(key=='m'){
        if(appStatuses["mode"]==CAPTURE)
            appStatuses["mode"]=MOSAIC;
        else appStatuses["mode"]=CAPTURE;
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

void fantasmas::gui2Event(ofxUIEventArgs &e)
{
    cout << "event";
	string name = e.widget->getName();
	int kind = e.widget->getKind();
    cout << "name" << name << "\n";
    cout << "kind" << kind << "\n";
    if(kind==2){
        if(name == "2")
        {
            currentImg=2;
        }
        if(name == "1")
        {
            currentImg=1;
        }
        if(name == "0")
        {
            currentImg=0;
        }
    }
    if(kind==OFX_UI_WIDGET_TOGGLE && name=="grabacion"){
        if(appStatuses["mode"]==CAPTURE)
            appStatuses["mode"]=MOSAIC;
        else appStatuses["mode"]=CAPTURE;
    }
}


//scene notifications
void fantasmas::sceneWillAppear( ofxScene * fromScreen ){  // reset our scene when we appear
    init_Escena();
};


//scene notifications
void fantasmas::sceneWillDisappear( ofxScene * toScreen ){
    
	exit_Escena();
};

void fantasmas::init_Escena(){
        gui2->enable();
        ofAddListener(gui2->newGUIEvent,this,&fantasmas::gui2Event);
        ofAddListener(finishedRecordingEvent,this, &fantasmas::onRecordingFinished);
        ofSetFrameRate(20);
}

void fantasmas::exit_Escena(){
    ofRemoveListener(finishedRecordingEvent,this, &fantasmas::onRecordingFinished);
    ofRemoveListener(gui2->newGUIEvent,this,&fantasmas::gui2Event);
    gui2->disable();
    
}
