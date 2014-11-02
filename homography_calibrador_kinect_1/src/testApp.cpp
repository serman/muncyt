#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup() {
	ofSetLogLevel(OF_LOG_VERBOSE);
	
	/// print the device list
	kinect.listDevices();
	
	// enable depth->video image calibration
	reg = true;
	kinect.setRegistration(reg);	// Con esto me quito la calibracion RGB<-->Depth calib1
    
	//kinect.init();	// true para ir
	kinect.init(false); // .init(true) shows infrared instead of RGB video image
	//kinect.init(false, false); // disable video image (faster fps)
	
	kinect.open();		// opens first available kinect
	//kinect.open(1);	// open a kinect by id, starting with 0 (sorted by serial # lexicographically))
	//kinect.open("A00362A08602047A");	// open a kinect using it's unique serial #
	
	// print the intrinsic IR sensor values
	if(kinect.isConnected()) {
		ofLogNotice() << "sensor-emitter dist: " << kinect.getSensorEmitterDistance() << "cm";
		ofLogNotice() << "sensor-camera dist:  " << kinect.getSensorCameraDistance() << "cm";
		ofLogNotice() << "zero plane pixel size: " << kinect.getZeroPlanePixelSize() << "mm";
		ofLogNotice() << "zero plane dist: " << kinect.getZeroPlaneDistance() << "mm";
	}
	
	colorImg.allocate(kinect.width, kinect.height);
	grayImage.allocate(kinect.width, kinect.height);
	
	ofSetFrameRate(60);
	
	// zero the tilt on startup
	angle = 0;
	kinect.setCameraTiltAngle(angle);
	
    modo = M400x300;
    
    string nmImage;
	if(modo == M320x240) nmImage = "imgsCalib/proyeccion_320x240.png";
	else if(modo == M400x300) nmImage = "imgsCalib/proyeccion_400x300.png";
	else if(modo == M640x480) nmImage = "imgsCalib/proyeccion_640x480.png";
    imgCalib.loadImage(nmImage);
	
	// zonas calibracion
	int ww = 400;
	int hh = 300;
	c1_Dst = ofRectangle(30,30, ww, hh);
	c1_Src = ofRectangle(c1_Dst.x+c1_Dst.width + 10, c1_Dst.y, ww, hh);
    
	c2_Dst = ofRectangle(c1_Dst.x, c1_Dst.y+c1_Dst.height + 10, ww, hh);
	c2_Src = ofRectangle(c1_Src.x, c2_Dst.y, ww, hh);
	
	
	// para control de puntos
	movingPoint = false;
	
	nombre1 = "calibracion/calib1";//+"_"+ofToString(ww)+"x"+ofToString(hh);
	nombre2 = "calibracion/calib2";//+"_"+ofToString(ww)+"x"+ofToString(hh);
	
	calib1.setRects(c1_Src, c1_Dst);
	calib2.setRects(c2_Src, c2_Dst);
	
	ofLogNotice("c2Dst: " + ofToString(c2_Dst));
	ofLogNotice("c2Src: " + ofToString(c2_Src));
}

//--------------------------------------------------------------
void testApp::update() {
	
	ofBackground(100, 100, 100);
	
	kinect.update();
	
	// there is a new frame and we are connected
	if(kinect.isFrameNew()) {
		// load grayscale depth image from the kinect source
        //		imgBN.setFromPixels(kinect.getDepthPixels(), kinect.width, kinect.height);
		imgRGB.setFromPixels(kinect.getPixels(), 400,300, OF_IMAGE_COLOR);
		imgBN.setFromPixels(kinect.getDepthPixels(), 400,300, OF_IMAGE_GRAYSCALE);
	}
	
	
    //	if(calib1.homographyReady) {
    //		warpPerspective(kinect.getRGB(), warpedColor, calib1.homography, CV_INTER_LINEAR);
    //		warped1.update();
    //	}
}

