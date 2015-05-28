#include "ofApp.h"


using namespace ofxCv;
using namespace cv;


//--------------------------------------------------------------
void testApp::setup() {
    
	//Depth camera setup
	
	/// print the device list
	kinect.listDevices();
	// enable depth->video image calibration
	kinect.setRegistration(true);
	kinect.init(false,true);  // no-IR, si-RGB
	//kinect.init(false, false); // disable video image (faster fps)
	kinect.open();		// opens first available kinect
	
	if(kinect.isConnected()) {
		ofLogNotice() << "Kinect Connected!!!" << endl;
		ofLogNotice() << "sensor-emitter dist: " << kinect.getSensorEmitterDistance() << "cm";
		ofLogNotice() << "sensor-camera dist:  " << kinect.getSensorCameraDistance() << "cm";
		ofLogNotice() << "zero plane pixel size: " << kinect.getZeroPlanePixelSize() << "mm";
		ofLogNotice() << "zero plane dist: " << kinect.getZeroPlaneDistance() << "mm";
	}
	
	// zero the tilt on startup (modelos  <= 1414)
	float angle = 0;
	kinect.setCameraTiltAngle(angle);
	
	// dims de las im‡genes
	wk = kinect.getWidth();
	hk = kinect.getHeight();
	ofLogNotice() << "Dims kinet: " << wk << "," << hk;
	
	modo = 1;
    
    bModoAdapt = true;
	
	//
	// MODO 1
	//
	// imagenes openCV para blob_substraction
	grayImage.allocate(wk, hk);
	grayThreshNear.allocate(wk, hk);
	grayThreshFar.allocate(wk, hk);
	
	colorImg.allocate(wk, hk, OF_IMAGE_COLOR);
	tempGrayImage.allocate(wk, hk);
    
//	imageMasked.allocate(wk, hk, OF_IMAGE_COLOR);// Asi OK
 	imageMasked.allocate(wk, hk, OF_IMAGE_GRAYSCALE); // As’ OK.
	maskImageColor.allocate(wk, hk);
	
	// backgroundImage
	backgroundImage.allocate(wk, hk);

    numFramesMax_newBckgrnd = 1800;
    
    
	// distances
    swDistMax = false;
	distMax = 173;
	maskShader.load("","shaders/maskVideo.frag");
	maskImage.allocate(wk, hk);
	maskFbo.allocate(wk,hk, GL_RGB);
	
	thresholdLow = 20;
	thresholdHigh = 200;
	threshold = 100;
	bLearnBakground = true;
	blurcv = 1;
    
    swErode = true;

	swErodeMask = false;
    
    //
    // 12-GGG
    // Background Adaptativo
    //
    background.setLearningTime(900);
    lr = 0.0001;
    lrUI = lr/1000.0;
    background.setLearningRate(lr);
    background.setThresholdValue(distMax); //(thresh);
//    background.setDifferenceMode(RunningBackground::BRIGHTER);
    //
    
	// ContourFinder
	max_blobs = 20;
	min_blob_size = 100;
	max_blob_size = 50;	// % del area de kinect
	bFindHoles = false;
	

	// dims en pantalla
	wk1 = 400;// 320;
	hk1 = 300;// 240;

    int margenX = 250;
	int margenY = 20;
	rect_RGB = ofRectangle(margenX, margenY, wk1, hk1);
	rect_Depth = ofRectangle(margenX+wk1+5, margenY, wk1, hk1);
	rect_Proy = ofRectangle(margenX,margenY+hk1+20, wk1, hk1);
	rect_Blobs = ofRectangle(margenX+wk1+5, margenY+hk1+20, wk1, hk1);
	rect_Bckgnd = ofRectangle(rect_Depth.x+rect_Depth.width+5,rect_Depth.y, wk1/2, hk1/2);
	
	//
	// BT BlobTracker
	// wait for half a frame before forgetting something
	contourFinderX.getTracker().setPersistence(15);
	// an object can move up to 32 pixels per frame
	contourFinderX.getTracker().setMaximumDistance(32);
	
	
	bWarp = true;

	setupGUI();
	
	gui1->loadSettings("gui1Settings.xml");
	
	bLearnBakground = true;
	
	contourFinderX.setMinAreaRadius((float)min_blob_size);
	contourFinderX.setMaxAreaRadius((float)max_blob_size*wk/100.0);
	contourFinderX.setFindHoles(bFindHoles);
	
	bt = true;
	
	// TUIO
	
    //	myTUIO.setup("localhost",3333);
	myTUIO.setup("192.168.98.24",3333);
	
	bPeque = false;

	nombreCalib = "calibracion/calib2";
//	calib1.setRects(c1_Src, c1_Dst);
	calib2.setRects(rect_Blobs, rect_Proy);
	
	bCalibActiva2 = false;
		
	movingPoint = false;
	
	//	proyImg.loadImage("imgsCalib/proyeccion_"+ofToString(wk1)+"x"+ofToString(hk1)+".png");
	proyImg.loadImage("imgsCalib/proyeccion_400x300.png");
    
    
//    ofLogNotice("smoothRate: " + ofToString(contourFinderX.getTracker().getSmoothingRate() ) );
    
    // Cargar calibracion automaticamente
    // y activarla
    calib2.loadMatrix(nombreCalib);
    bCalibActiva2 = true;
    
    
	
}

