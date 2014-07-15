#include "scanner_faces.h"
#include "utilsProcamtool.h"

using namespace ofxCv;
using namespace cv;

void scanner_faces::setup() {
	ofSetVerticalSync(true);
	ofEnableSmoothing();
	
	// utils
	colores.push_back(ofColor::paleVioletRed);
	colores.push_back(ofColor::lightSeaGreen);
	colores.push_back(ofColor::deepSkyBlue);
	colores.push_back(ofColor::papayaWhip);
	colores.push_back(ofColor::antiqueWhite);
	colores.push_back(ofColor::powderBlue);
	
	//  modos Haar
	modos_haar.push_back("haars/haarcascade_frontalface_alt2.xml");	// 1 (va ok)
	modos_haar.push_back("haars/haarcascade_frontalface_alt_tree.xml");	// 4 (va)
	modos_haar.push_back("haars/haarcascade_eye.xml");						// 5 
	modos_haar.push_back("haars/haarcascade_mcs_eyepair_big.xml");		// 6 ¿?
//	modos_haar.push_back("haars/haarcascade_mcs_mouth.xml");			// 7 no
//	modos_haar.push_back("haars/haarcascade_mcs_nose.xml");			// 9 no
	modos_haar.push_back("haars/haarcascade_upperbody.xml");			// 2 (no va muy bien. Creo que hay que hacer tridente)
//	modos_haar.push_back("haars/haarcascade_profileface.xml");			// 3 (no va del todo bien ¿barba?)
	id_modo_haar_act = 0;
	
	setupObjFinders();
	
	// FaceTracker
	tracker.setup();
	
	doHaarFace = true;
	doFaceTracker = false;
	
	doDrawHaarFace = false;
	doDrawFaceTracker = false;
	
	int wCam = 640;
	int hCam = 480;
	
	// start
	cam.initGrabber(wCam, hCam);
	
	// setup marco
	float wMarco = cam.width*0.7;
	float hMarco = wMarco*cam.height/cam.width;
	float xMarco = (cam.width-wMarco)/2;
	float yMarco = (cam.height-hMarco)/2;
	
	
	marco.setRectExt(ofRectangle(0,0, cam.width, cam.height));
	//	marco.setRectExt(ofRectangle(0,0, ofGetWidth(), ofGetHeight()));
	marco.setRect(ofRectangle(xMarco, yMarco, wMarco, hMarco));

	// fbo para resultado de FT
	// realmente podria hacer el fbo mas pequeño, del tamaño de la región visible
	fboFT.allocate(wCam, hCam, GL_RGB);
	fboFT.begin();
	ofClear(0,255);
	fboFT.end();
	
	// fbo para el scaneo
	fboScan.allocate(wCam, hCam, GL_RGB);	// por ahora coge toda la imagen de la camara
	fboScan.begin();
	ofClear(0,255);
	fboScan.end();
	
	// timers
	setupTimers();
	
	// fotos
	// poner dimensiones de las imagenes
	setupFotos();
	
	// escenas
	setupEscenas();
	setEscena(SCN_WAITING);
	
	// Grabar imagenes
	nPersonaAct = 0;
	nZonaAct = 0;
	snd_click.loadSound("sounds/click_214632.aiff");
	
	
}

void scanner_faces::setupFotos() {
//	NFOTOSMAX = 3;
//	nFotoAct = 0;
	
	WSCAN = 200;
	HSCAN = 300;
	
//	focusPts.push_back(ofPoint(ofGetWidth()/2, ofGetHeight()/2));
//	focusPts.push_back(ofPoint(ofGetWidth()*1/5, ofGetHeight()/2));
//	focusPts.push_back(ofPoint(ofGetWidth()*4/5, ofGetHeight()/2));

	// margenes para la captura para haar visualizer
//	rectHaar.width = 400;
//	rectHaar.height = rectHaar.width/640*480;
//	rectHaar.x = 0;
//	rectHaar.y = (480-rectHaar.height)/2;
	float hhh = cam.height;
	float www = hhh*3/4;
	rectHaar = ofRectangle((ofGetWidth()-www)/2,0, www,hhh);
}


