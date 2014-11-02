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
	
	wk = kinect.getWidth();
	hk = kinect.getHeight();
	ofLogNotice() << "Dims kinet: " << wk << "," << hk;
	
	modo = 0;
    
	// MODO 0
	//Set up drawing variables
	W = wk; //1024;					//Desired screen size
	H = hk; //768;
	outPixels.allocate(		W, H, OF_IMAGE_GRAYSCALE );
	
	calibrating = true;		//Set calibrating mode at start
	shift = ofPoint( 250, 20 );	//The value of shifting
	//camera's image from the corner of the screen
	
	rectCalib = ofRectangle(shift.x, shift.y, wk, hk);
	ofLogNotice("rectCalib: " + ofToString(rectCalib));
	
	minV = 30;		//Minimal distance in mm
	maxV = 150; 		//Maximal distance in mm
    
	
	
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
//	imageMasked.allocate(wk, hk, OF_PIXELS_MONO);	// Esto es para pixels
	maskImageColor.allocate(wk, hk);
	
	// backgroundImage
	backgroundImage.allocate(wk, hk);

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
	
	// ContourFinder
	max_blobs = 20;
	min_blob_size = 100;
	max_blob_size = 50;	// % del area de kinect
	bFindHoles = false;
	

	// dims
	wk1 = 400;// 320;
	hk1 = 300;// 240;
    
	rect_RGB = ofRectangle(shift.x, shift.y, wk1, hk1);
	rect_Depth = ofRectangle(shift.x+wk1+5, shift.y, wk1, hk1);
	rect_Proy = ofRectangle(shift.x,shift.y+hk1+20, wk1, hk1);
	rect_Blobs = ofRectangle(shift.x+wk1+5, shift.y+hk1+20, wk1, hk1);
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
	
}

//--------------------------------------------------------------
void testApp::update(){
    
	kinect.update();		//Update depth camera
	
	// there is a new frame and we are connected
	if(kinect.isFrameNew()) {
		if(modo==0) {
			updateModo0();
		}
		else if(modo==1) {
			updateModo1();
		}
	}
}



