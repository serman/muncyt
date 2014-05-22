#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofSetFrameRate(25);
    adminMode=true;
    bg.loadImage("bg2.png");
    //ofSetBackgroundAuto(false);
    #ifdef _USE_LIVE_VIDEO
        //cam.setVerbose(true);
        cam.initGrabber(VIDEOWITH,VIDEOHEIGHT);
    #else
        vidPlayer.loadMovie("test1.mov");
        vidPlayer.play();
    #endif
    consoleFont.loadFont("Menlo.ttc",17);
    
    ofAddListener(blobTracker.blobAdded, this, &testApp::blobAdded);
    ofAddListener(blobTracker.blobDeleted, this, &testApp::blobDeleted);
    
    grayImage.allocate(VIDEOWITH,VIDEOHEIGHT);
    floatBgImg.allocate(VIDEOWITH,VIDEOHEIGHT);	//ofxShortImage used for simple dynamic background subtraction
    grayBg.allocate(VIDEOWITH,VIDEOHEIGHT);

    exposureStartTime = ofGetElapsedTimeMillis();
	fbo.allocate(VIDEOWITH,VIDEOHEIGHT, GL_RGB);
    
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
    gui2->addSlider("learnRate", 0	, 0.1, &fLearnRate) ;
    
    individualTextureSyphonServer.setName("CameraOutput");
    tex.allocate(VIDEOWITH,VIDEOHEIGHT, GL_RGB);
    
    myComm.setup();
    mRecorder.setup();
    
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
            positions.push_back(ofPoint(2000,2000)); //�apa para que el bucle aguante un par de segundos mas
		    positions.push_back(ofPoint(2000,2000)); //�apa para que el bucle aguante un par de segundos mas
        	positions.push_back(ofPoint(2000,2000)); //�apa para que el bucle aguante un par de segundos mas
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
        #ifdef _USE_LIVE_VIDEO
                sourceColorImg.setFromPixels(cam.getPixels(), VIDEOWITH,VIDEOHEIGHT);
        #else
                sourceColorImg.setFromPixels(vidPlayer.getPixels(), VIDEOWITH,VIDEOHEIGHT);
        #endif
        
        
        
        //backgroundAddon.update(sourceColorImg);//img coming from camera = colorImg
        grayImage = sourceColorImg;
        
        if(adaptativeBackground){
            floatBgImg.addWeighted( grayImage, fLearnRate); //we add a new bg image to the current bg image but we add it with the weight of the learn rate
			//grayBg = floatBgImg;  // not yet implemented
            //cv::Mat weightedMat = ofxCv::toCv(floatBgImg);
            //cv::Mat greyBgMat = ofxCv::toCv(grayBg); //current img
        	//greyBgMat.convertTo(greyBgMat, 255.0f/65535.0f, 0 ) ; // (bgmag  , greyimg, 255.0f/65535.0f, 0 ); esto no hace falta hacerlo pq la conversion es automatica
            //cvConvertScale( floatBgImg.getCvImage(), grayBg.getCvImage(), 255.0f/65535.0f, 0 );
            grayBg=floatBgImg;
            grayBg.flagImageChanged();
        }
        //recapature the background until image/camera is fully exposed
        if((ofGetElapsedTimeMillis() - exposureStartTime) < CAMERA_EXPOSURE_TIME) bLearnBackground = true;
        
        if (bLearnBackground == true){
            floatBgImg = sourceColorImg;
			//cvConvertScale( floatBgImg.getCvImage(), grayBg.getCvImage(), 255.0f/65535.0f, 0 ); //conversion from float to gray
			//grayBg.flagImageChanged();
            blobTracker.setBg(grayBg); // poner el BG a 0
            bLearnBackground = false;
        }
        //blobTracker.setBg(grayBg);
        blobTracker.update(grayImage, blobThreshold,minBlobSize,maxBlobSize);
        //  grayImage.flagImageChanged();
        //  void    update( ofxCvGrayscaleImage& input, int _threshold = -1,  int minArea = 20 ,int maxArea = 40*240)/3, int nConsidered = 10, double hullPress = 20, bool bFindHoles = false, bool bUseApproximation = true);
	}    

    myComm.sendBlobs( blobTracker.trackedBlobs);

}

