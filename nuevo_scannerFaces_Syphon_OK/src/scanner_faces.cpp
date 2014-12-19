#include "scanner_faces.h"
#include "utilsProcamtool.h"

using namespace ofxCv;
using namespace cv;

void scanner_faces::setup() {
	ofSetVerticalSync(true);
	ofEnableSmoothing();
	
    ofSetLogLevel(OF_LOG_NOTICE);
    
    setupSerial();

    setupVarios();
	
    
    // **********
    // Dimensiones del monitor:
    //
    
#ifdef SYPHON_CAM	
    wMonitor = 1920;	// pantalla full HD
    hMonitor = 1080;	
	esc_HD2Mon = 1;
#else 
    wMonitor = 1200;	// Mi equipo
    hMonitor = 675; //720;	
	esc_HD2Mon = wMonitor/1920.0;
#endif
    
	//	***************
	fondo.loadImage("media/fondo_escaner.png");
	fondo.resize(wMonitor,hMonitor);
    ofLogNotice() << "w,h: " << ofGetWidth() << "," << ofGetHeight();

	//
	//	*************** POSICIONAR EN PANTALLA ************
	// El fondo es para dimension de 1920x1080
	//
	// Lo paso a 1280x675 (mi pantalla) con esc_HD2Mon
	//
	rectCamera   = ofRectangle(567*esc_HD2Mon, 256*esc_HD2Mon,	786*esc_HD2Mon, 786*esc_HD2Mon);
	rectMensajes = ofRectangle(567*esc_HD2Mon, 35*esc_HD2Mon,	786*esc_HD2Mon, 145*esc_HD2Mon);
	
//	rectData
//	rectCara
//	rectOjos
//	rectBoca
//	rectHistograma
	

	
	setupObjFinders();
	
	// FaceTracker
	tracker.setup();
	
	
	doHaarFace = true;
	doFaceTracker = false;
	
	doDrawHaarFace = false;
	doDrawFaceTracker = false;
	
	
	numModosDrawFT = 4;
	
	// start
#ifndef SYPHON_CAM
	wCam = 640.0;
	hCam = 480.0;
    cam.listDevices();
	cam.initGrabber(wCam, hCam);
    
#else
	wCam = 1024.0;
	hCam = 1024.0;
	camSyphon.setup();
    camSyphon.set("CapturaCamara",""); // serverName, AppName
    
    fboSyphon.allocate(wCam, hCam, GL_RGB);
    fboSyphon.begin();
    ofClear(0);
    fboSyphon.end();
    
#endif
	
	// 
	
	
    // setup marco	
//    float wMarco = wCam*1.0;//0.85;
//	float hMarco = wMarco*hCam/wCam;
//	float xMarco = (wCam-wMarco)/2;
//	float yMarco = (hCam-hMarco)/2;

    float wMarco = wCam;//rectCamera.width;//0.85;
	float hMarco = wMarco*hCam/wCam;
	float xMarco = 0;
	float yMarco = 0;
	
	marco.setRectExt(ofRectangle(0,0, wCam, hCam));
	marco.setRect(ofRectangle(xMarco, yMarco, wMarco, hMarco));
	
	// fbo para resultado de FT
	// realmente podria hacer el fbo mas pequeño, del tamaño de la región visible
	fboFT.allocate(wCam, hCam, GL_RGBA);
	fboFT.begin();
	ofClear(0,255);
	fboFT.end();
	
	// fbo para el scaneo
	fboScan.allocate(wCam, hCam, GL_RGBA);	// por ahora coge toda la imagen de la camara
	fboScan.begin();
	ofClear(0,255);
	fboScan.end();
	
	// imagen para flip camara
	camImage.allocate(wCam,hCam, OF_IMAGE_COLOR);
	bFlipH = false;
	bFlipV = false;
	
	// timers
	setupTimers();
	
	// fotos
	// poner dimensiones de las imagenes
	setupFotos();
	
	// escenas
	setupEscenas();
	setEscena(SCN_WAITING);
	
	// Grabar imagenes
	nPersonaAct = 0;	// el idVisitante habrá que guardarlo
	nZonaAct = 0;
	snd_click.loadSound("sounds/click_214632.aiff");
		
	swFullScreen = true;
	swShowInfo = false;
    setupGUI();

	gui->disable();
	
    // Light ON
    setLight(true);   // Esto no va del todo bien (la app no debe estar preparado el serial o algo asi)
    bSendSerialMessage = true;
    charSend = 'i';
}