void testApp::updateModo0() {
	// Cargar imagen en ShriPixels
	
	// restar background: backgroundDepth
	
	
	
	// Check zero values
	
	
	// l’mite min-max
	
	
	
	
	
	if ( !calibrating && corners.size() == 4 ) {
		//Analyze depth
		
		//Get current depth image
		//		ofShortPixels &input = openNIDevice.getDepthRawPixels();
		ofShortPixels &input = kinect.getRawDepthPixelsRef();
		
		//Process pixels
		int w = input.getWidth();
		int h = input.getHeight();
		for (int Y=0; Y<H; Y++) {
			for (int X=0; X<W; X++) {
				//Process pixel (X, Y)
				
				//Transform screen coordinates (X, Y)
				//to depth image coordinates (x, y)
				float a = float(X) / W;
				float b = float(Y) / H;
				ofPoint p =
				(1-a) * (1-b) * corners[0]
				+ a * (1-b) * corners[1]
				+ a * b * corners[2]
				+ (1-a) * b * corners[3];
				
				int x = int( p.x );
				int y = int( p.y );
				
				if ( x >= 0 && x < w && y >= 0 && y < h ) {
					//Getting depth values
					int inputZ = input.getPixels()[ x + w * y ];
					int backgroundZ = backgroundDepth.getPixels()[ x + w * y ];
					
					int delta;
					//Check for zero values - it means that depth camera
					//does not compute distance in the pixel
					if ( inputZ != 0 && backgroundZ != 0 ) {
						delta = backgroundZ - inputZ;
					}
					else {
						delta = -1;
					}
					//Output value
					if ( ofInRange( delta, minV, maxV ) ) {
						int value = ofMap( delta, minV, maxV, 0, 255, true );
						outPixels.getPixels()[ X + W * Y ] = value;
					}
					else
						outPixels.getPixels()[ X + W * Y ] = 0;
                    
                }
				
				
			}
		}
		outImage.setFromPixels( outPixels );
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
    
	if(modo==0) {
		if ( calibrating ) {
            
			//Draw color and depth image
			ofSetColor( 255, 255, 255 );
			kinect.draw( shift.x, shift.y );
			kinect.drawDepth( rectCalib.x+rectCalib.width-wk/2, rectCalib.y+rectCalib.height+10, wk/2, hk/2 );
            
			//Draw corners
			ofSetColor( 255, 0, 0 );
			ofFill();
			int n = corners.size();
			for (int i=0; i<n; i++) {
				ofCircle( corners[i] + shift, 10 );
			}
			if ( n == 4 ) {
				for (int i=0; i<n; i++) {
					ofLine( corners[i] + shift,
                           corners[(i+1)%n] + shift );
				}
			}
		}
		else {
			ofBackground( 0 );	//Set white background
			//Show current drawing
			ofSetColor( 255, 255, 255 );
			outImage.draw( 0, 0 );
		}
	}
	else if(modo==1) {
		// draw from the live kinect
		// RGB
		colorImg.draw(rect_RGB);
        //		kinect.draw( shift.x, shift.y );
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
		
		maskFbo.draw(0,0);
		
		imageMasked.draw(0,hk1);
		tempGrayImage.draw(350,2*hk1);
//			maskImageColor.draw(350,2*hk1);
						    
		// Proyeccion
//		proyImg.draw(rect_Proy);
//		drawMarco(rect_Proy);
//		ofDrawBitmapString("Proyeccion", rect_Proy.x,rect_Proy.y+rect_Proy.height+10);
//    
		
		// Contour Finder
//		grayImage.draw(rect_Blobs);
		grayImage.draw(rect_Blobs);
		drawBlobs(wk1, hk1);
		drawMarco(rect_Blobs);
		ofDrawBitmapString("DEPTH Kinect", rect_Blobs.x, rect_Blobs.y+hk1+10);
        
		
		// Calibrador
		calib2.drawPoints(rect_Blobs, rect_Proy);
		
	}
	
//	else if(modo==2) {
//		// MODO CALIBRACION
//		
//		
//	}
	
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
					// paso de Src (RGB-DEPTH) a Dst (PROY)
					posB = calib2.transf_Punto( posB, false ) ;
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
	if ( key == 'm' ) {
		calibrating = !calibrating;
		if ( !calibrating ) {	//Store background
            //			backgroundDepth = openNIDevice.getDepthRawPixels();
			backgroundDepth = kinect.getRawDepthPixelsRef();
		}
	}
	else if(key=='b') {
        //			backgroundDepth = openNIDevice.getDepthRawPixels();
		backgroundDepth = kinect.getRawDepthPixelsRef();
		bLearnBakground = true;
	}
	else if(key=='c') {
		calibrating = true;
		corners.clear();
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
			// en calib1
			calib2.calcHomography();
		}
	}
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	
	
	if(modo==0) {
		if ( calibrating && corners.size() < 4 ) {
			if(rectCalib.inside(x,y) ) {
				corners.push_back( ofPoint( x, y ) - shift );
			}
		}
	}
	else if(modo==1) {
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
	gui1->addIntSlider("Modo", 0, 2, &modo);
    
	//    gui1->addSpacer();
	gui1->addLabel("MODO 0 - BASICO");
	gui1->addLabel("Rango Valores");
	gui1->addRangeSlider("Rango", 10.0, 200.0, &minV, &maxV);
	
	gui1->addSpacer();
	gui1->addToggle("(m) Modo Calibracion", &calibrating );
    gui1->addButton("(c) Reset Calibration", false);
    gui1->addButton("(b) Background", false);
	
	gui1->addSpacer();
	
	gui1->addLabel("MODO 1 - Backgr Substr");
	gui1->addToggle("(b) Set Bckgrnd", &bLearnBakground);
	gui1->addSpacer();
	gui1->addToggle("Aplica Quitar Fondo",&swDistMax);
	gui1->addSlider("Quitar Zona Fondo", 0.0, 255.0, &distMax);
	gui1->addSpacer();
	
	gui1->addRangeSlider("rango resta images L/H", 0.0, 20.0, &thresholdLow, &thresholdHigh);
	gui1->addSlider("contraste", 0.0, 255.0, &threshold);
	gui1->addIntSlider("blur", 0, 11, &blurcv);
	
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
	if(name == "(m) Modo Calibracion")
	{
		if ( !calibrating ) {	// Store background
			//			backgroundDepth = openNIDevice.getDepthRawPixels();
			backgroundDepth = kinect.getRawDepthPixelsRef();
		}		
	}
	else if(name == "(c) Reset Calibration")
	{
		calibrating = true;
		corners.clear();
	}
	else if(name == "(b) Background")
	{
		//			backgroundDepth = openNIDevice.getDepthRawPixels();
		backgroundDepth = kinect.getRawDepthPixelsRef();
	}
	else if(name == "blob_min_radio") {
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




