#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {
//    ofDisableAntiAliasing();
	ofSetVerticalSync(true);
	ofBackgroundHex(0xfdefc2);
//	ofSetLogLevel(OF_LOG_NOTICE);
	ofSetLogLevel(OF_LOG_VERBOSE);

	// Cargar Textura de particulas
	// load the texure
	ofDisableArbTex();
	ofLoadImage(texPartic, "media/images/dot.png");
	

	// BOX2D
	box2d.init();
	box2d.setGravity(0, 0);
	box2d.createBounds();
	box2d.setFPS(30.0);
	box2d.registerGrabbing();	// Esto creo que es para detectar eventos
	
	// Poner Muro circular
	float radioMuro = MIN(ofGetWidth(), ofGetHeight()) / 2.0*0.8;
	int resol = 360/5;
	
	// Preparo el polyline
	ofPoint centro = ofPoint(ofGetWidth()/2.0, ofGetHeight()/2.0);
	bordeLine.arc(centro, radioMuro, radioMuro, 0.0, 360.0, true, resol);
	
	borde.clear();
    borde.addVertexes(bordeLine);
    borde.create(box2d.getWorld());
	
	// Fuerza que afecta a todas las particulas
	fuerzaVal = 100.0;
	fuerzaAng = 0.0;
	fuerzaWAng = ofDegToRad(90.0);
	
	swFuerza = false;
	
	// radios
	rNucleo	= 14;
	rNeutron = 5;
	
	
	ofLogVerbose("Add nucleos");
	// add some nucleos y boxes
	int nCircs = 40 + floor(  ofRandom(20) );
	for(int i =0; i<nCircs; i++) {
		addNucleo(centro.x, centro.y, rNucleo);
	}
	
//	int nBoxes = 10 + floor(  ofRandom(10) );
//	for(int i =0; i<nBoxes; i++) {
//		addBox(centro.x, centro.y);
//	}
	
	cargaSounds();
	
	
}

void testApp::cargaSounds() {
	// load the 8 sfx soundfile
	ofLogVerbose("XXXXXXXXXXXXXXXXXXXXXXX");
	for (int i=0; i<N_SOUNDS; i++) {
		ofLogVerbose("media/sound/"+ofToString(i,0,2,'0')+".mp3");
		sounds[i].loadSound("media/sound/"+ofToString(i,0,2,'0')+".mp3");
		sounds[i].setMultiPlay(true);
		sounds[i].setLoop(false);
		sounds[i].stop();
	}
}

//--------------------------------------------------------------
void testApp::update() {
	box2d.update();	
	
	// Aplica fuerzas
	fuerzaAng += fuerzaWAng/ofGetFrameRate();
	fuerza = ofPoint(fuerzaVal*cos(fuerzaAng), fuerzaVal*sin(fuerzaAng));
	
	if(swFuerza) {
		box2d.setGravity(fuerza.x, fuerza.y);
	}
	else box2d.setGravity(0,0); 
	
}


//--------------------------------------------------------------
void testApp::addNucleo(int xx, int yy){
	float r = ofRandom(10, 20);
	addNucleo(xx,yy,r);
}

void testApp::addNucleo(int xx, int yy, float r){
	nucleos.push_back(ofPtr<Particula>(new Particula));
	nucleos.back().get()->setPhysics(50.5, 0.53, 0.1); //setPhysics(float density, float bounce, float friction);
	nucleos.back().get()->setup(box2d.getWorld(), xx, yy, r);		// pos, rr
	nucleos.back().get()->setRotation(ofRandom(360));
	
	nucleos.back().get()->setTexture(texPartic);
	
	if(ofRandom(1)<0.5)	nucleos.back().get()->setExcitado(true);
	else				nucleos.back().get()->setExcitado(false);
	
	nucleos.back().get()->setData(nucleos.back().get());	
}

void testApp::addNeutron(int xx, int yy){
	neutrones.push_back(ofPtr<Particula>(new Particula));
	neutrones.back().get()->setPhysics(50.5, 0.53, 0.1); // setPhysics(float density, float bounce, float friction);
	neutrones.back().get()->setup(box2d.getWorld(), xx, yy, rNeutron);		// pos, rr
	neutrones.back().get()->setRotation(ofRandom(360));
	neutrones.back().get()->setTexture(texPartic);
	neutrones.back().get()->setColor(ofColor(0x5231c9));
	
	// * * * * * * *
	// A corregir:
	// http://forum.openframeworks.cc/t/box2d-contact-listening-and-userdata/3441/3
	//
	neutrones.back().get()->setData(neutrones.back().get());
	// * * * * * * *
	
	// Lanzarla con velocidad
	float vVal = 100.0;
	float vAng = ofRandom(TWO_PI);
	neutrones.back().get()->setVelocity(vVal*cos(vAng),vVal*sin(vAng));
	
	neutrones.back().get()->setExcitado(true);
	
}

