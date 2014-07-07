#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    setupStatus();
    ofSetFrameRate(20);
    adminMode=true;
    bg.loadImage("bg2.png");
    //ofSetBackgroundAuto(false);
    #ifdef _USE_LIVE_VIDEO
        //cam.setVerbose(true);
        cam.listDevices();
    	cam.setDeviceID(1);
		cam.listDevices();
		cam.setDesiredFrameRate(30);
    
        cam.initGrabber(VIDEOWITH,VIDEOHEIGHT);
    #else
        vidPlayer.loadMovie("test1.mov");
        vidPlayer.play();
    #endif
    consoleFont.loadFont("Menlo.ttc",17);
    
    
  //  ofAddListener(blobTracker.blobAdded, this, &testApp::blobAdded);
  //  ofAddListener(blobTracker.blobDeleted, this, &testApp::blobDeleted);
    
    grayImage.allocate(VIDEOWITH,VIDEOHEIGHT);
    floatBgImg.allocate(VIDEOWITH,VIDEOHEIGHT);	//ofxShortImage used for simple dynamic background subtraction
    grayBg.allocate(VIDEOWITH,VIDEOHEIGHT);
	previousImg.allocate(VIDEOWITH,VIDEOHEIGHT);
    mimage.allocate(640, 480, OF_IMAGE_COLOR_ALPHA);
    exposureStartTime = ofGetElapsedTimeMillis();
    
    gui1 = new ofxUICanvas(186,627,336,218);
    textString = "";
    gui1->addSpacer();
    msgArea=gui1->addTextArea("textarea", textString, OFX_UI_FONT_LARGE);
    bLearnBackground=true;
    gui2 = new ofxUICanvas(1038,583, 295,285);
    
    
    gui2->addIntSlider("blob_threshold", 0, 255, &blobThreshold);
	gui2->addIntSlider("min blob size", 0, 1000, &minBlobSize);
	gui2->addIntSlider("max blob size", 0, 6000, &maxBlobSize);
	gui2->addIntSlider("amplify", 0, 100, &amplify);
    gui2->addIntSlider("smooth", 0, 10, &smooth);
    gui2->addSpacer();
    gui2->addToggle("Adaptative", false);
    gui2->addSlider("learnRate", 0	, 0.001f, &fLearnRate) ;
    ofAddListener(gui2->newGUIEvent,this,&testApp::gui2Event);
    
    individualTextureSyphonServer.setName("CameraOutput");
    onlyBlobsImageSyphonServer.setName("onlyBlobs");
    tex.allocate(VIDEOWITH,VIDEOHEIGHT, GL_RGB);
    
    myComm.setup();
    //mRecorder.setup();
    
    int a=600;
    int b=231;
    for (int i=0; i<5; i++){ //
        positions.push_back(ofPoint(550+i*36,231));
    }

    for (int i=0; i<5; i++){ //
        positions.push_back(ofPoint(1155+i*37,231));
    }
	    positions.push_back(ofPoint(278,400));
    for (int i=0; i<32; i++){ //
        positions.push_back(ofPoint(278+i*37,450));
    }
        positions.push_back(ofPoint(1425,400));
        positions.push_back(ofPoint(1425,500));
        positions.push_back(ofPoint(2000,2000)); //–apa para que el bucle aguante un par de segundos mas
        positions.push_back(ofPoint(2000,2000)); //–apa para que el bucle aguante un par de segundos mas
        positions.push_back(ofPoint(2000,2000)); //–apa para que el bucle aguante un par de segundos mas
    	    
    if(appStatuses["sendTUIO"]==true){
        frameseq=0;
        setupTUIO();
    
    };
    
    maskMaker.setup("shaders/composite_rgb",ofRectangle(0, 0, 640, 480));
    fbo1.allocate(640,480,GL_RGBA);
}

void testApp::setupStatus(){
        appStatuses["syphonEnabled"]=true;
    	appStatuses["debug"]=true;
    	appStatuses["adaptativeBackground"]=false;
	    appStatuses["blobInSquare"]=false;
	    appStatuses["sendTUIO"]=true;
        appStatuses["isCameraReady"]=false;
    
}