void scanner_faces::setupObjFinders() {
	finderSmoothRate = 0.15; // si es mayor tiembla mas, y si es menor va mas lento y suave
	//finder caras;
	//	finder.setup("haars/haarcascade_frontalface_alt2.xml");
	finder.setup(modos_haar[id_modo_haar_act]);
	finder.setPreset(ObjectFinder::Fast);
	finder.getTracker().setSmoothingRate(finderSmoothRate);
	
	//finder ojos;
	finderEyes.setup("haars/haarcascade_eye.xml");
	finderEyes.setPreset(ObjectFinder::Fast);
	swHaarEyes = false;
}


// - - - - 

void scanner_faces::update() {
	
	cam.update();
	
	if(cam.isFrameNew()) {

		if(doHaarFace) {
			bFaceDetected = false;
			update_haarFinder();
		}
		
		// update faceTracker
		if(doFaceTracker) {
			update_faceTracker();
		}
	}
	
	// Update una cosa u otra según el modo
	updateEscena();
	
}

void scanner_faces::update_haarFinder() {
	finder.update(cam);
	
	if(finder.size()>0) {
		bFaceDetected = true;
		
		faceRect_Tgt = finder.getObject(0);
		faceRect = finder.getObjectSmoothed(0);
		
		// Modifico el Rect
		// quedara en proporcion 3/2 = 15/10 
		//
		float dyUp  = 3*faceRect.height/10.0;	// 
		float dyDwn = 2*faceRect.height/10.0;	// 			
		faceRectAmpl = ofRectangle(faceRect.x, faceRect.y-dyUp, faceRect.width, faceRect.height+ dyUp+dyDwn);
		
		// update rectangulo para la imagen para HaarViz
//		rectHaar.x = faceRectAmpl.x+faceRectAmpl.width/2-rectHaar.width/2;	// suave pero llega tarde
		rectHaar.x = faceRect_Tgt.x+faceRect_Tgt.width/2-rectHaar.width/2;	// tiembla mucho
		
		
		if(swHaarEyes) {
			// Paso la imagen al finder de ojos
			faceImage.setFromPixels(cam.getPixels(), cam.width, cam.height, OF_IMAGE_COLOR );
			faceImage.crop(faceRectAmpl.x, faceRectAmpl.y, faceRectAmpl.width, faceRectAmpl.height);
			finderEyes.update(faceImage);
		}
		
		// check si la cara esta dentro de los margenes
		caraOut = marco.isOut(faceRectAmpl);
		
	}
	
}

