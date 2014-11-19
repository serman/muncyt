#include "testApp.h"

using namespace ofxCv;
using namespace cv;

//--------------------------------------------------------------
void testApp::setup(){

	ofSetLogLevel(OF_LOG_VERBOSE);
	cargarFiles();
	
	setupFinders();

	setupBandas();

	draw_bandas2 = false;
	
}

void testApp::setupBandas() {
	// setup bandas mezcla OFXCV
	ofRectangle r0 = ofRectangle(0,0,				LADO_CARA, LADO_CARA*0.25);
	ofRectangle r1 = ofRectangle(0,r0.y+r0.height,  LADO_CARA, LADO_CARA*0.20);
	ofRectangle r2 = ofRectangle(0,r1.y+r1.height,  LADO_CARA, LADO_CARA*0.25);
	ofRectangle r3 = ofRectangle(0,r2.y+r2.height,  LADO_CARA, LADO_CARA*0.30);
	bandas1.push_back(r0);
	bandas1.push_back(r1);
	bandas1.push_back(r2);
	bandas1.push_back(r3);

	ofRectangle r01 = ofRectangle(0,0,				LADO_CARA/2.0, LADO_CARA*0.25);
	ofRectangle r02 = ofRectangle(LADO_CARA/2.0,0,	LADO_CARA/2.0, LADO_CARA*0.25);
	ofRectangle r11 = ofRectangle(0,r0.y+r0.height,				LADO_CARA/2.0, LADO_CARA*0.20);
	ofRectangle r12 = ofRectangle(LADO_CARA/2.0,r0.y+r0.height, LADO_CARA/2.0, LADO_CARA*0.20);
	ofRectangle r21 = ofRectangle(0,r1.y+r1.height,				LADO_CARA/2.0, LADO_CARA*0.25);
	ofRectangle r22 = ofRectangle(LADO_CARA/2.0,r1.y+r1.height, LADO_CARA/2.0, LADO_CARA*0.25);
	ofRectangle r31 = ofRectangle(0,r2.y+r2.height,				LADO_CARA/2.0, LADO_CARA*0.30);
	ofRectangle r32 = ofRectangle(LADO_CARA/2.0,r2.y+r2.height, LADO_CARA/2.0, LADO_CARA*0.30);
	bandas2.push_back(r01);
	bandas2.push_back(r02);
	bandas2.push_back(r11);
	bandas2.push_back(r12);
	bandas2.push_back(r21);
	bandas2.push_back(r22);
	bandas2.push_back(r31);
	bandas2.push_back(r32);
	
	iFaceAct = 0;	// idx de la imagen capturada que se representa.
	for(int i=0; i<4; i++) {
		ids_1.push_back(0);
	}
	for(int i=0; i<8; i++) {
		ids_2.push_back(0);
	}
	
}

void testApp::cargarFiles() {
	string path = "./images/imgs_faces/";
	ofDirectory dir;
	//only show jpg files
//	dir.allowExt("jpg");
	//populate the directory object
	int nFiles = dir.listDir(path);
	
	ofLogVerbose("Listar directorio - "+dir.path());
	//go through and print out all the paths
	if(nFiles) {
		for(int i = 0; i < dir.numFiles(); i++){
			string fn = dir.getPath(i);
			ofLogNotice(ofToString(i)+": "+ fn);
			fileNames.push_back(fn);
			ofImage imgTmp;
			imgTmp.loadImage(fn);
			images.push_back(imgTmp);
		}
	}
	ofLogVerbose("Listar directorio - FIN");
	
	iImgAct = 0;
	isAnalisisDone = false;
	lastTime = ofGetElapsedTimeMillis();
}

void testApp::setupFinders() {
	finder.setup("./haar/haarcascade_frontalface_default.xml");
	
	finder_ofxcv.setup("./haar/haarcascade_frontalface_default.xml");
//	finder_ofxcv.setPreset(ofxCv::ObjectFinder::Fast);
	finder_ofxcv.setPreset(ofxCv::ObjectFinder::Accurate); // Accurate Sensitive
	finder_ofxcv.setMultiScaleFactor(1.09);
	
	modoDetect = MODO_OFXCV;
	
	// Face Tracker
	tracker.setup();
	tracker.setRescale(.5);
	
}