//--------------------------------------------------------------
void testApp::update(){
    #ifdef _USE_LIVE_VIDEO
        cam.update();
        isNewFrame=cam.isFrameNew();
    #else
        vidPlayer.update();
        isNewFrame = vidPlayer.isFrameNew();
    #endif
    
    if (isNewFrame){
        appStatuses["isCameraReady"]=true;
        #ifdef _USE_LIVE_VIDEO
                sourceColorImg.setFromPixels(cam.getPixels(), VIDEOWITH,VIDEOHEIGHT);
        #else
                sourceColorImg.setFromPixels(vidPlayer.getPixels(), VIDEOWITH,VIDEOHEIGHT);
        #endif
        sourceColorImg.updateTexture();
        grayImage = sourceColorImg;

//MEJORA FONDO HACIENDOLO ADAPTATIVO
        if(appStatuses["adaptativeBackground"]==true){
            floatBgImg.addWeighted( grayImage, fLearnRate); //we add a new bg image to the current bg image but we add it with the weight of the learn rate
			//grayBg = floatBgImg;  // not yet implemented
            //cv::Mat weightedMat = ofxCv::toCv(floatBgImg);
            //cv::Mat greyBgMat = ofxCv::toCv(grayBg); //current img
        	//greyBgMat.convertTo(greyBgMat, 255.0f/65535.0f, 0 ) ; // (bgmag  , greyimg, 255.0f/65535.0f, 0 ); esto no hace falta hacerlo pq la conversion es automatica
            //cvConvertScale( floatBgImg.getCvImage(), grayBg.getCvImage(), 255.0f/65535.0f, 0 );
            grayBg=floatBgImg; //convertimos a la imagen a grises
            grayBg.flagImageChanged();
            //cvConvertScale( floatBgImg.getCvImage(), grayBg.getCvImage(), 255.0f/65535.0f, 0 );
           // grayBg.flagImageChanged();
            blobTracker.setBg(grayBg);
        }
        //recapature the background until image/camera is fully exposed
        if((ofGetElapsedTimeMillis() - exposureStartTime) < CAMERA_EXPOSURE_TIME) bLearnBackground = true;
//CAMBIA FONDO
        if (bLearnBackground == true){
            floatBgImg = sourceColorImg;
            blobTracker.setBg(grayImage);
            bLearnBackground = false;
        }
        //blobTracker.setBg(grayBg);
        blobTracker.update(grayImage, blobThreshold,minBlobSize,maxBlobSize);
        //  grayImage.flagImageChanged();
        //  void    update( ofxCvGrayscaleImage& input, int _threshold = -1,  int minArea = 20 ,int maxArea = 40*240)/3, int nConsidered = 10, double hullPress = 20, bool bFindHoles = false, bool bUseApproximation = true);
	}    
	
//CUENTA CANTIDAD DE MOVIMIENTO
#ifdef CANTIDADMOVIMIENTO
   previousImg-=grayImage;
	previousImg.threshold(128);
    
    nonZero= previousImg.countNonZeroInRegion(0,0, previousImg.width, previousImg.height);
	movementAverage[counterAverage]=nonZero;
    counterAverage++; if(counterAverage>=50) counterAverage=0;
    nonZero=0;
    for (int j=0;j<50;j++){
        nonZero+=movementAverage[j];
    }
    nonZero=nonZero/50;
    
	//cout  << "non zero" << nonZero << "\n";
    
    previousImg=grayImage;*/
#endif
    setMaskedImageBlobs();
    
    
    //myComm.sendBlobs( blobTracker.trackedBlobs);
	blobTracker.setFiltersParam(amplify, smooth);

    

}

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


void testApp::drawCoolGui(){
    ofPushStyle();
    ofEnableAlphaBlending();
    if( ofGetFrameNum() % 3 ==0){
        rectCounter++;
    	alphaCounter=0;
    }
    if(rectCounter>positions.size()) rectCounter=0;
    ofImage rect;
    rect.loadImage("rect.png");
    ofSetColor(255,255,255,255);
    for (int i=0; i<rectCounter-1; i++){ //
        //if(i==10)	{ofRotate(90); rect.draw( positions[i].x, positions[i].y); ;ofRotate(-90);}
        //if(i>41) 	   rect.draw(	positions[i].x, positions[i].y, 54, 54);
        //else
        rect.draw( positions[i].x, positions[i].y);
        
    }
        //rect.draw(positions[i].x,positions[i].y,40,24);
    ofSetColor(255,255,255,alphaCounter);
    alphaCounter+=50; if(alphaCounter>255) alphaCounter=255;
    rect.draw(positions[rectCounter-1].x,positions[rectCounter-1].y);
    

    
    float maxMovement=5000.0;
    float relMovement=nonZero/maxMovement;
    if(relMovement>1) relMovement=1;
    relMovement=relMovement*20;
    relMovement=ceil(relMovement);
    ofSetColor(255);
    for(int j=0; j<(int)relMovement; j++){
        ofRectRounded(670, 800-j*15, 200, 10, 3);
    }
//    ofRect( 660, 850-(relMovement*200),200,(relMovement*200) )
    ofPopStyle();
    ofDisableAlphaBlending();
}