void scanner_faces::update_faceTracker() {
	tracker.update(toCv(cam));
	
	position = tracker.getPosition();
	scale = tracker.getScale();
	rotationMatrix = tracker.getRotationMatrix();
	
	if(tracker.getFound()) {
		ofVec2f
		leftOuter = tracker.getImagePoint(36),
		leftInner = tracker.getImagePoint(39),
		rightInner = tracker.getImagePoint(42),
		rightOuter = tracker.getImagePoint(45);
		
		leftEye = tracker.getImageFeature(ofxFaceTracker::LEFT_EYE);
		rightEye = tracker.getImageFeature(ofxFaceTracker::RIGHT_EYE);
		
		ofVec2f leftCenter = leftEye.getBoundingBox().getCenter();
		ofVec2f rightCenter = rightEye.getBoundingBox().getCenter();
		
		float leftRadius = (leftCenter.distance(leftInner) + leftCenter.distance(leftOuter)) / 2;
		float rightRadius = (rightCenter.distance(rightInner) + rightCenter.distance(rightOuter)) / 2;

		// Preparar cuadritos en ojos
		
		// ModoII: Otra forma de hacer esto es con los puntos de imagen
		// Se toman los de los extremos de los ojos y se mide el angulo de la linea del ojo
		// a partir de ahi se forma el mesh.
		
		
		// Modo I: coger ptos 3d y proyectarlos. No sale bien: los rectangulos aparecen inclinados
		ofVec2f
		leftOuterObj = tracker.getObjectPoint(36),
		leftInnerObj = tracker.getObjectPoint(39),
		rightInnerObj = tracker.getObjectPoint(42),
		rightOuterObj = tracker.getObjectPoint(45);
		
		ofVec3f upperBorder(0, -3.5, 0), lowerBorder(0, 2.5, 0);
		ofVec3f leftDirection(-1, 0, 0), rightDirection(+1, 0, 0);
		float innerBorder = 1.5, outerBorder = 2.5;
		
		ofMesh leftRect, rightRect;
		leftRect.setMode(OF_PRIMITIVE_LINE_LOOP);
		leftRect.addVertex(leftOuterObj + upperBorder + leftDirection * outerBorder);
		leftRect.addVertex(leftInnerObj + upperBorder + rightDirection * innerBorder);
		leftRect.addVertex(leftInnerObj + lowerBorder + rightDirection * innerBorder);
		leftRect.addVertex(leftOuterObj + lowerBorder + leftDirection * outerBorder);
		rightRect.setMode(OF_PRIMITIVE_LINE_LOOP);
		rightRect.addVertex(rightInnerObj+ upperBorder + leftDirection * innerBorder);
		rightRect.addVertex(rightOuterObj + upperBorder + rightDirection * outerBorder);
		rightRect.addVertex(rightOuterObj + lowerBorder + rightDirection * outerBorder);
		rightRect.addVertex(rightInnerObj + lowerBorder + leftDirection * innerBorder);
		
		
		// Mouth
		featMouth = tracker.getObjectFeature(ofxFaceTracker::OUTER_MOUTH);
		ofRectangle mouthRectBox = featMouth.getBoundingBox();
		ofVec2f
		mULObj = ofVec2f(mouthRectBox.x,mouthRectBox.y),
		mURObj = ofVec2f(mouthRectBox.x+mouthRectBox.width,mouthRectBox.y),
		mDRObj = ofVec2f(mouthRectBox.x+mouthRectBox.width,mouthRectBox.y+mouthRectBox.height),
		mDLObj = ofVec2f(mouthRectBox.x,mouthRectBox.y+mouthRectBox.height);
//		ofVec2f
//		mLObj = tracker.getObjectPoint(48),
//		mUObj = tracker.getObjectPoint(51),
//		mRObj = tracker.getObjectPoint(42),
//		mDObj = tracker.getObjectPoint(57);
		ofMesh mRect;
		mRect.setMode(OF_PRIMITIVE_LINE_LOOP);
		mRect.addVertex(mULObj + upperBorder + leftDirection * outerBorder);
		mRect.addVertex(mURObj + upperBorder + rightDirection * outerBorder);
		mRect.addVertex(mDRObj + lowerBorder + rightDirection * outerBorder);
		mRect.addVertex(mDLObj + lowerBorder + leftDirection * outerBorder);
		
		ofPushMatrix();
		ofSetupScreenOrtho(640, 480, OF_ORIENTATION_UNKNOWN, true, -1000, 1000);
		ofScale(1, 1, -1);
		ofTranslate(position);
		applyMatrix(rotationMatrix);
		ofScale(scale, scale, scale);
		LEyeRect = getProjectedMesh(leftRect);
		REyeRect = getProjectedMesh(rightRect);
		mouthRect = getProjectedMesh(mRect);
		ofPopMatrix();
		
		// Dibujar fbo
		fboFT.begin();
		{
			ofClear(0, 100);
			// Dibujar la mascara
			//tracker.drawWireframe();
			//faceSolid.draw();
			// 	enum { FT_LINES, FT_WIREFRAME, FT_TEXTURE};
			if(modoDrawFT==FT_LINES) drawFaceTracker();
			else if(modoDrawFT==FT_WIREFRAME) {
				tracker.getImageMesh().drawWireframe();
//				ofMesh getObjectMesh() const;
//				ofMesh getMeanObjectMesh() const;
			}
			else if(modoDrawFT==FT_TEXTURE) {
				tracker.draw(true);
			}
		}
        fboFT.end();
		
	}
}