//--------------------------------------------------------------
void testApp::update(){
	if(!isAnalisisDone) {
		rectsFacesAct.clear();
//		ofLogError("analisis no hecho");
		// haz el analisis de la imagen que toque
		
		if(modoDetect==MODO_OFXCV) {
			finder_ofxcv.update(images[iImgAct]);
			
			if(finder_ofxcv.size()>0) {	// si se han encontrado caras
				
				ofLogNotice("num blobs por OFXCV: " + ofToString(finder_ofxcv.size()));
				// bucle entre los blobs
				for(unsigned int i = 0; i < finder_ofxcv.size(); i++) {
					faceData fd;
					fd.rect = finder_ofxcv.getObject(i);
					
					// TODO: resize el rectangulo de la cara segœn parametros de control de W,H
					
					// guardar los resultados en vector de structs faceData
					rectsFacesAct.push_back(fd);					
					
					// guardar la imagen en un vector de images capturadas
					ofImage imgTmp;	
					imgTmp.cropFrom(images[iImgAct], 
									fd.rect.x, fd.rect.y,
									fd.rect.width, fd.rect.height);
					imgTmp.resize(LADO_CARA, LADO_CARA);
					images_faces.push_back(imgTmp);
					
					
					
				}
				
				isAnalisisDone = true;			
			}
		}
		
		else if(modoDetect==MODO_HAAR) {
			finder.findHaarObjects((ofImage)images[iImgAct]);
		
			if(finder.blobs.size()>0) {	// si se han encontrado caras

				ofLogNotice("num blobs por OFXOPENCV: " + ofToString(finder.blobs.size()));
				// bucle entre los blobs
				for(unsigned int i = 0; i < finder.blobs.size(); i++) {
					faceData fd;
					fd.rect = finder.blobs[i].boundingRect;

					// resize el rectangulo de la cara segœn parametros de control de W,H
					
					// guardar los resultados en vector de structs faceData
					
					// guardar la imagen en un vector de images capturadas
					
				}

				isAnalisisDone = true;			
			}
		}
		
		else if(modoDetect==MODO_FCTRACKer) {

			// ANALIZAR TODA LA IMAGEN
			tracker.update(ofxCv::toCv(images[iImgAct]));
//			if(tracker.update(toCv((ofImage)images[iImgAct]))) {
//				classifier.classify(tracker);
//			}			
			
			if(tracker.getFound()) {
				
				// Guardar imagen recortada y escalada
				rectCara = tracker.getHaarRectangle();
				
				// Guardar imagen recortada y escalada pro con m‡s ‡rea de rostro
				float escAreaUp  = 0.2/10.0;
				float escAreaDwn = 2.2/10.0;
				float escArea = (escAreaUp + escAreaDwn);
				rectCara_X = tracker.getHaarRectangle();
				
				rectCara_X.x += -rectCara_X.width*escArea/2.0;
				rectCara_X.y += -rectCara_X.height*escAreaUp;
				rectCara_X.width = rectCara_X.width*(1+escArea);
				rectCara_X.height = rectCara_X.height*(1+escArea);
								
				
				
				
			}
			
		}
		
		
	}
	
	
}