void scanner_faces::setupSerial() {
    serial.listDevices();
	vector <ofSerialDeviceInfo> deviceList = serial.getDeviceList();
	int baud = 9600;
	serial.setup(0, baud); //open the first device
}

void scanner_faces::setupVarios() {
    // utils
//    colores.push_back(ofColor::paleVioletRed);
//    colores.push_back(ofColor::lightSeaGreen);
//    colores.push_back(ofColor::deepSkyBlue);
//    colores.push_back(ofColor::papayaWhip);
//    colores.push_back(ofColor::antiqueWhite);
//    colores.push_back(ofColor::powderBlue);

    colores.push_back(ofColor::blue);
    colores.push_back(ofColor::darkBlue);
    colores.push_back(ofColor::black);
    colores.push_back(ofColor::white);
	
	
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
}

void scanner_faces::setupFotos() {
	// Las fotos para HaarViz
    // salen con Proporcion W/H 3/4
    float proporWH = 3.0/4.0;
    
//	NFOTOSMAX = 3;
//	nFotoAct = 0;
	
	WSCAN = 400;//200;
	HSCAN = 400;//300;
	
	// Fotos para el mosaico 
	// Serán cuadradas. Con tenerlas a resolución 384x384 valdría, pero se puede gardar a más reslución 600x600 o 768x768
	ladoCaraMosaico = 400;	// Genero imágenes de resolución 400x400
	
	
	// margenes para la captura para haar visualizer
    // La dimension final será 480x640.
    // esto es 3x4

	rectHaar.width = hCam*proporWH; // 768;//480;//400;
	rectHaar.height = hCam; // 1024;//640;//rectHaar.width/640*480;
	rectHaar.x = (wCam-rectHaar.width)/2;// (1024-768)/2;
	rectHaar.y = 0;//(480-rectHaar.height)/2;
	
}