//--------------------------------------------------------------
void testApp::update(){
    
	kinect.update();		//Update depth camera
	
	// there is a new frame and we are connected
	if(kinect.isFrameNew()) {
        if(bModoAdapt) {
            updateModo2();
        }
        else updateModo1();
	}
}





void testApp::updateModo1() {
	// Leer Camara
	// load grayscale depth image from the kinect source
	//		grayImage.setFromPixels(kinect.getDepthPixels(), wk, hk);
	//		colorImg.setFromPixels(kinect.getDepthPixelsRef()); //.getDepthPixels());
	colorImg.setFromPixels(kinect.getPixelsRef()); //.getDepthPixels());
	tempGrayImage.setFromPixels(kinect.getDepthPixelsRef()); //.getDepthPixels());		// GrayScale
	//		tempGrayImage.setFromPixels(kinect.getRawDepthPixelsRef()); //.getDepthPixels());  // Short
	//		monoPixels.setFromPixels(tempGrayImage.getPixels(), 320,240, 1);
    
	if(ofGetFrameNum()<numFramesMax_newBckgrnd && ofGetFrameNum()%60==0) bLearnBakground=true;
    
	if (bLearnBakground == true){
		//			backgroundImage.setFromPixels(tempGrayImage.getPixels(), 320,240);
		backgroundImage = tempGrayImage;
		
		// Calcular mascara a partir del background
		if(swDistMax) {
			maskImage = tempGrayImage;
			maskImage.threshold(distMax);
		}
		
		// Aplicarla al mismo background? Por ahora NO
//		maskShader.begin();
//		maskShader.end();
		
		bLearnBakground = false;
	}
	
	
	
	// quitar zonas mas lejos de una dist dada
	if(swDistMax) {
//		tempGrayImage.threshold(distMax);
		
		// Aplicar mascara a tempGrayImage
		maskFbo.begin();
		ofClear(0,0,0,0);
		maskShader.begin();
        maskShader.setUniformTexture( "texture1", maskImage.getTextureReference(),  1 );
		tempGrayImage.draw(0,0);
		maskShader.end();
		maskFbo.end();

		
		
//		maskFbo.readToPixels(tempGrayImage.getPixelsRef());

		maskFbo.readToPixels(imageMasked.getPixelsRef());
		imageMasked.update();	
		
//		tempGrayImage.update();
//		tempGrayImage.setFromPixels(maskFbo.getPixelsRef());
//		tempGrayImage.update();		
		
//		maskImageColor.setFromPixels(imageMasked.getPixelsRef());
//		maskFbo.readToPixels(maskImageColor.getPixelsRef());
//		maskImageColor.flagImageChanged();
		
//		tempGrayImage.setFromPixels(imageMasked.getPixelsRef());
//		tempGrayImage = maskImageColor;// imageMasked; 
		maskImageColor.setFromPixels(imageMasked.getPixelsRef());
		tempGrayImage = maskImageColor;
    }
	
	
	
	
	// substraci—n openCV.
	//
	tempGrayImage-=backgroundImage;
//	
	grayThreshNear = tempGrayImage;
	grayThreshFar  = tempGrayImage;
	grayThreshNear.threshold(thresholdHigh, true);
	grayThreshFar.threshold(thresholdLow);
	cvAnd(grayThreshNear.getCvImage(), grayThreshFar.getCvImage(), tempGrayImage.getCvImage(), NULL);
//	
	// update the cv images
	tempGrayImage.flagImageChanged();
	
    if(swErode) {
        tempGrayImage.erode_3x3();
        tempGrayImage.dilate_3x3();
    }
    
	// WARP
	//	if(bWarp) 	{
	//		//grayImage.warpIntoMe(tempGrayImage, entrada, destino_camera_warp);  
	//		// mejor warp con calib2
	//	}
	//	else {	grayImage = tempGrayImage; }
	grayImage = tempGrayImage;
    
	// NO SE SI VA MEJOR EL THREHOLD ANTES O DESPUES DEL BLUR
	if(bt) grayImage.threshold(threshold);  
	grayImage.blur(blurcv);  
	
	contourFinderX.setThreshold(threshold);
	contourFinderX.findContours(grayImage);
	
}

