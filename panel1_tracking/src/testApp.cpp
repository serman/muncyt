#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    setupStatus();
    loadCameraOrVideo();
    if(ofToString(getenv("USER"))=="instalaciones"){
        ofSetFrameRate(40);
    }
    else{
        ofSetFrameRate(19);
    }
    
    adminMode=false;
    bg.loadImage("bg.jpg");
    ofSetBackgroundAuto(false);
    if( USE_LIVE_VIDEO){
        camCeil.listDevices();
    	camCeil.setDeviceID(1);

		camCeil.setDesiredFrameRate(30);
        camCeil.initGrabber(VIDEOWITH,VIDEOHEIGHT);
    //cam
        camFront.setDeviceID(2);
        camFront.setDesiredFrameRate(30);
        camFront.initGrabber(VIDEOWITH,VIDEOHEIGHT);
        currentCam=&camCeil;
    }else{
        vidPlayerCeil.loadMovie("muncyt2-menoszoom.mov");
        vidPlayerCeil.play();
    
        vidPlayerFront.loadMovie("muncyt-test-intermedio.mov");
        vidPlayerFront.play();
        currentVid=&vidPlayerCeil;
    }
    
    consoleFont.loadFont("Menlo.ttc",17);
    sourceColorImg.allocate(VIDEOWITH,VIDEOHEIGHT);
    sourceGrayImage.allocate(VIDEOWITH,VIDEOHEIGHT);
    floatBgImgCameraCeil.allocate(VIDEOWITH,VIDEOHEIGHT);	//ofxShortImage used for simple dynamic background subtraction
    floatBgImgCameraFront.allocate(VIDEOWITH,VIDEOHEIGHT);
    floatBgImg=&floatBgImgCameraCeil;
    
    grayBg.allocate(VIDEOWITH,VIDEOHEIGHT);
   // grayBgCameraFront.allocate(VIDEOWITH,VIDEOHEIGHT);
   // grayBg=&grayBgCameraCeil;
    setFrontCamera();
    exposureStartTime = ofGetElapsedTimeMillis();
    bCaptureBackground=true;
    firstTimeFrontCamera=true;
    firstTimeCeilCamera=true;
    
    setupGui();
    individualTextureSyphonServer.setName("CameraOutput");
    onlyBlobsImageSyphonServer.setName("onlyBlobs");
    tex.allocate(VIDEOWITH,VIDEOHEIGHT, GL_RGB);
    
    myComm=new cheapComm();
    myComm->setup();
    //mRecorder.setup();
    
    
    if(appStatuses["sendTUIO"]==true){
        frameseq=0;
        setupTUIO();
    }
    
    maskMaker.setup("shaders/composite_rgb",ofRectangle(0, 0, VIDEOWITH,VIDEOHEIGHT));
    fbo1.allocate(VIDEOWITH,VIDEOHEIGHT,GL_RGBA);
    videoMirror = new unsigned char[VIDEOWITH*VIDEOHEIGHT*3];
}



//--------------------------------------------------------------
void testApp::update(){
        myComm->oscRcvUpdate();
    
    if( USE_LIVE_VIDEO){
        currentCam->update();
        isNewFrame=currentCam->isFrameNew();
    }else{
        currentVid->update();
        isNewFrame = currentVid->isFrameNew();
    }
    
#ifndef TESTMODE
    if (isNewFrame){
        appStatuses["isCameraReady"]=true;
        unsigned char * pixels ;
        if( USE_LIVE_VIDEO){
               pixels = currentCam->getPixels();
            //invertir imagen
         }else{
               // sourceColorImg.setFromPixels(currentVid->getPixels(), VIDEOWITH,VIDEOHEIGHT);
              pixels = currentVid->getPixels();
        } //invertir imagen
   /*     for (int i = 0; i < VIDEOHEIGHT; i++) {
            for (int j = 0; j < VIDEOWITH*3; j+=3) {
                // pixel number
                int pix1 = (i*VIDEOWITH*3) + j;
                int pix2 = (i*VIDEOWITH*3) + (j+1);
                int pix3 = (i*VIDEOWITH*3) + (j+2);
                // mirror pixel number
                int mir1 = (i*VIDEOWITH*3)+1 * (VIDEOWITH*3 - j-3);
                int mir2 = (i*VIDEOWITH*3)+1 * (VIDEOWITH*3 - j-2);
                int mir3 = (i*VIDEOWITH*3)+1 * (VIDEOWITH*3 - j-1);
                // swap pixels
                videoMirror[pix1] = pixels[mir1];
                videoMirror[pix2] = pixels[mir2];
                videoMirror[pix3] = pixels[mir3];
            }*/
        
        sourceColorImg.setFromPixels(pixels, VIDEOWITH,VIDEOHEIGHT);
        sourceColorImg.updateTexture();
        sourceColorImg.mirror(false, true);
        sourceGrayImage = sourceColorImg;
        
//MEJORA FONDO HACIENDOLO ADAPTATIVO
        if(appStatuses["adaptativeBackground"]==true){
            fLearnRateDividido=fLearnRate/10000;
            floatBgImg->addWeighted( sourceGrayImage, fLearnRateDividido); //we add a new bg image to the current bg image but we add it with the weight of the learn rate
            grayBg= *floatBgImg; //convertimos a la imagen a grises
            grayBg.flagImageChanged();
            blobTracker.setBg(grayBg);
        }
        //recapature the background until image/camera is fully exposed
        if((ofGetElapsedTimeMillis() - exposureStartTime) < CAMERA_EXPOSURE_TIME) bCaptureBackground = true;

        //Capura fondo si se ha indicado
        if (bCaptureBackground == true){
            *floatBgImg = sourceColorImg;
            blobTracker.setBg(sourceGrayImage);
            bCaptureBackground = false;
            
        }
        /* Esta funcion devuelve sourceGrayImage modificada. Dentro se le aplica el Bg substraction */
        blobTracker.update(sourceGrayImage, blobThreshold[configIndex],minBlobSize[configIndex],maxBlobSize[configIndex]);
        

        setMaskedImageBlobs();
        //myComm.sendBlobs( blobTracker.trackedBlobs);
        blobTracker.setFiltersParam(amplify[configIndex], smooth[configIndex]);


	}    
#endif
//CUENTA CANTIDAD DE MOVIMIENTO

}
//camara 1
void testApp::setFrontCamera(){
        if( USE_LIVE_VIDEO){
            currentCam=&camFront;
        }else{
                    currentVid=&vidPlayerFront;
        }
    floatBgImg=&floatBgImgCameraFront;
    //grayBg=&grayBgCameraFront;
    if(firstTimeFrontCamera){
        bCaptureBackground=true;
        firstTimeFrontCamera=false;
    }
    configIndex=FRONT_INDEX;

}