//--------------------------------------------------------------
void testApp::draw(){
	
	ofBackground(ofColor::black);
	
	images[iImgAct].draw(0,0);
	
	
	if(isAnalisisDone) {
		// dibujar rectangulo
		ofPushStyle();
		ofNoFill();
		ofSetColor(0,0,255);
		
		float xSubsect = ofGetWidth();
		
		// General, con vector rectsFacesAct
		for(unsigned int i = 0; i < rectsFacesAct.size(); i++) {
			ofSetColor(0,0,255);
			ofRectangle cur = rectsFacesAct[i].rect;
			ofRect(cur.x, cur.y, cur.width, cur.height);
			
			ofSetColor(255);
			xSubsect-=cur.width;
			images[iImgAct].drawSubsection(xSubsect, ofGetHeight()-cur.height, cur.width, cur.height,
										   cur.x, cur.y);
			
			ofDrawBitmapString("w/h: " + ofToString(cur.width/cur.height), ofPoint(xSubsect,ofGetHeight()-cur.height-30));
		}		

		// DIBUJAR IMAGENES
		if(images_faces.size()>0) {
			ofPushMatrix();
			ofTranslate(ofGetWidth()-images_faces[iFaceAct].width, 0);
			// mezcla de imagenes
			ofSetColor(255,255,255);			
			if(!draw_bandas2) {
				for(int i=0; i<bandas1.size(); i++) {
					drawBanda(ids_1[i], i, bandas1);
				}
			}
			else {
				for(int i=0; i<bandas2.size(); i++) {
					drawBanda(ids_2[i], i, bandas2);
				}
			}
			ofSetColor(255);
			
			// Paso de tiempo
			if(ofGetElapsedTimeMillis()>(lastTime+500)) {
				if(0.9>ofRandom(1.0)) {
					iFaceAct++;
					iFaceAct%=images_faces.size();
				}
				
				if(0.08>ofRandom(1.0)) ids_1[0] = floor(ofRandom(images_faces.size()));
				if(0.55>ofRandom(1.0)) ids_1[1] = floor(ofRandom(images_faces.size()));
				if(0.22>ofRandom(1.0)) ids_1[2] = floor(ofRandom(images_faces.size()));
				if(0.15>ofRandom(1.0)) ids_1[3] = floor(ofRandom(images_faces.size()));
				
				
				for(int i=0; i<ids_2.size(); i++) {
					if(0.10>ofRandom(1.0)) ids_2[i] = floor(ofRandom(images_faces.size()));
				}
				
				lastTime = ofGetElapsedTimeMillis();				
			}
			
		}
		//
		
		ofPopStyle();
		
		
		// dibujar la cara recortada en un lateral		
		
	}
	
	if(modoDetect==MODO_FCTRACKer) {
		ofPushStyle();
		tracker.draw();

		
		ofPushMatrix();
//		ofTranslate(ofGetWidth()-rectCara_X.width, 0);
		ofTranslate(ofGetWidth()-LADO_CARA, 0);
		ofScale(LADO_CARA/rectCara_X.width, LADO_CARA/rectCara_X.height, 1.0);
		images[iImgAct].drawSubsection(0,0, 
									  rectCara_X.width, rectCara_X.height,
									  rectCara_X.x, rectCara_X.y);
		ofPopMatrix();

		
//		ofTranslate(ofGetWidth()-300,100,0);
		//ofScale(5,5,5);
		//tracker.getObjectMesh().drawWireframe();

		// DE SERIE
		ofSetColor(200,0,0, 180);
		tracker.draw();
		
		
		ofSetColor(200,200,0, 30);
		ofRect(rectCara);
		ofSetColor(200,60,180, 60);
		ofRect(rectCara_X);
		
		// IMAGE
		ofSetColor(200,0,200,150);
		tracker.getImageMesh().drawWireframe();
		
		ofPopStyle();		
		
		// 3D
		ofPushStyle();
		ofPushMatrix();
		ofTranslate(tracker.getPosition().x, tracker.getPosition().y,0);
		ofScale(tracker.getScale(), tracker.getScale(), 1);//tracker.getScale());
		ofSetColor(0,0,200);
		tracker.getObjectMesh().drawWireframe();
		
		ofPopMatrix();
		
		ofNoFill();
		ofSetColor(0,200,0);
		ofRect(tracker.getHaarRectangle());
		ofPopStyle();
	}

	ofDrawBitmapString("(b) Draw en 4 u 8 zonas", ofPoint(10,ofGetHeight()-130));
	ofDrawBitmapString("Seleccion Images (n/m): " + ofToString(iImgAct)+"/"+ofToString(images.size()), ofPoint(10,ofGetHeight()-110));
	ofDrawBitmapString("ModoAct (z): " + ofToString(modoDetect), ofPoint(10,ofGetHeight()-90));
	ofDrawBitmapString("analisis hecho: " + ofToString(isAnalisisDone), ofPoint(10,ofGetHeight()-70));
	ofDrawBitmapString("num faces guardadas: " + ofToString(images_faces.size()), ofPoint(10,ofGetHeight()-50));
	ofDrawBitmapString("fr: " + ofToString(ofGetFrameRate()), ofPoint(10,ofGetHeight()-30));
}