void scanner_faces::draw() {
	ofBackground(ofColor::black);
	// Dibujar capas
	ofSetColor(255);
	if(scnAct==SCN_WAITING) {
		draw_ScnWait();
	}
	else {
		
		// CONTENIDOS REDIMENSIONADOS Y CON FLIP_H
		float hh = ofGetScreenHeight();
		float ww = hh*cam.width/cam.height;
		
		//		cam.draw((ofGetWidth()-ww)/2,(ofGetHeight()-hh)/2, ww,hh);
		ofPushMatrix();
		{
//			ofTranslate((ofGetWidth()-ww)/2,(ofGetHeight()-hh)/2);
			// centrar la imagen y
			// flip H
			
			ofTranslate(ofGetWidth()-(ofGetWidth()-ww)/2,(ofGetScreenHeight()-hh)/2);
			ofScale(-ww/cam.width,hh/cam.height);
			
			cam.draw(0,0);

			// - - - - 
			// Este bloque debe ir enla zona de escenas 
			// 
			// la transicion será con shader
			//
			
			//	
			if(scnAct==SCN_PRESCAN) {
				// dibujar Haar finder
//				if(doDrawHaarFace && doHaarFace && show_HaarRect && !guidesFace) {
				if(show_HaarRect && !guidesFace) {
					draw_haarFinder();
				}
				// las guidelines las dibujare encima del marco
				
				
			}
			else if(scnAct==SCN_SCAN) {
				// Aqui hay que dibujar el fbo con la transicion de scan
				
				
				// mientras tanto dibujo todo
				if(doFaceTracker && doDrawFaceTracker) {
//					drawFaceTracker();
					fboFT.draw(0,0);
				}	
				if(doDrawHaarFace && doHaarFace) {
					draw_haarFinder();
				}				
				
			}
			else if(scnAct==SCN_FIN) {
				if(doFaceTracker && doDrawFaceTracker) {
//					drawFaceTracker();
					fboFT.draw(0,0);
				}
				
			}
			
			ofSetColor(255);
			marco.draw();
			
			// guidelines
//			drawGuideLines();
			if(scnAct==SCN_PRESCAN) {
				if(guidesFace) {
					ofLogNotice("DIbujar Face");
					ofPushStyle();
					ofSetColor(ofColor::lime, 200);
					ofSetLineWidth(1);

					ofLine(marco.rect.x+marco.rect.width+marco.gap, faceRectAmpl.y, faceRectAmpl.x, faceRectAmpl.y);
					ofLine(marco.rect.x+marco.rect.width+marco.gap, faceRectAmpl.y+faceRectAmpl.height, 
						   faceRectAmpl.x, faceRectAmpl.y+faceRectAmpl.height);

					ofLine(faceRectAmpl.x, faceRectAmpl.y, faceRectAmpl.x, marco.rect.y-marco.gap);
					ofLine(faceRectAmpl.x+faceRectAmpl.width, faceRectAmpl.y, 
						   faceRectAmpl.x+faceRectAmpl.width, marco.rect.y-marco.gap);
					
					ofSetLineWidth(2);
					ofRect(faceRectAmpl);
					
					// data
					
					
					ofPopStyle();
				}
				if(guidesEyes) {
					ofPushStyle();
					ofSetColor(ofColor::lightBlue, 220);
					draw_featEnMarco(leftEye,15, true, true);
					draw_featEnMarco(rightEye,15, true, true);
					
					ofPopStyle();
				}
				if(guidesBoca) {
					ofPushStyle();
					ofSetColor(ofColor::darkorange, 200);
					//featMouth
					ofPolyline featMouthImg = tracker.getImageFeature(ofxFaceTracker::OUTER_MOUTH);
					draw_featEnMarco(featMouthImg, 15, false, false);
//					ofRect(featMouthImg.getBoundingBox());
					
					ofPopStyle();	
				}
				if(guidesSkin) {
					ofPushStyle();
					
					ofPopStyle();					
				}
			}
			
			// marco con la imagen para HaarViz
			ofSetColor(255,0,255);
			ofNoFill();
			ofRect(rectHaar);
			ofSetColor(255);
			
		}
		ofPopMatrix();
		
		// MENSAJES E INFO EN PANTALLA
		if(scnAct==SCN_PRESCAN) {
			ofDrawBitmapStringHighlight("SCN_PRESCAN: ", ofGetWidth()-100, ofGetHeight()-40);
			mensajesT[idMensajeAct].draw(fontMensajes, fontMensajes_peque);
		}
		else if(scnAct==SCN_SCAN) {
			ofDrawBitmapStringHighlight("SCN_SCAN: ", ofGetWidth()-100, ofGetHeight()-40);
			
			
		}
		else if(scnAct==SCN_FIN) {
			ofDrawBitmapStringHighlight("SCN_FIN: ", ofGetWidth()-100, ofGetHeight()-40);
			
			
		}
		
		
		ofDrawBitmapStringHighlight("(w/s) haar: " + modos_haar[id_modo_haar_act], 10, 150);
		ofDrawBitmapStringHighlight("#objs: " + ofToString(finder.size()), 10, 170);
		
		
	}
	
	int hLin = 10;
	ofDrawBitmapStringHighlight("(t) FullScreen", 10,hLin); hLin+=20;
	ofDrawBitmapStringHighlight("(q) " + nombreEscena, 10,hLin); hLin+=20;
	ofDrawBitmapStringHighlight("(d) do HaarFace: " + ofToString(doHaarFace), 10,hLin); hLin+=20;
	ofDrawBitmapStringHighlight("(f) do FaceTracker: " + ofToString(doFaceTracker), 10,hLin); hLin+=20;
	ofDrawBitmapStringHighlight("(e) draw HaarFace: " + ofToString(doDrawHaarFace), 10,hLin); hLin+=20;
	ofDrawBitmapStringHighlight("(r) draw FaceTracker: " + ofToString(doDrawFaceTracker), 10,hLin); hLin+=20;
	ofDrawBitmapStringHighlight("fr: " + ofToString(ofGetFrameRate()), 10,hLin); hLin+=20;
	ofDrawBitmapStringHighlight("numCaras Acum: " + ofToString(contCaras), 10,hLin); hLin+=20;
	ofDrawBitmapStringHighlight("nPersonaAct: " + ofToString(nPersonaAct), 10, hLin); hLin+=20;
	ofDrawBitmapStringHighlight("nZonaAct: " + ofToString(nZonaAct), 10, hLin); hLin+=20;
	ofDrawBitmapStringHighlight("(123) modo Draw FT: " + ofToString(modoDrawFT), 10, hLin); hLin+=20;
	
	// cuadrito esquina UR piloto cara_haar detectada
	if(hayPersona()) {
		ofPushStyle();
		ofSetColor(ofColor::darkGreen);
		ofFill();
		ofRect(ofGetWidth()-30,0,30,30);
		ofPopStyle();
	}
	if(tracker.getFound()) {
		ofPushStyle();
		ofSetColor(ofColor::mediumBlue);
		ofFill();
		ofRect(ofGetWidth()-60,0,30,30);
		ofPopStyle();
	}
	
	
}