void testApp::updateModo2() {
	// Leer Camara
	// load grayscale depth image from the kinect source
	//		grayImage.setFromPixels(kinect.getDepthPixels(), wk, hk);
	//		colorImg.setFromPixels(kinect.getDepthPixelsRef()); //.getDepthPixels());
	colorImg.setFromPixels(kinect.getPixelsRef()); //.getDepthPixels());
	depthImg.setFromPixels(kinect.getDepthPixelsRef());
	tempGrayImage.setFromPixels(kinect.getDepthPixelsRef()); //.getDepthPixels());		// GrayScale
	//		tempGrayImage.setFromPixels(kinect.getRawDepthPixelsRef()); //.getDepthPixels());  // Short
	//		monoPixels.setFromPixels(tempGrayImage.getPixels(), 320,240, 1);
    
	if(ofGetFrameNum()<numFramesMax_newBckgrnd && ofGetFrameNum()%60==0) bLearnBakground=true;
    
	if (bLearnBakground == true){
        // Update Total el background Adaptativo
        background.reset();
        //
        
        // Update imagen background (no adaptativo)
		backgroundImage = tempGrayImage;
		
		// Calcular mascara a partir del background
        //
//		if(swDistMax) {
//			maskImage = tempGrayImage;
//			maskImage.threshold(distMax);
//		}
		
		bLearnBakground = false;
	}
	
    //
	// Background Adaptativo
    //
    ofImage tmpG2RGB;
    convertColor(depthImg, tmpG2RGB, CV_GRAY2BGR);
    background.update(tmpG2RGB, thresholded);
    thresholded.update();  // Esta la puedo utilizar para quitar zonas m‡s lejos de distMax
    //
    toOf(background.getBackground(), bckCvImg);
    bckCvImg.update();
    
    // para poder restar el background necesitamos pasar
    // a una ofxCvGrayScaleImage (a backgroundImage)
    // pero habr‡ forma de hacerlo con funciones de ofxCv
    ofImage tmpimgGray;
    convertColor(bckCvImg, tmpimgGray, CV_BGR2GRAY);
    backgroundImage.setFromPixels(tmpimgGray.getPixelsRef());
    backgroundImage.updateTexture();
    
    // Calcular mascara a partir del background
    // La calculo siempre por que siempre se modifica el background
    // otra opcion es calcularla cada x frames
    if(swDistMax) {
        maskImage = tempGrayImage;
        maskImage.threshold(distMax);
        
        if(swErodeMask) maskImage.erode_3x3();
    }
    
    
	// quitar zonas mas lejos de una dist dada
	if(swDistMax) {
        
		// Aplicar mascara a tempGrayImage
		maskFbo.begin();
		ofClear(0,0,0,0);
		maskShader.begin();
        maskShader.setUniformTexture( "texture1", maskImage.getTextureReference(),  1 );
		tempGrayImage.draw(0,0);
		maskShader.end();
		maskFbo.end();
		
        //		maskFbo.readToPixels(tempGrayImage.getPixelsRef());
        
		maskFbo.readToPixels(imageMasked.getPixelsRef());
		imageMasked.update();
		
		maskImageColor.setFromPixels(imageMasked.getPixelsRef());
		tempGrayImage = maskImageColor;
    }
	
	
	
	
	// substraci—n openCV.
	//
    // Restar el background de la imagen actual (masked)
	tempGrayImage-=backgroundImage;
    //
	grayThreshNear = tempGrayImage;
	grayThreshFar  = tempGrayImage;
	grayThreshNear.threshold(thresholdHigh, true);
	grayThreshFar.threshold(thresholdLow);
	cvAnd(grayThreshNear.getCvImage(), grayThreshFar.getCvImage(), tempGrayImage.getCvImage(), NULL);

    // Tb de esta forma con ofxCv
    // http://forum.openframeworks.cc/t/ofxcv-and-kinect-cvand
    // que utiliza Mat o ofImage:
//    copy(tempGrayMat, nearThresMat);
//    copy(tempGrayMat, farThresMat);
//    threshold(nearThreshMat, nearThreshold);
//    threshold(farThreshMat, farThreshold);
//    // bitwise_and(const Mat& src1, const Scalar& src2, CV_OUT Mat& dst, const Mat& mask=Mat());
//    bitwise_and(nearThreshMat, farThreshMat, grayImageMat);
    //
    
    //
	// update the cv images
	tempGrayImage.flagImageChanged();
	
    if(swErode) {
        tempGrayImage.erode_3x3();
        tempGrayImage.dilate_3x3();
    }
    
	// WARP
	//	if(bWarp) 	{
	//		//grayImage.warpIntoMe(tempGrayImage, entrada, destino_camera_warp);
	//		// mejor warp con calib2
	//	}
	//	else {	grayImage = tempGrayImage; }
	grayImage = tempGrayImage;
    
	// NO SE SI VA MEJOR EL THREHOLD ANTES O DESPUES DEL BLUR
	if(bt) grayImage.threshold(threshold);
	grayImage.blur(blurcv);  
	
	contourFinderX.setThreshold(threshold);
	contourFinderX.findContours(grayImage);
	
}


