#include "testApp.h"

using namespace ofxCv;
using namespace cv;

//--------------------------------------------------------------
void testApp::setup(){

	ofSetLogLevel(OF_LOG_VERBOSE);
	cargarFiles();
	
	setupFinders();
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
	iFaceAct = 0;	// idx de la imagen capturada que se representa.
	if1 = 0;
	if2 = 0;
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
				
				ofLogError("num blobs por OFXCV: " + ofToString(finder_ofxcv.size()));
				// bucle entre los blobs
				for(unsigned int i = 0; i < finder_ofxcv.size(); i++) {
					faceData fd;
					fd.rect = finder_ofxcv.getObject(i);
					
					// resize el rectangulo de la cara segœn parametros de control de W,H
					
					// guardar los resultados en vector de structs faceData
					rectsFacesAct.push_back(fd);					
					
					
					// guardar la imagen en un vector de images capturadas
					ofImage imgTmp;
					imgTmp.cropFrom(images[iImgAct], 
									fd.rect.x, fd.rect.y,
									fd.rect.width, fd.rect.height);
					imgTmp.resize(320, 320);
					images_faces.push_back(imgTmp);
				}
				
				isAnalisisDone = true;			
			}
		}
		
		else if(modoDetect==MODO_HAAR) {
			finder.findHaarObjects((ofImage)images[iImgAct]);
		
			if(finder.blobs.size()>0) {	// si se han encontrado caras

				ofLogError("num blobs por OFXOPENCV: " + ofToString(finder.blobs.size()));
				// bucle entre los blobs
				for(unsigned int i = 0; i < finder.blobs.size(); i++) {
					faceData fd;
					fd.rect = finder.blobs[i].boundingRect;

					// resize el rectangulo de la cara segœn parametros de control de W,H
					
					// guardar los resultados en vector de structs faceData
					rectsFacesAct.push_back(fd);
					
					// guardar la imagen en un vector de images capturadas
					
				}

				isAnalisisDone = true;			
			}
		}
		
//		else if(mode==MODO_FCTRACKer) {
//			tracker.update(toCv((ofImage)images[iImgAct]));
////			if(tracker.update(toCv((ofImage)images[iImgAct]))) {
////				classifier.classify(tracker);
////			}			
//		}
		
		
	}
	
	if(modoDetect==MODO_FCTRACKer) {
		tracker.update(toCv(images[iImgAct]));
		//			if(tracker.update(toCv((ofImage)images[iImgAct]))) {
		//				classifier.classify(tracker);
		//			}			
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
			images_faces[iFaceAct].draw(0,0);
			
			// mezcla de imagenes
			images_faces[if1].drawSubsection(0,0, 
											images_faces[iFaceAct].width, 
											images_faces[iFaceAct].height * 0.25, 
											0,0);

			images_faces[if2].drawSubsection(0,
											 images_faces[iFaceAct].height * 0.45,
											 
											 images_faces[iFaceAct].width, 
											 images_faces[iFaceAct].height * 0.25, 
											 
											 0,
											 images_faces[iFaceAct].height * 0.45);
			
			
			// Paso de tiempo
			if(ofGetElapsedTimeMillis()>(lastTime+500)) {
				iFaceAct++;
				iFaceAct%=images_faces.size();
				
				ofLogError(ofToString(ofRandom(1.0)));
				if(0.05>ofRandom(1.0)) if1 = floor(ofRandom(images_faces.size()));
				if(ofRandom(1.0)<0.05) if2 = floor(ofRandom(images_faces.size()));
				
				lastTime = ofGetElapsedTimeMillis();				
			}
		}
		//
		
		ofPopStyle();
		
		
		// dibujar la cara recortada en un lateral		
		
	}
	
	if(modoDetect==MODO_FCTRACKer) {
		tracker.draw();
		ofPushMatrix();
//		ofTranslate(ofGetWidth()-300,100,0);
			//ofScale(5,5,5);
			//tracker.getObjectMesh().drawWireframe();
			tracker.getImageMesh().drawWireframe();
		ofPopMatrix();
	}

	ofDrawBitmapString("Seleccion Images (n/m): " + ofToString(iImgAct)+"/"+ofToString(images.size()), ofPoint(10,ofGetHeight()-110));
	ofDrawBitmapString("ModoAct (z): " + ofToString(modoDetect), ofPoint(10,ofGetHeight()-90));
	ofDrawBitmapString("analisis hecho: " + ofToString(isAnalisisDone), ofPoint(10,ofGetHeight()-70));
	ofDrawBitmapString("num faces guardadas: " + ofToString(images_faces.size()), ofPoint(10,ofGetHeight()-50));
	ofDrawBitmapString("fr: " + ofToString(ofGetFrameRate()), ofPoint(10,ofGetHeight()-30));
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
		}
		
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