//--------------------------------------------------------------
void testApp::draw() {
	
	ofSetColor(255, 255, 255);
	
	// draw from the live kinect
	kinect.drawDepth(c1_Src);
	kinect.draw(c1_Dst);
	
    //	grayImage.draw(10, 320, 400, 300);
	
	kinect.draw(c2_Src);
	imgCalib.draw(c2_Dst);
	
	
	// draw mouse
	// CALIB RGB DEPTH
	if(c1_Dst.inside(mouseX, mouseY)) {
		ofSetColor(ofColor::lime);
		ofNoFill();
		ofEllipse(ofGetMouseX(), ofGetMouseY(), 15,15);
		
		// me pasa el punto desde Dst a Src
		// Este no va bien, pero antes s’!!!!
		ofVec2f proy;
		if(calib1.homographyReady) {
			proy = calib1.transf_Punto( ofVec2f(ofGetMouseX() - c1_Dst.x, ofGetMouseY() - c1_Dst.y), true ) ;
		}
		else {
			proy = ofVec2f(ofGetMouseX() - c1_Dst.x, ofGetMouseY() - c1_Dst.y);
		}
		ofSetColor(ofColor::green);
		ofPushMatrix();
		ofTranslate(c1_Src.x, c1_Src.y, 0);
		ofEllipse(proy.x, proy.y, 20,20);
		ofPopMatrix();
		
	}
	else if(c1_Src.inside(mouseX, mouseY)) {
		ofSetColor(ofColor::pink);
		ofNoFill();
		ofEllipse(ofGetMouseX(), ofGetMouseY(), 15,15);
		
		// me pasa el punto desde Src a Dst
		// Este va muy bien
		// Este no va bien, pero antes s’!!!!
		ofVec2f proy;
		if(calib1.homographyReady) {
			proy = calib1.transf_Punto( ofVec2f(ofGetMouseX() - c1_Src.x, ofGetMouseY() - c1_Src.y) , false);
		}
		else {
			proy = ofVec2f(ofGetMouseX() - c1_Src.x, ofGetMouseY() - c1_Src.y);
		}
		ofSetColor(ofColor::green);
		ofPushMatrix();
		ofTranslate(c1_Dst.x, c1_Dst.y, 0);
		ofEllipse(proy.x, proy.y, 20,20);
		ofPopMatrix();
	}
	// dibuja puntos y lineas que los unen
	calib1.drawPoints(c1_Dst, c1_Src);
	
	
	// CALIB PROY RGB
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
		// Este no va bien, pero antes s’!!!!
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
	
	// dibuja puntos y lineas que los unen
	calib2.drawPoints(c2_Dst, c2_Src);
	
	
	
	
	
	
	
	
	
	// draw instructions
	ofSetColor(255, 255, 255);
	stringstream reportStream;
	
    //	reportStream << "registration: " << ofToString(reg) << endl;
	
    if(kinect.hasAccelControl()) {
        reportStream << "accel is: " << ofToString(kinect.getMksAccel().x, 2) << " / "
        << ofToString(kinect.getMksAccel().y, 2) << " / "
        << ofToString(kinect.getMksAccel().z, 2) << endl;
    } else {
        reportStream << "Note: this is a newer Xbox Kinect or Kinect For Windows device," << endl
		<< "motor / led / accel controls are not currently supported" << endl << endl;
    }
    
	reportStream << ", fps: " << ofGetFrameRate() << endl
	<< "press c to close the connection and o to open it again, connection is: " << kinect.isConnected() << endl;
    
    if(kinect.hasCamTiltControl()) {
    	reportStream << "press UP and DOWN to change the tilt angle: " << angle << " degrees" << endl
        << "press 1-5 & 0 to change the led mode" << endl;
    }
    
	ofDrawBitmapString(reportStream.str(), 20, 652);
    
}

//--------------------------------------------------------------
void testApp::exit() {
	kinect.setCameraTiltAngle(0); // zero the tilt on exit
	kinect.close();
	
}

//--------------------------------------------------------------
void testApp::keyPressed (int key) {
	switch (key) {
			
		case 'w':
			kinect.enableDepthNearValueWhite(!kinect.isDepthNearValueWhite());
			break;
			
		case 'o':
			kinect.setCameraTiltAngle(angle); // go back to prev tilt
			kinect.open();
			break;
			
		case 'c':
			kinect.setCameraTiltAngle(0); // zero the tilt
			kinect.close();
			break;
			
		case '1':
			kinect.setLed(ofxKinect::LED_GREEN);
			break;
			
		case '2':
			kinect.setLed(ofxKinect::LED_YELLOW);
			break;
			
		case '3':
			kinect.setLed(ofxKinect::LED_RED);
			break;
			
		case '4':
			kinect.setLed(ofxKinect::LED_BLINK_GREEN);
			break;
			
		case '5':
			kinect.setLed(ofxKinect::LED_BLINK_YELLOW_RED);
			break;
			
		case '0':
			kinect.setLed(ofxKinect::LED_OFF);
			break;
			
		case OF_KEY_UP:
			angle++;
			if(angle>30) angle=30;
			kinect.setCameraTiltAngle(angle);
			break;
			
		case OF_KEY_DOWN:
			angle--;
			if(angle<-30) angle=-30;
			kinect.setCameraTiltAngle(angle);
			break;
			
            //		case 'r':
            //			reg = !reg;
            //			kinect.setRegistration(reg);
            //			break;
	}
	
	if(key == 's') {
		calib2.saveMatrix(nombre2);
	}
	else if(key=='l') {
		calib2.loadMatrix(nombre2);
	}
	
	// GGGG reset
	if(key=='r') {
		calib2.reset();
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
void testApp::mousePressed(int x, int y, int button)
{
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
		
		if(c2_Dst.inside(cur)) {
			calib2.addPts(cur-offsetDst, cur-offsetDst);
			// calib1 se encarga de updateHomogr
		}
		else if(c2_Src.inside(cur)) {
			calib2.addPts(cur-offsetSrc, cur-offsetSrc);
			// calib1 se encarga de updateHomogr
		}
		
		
	}
	
	else {
		// Si ha seleccionado algun punto anotar si es de Dst o de Src
		if(selectPtDst) isDstPt_c2 = true;
		else if(selectPtSrc) isDstPt_c2 = false;
	}
	
}

void testApp::mouseDragged(int x, int y, int button)
{
	if(movingPoint) {
		if(isDstPt_c2)  curPoint->set(x-c2_Dst.x, y-c2_Dst.y);
		else			curPoint->set(x-c2_Src.x, y-c2_Src.y);
		
		// update homography
		// en calib1
		calib2.calcHomography();
	}
}


void testApp::mouseReleased(int x, int y, int button) {
	movingPoint = false;
	calib2.calcHomography();
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h)
{}