//--------------------------------------------------------------
void testApp::draw(){
    
	ofBackground( 0 );	//Set white background
	ofSetColor( 255, 255, 255 );
    
	// modo==1
	//
	// draw from the live kinect
	// RGB
	colorImg.draw(rect_RGB);
	//		kinect.draw( margenX, margenY );
	drawMarco(rect_RGB);
	ofDrawBitmapString("RGB Kinect", rect_RGB.x, rect_RGB.y+hk1+10);
	
	// Depth
	kinect.drawDepth(rect_Depth);
	drawMarco(rect_Depth);
	ofDrawBitmapString("DEPTH Kinect", rect_Depth.x, rect_Depth.y+hk1+10);
	
	// background
	backgroundImage.draw(rect_Bckgnd);
	drawMarco(rect_Bckgnd);
	ofDrawBitmapString("Background (b)", rect_Bckgnd.x,rect_Bckgnd.y+hk1+10);

	maskImage.draw(rect_Bckgnd.x, rect_Bckgnd.y+hk1+5, rect_Bckgnd.width, rect_Bckgnd.height);
	drawMarco(rect_Bckgnd.x, rect_Bckgnd.y+hk1+5, rect_Bckgnd.width, rect_Bckgnd.height);
	ofDrawBitmapString("Mask Image", rect_Bckgnd.x,rect_Bckgnd.y+hk1+10+5);
	
//	maskFbo.draw(0,0);
	
//	imageMasked.draw(0,hk1);
//	tempGrayImage.draw(350,2*hk1);
//			maskImageColor.draw(350,2*hk1);
						
	// Proyeccion
		proyImg.draw(rect_Proy);
		drawMarco(rect_Proy);
		ofDrawBitmapString("Proyeccion", rect_Proy.x,rect_Proy.y+rect_Proy.height+10);
//    
	
	// Contour Finder
//		grayImage.draw(rect_Blobs);
	grayImage.draw(rect_Blobs);
	drawBlobs(wk1, hk1);
	drawMarco(rect_Blobs);
	ofDrawBitmapString("DEPTH Kinect", rect_Blobs.x, rect_Blobs.y+hk1+10);
	
	
	// Calibrador
//	calib2.drawPoints(rect_Blobs, rect_Proy);
	calib2.drawPoints(rect_Proy, rect_Blobs);
	
    
    // MOUSE
	// draw mouse
    ofRectangle c2_Dst = rect_Proy;
    ofRectangle c2_Src = rect_Blobs;
	if(c2_Dst.inside(mouseX, mouseY)) {
		ofSetColor(ofColor::lime);
		ofNoFill();
		ofEllipse(ofGetMouseX(), ofGetMouseY(), 15,15);
		
		// me pasa el punto desde Dst a Src
		ofVec2f proy;
		if(calib2.homographyReady) {
			proy = calib2.transf_Punto( ofVec2f(ofGetMouseX() - c2_Dst.x, ofGetMouseY() - c2_Dst.y), true ) ;
		}
		else {
			proy = ofVec2f(ofGetMouseX() - c2_Dst.x, ofGetMouseY() - c2_Dst.y);
		}
		ofSetColor(ofColor::green);
		ofPushMatrix();
		ofTranslate(c2_Src.x, c2_Src.y, 0);
		ofEllipse(proy.x, proy.y, 20,20);
		ofPopMatrix();
		
	}
	else if(c2_Src.inside(mouseX, mouseY)) {
		ofSetColor(ofColor::pink);
		ofNoFill();
		ofEllipse(ofGetMouseX(), ofGetMouseY(), 15,15);
		
		// me pasa el punto desde Src a Dst
		// Este va muy bien
		ofVec2f proy;
		if(calib2.homographyReady) {
			proy = calib2.transf_Punto( ofVec2f(ofGetMouseX() - c2_Src.x, ofGetMouseY() - c2_Src.y) , false);
		}
		else {
			proy = ofVec2f(ofGetMouseX() - c2_Src.x, ofGetMouseY() - c2_Src.y);
		}
		ofSetColor(ofColor::green);
		ofPushMatrix();
		ofTranslate(c2_Dst.x, c2_Dst.y, 0);
		ofEllipse(proy.x, proy.y, 20,20);
		ofPopMatrix();
	}
	
    
    // Backgrounds
    ofSetColor(255);
    float www = 200;
    float hhh = backgroundImage.height*www/backgroundImage.width;
//    backgroundImage.draw(ofGetWidth()/2-2*www, ofGetHeight()-hhh, www, hhh);
//    bckCvImg.draw(ofGetWidth()/2-www, ofGetHeight()-hhh, www, hhh);
////
//    thresholded.draw(ofGetWidth()/2-www, ofGetHeight()-2*hhh, www, hhh);
//    drawMat(background.getBackground(),ofGetWidth()/2, ofGetHeight()-2*hhh, www, hhh);
//    drawMat(background.getForeground(),ofGetWidth()/2, ofGetHeight()-hhh, www, hhh);
    maskImage.draw(ofGetWidth()/2+www, ofGetHeight()-hhh, www, hhh);
    
    
    
    
    
    
    // FR
	ofSetColor(255);
	ofDrawBitmapString("fr: " +ofToString(ofGetFrameRate()), 10,ofGetHeight()-20);
	
}

