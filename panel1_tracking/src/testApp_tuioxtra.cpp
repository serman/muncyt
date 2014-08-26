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
                //cout << blob_obj->boundingRect.getWidth();
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
	if(key=='a'){
        adminMode=!adminMode;
        if(adminMode)
            gui2->enable();
        else
            gui2->disable();
    }
    else if(key==' '){
        bLearnBackground=true;
    }
    else if(key=='r'){
        
    }
}
void testApp::setupGui(){
    gui2 = new ofxUICanvas(790,280, 295,285);
    
    
    gui2->addIntSlider("blob_threshold", 0, 255, &blobThreshold);
	gui2->addIntSlider("min blob size", 0, 1000, &minBlobSize);
	gui2->addIntSlider("max blob size", 0, 6000, &maxBlobSize);
	gui2->addIntSlider("amplify", 0, 100, &amplify);
    gui2->addIntSlider("smooth", 0, 10, &smooth);
    gui2->addSpacer();
    gui2->addToggle("Adaptative", false);
    gui2->addSlider("learnRate", 0	, 0.001f, &fLearnRate) ;
    ofAddListener(gui2->newGUIEvent,this,&testApp::gui2Event);

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