void testApp::drawCoolGui(){
    ofEnableAlphaBlending();
    if( ofGetFrameNum() % 5 ==0){
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
    alphaCounter+=40; if(alphaCounter>255) alphaCounter=255;
    rect.draw(positions[rectCounter-1].x,positions[rectCounter-1].y);
    
    ofPushStyle();
    ofPopStyle();
    ofDisableAlphaBlending();
    
}

//--------------------------------------------------------------
void testApp::draw(){
    // intentando enviar solo blobs por syphon
    cv::Mat fullimageCV;
    fullimageCV=ofxCv::toCv(sourceColorImg);
    cv::Mat grayimageCV;
    grayimageCV=ofxCv::toCv(grayImage);

    //blobImgOF.allocate(640, 480, OF_IMAGE_COLOR);
    
    //ofImage maskImage;
    //maskImage.allocate(640,480,OF_I)
    //cout << "tamanio"<< blobTracker.size()<<"\n";
    //grayImage
    //convierto imagen en escala de grises a imagen binaria

    cv::Mat contourMaskCV(640,480,CV_8U);
    cv::threshold(grayimageCV, contourMaskCV, 128, 255, cv::THRESH_BINARY);
    //cv::Mat contourMaskCV;
    //contourMaskCV = ofxCv::toCv(grayImage);
    //2� aplico la mascara
    
    cv::Mat maskedImage;
    fullimageCV.copyTo(maskedImage, contourMaskCV);
	ofxCv::toOf(maskedImage, blobImgOF);
    /**
     fbo.begin();
     ofRect(0,0,640,480);
     for (int i=0; i< blobTracker.size(); i++){
        cv::Rect blob1Roi = ofxCv::toCv(blobTracker.trackedBlobs[i].angleBoundingRect);  //roi de la zona del blob
        cout << "entro " << i << " --- widt"<< blobTracker.trackedBlobs[i].angleBoundingRect.getWidth() << "\n";
        fullimageMat=ofxCv::toCv(sourceColorImg);
        cv::Mat mat1( fullimageMat,blob1Roi);
        ofxCv::toOf(mat1, blobImg);
        blobImg.draw(blobTracker[i].angleBoundingRect.x,blobTracker[i].angleBoundingRect.y);
    } 
    fbo.end();
    //fin intentando enviar solo blobs por syphon
    **/
    
    
    //if(ofGetFrameNum() == 3 || (ofGetFrameNum() % 50 == 0)){
     //bg.draw(0,0,1920,1080);
   // }
    //cleanBackgrounds();
       // drawCoolGui();
    sourceColorImg.draw(194,139,320,240);
    //backgroundAddon.draw(800,139);
    blobImgOF.update();
    blobImgOF.draw(800,139,320,240); //REAL IMAGE WITH ONLY THE BLOBS
    blobImgOF_min=blobImgOF;
    blobImgOF_min.resize(320, 240);
    
    //backgroundAddon.backgroundCodeBookConnectedComponents.draw(800,139,320,240);
    // or, instead we can draw each blob individually,
	// this is how to get access to them:
    blobTracker.draw(1391,139,320,240);
    //consoleFont.drawString("prueba de cadena", 189, 620);
    #ifdef _USE_LIVE_VIDEO
        tex.loadData(cam.getPixels(), VIDEOWITH,VIDEOHEIGHT, GL_RGB);
    #else
        tex.loadData(vidPlayer.getPixels(), VIDEOWITH,VIDEOHEIGHT, GL_RGB);
	    //tex=fbo.getTextureReference();
    #endif
    tex.loadData(blobImgOF.getPixels(),VIDEOWITH,VIDEOHEIGHT, GL_RGB);
    //fbo.draw(0,0);
	individualTextureSyphonServer.publishTexture(&tex);
    mRecorder.update();
	
    if(debug) showDebug();
}

void testApp::showDebug(){
    ofDrawBitmapString("Framerate " + ofToString(ofGetFrameRate()), 20, 20);

    
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
    }
    else if(key==' '){
        bLearnBackground=true;
    }
    else if(key=='r'){
        blobImgOF_min.resize(320,240);
        mRecorder.start(3000, 25, &blobImgOF_min);
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