void testApp::drawBlobs(int wk1, int hk1) {
    
	// preparar vector de BlobX
	blobs.clear();
	
	// tracker
	RectTracker& tracker = contourFinderX.getTracker();
	
	ofPushMatrix();
	ofTranslate(rect_Blobs.x, rect_Blobs.y, 0);
	ofScale((float)wk1/wk, (float)hk1/hk, 1);
	contourFinderX.draw();
    
//    ofLogNotice("smoothRate: " + ofToString(contourFinderX.getTracker().getSmoothingRate() ) );
    
	int n = contourFinderX.size();
	for(int i=0; i<n; i++) {
		// smallest rectangle that fits the contour
		ofSetColor(cyanPrint);
		ofPolyline minAreRect = toOf(contourFinderX.getMinAreaRect(i));
		minAreRect.draw();
		
		// ellipse that best fits the contour
		ofSetColor(magentaPrint);
		cv::RotatedRect ellipse = contourFinderX.getFitEllipse(i);
		ofPushMatrix();
		ofVec2f ellipseCenter = toOf(ellipse.center);
		ofVec2f ellipseSize = toOf(ellipse.size);
		ofTranslate(ellipseCenter.x, ellipseCenter.y);
		ofRotate(ellipse.angle);
		ofNoFill();
		ofEllipse(0, 0, ellipseSize.x, ellipseSize.y);
		ofPopMatrix();
		
		// convex hull of the contour
		ofSetColor(yellowPrint);
		ofPolyline convexHull = toOf(contourFinderX.getConvexHull(i));
		convexHull.draw();
		
		
		// Label
		ofPoint center = toOf(contourFinderX.getCenter(i));
		ofPushMatrix();
		ofSetColor(255,0,0);
		ofTranslate(center.x, center.y);
		int label = contourFinderX.getLabel(i);
		string msg = ofToString(label) + ":" + ofToString(tracker.getAge(label));
		ofDrawBitmapString(msg, 0, 0);
		ofPopMatrix();
		
		// preparar vector de BlobX
		if(bTUIO) {
            //		int label;
            //		ofVec2f pos;
            //		ofVec2f vel;	// ult desplazamiento
            //		float   maccel;	// veloc = despl/dt
            //		ofVec2f	boundingRect;
            
			BlobX blobTmp;
			blobTmp.label = contourFinderX.getLabel(i);	// Por ahora env’o el centroid
			ofVec2f posB = toOf(contourFinderX.getCenter(i));
			posB.x/=wk;
			posB.y/=hk;
			blobTmp.pos = posB;	// Por ahora env’o el centroid
			ofVec2f velB = toOf(contourFinderX.getVelocity(i));
			velB.x/=wk;
			velB.y/=hk;
			blobTmp.vel = velB;	// Por ahora env’o el centroid
			blobTmp.maccel = blobTmp.vel.length();	// Aœn puedo dividir para dt (millis)

			//
			// Transformar las posiciones si hay calibracion activada
			//
			if(bCalibActiva2) {
				if(calib2.homographyReady) {
					
                    posB.x*=wk;
                    posB.y*=hk;
                    
                    float escalax = (float)wk1/wk;
                    float escalay = (float)hk1/hk;

                    ofVec2f posBP;
					
                    posB.x *= escalax;
                    posB.y *= escalay;
					
					// paso de Src (RGB-DEPTH) a Dst (PROY)
					posBP = calib2.transf_Punto( posB, false ) ;

                    // Redimensionar x e y si es diferente
                    // el espacio de calculo de blobs y el de calibracion
                    
                    posBP.x/=wk1;
                    posBP.y/=hk1;
					
					blobTmp.pos = posBP;	// Por ahora env’o el centroid
				}
			}
			
			blobs.push_back(blobTmp);
		}
	}
	
	// enviar TUIO con los blobs
	if(bTUIO) 	myTUIO.sendTUIO(blobs);
	
	ofPopMatrix();
}

