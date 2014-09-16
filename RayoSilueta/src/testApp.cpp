#include "testApp.h"
#include "utilsGeom.cpp"

//--------------------------------------------------------------
void testApp::setup(){

	ofSetVerticalSync(true);
	
	zentro = ofVec3f(ofGetWidth()/2.0,ofGetHeight()/2.0,0);
	
	swDifraccion = false;
	
	ratePartic = 20;
	
	bDrawCaminos = true;
	bDrawPtosChoque = true;
	bTiltCamino = false;	
	
	
	
	testPath.setFilled(false);
	testPath.setStrokeColor(ofColor::blueSteel);
	testPath.setColor(ofColor::blueSteel);	
	
}

//--------------------------------------------------------------
void testApp::update(){
	// add alguna particula desde un lateral
	addParticleLateral();
	
	// cambiar camino
	if(bTiltCamino) tiltCamino();
	
	
	// Mover particulas y chequear si hay colision
	for(int i=0;i<particulas.size();i++) {
		
		// mover 
		particulas[i].update();
		
		if(camino.size()>0) {
			bool bCruce = false;
			if(GeometryUtils::estaDentro(particulas[i].position, camino)) {
				particulas[i].color = ofColor::red;
				bCruce = particulas[i].setInside(true);	
			}
			else {
				particulas[i].color = particulas[i].color_orig; 
				bCruce = particulas[i].setInside(false);	
			}
			
			if(bCruce) {
				// check si partic choca con shillouette

				ofVec2f ptChoque = GeometryUtils::getIntersectionPoly(particulas[i].position_prev, particulas[i].position, camino);
				if(ptChoque!=ofVec2f(0,0)) {
					ptsChoque.push_back(ptChoque);
					
					// add el punto al camino de la particula
					particulas[i].insertPtChoque(ptChoque);
					
					float angRotMax = 15;
					float angRot =  ofRandom(-angRotMax, angRotMax);
					ofColor cc = ofColor::fromHsb(ofMap(abs(angRot), 0, angRotMax, 0,255), 255, 255, 100);
					particulas[i].colorExcited = cc;
					particulas[i].velocity.rotate(angRot, ofVec3f(0,0,1));
					
				}
			}
		}
		
		
	}
	
	
	// remove particulas marcadas para borrar (por ej que esten fuera de screen)
	for(int i=particulas.size()-1; i>0;i--) {
		if(particulas[i].position.x>ofGetWidth()+50) particulas.erase(particulas.begin()+i);
	}

}

void testApp::tiltCamino() {
	ofPolyline pl = camino;
	camino.clear();
	
	float maxDespl = 2.0;
	for(int i=0; i<pl.size(); i++) {
		camino.addVertex( pl[i]+ofVec2f(ofRandom(-maxDespl, maxDespl), ofRandom(-maxDespl, maxDespl)) );
	}
	camino.close();
	
	
}


void testApp::addParticleLateral() {
	
	
	//	float rnd = ofRandom(1.0);
	//	if(rnd<0.15) {
	if(ofGetFrameNum()%ratePartic==0) {
		//		ParticleX( ofVec3f _position , ofColor _color, float mass = 1.0, float charge = 0.0 )
		//		ParticleX( ofVec3f _position , ofVec3f _vel , ofColor _color, float mass = 1.0, float charge = 0.0 )
		float v = ofMap(mouseX,0,ofGetHeight(), 4, 10);
		ParticleX p = ParticleX(ofVec3f(0,ofRandom(ofGetHeight()),0), ofVec3f(v,0,0), ofColor(255,255,255) );
		particulas.push_back(p);
		
		
		// pos, vel, color, mass, charge
//		ParticleX pTmp = ParticleX(ofVec3f(0,ofRandom(ofGetHeight())) , ofVec3f(ofRandom(10.0, 30.0), 0) , ofColor::white, 1.0, 0.0);
		
	}
	
	
}

//--------------------------------------------------------------
void testApp::draw(){

	ofBackground(ofColor::black);
	
	//
	//
	// PARTICULAS 
	//
	
	ofEnableSmoothing();
	ofEnableAntiAliasing();

	if(bDrawCaminos) {
		ofPushStyle();
		ofSetColor(ofColor::lime, 60);
		ofSetLineWidth(15);
		camino.draw();
		ofSetColor(ofColor::white, 200);
		ofSetLineWidth(1);
		camino1.draw();
		ofPopStyle();
		
		testPath.draw();
	}
	
	for(int i=0;i<particulas.size();i++) {
		particulas[i].draw();
	}
	
	
	if(bDrawPtosChoque) {
		ofPushStyle();
		for(int i=0;i<ptsChoque.size();i++) {
			ofCircle(ptsChoque[i].x, ptsChoque[i].y, 3);
		}
		ofPopStyle();
	}
	
	
	
	
	
	//
	// INFO
	// 
	ofPushStyle();
	int hLin = 15; int dLin = 15;
	ofDrawBitmapString("'c' clear camino", 10,hLin); hLin+=dLin;
	ofDrawBitmapString("num pts camino1: " + ofToString(camino1.getVertices().size()), 10,hLin); hLin+=dLin;
	ofDrawBitmapString("num pts camino simpl: " + ofToString(camino.getVertices().size()), 10,hLin); hLin+=dLin;
	ofDrawBitmapString("Num Partics: " + ofToString(particulas.size()), 10,hLin); hLin+=dLin;
	ofDrawBitmapString("p clear partics", 10,hLin); hLin+=dLin;
	ofDrawBitmapString("q/a rate creacion partic: " + ofToString(ratePartic), 10,hLin); hLin+=dLin;
	ofDrawBitmapString("(d) difraccion: " + ofToString(swDifraccion), 10,hLin); hLin+=dLin;

	ofDrawBitmapString("(e) drawCamino: " + ofToString(bDrawCaminos), 10,hLin); hLin+=dLin;
	ofDrawBitmapString("(r) drawChoques: " + ofToString(bDrawPtosChoque), 10,hLin); hLin+=dLin;
	ofDrawBitmapString("(t) tiltCamino: " + ofToString(bTiltCamino), 10,hLin); hLin+=dLin;
	
	ofDrawBitmapString("FR: " + ofToString(ofGetFrameRate()), 10,hLin); hLin+=dLin;
	ofPopStyle();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

	if(key=='c') {
		camino.clear();
		camino1.clear();
		ptsChoque.clear();
		
		testPath.clear();
	}
	else if(key=='d') swDifraccion=!swDifraccion;
	else if(key=='q') ratePartic++;
	else if(key=='a') ratePartic = (ratePartic>2)? ratePartic-1 : 1;
	else if(key=='p') {
		particulas.clear();
	}
	else if(key=='e') bDrawCaminos=!bDrawCaminos;
	else if(key=='r') bDrawPtosChoque=!bDrawPtosChoque;
	else if(key=='t') bTiltCamino=!bTiltCamino;
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	camino1.addVertex(x,y);

	
	testPath.lineTo(x-15+30*ofRandom(1.0),y);	
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	
	camino1.addVertex(x,y);
	
	testPath.newSubPath();
	testPath.moveTo(x+30,y);
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

	camino = camino1;
	camino.simplify(5.2f);

	camino.setClosed(true);
	camino1.setClosed(true);

	testPath.close();

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