void scanner_faces::draw_featEnMarco(ofPolyline feat, int gap, bool uu, bool rr) {
	
	ofRectangle featRect;
	featRect = feat.getBoundingBox();
	featRect.x-=gap;
	featRect.y-=gap;
	featRect.width+=2*gap;
	featRect.height+=2*gap;
	ofSetLineWidth(1);
	if(rr) {
		ofLine(marco.rect.x-marco.gap, featRect.y, featRect.x, featRect.y);
		ofLine(marco.rect.x-marco.gap, featRect.y+featRect.height, featRect.x, featRect.y+featRect.height);
	}
	else {
		ofLine(marco.rect.x+marco.rect.width+marco.gap, featRect.y, featRect.x, featRect.y);
		ofLine(marco.rect.x+marco.rect.width+marco.gap, featRect.y+featRect.height, featRect.x, featRect.y+featRect.height);
	}
	
	if(uu) {
		ofLine(featRect.x, featRect.y, featRect.x, marco.rect.y-marco.gap);
		ofLine(featRect.x+marco.rect.width, featRect.y, featRect.x+marco.rect.width+marco.gap, featRect.y);
	}
	else {
		ofLine(featRect.x, featRect.y, featRect.x, marco.rect.y+marco.rect.height+marco.gap);
		ofLine(featRect.x+featRect.width, featRect.y, 
			   featRect.x+featRect.width, marco.rect.y+marco.rect.height+marco.gap);
	}
	ofSetLineWidth(2.5);
	ofRect(featRect);
	
}