void scanner_faces::setupObjFinders() {
	finderSmoothRate = 0.10; // si es mayor tiembla mas, y si es menor va mas lento y suave
	
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


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void scanner_faces::update() {
	
    
    bool    nuevaImagen = false;
    
#ifndef SYPHON_CAM
	cam.update();
	
	if(cam.isFrameNew()) {
        nuevaImagen = true;
		// Flip Cam
		// http://forum.openframeworks.cc/t/reversing-mirroring-video-input-to-ofxfacetracker/8701/3
		//
        //		if(bFlipH || bFlipV) {
		camImage.setFromPixels(cam.getPixelsRef());
		camImage.mirror(bFlipV, bFlipH);
        //		}
    }
#else
    // pasar de Syphon a camImage
    if(camSyphon.getWidth()>0) {
        nuevaImagen = true;
        
        fboSyphon.begin();
        ofSetColor(255);
        camSyphon.draw(0,0);
        fboSyphon.end();
        
        fboSyphon.readToPixels(camImage.getPixelsRef());
        camImage.update();
//        ofLogNotice() << "Paso de ofFbo a ofImage";
        
    }
    
#endif
    
    
    if(nuevaImagen) {
		// HaarFinder
		if(doHaarFace) {
			bFaceDetected = false;
			bFTDetected = false;
			update_haarFinder();
		}
		
		// update faceTracker
		if(doFaceTracker) {
			bFTDetected = false;
			update_faceTracker();
		}
	}
	
	// Update una cosa u otra según el modo
	updateEscena();
	
}

void scanner_faces::update_Serial() {
	if (bSendSerialMessage){
		
        ofLogNotice() << "Update_Serial  " << charSend;
        
		// Escribir orden en serial
		serial.writeByte(charSend);
		
		bSendSerialMessage = false;
        
	}
}

void scanner_faces::update_haarFinder() {
    //	finder.update(cam);
	finder.update(camImage);
	
	if(finder.size()>0) {
		bFaceDetected = true;
		
		faceRect_Tgt = finder.getObject(0);
		faceRect = finder.getObjectSmoothed(0);
		
		// Modifico el Rect
		// quedara en proporcion 2/3 = 10/15
		//
		float dyUp  = 3*faceRect.height/10.0;	//
		float dyDwn = 2*faceRect.height/10.0;	//
		if(faceRect.y-dyUp<0) {
			// si se sale por arriba
			dyUp = 0;
			dyDwn = faceRect.height*5/10;
		}
		if(faceRect.y+faceRect.height+dyDwn>camImage.height) {
			// si se sale por arriba
			dyUp = faceRect.height*5/10;
			dyDwn = 0;
		}
		if(faceRect.y-dyUp<0 && faceRect.y+faceRect.height+dyDwn>camImage.height) {
		   // Si despues de las correcciones se sale por los dos lados (cara muy cerca)
			float www = faceRect.height/15*10;
			
			
		}
		else {
			faceRectAmpl = ofRectangle(faceRect.x, faceRect.y-dyUp, faceRect.width, faceRect.height+ dyUp+dyDwn);
		}
		
		
		// update rectangulo para la imagen para HaarViz
        //		rectHaar.x = faceRectAmpl.x+faceRectAmpl.width/2-rectHaar.width/2;	// suave pero llega tarde
		rectHaar.x = faceRect_Tgt.x-faceRect_Tgt.width/2;	// tiembla mucho
		
		
		if(swHaarEyes) {
			// Paso la imagen al finder de ojos
//			faceImage.setFromPixels(cam.getPixels(), cam.width, cam.height, OF_IMAGE_COLOR );
			faceImage.setFromPixels(camImage.getPixels(), camImage.getWidth(), camImage.getHeight(), OF_IMAGE_COLOR );
			faceImage.crop(faceRectAmpl.x, faceRectAmpl.y, faceRectAmpl.width, faceRectAmpl.height);
			finderEyes.update(faceImage);
		}
		
		//
		// QUITO ESTO PORQUE LO QUE DEBE HACER LA GENTE ES PONER LA CARA EN CAMARA
		//
		// check si la cara esta dentro de los margenes
//		caraOut = marco.isOut(faceRectAmpl);
		
	}
	
}

void scanner_faces::update_faceTracker() {
	// Actualiza FT y el ofFBO con el resultado 3D
	
    //	tracker.update(toCv(cam));
	tracker.update(toCv(camImage));
	

	
	
	position = tracker.getPosition();
	scale = tracker.getScale();
	rotationMatrix = tracker.getRotationMatrix();
	
	if(tracker.getFound()) {
		bFTDetected = true;
		// Coger recuadros de los componentes 
		
		// Posiciones de los ojos
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
		
		//
		// Boca
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
		

		// Recálculo de las meshes segun la orientacion
		if(doProjectFT) {
			ofPushMatrix();
//			ofSetupScreenOrtho(640, 480, OF_ORIENTATION_UNKNOWN, true, -1000, 1000);
			ofSetupScreenOrtho(wCam, hCam, OF_ORIENTATION_UNKNOWN, true, -1000, 1000);
			ofScale(1, 1, -1);
			ofTranslate(position);
			applyMatrix(rotationMatrix);
			ofScale(scale, scale, scale);
			LEyeRect = getProjectedMesh(leftRect);
			REyeRect = getProjectedMesh(rightRect);
			mouthRect = getProjectedMesh(mRect);
			ofPopMatrix();
		}

		
		//
		// Dibujar fbo con la máscara
		//
		fboFT.begin();
		{
			ofClear(0, 100);
			drawFT();
		}
        fboFT.end();
		
	}
}

void scanner_faces::drawFT() {

	// Dibujar la mascara
	//tracker.drawWireframe();
	//faceSolid.draw();
	// 	enum { FT_LINES, FT_WIREFRAME, FT_TEXTURE};
	ofPushStyle();
	ofEnableSmoothing();
	float chch = ofRandom(1.0);
	modoDrawFT += (chch<0.01)? 1:0;
	modoDrawFT%=numModosDrawFT;
	
	if(modoDrawFT==FT_LINES) {
		drawFaceTracker();
	}
	else if(modoDrawFT==FT_WIREFRAME) {
        ofSetLineWidth(2.0);
		tracker.getImageMesh().drawWireframe();
//			ofMesh getObjectMesh() const;
//			ofMesh getMeanObjectMesh() const;
	}
	else if(modoDrawFT==FT_TEXTURE) {
        ofSetLineWidth(2.0);
		tracker.draw(true);
	}
	else if(modoDrawFT==FT_OUTLINE) {
		ofSetLineWidth(2.0);
		tracker.getImageFeature(ofxFaceTracker::ALL_FEATURES).draw();	// FACE_OUTLINE
	}
	ofDisableSmoothing();
	ofPopStyle();
}
	
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void scanner_faces::draw() {
	ofBackground(ofColor::black);
	// Dibujar capas
	ofSetColor(255);
    
	fondo.draw(0,0);

#ifdef SYPHON_CAM
    ofPushMatrix();
    ofTranslate((ofGetWidth()-camImage.width)/2,0);
    
    camSyphon.draw(-1,-1, 1,1);   // Cagate lorito. Si no se pone esto, no dibuja camImage
    
    ofPopMatrix();
    
#endif

    
    if(true) {
    if(scnAct==SCN_WAITING) {
        draw_ScnWait();
    }
    else {
		// Imagen de la cámara (color)
        camImage.draw(rectCamera);
        
//        ofEnableAlphaBlending();
		ofEnableSmoothing();
		
        //
		ofPushMatrix();
        ofTranslate(rectCamera.x,rectCamera.y);
        ofScale(rectCamera.width/wCam,rectCamera.height/hCam);
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
            
			ofPushStyle();
			float ll = (float)(ofGetElapsedTimeMillis()%1000)/1000.0;	// lerp
			ofSetColor(ofColor::red);
			ofSetLineWidth(2.0);
//			ofLine(0,rectCamera.height*ll, rectCamera.width,rectCamera.height*ll);
			ofLine(0,hCam*ll, wCam,hCam*ll);
			ofPopStyle();
			
            
            // mientras tanto dibujo todo
            if(doFaceTracker && doDrawFaceTracker) {
                //					drawFaceTracker();
//                fboFT.draw(0,0);
				drawFT();
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
        
        // guidelines
        //	drawGuideLines();
        if(scnAct==SCN_PRESCAN) {
            if(guidesFace) {
//                ofLogNotice("DIbujar Face");
                ofPushStyle();
                ofSetColor(ofColor::lime, 200);
					
				// líneas hasta el marco
                ofSetLineWidth(0.7);
//                ofLine(marco.rect.x+marco.rect.width+marco.gap, faceRectAmpl.y, faceRectAmpl.x, faceRectAmpl.y);
//                ofLine(marco.rect.x+marco.rect.width+marco.gap, faceRectAmpl.y+faceRectAmpl.height,
//                       faceRectAmpl.x, faceRectAmpl.y+faceRectAmpl.height);                
                ofLine(marco.rect.x+marco.rect.width+marco.gap, faceRectAmpl.y+faceRectAmpl.height/2.0, 
					   faceRectAmpl.x, faceRectAmpl.y+faceRectAmpl.height/2.0);				
				
				
                ofLine(faceRectAmpl.x, faceRectAmpl.y, faceRectAmpl.x, marco.rect.y-marco.gap);
                ofLine(faceRectAmpl.x+faceRectAmpl.width, faceRectAmpl.y,
                       faceRectAmpl.x+faceRectAmpl.width, marco.rect.y-marco.gap);
                
				
				// Rectángulo cara
                ofSetLineWidth(2);
                ofRect(faceRectAmpl);
                
				// Rectangulo cara_Tgt (actual, sin suavizado)
                ofSetColor(ofColor::lime, 250);
                ofRect(faceRect_Tgt);
				
                // data
                
                
                ofPopStyle();
            }
            if(guidesEyes) {
                ofPushStyle();
                ofSetColor(ofColor::lightBlue, 220);
                draw_featEnMarco(leftEye,15, true, true);	// (FT_feat, gap (antes 15), uu, rr)
                draw_featEnMarco(rightEye,15, true, false);
                
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
        }
		marco.draw();
        ofPopMatrix();

		
		ofDisableSmoothing();
//		ofDisableAlphaBlending();

        
        
    }
    }	// fin if(true)
	
    

	// MENSAJES E INFO EN PANTALLA
	// ESCENA
//	mensajesT[idMensajeAct].drawImage(rectMensajes);
	if(scnAct==SCN_PRESCAN) {
		mensajesT[idMensajeAct].drawImage(rectMensajes);
	}
	else if(scnAct==SCN_SCAN) {
	}
	else if(scnAct==SCN_FIN) {
	}
		
		
	if(swShowInfo) {
		if(scnAct==SCN_PRESCAN) {
			ofDrawBitmapStringHighlight("SCN_PRESCAN: ", ofGetWidth()-100, ofGetHeight()-40);
			//			mensajesT[idMensajeAct].draw(fontMensajes, fontMensajes_peque);
			mensajesT[idMensajeAct].drawImage(rectMensajes);
		}
		else if(scnAct==SCN_SCAN) {
			ofDrawBitmapStringHighlight("SCN_SCAN: ", ofGetWidth()-100, ofGetHeight()-40);
		}
		else if(scnAct==SCN_FIN) {
			ofDrawBitmapStringHighlight("SCN_FIN: ", ofGetWidth()-100, ofGetHeight()-40);
		}
		int hLin = ofGetHeight()/2;
		ofDrawBitmapStringHighlight("(t) FullScreen", 10,hLin); hLin+=20;
		ofDrawBitmapStringHighlight("(q) " + nombreEscena, 10,hLin); hLin+=20;
	//	ofDrawBitmapStringHighlight("(i) FlipH : " + ofToString(bFlipH), 10,hLin); hLin+=20;
	//	ofDrawBitmapStringHighlight("(o) FlipV : " + ofToString(bFlipV), 10,hLin); hLin+=20;
		ofDrawBitmapStringHighlight("(d) do HaarFace: " + ofToString(doHaarFace), 10,hLin); hLin+=20;
		ofDrawBitmapStringHighlight("(f) do FaceTracker: " + ofToString(doFaceTracker), 10,hLin); hLin+=20;
		ofDrawBitmapStringHighlight("(e) draw HaarFace: " + ofToString(doDrawHaarFace), 10,hLin); hLin+=20;
		ofDrawBitmapStringHighlight("(r) draw FaceTracker: " + ofToString(doDrawFaceTracker), 10,hLin); hLin+=20;
		ofDrawBitmapStringHighlight("fr: " + ofToString(ofGetFrameRate()), 10,hLin); hLin+=20;
		ofDrawBitmapStringHighlight("numCaras Acum: " + ofToString(contCaras), 10,hLin); hLin+=20;
		ofDrawBitmapStringHighlight("nPersonaAct: " + ofToString(nPersonaAct), 10, hLin); hLin+=20;
		ofDrawBitmapStringHighlight("nZonaAct: " + ofToString(nZonaAct), 10, hLin); hLin+=20;
		ofDrawBitmapStringHighlight("(123) modo Draw FT: " + ofToString(modoDrawFT), 10, hLin); hLin+=20;

		ofDrawBitmapStringHighlight("(i/o) Luces ON/OFF", 10,hLin); hLin+=20;
		ofDrawBitmapStringHighlight("(t/g) Luz +10/-10", 10,hLin); hLin+=20;
		ofDrawBitmapStringHighlight("(y/h) Luz +1/-1", 10,hLin); hLin+=20;
	}
    
	
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
		ofLine(marco.rect.x-marco.gap, featRect.y+featRect.height/2.0, featRect.x, featRect.y+featRect.height/2.0);
	}
	else {
		ofLine(marco.rect.x+marco.rect.width+marco.gap, featRect.y+featRect.height/2.0, featRect.x+featRect.width, featRect.y+featRect.height/2.0);
	}
	
	if(uu) {
		ofLine(featRect.x+marco.rect.width/2.0, featRect.y, featRect.x+marco.rect.width/2.0+marco.gap, featRect.y);
	}
	else {
		ofLine(featRect.x+featRect.width/2.0, featRect.y+featRect.height, 
			   featRect.x+featRect.width/2.0, marco.rect.y+marco.rect.height+marco.gap);
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


// - - - - - 

void	scanner_faces::setLight(bool swlight) {
    bSendSerialMessage = true;
    if(swlight)   charSend = 'i';
    else          charSend = 'o';
    
//    serial.writeByte(charSend);
    serial.writeByte('i');

    ofLogNotice() << "setLight " << swlight;
    
    bSendSerialMessage = false;
}



// - - -

void scanner_faces::hacerFoto() {
	
	if(hayPersona()) {
//		faceImage.setFromPixels(cam.getPixels(), cam.width, cam.height, OF_IMAGE_COLOR );
		faceImage.setFromPixels(camImage.getPixels(), camImage.getWidth(), camImage.getHeight(), OF_IMAGE_COLOR );
//		faceImage.crop(faceRectAmpl.x, faceRectAmpl.y, faceRectAmpl.width, faceRectAmpl.height);
		faceImageAmpl.cropFrom(faceImage, faceRectAmpl.x, faceRectAmpl.y, faceRectAmpl.width, faceRectAmpl.height);
		faceImage.crop(faceRect_Tgt.x, faceRect_Tgt.y, faceRect_Tgt.width, faceRect_Tgt.height);
		
		faceImage.resize(ladoCaraMosaico, ladoCaraMosaico);
		
		saveScanImg(faceImage, nPersonaAct, nZonaAct);
		nZonaAct++;
	}				
	
}

void scanner_faces::saveScanImg(ofImage &imgScan, int nPers, int nImg) {
	
	string nmImgScan = ofToString( getenv("HOME") ) + "/fotosCarasMosaico/1x1/";
//	string nmImgScan = "images/scan/";
	nmImgScan += ofToString(ofGetYear())+ofToString(ofGetMonth(),2,'0')+ofToString(ofGetDay(),2,'0')+
	ofToString(ofGetHours(),2,'0')+ofToString(ofGetMinutes(),2,'0');
	nmImgScan += "_cara_"+ofToString(nPers,3,'0')+"_"+ofToString(nImg,2,'0')+".png";
	ofLogNotice("grabar:_"+nmImgScan);
	
	// redimensionar
	imgScan.resize(WSCAN, HSCAN);
	imgScan.saveImage(nmImgScan);
	
	snd_click.play();
	
}


void scanner_faces::hacerFoto_CamMesh() {
	if(bFTDetected) {
		string nmImgScan, nmMeshImg, nmMeshObj, nmMeshMObj;
		
//		string directImgs = "images/cam/";
		string directImgs = ofToString( getenv("HOME") ) + "/fotosCarasMosaico/mesh/";
		
		nmImgScan = directImgs;
		string chorraco = ofToString(ofGetYear())+ofToString(ofGetMonth(),2,'0')+ofToString(ofGetDay(),2,'0')+
		ofToString(ofGetHours(),2,'0')+ofToString(ofGetMinutes(),2,'0');
		nmImgScan += chorraco + "_cam_"+ofToString(nPersonaAct,3,'0')+".png";
		
		camImage.saveImage(nmImgScan);
		
		nmMeshImg = directImgs + chorraco + "_imgMesh_"+ofToString(nPersonaAct,3,'0')+".ply";
		nmMeshObj = directImgs + chorraco + "_imgObj_"+ofToString(nPersonaAct,3,'0')+".ply";
		nmMeshMObj = directImgs + chorraco + "_imgMObj_"+ofToString(nPersonaAct,3,'0')+".ply";
		// guardar mesh
		tracker.getImageMesh().save(nmMeshImg);
		tracker.getObjectMesh().save(nmMeshObj);
		tracker.getMeanObjectMesh().save(nmMeshMObj);
		
	}
}


void scanner_faces::hacerFoto_haarViz() {
	// Guardar la foto para HaarViz
	if(hayPersona()) {
//		faceImage.setFromPixels(cam.getPixels(), cam.width, cam.height, OF_IMAGE_COLOR );
		faceImage.setFromPixels(camImage.getPixels(), camImage.getWidth(), camImage.getHeight(), OF_IMAGE_COLOR );
//		faceImageAmpl.cropFrom(faceImage, rectHaar);
		faceImage.crop(rectHaar.x, rectHaar.y, rectHaar.width, rectHaar.height);
        faceImage.resize(480,640);//
		saveScanImg2HaarViz(faceImage, nPersonaAct);
	}
	
}


void scanner_faces::saveScanImg2HaarViz(ofImage &imgScan, int nPers) {
	
	string nmImgScan = "images/scan_2_haarViz/";
	string nmImgScan1 = ofToString( getenv("HOME") ) + "/fotosHaarViz/";
	string chorraco = ofToString(ofGetYear())+ofToString(ofGetMonth(),2,'0')+ofToString(ofGetDay(),2,'0')+
	ofToString(ofGetHours(),2,'0')+ofToString(ofGetMinutes(),2,'0');
	chorraco += "_cara_"+ofToString(nPers,3,'0')+".png";
    
    nmImgScan += chorraco;
	ofLogNotice("grabar:_"+nmImgScan);
	imgScan.saveImage(nmImgScan);
    
    nmImgScan1 += chorraco;
	imgScan.saveImage(nmImgScan1);
	
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


// - - - - - -



void scanner_faces::exit() {
    ofLogNotice() << "Exit() Apagar Luz";
    setLight(false);
    ofLogNotice() << "Exit() Bye Bye";
    //	gui->saveSettings("settings.xml");
    //	delete gui;
}

