#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    setupStatus();
    ofSetFrameRate(30);
    adminMode=true;
    bg.loadImage("bg.jpg");
    ofSetBackgroundAuto(false);
    #ifdef _USE_LIVE_VIDEO
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
    
    grayImage.allocate(VIDEOWITH,VIDEOHEIGHT);
    floatBgImg.allocate(VIDEOWITH,VIDEOHEIGHT);	//ofxShortImage used for simple dynamic background subtraction
    grayBg.allocate(VIDEOWITH,VIDEOHEIGHT);
	previousImg.allocate(VIDEOWITH,VIDEOHEIGHT);
    exposureStartTime = ofGetElapsedTimeMillis();
       bLearnBackground=true;
    setupGui();
    individualTextureSyphonServer.setName("CameraOutput");
    onlyBlobsImageSyphonServer.setName("onlyBlobs");
    tex.allocate(VIDEOWITH,VIDEOHEIGHT, GL_RGB);
    
    myComm.setup();
    //mRecorder.setup();
    
    
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
#ifndef TESTMODE
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
            grayBg=floatBgImg; //convertimos a la imagen a grises
            grayBg.flagImageChanged();
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
        
        #ifndef TESTMODE
                setMaskedImageBlobs();
                //myComm.sendBlobs( blobTracker.trackedBlobs);
                blobTracker.setFiltersParam(amplify, smooth);
        #endif

	}    
#endif
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
    

    

}


void testApp::setMaskedImageBlobs(){
    //Paso las imagenes originales a openCV
    cv::Mat fullimageCV;
    fullimageCV=ofxCv::toCv(sourceColorImg);
    cv::Mat grayimageCV;
    grayimageCV=ofxCv::toCv(grayImage); //imagen en escala de grises con los blobs
    
    //creo la matriz donde se va a guardar la imagen de la mascara en blanco y negro. Negro donde no hay blob blanco donde lo hay
    cv::Mat contourMaskCV;//(640,480,CV_8U);
    cv::threshold(grayimageCV, contourMaskCV,128, 255, cv::THRESH_BINARY); // esto s—lo lo tengo que hacer para convertir la imagen a tipo 8 bits
    
    ofxCv::toOf(contourMaskCV, contourMaskOF);
    contourMaskOF.update();

    //2¼ aplico la mascara
    cv::Mat maskedImage; //Matriz donde almacenar la imagen de destino
    //hago una copia de fullImageCV a maskedImg pero aplicandole la mascara previamente calculada.
    fullimageCV.copyTo(maskedImage, contourMaskCV);
    
	ofxCv::toOf(maskedImage, maskedImageOF);
	maskedImageOF.update();

    
}

//--------------------------------------------------------------
void testApp::draw(){
    if(ofGetFrameNum()==20 || ofGetFrameNum()%150==0)
        bg.draw(0,0,1280,1024);
    ofPushMatrix();
    cleanBackgrounds();
    
#ifndef TESTMODE
    sourceColorImg.draw(59,169,480,360); //img original
 #endif
// fbo1 contiene  las imagenes con la mascara ya aplicada.
    fbo1.begin();
	ofClear(0, 0, 0, 0);    //ofSetColor(255,255,255,255);
	maskMaker.drawMask(sourceColorImg.getTextureReference(), contourMaskOF.getTextureReference(), 0,0,255,640,480 );
    fbo1.end();
    //maskMaker.drawScrollingMask(sourceColorImg.getTextureReference(), contourMaskOF.getTextureReference(), 0, 255);
 #ifndef TESTMODE
    ofSetColor(0, 255, 123, 100);
    ofSetColor(255,255,255,255);
    contourMaskOF.draw(766,275,480,360); //masked image
#endif

    ofSetColor(0);
    ofRect(1391,139,320,240);
        ofSetColor(255,255,255,255);
    	blobTracker.draw(57,602,480,360);//img + blobs}
   // 	maskedImageOF.draw(1391,139,320,240);

    if(appStatuses["syphonEnabled"]==true && appStatuses["isCameraReady"]){
        individualTextureSyphonServer.publishTexture(&sourceColorImg.getTextureReference());
        onlyBlobsImageSyphonServer.publishTexture(&fbo1.getTextureReference());
    }
    if(appStatuses["debug"] && adminMode) showDebug();
    if(appStatuses["sendTUIO"]==true){
        sendTUIO(&(blobTracker.trackedBlobs));
    };

    ofPopMatrix();
  //  maskedImageOF.draw(0,0,320,240);
}

void testApp::showDebug(){
    ofDrawBitmapString("Framerate " + ofToString(ofGetFrameRate()) + "/"+ofToString(ofGetTargetFrameRate()), 20, 20);
        ofDrawBitmapString("NonZero " + ofToString(nonZero), 20, 40);
}





void testApp::cleanBackgrounds(){
    ofPushStyle();
    ofFill();
    ofSetColor(0);
    ofRect(59, 169, 480, 360);
    ofRect(766, 275, 480, 360);
    ofRect(57, 602, 480, 360);
    ofRect(20,0,300,60);
    ofPopStyle();
    
}