void scanner_faces::draw_haarFinder() {
	//		finder.draw();
	if(finder.size()>0) {
		//			ofRectangle caraRect = finder.getObjectSmoothed(0);
		ofNoFill();
		ofRect(faceRect);
		
		// Parrilla
		// H
		for(int i=1; i<10; i++) {
			ofLine(faceRect.x, faceRect.y+faceRect.height/10*i , faceRect.x+faceRect.width, faceRect.y+faceRect.height/10*i);
		}
		// V
		ofLine(faceRect.x+faceRect.width/2, faceRect.y, faceRect.x+faceRect.width/2, faceRect.y+faceRect.height);
		ofSetColor(200);
		ofLine(faceRect.x+faceRect.width/3, faceRect.y, faceRect.x+faceRect.width/3, faceRect.y+faceRect.height);
		ofLine(faceRect.x+faceRect.width*2/3, faceRect.y, faceRect.x+faceRect.width*2/3, faceRect.y+faceRect.height);
		ofSetColor(150);
		ofLine(faceRect.x+faceRect.width/4, faceRect.y, faceRect.x+faceRect.width/4, faceRect.y+faceRect.height);
		ofLine(faceRect.x+faceRect.width*3/4, faceRect.y, faceRect.x+faceRect.width*3/4, faceRect.y+faceRect.height);
		
		
		// Colores: http://www.html-color-names.com/color-chart.php
		if(posicionOK()) 	ofSetColor(ofColor::limeGreen);
		else				ofSetColor(ofColor::crimson);
		
		ofPushStyle();
		ofSetLineWidth(10);
		ofRectRounded(faceRectAmpl, 2);
		ofPopStyle();
		
		ofSetColor(255);
		// Eyes
		if(swHaarEyes) {
			ofRectangle caraRect = finder.getObjectSmoothed(0);
			ofPushMatrix();
			ofTranslate(caraRect.x, caraRect.y,0);
			
			if(finderEyes.size()>0) {
				ofPushStyle();
				ofSetColor(ofColor::red);
				ofSetLineWidth(2);
				finderEyes.draw();
				ofPopStyle();
			}
			ofPopMatrix();
		}
	}
	
}

void scanner_faces::drawFaceTracker() {

	tracker.draw(true);
	// 
	ofSetColor(0,200,0);
	ofPolyline featMouthImg = tracker.getImageFeature(ofxFaceTracker::OUTER_MOUTH);
	featMouthImg.draw();
	ofNoFill();
	ofRect(featMouthImg.getBoundingBox());
	
	// dibujar meshes de la cara
	ofSetColor(ofColor::seaGreen);
	LEyeRect.draw();
	REyeRect.draw();
	mouthRect.draw();

}