void testApp::addBox(int xx, int yy){
	float w = ofRandom(10, 20);
	float h = ofRandom(10, 20);
	boxes.push_back(ofPtr<ofxBox2dRect>(new ofxBox2dRect));
	boxes.back().get()->setPhysics(3.0, 0.53, 0.1);
	boxes.back().get()->setup(box2d.getWorld(), xx, yy, w, h);	
	boxes.back().get()->setRotation(ofRandom(360));
}


//--------------------------------------------------------------
void testApp::draw() {

	// dibuja lineas selector fuerza
	drawFuerzaSelector();
	
	for(int i=0; i<nucleos.size(); i++) {
		ofFill();
		ofSetHexColor(0xf6c738);
		nucleos[i].get()->draw();
	}
	for(int i=0; i<neutrones.size(); i++) {
//		ofFill();
//		ofSetHexColor(0xf6c738);
		neutrones[i].get()->draw();
	}
	
	for(int i=0; i<boxes.size(); i++) {
		ofFill();
		ofSetHexColor(0xBF2545);
		boxes[i].get()->draw();
	}

	// draw the ground
	//box2d.drawGround();
	
	bordeLine.draw();
	
	// dibuja valor fuerza
	drawFuerza(bordeLine.getCentroid2D(), fuerza);

	
	// Mascara exterior
	// ...Con algun Shape...

	
	// Info
	string info = "";
	info += "Press [c] for nucleos\n";
	info += "Press [b] for blocks\n";
	info += "Total Bodies: "+ofToString(box2d.getBodyCount())+"\n";
	info += "Total Joints: "+ofToString(box2d.getJointCount())+"\n\n";
	info += "FPS: "+ofToString(ofGetFrameRate(), 1)+"\n\n";
	info += "Aplica Fuerza [f]: "+ofToString(swFuerza)+"\n";
	info += "w Giro Fuerza [mouseX+Click]: "+ofToString(ofRadToDeg(fuerzaWAng)/360)+" Hz\n\n";
	ofSetHexColor(0x444342);
	ofDrawBitmapString(info, 30, 30);
}


void testApp::drawFuerza(ofPoint p, ofPoint gravity) {
    
    float angle = (atan2(gravity.y, gravity.x) * 180 / PI) - 90;
//    float len   = MIN(100, gravity.length()*10); // scale it up a bit
    float len   = gravity.length(); // scale it up a bit
    
    ofPushStyle();
	ofPushMatrix();
    ofTranslate(p.x, p.y);
    ofRotate(angle);
	ofSetColor(200,0,0);
	ofSetLineWidth(2.0);
    ofLine(0, 0, 0, len);
    ofTriangle(0, len, 
               -5, len-10, 
               5, len-10);
    ofPopMatrix();
	ofPopStyle();
}

void testApp::drawFuerzaSelector() {
//	fuerzaWAng = ofDegToRad( ofMap(x, 0, ofGetWidth(), 30, 360*6) );
//	fuerzaVal = ofDegToRad( ofMap(y, 0, ofGetHeight(), 0, 200) );
	ofPushStyle();
	ofPushMatrix();
		ofSetColor(0,0,100);
		// valor de la fuerza
		float posY = ofMap(fuerzaVal, 0, 200, 0, ofGetHeight());
		ofLine(0, posY, ofGetWidth(), posY);
		// valor de la veloc de rotacion de la fuerza
		float posX = ofMap(ofRadToDeg(fuerzaWAng), 30, 360*6, 0, ofGetWidth());
		ofLine(posX, 0, posX, ofGetHeight());
		
	ofPopMatrix();
	ofPopStyle();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key) {
	
	if(key == 'c') {
		addNucleo(mouseX, mouseY, rNucleo);
	}
	
	if(key == 'b') {
		addBox(mouseX, mouseY);
	}

	if(key == 'n') {
		addNeutron(mouseX, mouseY);
	}
	
	if(key == 't') ofToggleFullscreen();
	
	if(key=='f') swFuerza = !swFuerza;
}

//--------------------------------------------------------------
void testApp::keyReleased(int key) {
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ) {
	
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button) {
	if(swFuerza) {
	   fuerzaWAng = ofDegToRad( ofMap(x, 0, ofGetWidth(), 30, 360*6) );
	   fuerzaVal = ( ofMap(y, 0, ofGetHeight(), 0, 200) );
	}
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button) {
	if(swFuerza) {
		fuerzaWAng = ofDegToRad( ofMap(x, 0, ofGetWidth(), 30, 360*6) );
		fuerzaVal = ( ofMap(y, 0, ofGetHeight(), 0, 200) );
	}
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button) {
}

//--------------------------------------------------------------
void testApp::resized(int w, int h){
}