//camara0
void testApp::setCeilCamera(){
        if( USE_LIVE_VIDEO){
            currentCam=&camCeil;
        }else{
           currentVid=&vidPlayerCeil;
        }
    floatBgImg=&floatBgImgCameraCeil;
    //grayBg=&grayBgCameraCeil;
    if(firstTimeCeilCamera){
        bCaptureBackground=true;
        firstTimeCeilCamera=false;
    }
    configIndex=CEIL_INDEX;
    
}
/* esta funcion toma el resultado del blob tracking y lo usa como m‡scara sobre la imagen original. De esta fomra queda la imagen original, pero solo la zona donde hay blob */
void testApp::setMaskedImageBlobs(){
    //Paso las imagenes originales a openCV
    cv::Mat fullimageCV;
    fullimageCV=ofxCv::toCv(sourceColorImg);
    cv::Mat grayimageCV;
    grayimageCV=ofxCv::toCv(sourceGrayImage); //imagen en escala de grises con los blobs
    
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
    //ofScale(0.8, 0.8);
    if(ofGetFrameNum()==19 || ofGetFrameNum()%150==0)
        bg.draw(-1,-1,1281,1025);
    ofPushMatrix();
    cleanBackgrounds();
    
#ifndef TESTMODE
    sourceColorImg.draw(59,169,480,360); //img original
 #endif
// fbo1 contiene  las imagenes con la mascara ya aplicada.
    fbo1.begin();
	ofClear(0, 0, 0, 0);    //ofSetColor(255,255,255,255);
	maskMaker.drawMask(sourceColorImg.getTextureReference(), contourMaskOF.getTextureReference(), 0,0,255,VIDEOWITH,VIDEOHEIGHT);
    fbo1.end();
    //maskMaker.drawScrollingMask(sourceColorImg.getTextureReference(), contourMaskOF.getTextureReference(), 0, 255);
 #ifndef TESTMODE
    ofSetColor(0, 255, 123, 100);
    ofSetColor(255,255,255,255);
    contourMaskOF.draw(766,275,480,360); //masked image = sourceGrayImage en este momento, aunque en otro formato de pixels (8 bits)
#endif


        ofSetColor(255,255,255,255);
    	blobTracker.draw(57,602,480,360);//img + blobs}
   // 	maskedImageOF.draw(1391,139,320,240);

    if(appStatuses["syphonEnabled"]==true && appStatuses["isCameraReady"]){
        individualTextureSyphonServer.publishTexture(&sourceColorImg.getTextureReference());
        onlyBlobsImageSyphonServer.publishTexture(&fbo1.getTextureReference());
    }
    if( adminMode) showDebug();
    if(appStatuses["sendTUIO"]==true){
        sendTUIO(&(blobTracker.trackedBlobs));
    };

   // sourceGrayImage.draw(0,0);
    ofPopMatrix();
  //  maskedImageOF.draw(0,0,320,240);
}

void testApp::showDebug(){
    ofPushStyle();
    ofFill();
    ofSetColor(0);
    ofRect(20,0,400,80);
    ofSetColor(200, 100, 0);
    ofDrawBitmapString("Framerate " + ofToString(ofGetFrameRate()) + "/"+ofToString(ofGetTargetFrameRate()), 20, 20);
        ofDrawBitmapString("NonZero " + ofToString(nonZero), 20, 40);
    ofDrawBitmapString("learnrate"+ ofToString(fLearnRateDividido),200,20 );

    if(configIndex==FRONT_INDEX)
       ofDrawBitmapString("FRONT CAMERA", 20, 60);
     else  ofDrawBitmapString("CEIL CAMERA", 20, 60);

     ofPopStyle();
}

void testApp::setupStatus(){
    //cout << "staupstatus" <<endl;
    appStatuses["syphonEnabled"]=true;
    appStatuses["debug"]=false;
    appStatuses["adaptativeBackground"]=true;
    appStatuses["blobInSquare"]=false;
    appStatuses["sendTUIO"]=true;
    appStatuses["isCameraReady"]=false;
    
}



void testApp::cleanBackgrounds(){
    ofPushStyle();
    ofFill();
    ofSetColor(0);
    ofRect(59, 169, 480, 360);
    ofRect(766, 275, 480, 360);
    ofRect(57, 602, 480, 360);
    
    ofPopStyle();
    
}