//--------------------------------------------------------------
void scanner_faces::keyPressed(int key) {
	
	if(key == 'v') {
		cam.videoSettings();
	}
	else if(key == 't') ofToggleFullscreen();
	
	// Presets ObjectFinder
	if(key=='1') {
		modoDrawFT = FT_LINES;
	}
	else if(key=='2') {
		modoDrawFT = FT_WIREFRAME;		
	}
	else if(key=='3') {
		modoDrawFT = FT_TEXTURE;
	}
	
		
	if(key=='6') {
		finder.setPreset(ObjectFinder::Fast);
	}
	else if(key=='7') {
		finder.setPreset(ObjectFinder::Accurate);
	}
	else if(key=='8') {
		finder.setPreset(ObjectFinder::Sensitive);
	}
	
	else if(key=='d') doHaarFace=!doHaarFace;
	else if(key=='f') doFaceTracker=!doFaceTracker;
	else if(key=='e') doDrawHaarFace=!doDrawHaarFace;
	else if(key=='r') doDrawFaceTracker=!doDrawFaceTracker;
	
	if(key=='q') {
		nextEscena();
	}
	if(key=='a') {
		scnAct--;
		if(scnAct<0) scnAct+=4;
		setEscena(scnAct);
	}
	
	if(key=='w') {
		id_modo_haar_act++;
		id_modo_haar_act%=modos_haar.size();
		finder.setup(modos_haar[id_modo_haar_act]);
	}
	else if(key=='s') {
		id_modo_haar_act--;
		if(id_modo_haar_act<0) id_modo_haar_act+=modos_haar.size();;
		finder.setup(modos_haar[id_modo_haar_act]);
	}
	
	else if(key=='i') {
		// grabar imagen
		hacerFoto(); // Ya suma nZonaAct++ si se hace la foto
	}
	
	
}

// - - - 

void scanner_faces::hacerFoto() {
	
	if(hayPersona()) {
		faceImage.setFromPixels(cam.getPixels(), cam.width, cam.height, OF_IMAGE_COLOR );
		faceImage.crop(faceRectAmpl.x, faceRectAmpl.y, faceRectAmpl.width, faceRectAmpl.height);
		saveScanImg(faceImage, nPersonaAct, nZonaAct);
		nZonaAct++;
	}				
	
}



void scanner_faces::saveScanImg(ofImage &imgScan, int nPers, int nImg) {
	
	string nmImgScan = "images/scan/";
	nmImgScan += ofToString(ofGetYear())+ofToString(ofGetMonth(),2,'0')+ofToString(ofGetDay(),2,'0')+
	ofToString(ofGetHours(),2,'0')+ofToString(ofGetMinutes(),2,'0');
	nmImgScan += "_cara_"+ofToString(nPers,3,'0')+"_"+ofToString(nImg,2,'0')+".png";
	ofLogNotice("grabar:_"+nmImgScan);
	
	// redimensionar
	imgScan.resize(WSCAN, HSCAN);
	imgScan.saveImage(nmImgScan);
	
	snd_click.play();
	
}

void scanner_faces::hacerFoto_haarViz() {

	if(hayPersona()) {
		faceImage.setFromPixels(cam.getPixels(), cam.width, cam.height, OF_IMAGE_COLOR );
		faceImage.crop(rectHaar.x, rectHaar.y, rectHaar.width, rectHaar.height);
		saveScanImg2HaarViz(faceImage, nPersonaAct);
	}
}


void scanner_faces::saveScanImg2HaarViz(ofImage &imgScan, int nPers) {
	
	string nmImgScan = "images/scan_2_haarViz/";
	nmImgScan += ofToString(ofGetYear())+ofToString(ofGetMonth(),2,'0')+ofToString(ofGetDay(),2,'0')+
	ofToString(ofGetHours(),2,'0')+ofToString(ofGetMinutes(),2,'0');
	nmImgScan += "_cara_"+ofToString(nPers,3,'0')+".png";
	ofLogNotice("grabar:_"+nmImgScan);
	
	imgScan.saveImage(nmImgScan);
	
	snd_click.play();
	
}


// - - - 


//
bool	scanner_faces::hayPersona() {
//	return (bFaceDetected || tracker.getFound());
	
	return bFaceDetected;
}

bool	scanner_faces::posicionOK() {
	caraOut = marco.isOut(faceRectAmpl);
	return !caraOut;
}


// - - - ARDUINO - - - 

void	scanner_faces::setLight(bool swlight) {
	
	
}