void testApp::setMaskedImageBlobs(){
    //Paso las imagenes originales a openCV
    cv::Mat fullimageCV;
    fullimageCV=ofxCv::toCv(sourceColorImg);
    cv::Mat grayimageCV;
    grayimageCV=ofxCv::toCv(grayImage);
    
    //creo la matriz donde se va a guardar la imagen de la mascara en blanco y negro. Negro donde no hay blob blanco donde lo hay
    cv::Mat contourMaskCV(640,480,CV_8U);
    cv::threshold(grayimageCV, contourMaskCV, 128, 255, cv::THRESH_BINARY);
    
//    mimage.allocate(640,480);
    ofxCv::toOf(contourMaskCV,mimage);
    mimage.update();
	//maskMaker.setMask(mimage);
    //cv::Mat contourMaskCV;
    //contourMaskCV = ofxCv::toCv(grayImage);
    //2¼ aplico la mascara
    cv::Mat maskedImage; //Matriz donde almacenar la imagen de destino
    //hago una copia de fullImageCV a maskedImg pero aplicandole la mascara previamente calculada.
    fullimageCV.copyTo(maskedImage, contourMaskCV);
	ofxCv::toOf(maskedImage, maskedImageOF);
	maskedImageOF.update();
    ofxCv::toOf(contourMaskCV, contourMaskOF);
    contourMaskOF.update();
    
}

//--------------------------------------------------------------
void testApp::draw(){
    ofPushMatrix();
    ofEnableAlphaBlending();
    ofScale(0.75,0.75);
    
    //if(ofGetFrameNum() == 3 || (ofGetFrameNum() % 50 == 0))
    // bg.draw(0,0,1920,1080);

    //cleanBackgrounds();
   // drawCoolGui();
    
#ifndef TESTMODE
    //sourceColorImg.draw(194,139,320,240); //img original
	

    fbo1.begin();
	ofClear(0, 0, 0, 0);    //ofSetColor(255,255,255,255);
    //ofBackground(255,255,255);
	maskMaker.drawMask(sourceColorImg.getTextureReference(), maskedImageOF.getTextureReference(), 0,0,255,640,480 );
    fbo1.end();
    fbo1.draw(50,50);
    //maskMaker.drawScrollingMask(sourceColorImg.getTextureReference(), contourMaskOF.getTextureReference(), 0, 255);
    
    ofSetColor(0, 255, 123, 100);
    ofSetColor(255,255,255,255);
    contourMaskOF.draw(800,139,320,240); //masked image
    

    ofSetColor(0);
    ofRect(1391,139,320,240);
        ofSetColor(255,255,255,255);
	if(adminMode){
    	blobTracker.draw(1391,139,320,240);//img + blobs}
    }else{
    	maskedImageOF.draw(1391,139,320,240);
    }
	maskedImageOF.draw(1391,604,320,240); //mask + blobs
 #endif
    if(appStatuses["syphonEnabled"]==true && appStatuses["isCameraReady"]){
        individualTextureSyphonServer.publishTexture(&sourceColorImg.getTextureReference());
        onlyBlobsImageSyphonServer.publishTexture(&fbo1.getTextureReference());
    }
    if(appStatuses["debug"]) showDebug();
    if(appStatuses["sendTUIO"]==true){
        sendTUIO(&(blobTracker.trackedBlobs));
    };
    
//resized Image, READY 4 RECORDING
    //blobImgOF_min=maskedImageOF;
    //blobImgOF_min.resize(320, 240);
    
 /**   if(moveandrecord.state==moveandrecord.blobInSquare){
        blobImgOF_min=maskedImageOF;
        blobImgOF_min.resize(320, 240);
        blobImgOF_min.update();
    }**/
  //  mRecorder.update();
    ofPopMatrix();
// END RECORDING-READY BLOCK
    
 //   moveandrecord.draw();
}

void testApp::showDebug(){
    ofDrawBitmapString("Framerate " + ofToString(ofGetFrameRate()), 20, 20);
        ofDrawBitmapString("NonZero " + ofToString(nonZero), 20, 40);
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

void testApp::cleanBackgrounds(){
    ofPushStyle();
    ofFill();
    ofSetColor(0);
    ofRect(194, 139, 320, 240);
    ofRect(800, 139, 320, 240);
    ofRect(1391, 139, 320, 240);
    ofRect(634, 586, 375, 280);
    ofRect(166, 586, 375, 280);
    ofRect(1363, 586, 375, 280);
    //cuadraditos que se mueven
    ofRect(557,175,199,140);
    ofRect(1159,202,187,120);
    ofRect(261,414,1202,88);
    ofPopStyle();
    
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
        blobImgOF_min.resize(320,240);
       // mRecorder.start(3000, 25, &blobImgOF_min);
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
