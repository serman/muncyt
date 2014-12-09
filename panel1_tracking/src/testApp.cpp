#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    setupStatus();
    loadCameraOrVideo();
    if(ofToString(getenv("USER"))=="instalaciones"){
        ofSetFrameRate(25);
    }
    else{
        ofSetFrameRate(25);
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
        vidPlayerCeil.loadMovie("masgentearriba.mov");
        vidPlayerCeil.play();
    
        vidPlayerFront.loadMovie("archive/maniana_frontal.mov");
        vidPlayerFront.play();
        currentVid=&vidPlayerCeil;
    }
    
    consoleFont.loadFont("Menlo.ttc",17);
    sourceColorImg.allocate(VIDEOWITH,VIDEOHEIGHT);
    sourceGrayImage.allocate(VIDEOWITH,VIDEOHEIGHT);
    sourceColorImgMirror.allocate(VIDEOWITH,VIDEOHEIGHT);
    
    grayBg.allocate(VIDEOWITH,VIDEOHEIGHT);
   // grayBgCameraFront.allocate(VIDEOWITH,VIDEOHEIGHT);
   // grayBg=&grayBgCameraCeil;
    setCeilCamera();
    exposureStartTime = ofGetElapsedTimeMillis();
    bCaptureBackground=true;
    firstTimeFrontCamera=true;
    firstTimeCeilCamera=true;
    
    setupGui();
    appStatuses["forceCamera"]=false; //NO IMPORTA QUE HAYAS GRABADO EL SETTING DE FORZAR CAMARA. POR DEFECTO CARGA LA CAMARA AUTOMATICA QUE DEMANDA CADA ESCENA PARA EVITAR FALLOS EN PRODUCCION
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
    blobTrackerFront.setup(ofxBlobTracker::OFXCV);
    blobTrackerCeil.setup(ofxBlobTracker::OPENCV);
}