void testApp::drawBanda(int idFace, int idBanda, vector<ofRectangle> _bandas) {
	images_faces[idFace].drawSubsection(_bandas[idBanda].x,
									 _bandas[idBanda].y,
									 _bandas[idBanda].width, 
									 _bandas[idBanda].height, 
									 _bandas[idBanda].x,
									 _bandas[idBanda].y);
	
}


//--------------------------------------------------------------
void testApp::keyPressed(int key){
	if(key=='m') {
		iImgAct++;
		iImgAct%=images.size();
		isAnalisisDone = false;
		tracker.reset();
	}
	else if(key=='n') {
		iImgAct--;
		iImgAct= (iImgAct<0)? images.size()-1 : iImgAct;
		isAnalisisDone = false;
		tracker.reset();
	}
	else if(key=='z') {
		modoDetect++;
		if(modoDetect>=3) modoDetect=0;
		isAnalisisDone = false;
	}
	if(key=='q') {
		finder_ofxcv.setPreset(ofxCv::ObjectFinder::Fast);
		if(modoDetect==MODO_OFXCV) 		isAnalisisDone = false;
	}
	else if(key=='w') {
		finder_ofxcv.setPreset(ofxCv::ObjectFinder::Accurate);
		finder_ofxcv.setMultiScaleFactor(1.09);
		if(modoDetect==MODO_OFXCV) 		isAnalisisDone = false;
	}
	else if(key=='e') {
		finder_ofxcv.setPreset(ofxCv::ObjectFinder::Sensitive);
		if(modoDetect==MODO_OFXCV) 		isAnalisisDone = false;
	}
	
	else if(key=='s') {
		if(modoDetect==MODO_FCTRACKer) {
			string nombre = "capturas/imagen_"+ofToString(iImgAct);
			
			images[iImgAct].saveImage(nombre+".jpg");			
			tracker.getImageMesh().save(nombre+"_ImageMesh.ply");
			tracker.getObjectMesh().save(nombre+"_ObjectMesh.ply");
			tracker.getMeanObjectMesh().save(nombre+"_MeanObjectMesh.ply");
			
			// Guardar imagen recortada y escalada
			ofImage imgCara;
			string nombreCara = "capturas/imagen_Cara_"+ofToString(iImgAct);
//			ofRectangle rectCara = tracker.getHaarRectangle();
			imgCara.cropFrom(images[iImgAct], rectCara.x, rectCara.y, rectCara.width, rectCara.height); 
			imgCara.resize(LADO_CARA, LADO_CARA);
			imgCara.saveImage(nombreCara+".jpg");

			
			// Guardar imagen recortada y escalada pro con m‡s ‡rea de rostro
 
//			ofLogNotice() << "rectCara_X: " << rectCara_X.x <<","<< rectCara_X.y <<","<< rectCara_X.width <<","<< rectCara_X.height;
						
			nombreCara = "capturas/imagen_Cara_X_"+ofToString(iImgAct);
			if(rectCara_X.x>=0 && (rectCara_X.x+rectCara_X.width)<=images[iImgAct].width &&
			   rectCara_X.y>=0 && (rectCara_X.y+rectCara_X.height)<=images[iImgAct].height ) 
			{
				imgCara.cropFrom(images[iImgAct], rectCara_X.x, rectCara_X.y, rectCara_X.width, rectCara_X.height); 
				imgCara.resize(LADO_CARA, LADO_CARA);
				imgCara.saveImage(nombreCara+".jpg");
			}
			
			// Analizar y guardar. Ya deber’a llegar analizada
			// 
			
			
		}
		
	}
	else if(key=='b') draw_bandas2=!draw_bandas2;
	
	
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
