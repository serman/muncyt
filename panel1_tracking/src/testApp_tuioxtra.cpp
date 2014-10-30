//
//  testApp_tuioxtra.cpp
//  panel1_tracking
//
//  Created by Sergio Galan on 8/25/14.
//
//

#include "testApp.h"

void testApp::setupTUIO(){
    string localhost = "127.0.0.1";
    int TUIOPort= 3333;
    TUIOSocket.setup(localhost, TUIOPort);
}

void testApp::sendTUIO(std::vector<ofxBlob> * objectBlobs){
    bool bHeightWidth=true;
    frameseq++;
    ofxOscBundle b_obj;
    ofxOscMessage alive_obj;
    // Sends alive message - saying 'Hey, there's no alive blobs'
    alive_obj.setAddress("/tuio/2Dcur");
    alive_obj.addStringArg("alive");
    
    // Send fseq message
    ofxOscMessage fseq_obj;
    fseq_obj.setAddress( "/tuio/2Dcur" );
    fseq_obj.addStringArg( "fseq" );
    fseq_obj.addIntArg(frameseq);
    
    if(objectBlobs->size() == 0)
    {
        b_obj.addMessage( alive_obj );		// add message to bundle
        b_obj.addMessage( fseq_obj );		// add message to bundle
        TUIOSocket.sendBundle( b_obj ); // send bundle
    }
    else
    {
        //map<int, ofxBlob>::iterator blob_obj;
        //std::vector<ofxBlob>::iterator blob_obj=objectBlobs->begin();
        for( std::vector<ofxBlob>::iterator blob_obj = objectBlobs->begin(); blob_obj != objectBlobs->end(); blob_obj++)
        {
            // omit point (0,0) since this means that we are outside of the range
            if(blob_obj->centroid.x == 0 && blob_obj->centroid.y == 0)
                continue;
            //Set Message
            ofxOscMessage set_obj;
            set_obj.setAddress( "/tuio/2Dcur" );
            set_obj.addStringArg("set");
            set_obj.addIntArg(blob_obj->id);				// id
            set_obj.addFloatArg(blob_obj->boundingRect.getX());	// x
            set_obj.addFloatArg(blob_obj->boundingRect.getY());	// y
            set_obj.addFloatArg(blob_obj->D.x);			// dX
            set_obj.addFloatArg(blob_obj->D.y);			// dY
            set_obj.addFloatArg(blob_obj->maccel);		// m
            if(bHeightWidth)
            {
                set_obj.addFloatArg(blob_obj->boundingRect.getWidth());	// wd
                set_obj.addFloatArg(blob_obj->boundingRect.getHeight());	// ht
               // if (blob_obj->id%3==0)
               // cout << blob_obj->id << " : " << blob_obj->boundingRect.getHeight()*VIDEOHEIGHT  <<endl;
            }
            b_obj.addMessage( set_obj );							// add message to bundle
            alive_obj.addIntArg(blob_obj->id);				// add blob to list of ALL active IDs
        }
        b_obj.addMessage( alive_obj );		//add message to bundle
        b_obj.addMessage( fseq_obj );		//add message to bundle
        TUIOSocket.sendBundle( b_obj ); //send bundle
    }
}


void testApp::blobAdded(ofxBlob &_blob){
    ofLog(OF_LOG_NOTICE, "Blob ID " + ofToString(_blob.id) + " added" );
    textString+= "Blob ID " + ofToString(_blob.id) + " added\n";
    //msgArea->setTextString(textString);
}


void testApp::blobDeleted(ofxBlob &_blob){
    ofLog(OF_LOG_NOTICE, "Blob ID " + ofToString(_blob.id) + " deleted" );
    textString+="Blob ID " + ofToString(_blob.id) + " deleted\n";
    //msgArea->setTextString(textString);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	if(key=='g'){
        adminMode=!adminMode;
        if(adminMode)
            guiTabBar->enable();
        else
            guiTabBar->disable();
    }
    else if(key==' '){
        bCaptureBackground=true;
    }
    else if(key=='r'){
        
    }
    else if(key=='s'){
      //  gui0->saveSettings("gui_settingsCamera0.xml");
      //  gui1->saveSettings("gui_settingsCamera1.xml");
        guiTabBar->saveSettings("./","camera_settings");
    }
}
void testApp::setupGui(){
  //CEIL CAMARA
    guiTabBar = new ofxUITabBar(50,100, 295,285);
    gui0 = new ofxUICanvas(50,100, 295,285);
    gui0->setName("Camera0");
    gui0->addIntSlider("blob_threshold0", 0, 255, &blobThreshold[0]);
	gui0->addIntSlider("min blob size0", 0, 1000, &minBlobSize[0]);
	gui0->addIntSlider("max blob size0", 0, 20000, &maxBlobSize[0]);
	gui0->addIntSlider("amplify0", 0, 100, &amplify[0]);
    gui0->addIntSlider("smooth0", 0, 10, &smooth[0]);

    guiTabBar->addCanvas(gui0);
    
 //FRONT CAMARA
    gui1 = new ofxUICanvas(50,100, 295,285);
    gui1->setName("Camera1" );
    gui1->addIntSlider("blob_threshold", 0, 255, &blobThreshold[1]);
	gui1->addIntSlider("min blob size", 0, 1000, &minBlobSize[1]);
	gui1->addIntSlider("max blob size", 0, 20000, &maxBlobSize[1]);
	gui1->addIntSlider("amplify", 0, 100, &amplify[1]);
    gui1->addIntSlider("smooth", 0, 10, &smooth[1]);
    
    gui1->addSpacer();
    gui1->addToggle("Adaptative", false);
    gui1->addSlider("learnRate", 0	, 0.001f, &fLearnRate) ;
    ofAddListener(gui1->newGUIEvent,this,&testApp::gui2Event);
   // gui1->loadSettings("gui_settingsCamera1.xml");
    guiTabBar->addCanvas(gui1);
    guiTabBar->loadSettings("./","camera_settings");
    guiTabBar->disable();
}

void testApp::loadCameraOrVideo(){
    ofLogVerbose()<<"entrando en live video"  <<endl;
    ofFile fileRead(ofToString(getenv("HOME"))+"/cameraOrVideo.txt");
    
    int id;
    fileRead >> id;
    USE_LIVE_VIDEO=id;
    ofLogVerbose()<<"using live video" << id<<endl;
    if(id==32){
        ofLog() << "Falta archivo cameraOrVideo.txt" << id;
        USE_LIVE_VIDEO=0;
    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){
    
}

void testApp::gui2Event(ofxUIEventArgs &e)
{
    cout << "event";
	string name = e.widget->getName();
	int kind = e.widget->getKind();
    
    if(name == "Adaptative")
    {
        if(appStatuses["adaptativeBackground"]) appStatuses["adaptativeBackground"]=false;
        else appStatuses["adaptativeBackground"]=true;
    }
}