//--------------------------------------------------------------
void testApp::update(){
        myComm->oscRcvUpdate();
    
    if( USE_LIVE_VIDEO){
        //currentCam->update();
        camCeil.update();
        camFront.update();
        isNewFrame=currentCam->isFrameNew();
    }else{
        //currentVid->update();
        vidPlayerCeil.update();
        vidPlayerFront.update();
        
        isNewFrame = currentVid->isFrameNew();
    }
    
    
#ifndef TESTMODE
    if (isNewFrame){
        appStatuses["isCameraReady"]=true;
        unsigned char * pixels ;
        
        
//camara del techo que no se invierte
        if( USE_LIVE_VIDEO){
               pixels = camCeil.getPixels();
            //invertir imagen
         }else{
               // sourceColorImg.setFromPixels(currentVid->getPixels(), VIDEOWITH,VIDEOHEIGHT);
              pixels = vidPlayerCeil.getPixels();
        }
        sourceColorImg.setFromPixels(pixels, VIDEOWITH,VIDEOHEIGHT);
        sourceColorImg.updateTexture();
        
     //camara frontal que  se invierte
        if( USE_LIVE_VIDEO){
            pixels = camFront.getPixels();
            //invertir imagen
        }else{
            // sourceColorImg.setFromPixels(currentVid->getPixels(), VIDEOWITH,VIDEOHEIGHT);
            pixels = vidPlayerFront.getPixels();
        } //invertir imagen
        

        sourceColorImgMirror.setFromPixels(pixels, VIDEOWITH,VIDEOHEIGHT);
        sourceColorImgMirror.mirror(false, true);
        
        //recapature the background until image/camera is fully exposed
        if((ofGetElapsedTimeMillis() - exposureStartTime) < CAMERA_EXPOSURE_TIME) {
            blobTrackerCeil.resetBG();
            blobTrackerFront.resetBG();
        }
        
        //Capura fondo si se ha indicado
        if (bCaptureBackground == true){
            bCaptureBackground = false;
            currentBlobTracker->resetBG();
        }
        
/********actualiacion de la camara frontal *********/

//        sourceColorImg.mirror(false, true);
        
        grabberMat = ofxCv::toCv(sourceColorImg.getPixelsRef());
        grabberMatInvert = ofxCv::toCv(sourceColorImgMirror.getPixelsRef());
        
        
        if(blobTrackerCeil.kindOfBG==blobTrackerCeil.CUSTOM){
            sourceGrayImage = sourceColorImg;
            blobTrackerCeil.updateBG(sourceGrayImage);
        }
        
        if(blobTrackerFront.kindOfBG==blobTrackerFront.CUSTOM){
            sourceGrayImage = sourceColorImgMirror;
            blobTrackerFront.updateBG(sourceGrayImage);
        }
     
        blobTrackerFront.update1(grabberMatInvert, blobThreshold[FRONT_INDEX]);
        blobTrackerCeil.update1(grabberMat, blobThreshold[CEIL_INDEX]);
        
        /*** TRACKING solo en la camara seleccionada ****/
        
        currentBlobTracker->update2( blobThreshold[configIndex],minBlobSize[configIndex],maxBlobSize[configIndex]);

        
        currentBlobTracker->setFiltersParam(learnRate[configIndex], smooth[configIndex]);

        setMaskedImageBlobs();

        if(appStatuses["sendTUIO"]==true){
            sendTUIO(&(currentBlobTracker->trackedBlobs));
        };
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
	
	bFlipHCameraAct = true;
    currentBlobTracker=&blobTrackerFront;
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
	
	bFlipHCameraAct = false;
	currentBlobTracker=&blobTrackerCeil;
    
}
/* esta funcion toma el resultado del blob tracking y lo usa como m‡scara sobre la imagen original. De esta fomra queda la imagen original, pero solo la zona donde hay blob */
void testApp::setMaskedImageBlobs(){
    //Paso las imagenes originales a openCV
    cv::Mat fullimageCV;
    //elegimos la mmatrix invertida o la normal segun toque. La camara frontal esta invertida
    if(configIndex==FRONT_INDEX)
        fullimageCV=grabberMatInvert;
    else
        fullimageCV=grabberMat;
    
    cv::Mat grayimageCV;
    grayimageCV=currentBlobTracker->fore; //imagen en escala de grises con los blobs
    
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
   // ofScale(0.8, 0.8);
    if(ofGetFrameNum()==19 || ofGetFrameNum()%150==0)
        bg.draw(-1,-1,1281,1025);
    ofPushMatrix();
    cleanBackgrounds();
   

// fbo1 contiene  las imagenes con la mascara ya aplicada.
    fbo1.begin();
	ofClear(0, 0, 0, 0);    //ofSetColor(255,255,255,255);
    if(configIndex==FRONT_INDEX)
        maskMaker.drawMask(sourceColorImgMirror.getTextureReference(), contourMaskOF.getTextureReference(), 0,0,255,VIDEOWITH,VIDEOHEIGHT);
    else
        maskMaker.drawMask(sourceColorImg.getTextureReference(), contourMaskOF.getTextureReference(), 0,0,255,VIDEOWITH,VIDEOHEIGHT);
    fbo1.end();
    //maskMaker.drawScrollingMask(sourceColorImg.getTextureReference(), contourMaskOF.getTextureReference(), 0, 255);

    //imagen original
    ofSetColor(255);
    if(configIndex==FRONT_INDEX)
        sourceColorImgMirror.draw(59,169,480,360); //img original
    else
        sourceColorImg.draw(59,169,480,360); //img original

    currentBlobTracker->openCVInputImage.draw(766,275,480,360);
    ofSetColor(255,255);
    currentBlobTracker->draw(57,602,480,360);//img + blobs}
   // 	maskedImageOF.draw(1391,139,320,240);


    if( adminMode) showDebug();
    

    
    ofPopMatrix();
 //   currentBlobTracker->drawDebug();
  //  maskedImageOF.draw(0,0,320,240);
    
    if(appStatuses["syphonEnabled"]==true && appStatuses["isCameraReady"]){
        if(configIndex==FRONT_INDEX)
            individualTextureSyphonServer.publishTexture(&sourceColorImgMirror.getTextureReference());
        else
            individualTextureSyphonServer.publishTexture(&sourceColorImg.getTextureReference());
        onlyBlobsImageSyphonServer.publishTexture(&fbo1.getTextureReference());
    }
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
    appStatuses["forceCamera"]=false;
    appStatuses["selectedForcedCamera"]=FRONT_INDEX;
    
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