void testApp::drawMarco(ofRectangle rect) {
	drawMarco(rect.x, rect.y, rect.width, rect.height);
}

void testApp::drawMarco(float xx, float yy, float ww, float hh) {
	ofPushStyle();
	ofSetColor(0,0,150);
	ofNoFill();
	ofRect(xx,yy,ww,hh);
	ofPopStyle();
}


//--------------------------------------------------------------
void testApp::keyPressed(int key){
	if(key==' ') {
		if(bPeque) {
			// hacer ventana peque–a
		}
		else {
			// ventana grande
		}
	}
	else if(key=='b') {
		bLearnBakground = true;
	}
    
    else if(key=='q') {
        float sr = contourFinderX.getTracker().getSmoothingRate();
        sr+=0.1;
        contourFinderX.getTracker().setSmoothingRate(sr);
        ofLogNotice("smoothRate: " + ofToString(contourFinderX.getTracker().getSmoothingRate() ) );
    
    }
    else if(key=='a') {
        float sr = contourFinderX.getTracker().getSmoothingRate();
        sr=(sr<=0.1)? 0.1 : sr-0.1;
        contourFinderX.getTracker().setSmoothingRate(sr);
        ofLogNotice("smoothRate: " + ofToString(contourFinderX.getTracker().getSmoothingRate() ) );
        
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
	if(modo==1) {
		ofRectangle c2_Dst = rect_Proy;
		ofRectangle c2_Src = rect_Blobs;
		if(movingPoint) {
			if(isDstPt_c2)  curPoint->set(x-c2_Dst.x, y-c2_Dst.y);
			else			curPoint->set(x-c2_Src.x, y-c2_Src.y);
			
			// update homography
			// en calib2
			calib2.calcHomography();
		}
	}
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	
	
	if(modo==1) {
		ofRectangle c2_Dst = rect_Proy;
		ofRectangle c2_Src = rect_Blobs;
		
		ofVec2f cur(x, y);
		ofVec2f offsetDst = ofVec2f(c2_Dst.x, c2_Dst.y);
		ofVec2f offsetSrc = ofVec2f(c2_Src.x, c2_Src.y);
		
		// selecciona un punto de calibracion de los del area de interes 
		bool selectPtDst = movePoint(calib2.ptsDst, cur-offsetDst);
		bool selectPtSrc = movePoint(calib2.ptsSrc, cur-offsetSrc);
		
		if(!selectPtDst && !selectPtSrc) {
			// Si no selecciona ni un punto de los que ya hay en la calibracion
			// add puntos para calibrar
			
			// ver si esta en una zona de la imagen
			// Supongo tama–os de imagenes iguales!!!!
			//			if(c2_Dst.inside(cur)) {
			//				calib2.addPts(cur-offsetDst, cur-offsetDst);	
			//				// calib1 se encarga de updateHomogr
			//			}
			//			else if(c2_Src.inside(cur)) {
			//				calib2.addPts(cur-offsetSrc, cur-offsetSrc);
			//				// calib1 se encarga de updateHomogr
			//			}
			
			
		}
		
		else {
			// Si ha seleccionado algun punto anotar si es de Dst o de Src
			if(selectPtDst) isDstPt_c2 = true;
			else if(selectPtSrc) isDstPt_c2 = false;
		}
		
	}
}


bool testApp::movePoint(vector<ofVec2f>& points, ofVec2f point) {
	// devuelve true se hay un punto cerca de point
	// y guarda su referencia en curPoint
	//
	for(int i = 0; i < points.size(); i++) {
		if(points[i].distance(point) < 20) {
			movingPoint = true;
			curPoint = &points[i];
			return true;
		}
	}
	return false;
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	movingPoint = false;
	calib2.calcHomography();	
}


//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
    
}


