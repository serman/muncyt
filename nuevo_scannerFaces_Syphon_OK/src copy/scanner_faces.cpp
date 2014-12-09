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
#else 
    wMonitor = 1200;	// Mi equipo
    hMonitor = 720;	
#endif
    
	//	***************
	fondo.loadImage("media/fondo_escaner.png");
	fondo.resize(wMonitor,hMonitor);
    ofLogNotice() << "w,h: " << ofGetWidth() << "," << ofGetHeight();
	//	***************
	// El fondo es para dimension de 1920x1080
	//
	// Lo paso a 1280x720 (mi pantalla)
	float esc = (float) wMonitor/1920;
	rectCamera = ofRectangle(567*esc, 256*esc, 786*esc, 786*esc);
	rectMensajes = ofRectangle(567*esc, 35*esc, 786*esc, 145*esc);
	
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
	
	
	// start
#ifndef SYPHON_CAM
	wCam = 640.0;
	hCam = 480.0;
    cam.listDevices();
	cam.initGrabber(wCam, hCam);
    
//	float wMarco = cam.getWidth()*0.7;
//	float hMarco = wMarco*hCam/cam.getWidth();
//	float xMarco = (cam.getWidth()-wMarco)/2;
//	float yMarco = (cam.getHeight()-hMarco)/2;
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
    float wMarco = wCam*0.7;
	float hMarco = wMarco*hCam/wCam;
	float xMarco = (wCam-wMarco)/2;
	float yMarco = (hCam-hMarco)/2;

	marco.setRectExt(ofRectangle(0,0, wCam, hCam));
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
	
	// imagen para flip camara
	camImage.allocate(wCam,hCam, OF_IMAGE_COLOR);
	
	bFlipH = false;
	bFlipV = true;
	
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
		
	swFullScreen = true;
    
    setupGUI();
    
    // Light ON
    setLight(true);   // Esto no va bien (nodebe estar preparado el serial o algo asi
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
}

void scanner_faces::setupFotos() {
	// Las fotos para HaarViz
    // salen con Proporcion W/H 3/4
    float proporWH = 3.0/4.0;
    
//	NFOTOSMAX = 3;
//	nFotoAct = 0;
	
	WSCAN = 200;
	HSCAN = 300;
	
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
            //			faceImage.setFromPixels(cam.getPixels(), cam.width, cam.height, OF_IMAGE_COLOR );
			faceImage.setFromPixels(camImage.getPixels(), camImage.getWidth(), camImage.getHeight(), OF_IMAGE_COLOR );
			faceImage.crop(faceRectAmpl.x, faceRectAmpl.y, faceRectAmpl.width, faceRectAmpl.height);
			finderEyes.update(faceImage);
		}
		
		// check si la cara esta dentro de los margenes
		caraOut = marco.isOut(faceRectAmpl);
		
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

    
//    ofNoFill();
//	ofSetColor(255,0,0);
//	ofRect(rectCamera);
//    ofRect(rectMensajes);
	

    if(true) {
    if(scnAct==SCN_WAITING) {
        draw_ScnWait();
    }
    else {
        camImage.draw(rectCamera);
        
        // - - - -
        // Este bloque debe ir enla zona de escenas
        //
        // la transicion será con shader
        //
        
        //
        ofTranslate(rectCamera.x,rectCamera.y);
        
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
        
    }
    }
	
    
    if(false) {
	if(scnAct==SCN_WAITING) {
		draw_ScnWait();
	}
	else {
		
		// CONTENIDOS REDIMENSIONADOS Y CON FLIP_H
		float hh = ofGetScreenHeight();
//		float ww = hh*cam.getWidth()/cam.getHeight();
		float ww = hh*camImage.getWidth()/camImage.getHeight();
		
		//		cam.draw((ofGetWidth()-ww)/2,(ofGetHeight()-hh)/2, ww,hh);
		ofPushMatrix();
		{
//			ofTranslate((ofGetWidth()-ww)/2,(ofGetHeight()-hh)/2);
			// centrar la imagen y
			// flip H
			
			ofTranslate(ofGetWidth()-(ofGetWidth()-ww)/2,(ofGetScreenHeight()-hh)/2);
//			ofScale(-ww/cam.getWidth(),hh/cam.getHeight());
			ofScale(-ww/camImage.getWidth(),hh/camImage.getHeight());
			
//			cam.draw(0,0);
			camImage.draw(0,0);

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
	}   // fin if(false);
    
    
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


// - - - - - 

void	scanner_faces::setLight(bool swlight) {
    bSendSerialMessage = true;
    if(swlight)   charSend = 'i';
    else          charSend = 'o';
    
    serial.writeByte(charSend);

    ofLogNotice() << "setLight " << swlight;
    
    bSendSerialMessage = false;
}



// - - -

void scanner_faces::hacerFoto() {
	
	if(hayPersona()) {
//		faceImage.setFromPixels(cam.getPixels(), cam.width, cam.height, OF_IMAGE_COLOR );
		faceImage.setFromPixels(camImage.getPixels(), camImage.getWidth(), camImage.getHeight(), OF_IMAGE_COLOR );
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
//		faceImage.setFromPixels(cam.getPixels(), cam.width, cam.height, OF_IMAGE_COLOR );
		faceImage.setFromPixels(camImage.getPixels(), camImage.getWidth(), camImage.getHeight(), OF_IMAGE_COLOR );
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