// - - - - GUI - - - -
void testApp::setupGUI() {
	
	gui1 = new ofxUISuperCanvas("K-Touch Surface");
    gui1->addSpacer();
	//    gui1->addLabel("Press 'h' to Hide GUIs", OFX_UI_FONT_SMALL);
	gui1->addLabel("SELECTOR MODO");
	gui1->addIntSlider("Modo", 1, 1, &modo);
    
	gui1->addSpacer();
	
	gui1->addLabel("MODO 1 - Backgr Substr");
    gui1->addToggle("modo adaptativo", &bModoAdapt);
	gui1->addToggle("(b) Set Bckgrnd", &bLearnBakground);
	gui1->addSpacer();
    gui1->addSlider("learningRate*1000", 0.01, 10.0, &lrUI);
    gui1->addSpacer();
	gui1->addToggle("Aplica Quitar Fondo",&swDistMax);
	gui1->addSlider("Quitar Zona Fondo", 0.0, 255.0, &distMax);
    gui1->addToggle("Erodde Mask", &swErodeMask);
	gui1->addSpacer();
	
	gui1->addRangeSlider("rango resta images L/H", 0.0, 20.0, &thresholdLow, &thresholdHigh);
	gui1->addSlider("contraste", 0.0, 255.0, &threshold);
	gui1->addIntSlider("blur", 0, 11, &blurcv);
    gui1->addToggle("Erode ", &swErode);
	
	gui1->addLabel("Contour Finder");
	gui1->addSlider("blob_min_radio", 0.0, 200, &min_blob_size);
	gui1->addSlider("blob_max_radio_%W", 0.0, 100.0, &max_blob_size);
	gui1->addIntSlider("max_blobs", 0, 20, &max_blobs);
	gui1->addToggle("FindHoles", &bFindHoles);
	gui1->addSpacer();
	gui1->addToggle("bt", &bt);
	gui1->addToggle("doWarp", &bWarp);
	
	gui1->addSpacer();
	gui1->addLabel("Calibr. RGB=>Proyeccion");
	gui1->addButton("Carga Calibracion", &cargaCalib);	
	gui1->addToggle("Activa Calibracion", &bCalibActiva2);	
	gui1->addSpacer();
	gui1->addToggle("Send TUIOs", &bTUIO);
	
    gui1->autoSizeToFitWidgets();
	ofAddListener(gui1->newGUIEvent,this,&testApp::guiEvent);	
}

void testApp::guiEvent(ofxUIEventArgs &e) {
	string name = e.getName();
	int kind = e.getKind();
	//	cout << "got event from: " << name << endl;
	if(name == "blob_min_radio") {
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		min_blob_size = slider->getScaledValue();
		contourFinderX.setMinAreaRadius((float)min_blob_size);
	}	
	else if(name == "blob_max_radio_%W") {
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		max_blob_size = slider->getScaledValue();
		contourFinderX.setMaxAreaRadius((float)max_blob_size*wk/100.0);
	}
	
	
	else if(name == "Carga Calibracion") {
		calib2.loadMatrix(nombreCalib);		
	}

	else if(name == "learningRate*1000") {
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		lrUI = slider->getScaledValue();
        lr = lrUI/1000.0;
		background.setLearningRate(lr);
	}
	
	//	contourFinderX.setMinArea(min_blob_size);
	//	contourFinderX.setFindHoles(bFindHoles);
	
}	


void testApp::exit()
{
	
    //	openNIDevice.stop();
	kinect.close();
	
	gui1->saveSettings("gui1Settings.xml");    
	delete gui1;
	
	//    float *buffer;
	//    ofImage *img;
	
	//	delete[] buffer;
	//    delete img;
	
	// borrar
	// - emitters
	// - particles
	
}




