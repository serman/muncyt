#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {
//    ofDisableAntiAliasing();
	ofSetVerticalSync(true);
	ofBackgroundHex(0xfdefc2);
//	ofSetLogLevel(OF_LOG_NOTICE);
	ofSetLogLevel(OF_LOG_VERBOSE);

	ofEnableSmoothing();
	
	// Cargar Textura de particulas
	// load the texure
	ofDisableArbTex();
	ofLoadImage(texPartic, "media/images/dot.png");
	

	fpsAct = 30.0;
	
	// BOX2D
	box2d.init();
	box2d.setGravity(0, 0);
	box2d.createBounds();
//	box2d.setFPS(30.0);
	box2d.setFPS(fpsAct);
//	box2d.registerGrabbing();	// Esto creo que es para interaccion con mouse

	// EVENTOS
	// register the listener so that we get the events
    box2d.enableEvents();
	ofAddListener(box2d.contactStartEvents, this, &testApp::contactStart);
	ofAddListener(box2d.contactEndEvents, this, &testApp::contactEnd);
	
	
	// Poner Muro circular
	float radioMuro = MIN(ofGetWidth(), ofGetHeight()) / 2.0*0.8;
	int resol = 360/5;

	radioInt = radioMuro;
	radioExt = MIN(ofGetWidth(), ofGetHeight()) / 2.0*0.98;

	// Preparo el polyline
	ofPoint centro = ofPoint(ofGetWidth()/2.0, ofGetHeight()/2.0);
	bordeLine.arc(centro, radioMuro, radioMuro, 0.0, 360.0, true, resol);
	
	borde.clear();
    borde.addVertexes(bordeLine);
    borde.create(box2d.getWorld());
	
	// Paredes camara
//	circuloInt.arc(centro, radioMuro, radioMuro, 0.0, 360.0, true, resol);
	circuloExt.arc(centro, radioExt, radioExt, 0.0, 360.0, true, resol);
	
	// Fuerza que afecta a todas las particulas
	fuerzaVal = 100.0;
	fuerzaAng = 0.0;
	fuerzaWAng = ofDegToRad(90.0);

//	fuerzaAng += fuerzaWAng/ofGetFrameRate();
	fuerza = ofPoint(fuerzaVal*cos(fuerzaAng), fuerzaVal*sin(fuerzaAng));

	swFuerza = false;
	
	// radios
	rNucleo	= 14;
	rNeutron = 5;
	
	velocNeutronLim = 80;
	velocNeutronLanz = 120;
	
	
	ofLogVerbose("Add nucleos");
	// add some nucleos y boxes
	int nCircs = 40 + floor(  ofRandom(20) );
	for(int i =0; i<nCircs; i++) {
		addNucleo(centro.x+ofRandom(-1,1), centro.y+ofRandom(-1,1), rNucleo);
	}
	
//	int nBoxes = 10 + floor(  ofRandom(10) );
//	for(int i =0; i<nBoxes; i++) {
//		addBox(centro.x, centro.y);
//	}
	
	cargaSounds();
	
	
	//GUI Fuerza
	float ladoMarco = 90;
	float margenMarco = 20;
	marco = ofRectangle(ofGetWidth()-ladoMarco-margenMarco,margenMarco,ladoMarco,ladoMarco);
	
	
	// Efecto
	chispa = Destello(0,0, 80, 0.3);
	
	
}

void testApp::cargaSounds() {
	// load the 8 sfx soundfile
	ofLogVerbose("Cargar Sonidos");
	for (int i=0; i<N_SOUNDS; i++) {
		ofLogVerbose("media/sound/"+ofToString(i,0,2,'0')+".mp3");
		sounds[i].loadSound("media/sound/"+ofToString(i,0,2,'0')+".mp3");
		sounds[i].setMultiPlay(true);
		sounds[i].setLoop(false);
		sounds[i].stop();
	}
}


//--------------------------------------------------------------
void testApp::contactStart(ofxBox2dContactArgs &e) {
	if(e.a != NULL && e.b != NULL) { 
		// Si chocan dos objetos Circle (dos particulas) suena un tipo de sonido
		// Se puede hacer dependiente de 
		if(e.a->GetType() == b2Shape::e_circle && e.b->GetType() == b2Shape::e_circle) {
			sounds[0].play();
			
//			ofLogVerbose("Choque cirle-circle");

//			SoundData * aData = (SoundData*)e.a->GetBody()->GetUserData();
//			SoundData * bData = (SoundData*)e.b->GetBody()->GetUserData();
			
//			if(aData) {
//				aData->bHit = true;
//				sound[aData->soundID].play();
//			}
//			
//			if(bData) {
//				bData->bHit = true;
//				sound[bData->soundID].play();
//			}
		}
		else {
			// Si el choque no es entre dos particulas (circles), suena otra cosa.
			// Por ejemplo el choque con la pared de la c‡mara
			sounds[1].play();
//			ofLogVerbose("Choque xx-xx");
		}
	}
}

//--------------------------------------------------------------
void testApp::contactEnd(ofxBox2dContactArgs &e) {
	if(e.a != NULL && e.b != NULL) { 
		// si chocan dos particulas
		if(e.a->GetType() == b2Shape::e_circle && e.b->GetType() == b2Shape::e_circle) {
			Particula * pA = (Particula*)e.a->GetBody()->GetUserData();
			Particula * pB = (Particula*)e.b->GetBody()->GetUserData();
			
			// Aqu’ indica como recoger el punto de contacto en box2D:
			// https://www.iforce2d.net/b2dtut/collision-anatomy
			//
			
			if((pA->tipo == tipoNeutron && pB->tipo == tipoNucleo) ||
			   (pA->tipo == tipoNucleo  && pB->tipo == tipoNeutron)) {
				
				//ofLogVerbose("Fin choque. pa-pb: " + ofToString(pA->tipo) + "-" + ofToString(pB->tipo));
				// add un destello en el punto de colision
				
				ofVec2f ptChoque = (pA->getPosition() + pB->getPosition())/2.0;
							
				// desexcitar el nucleo
				if(pA->tipo == tipoNucleo) {
					if(pA->swExcitado) {
						pA->setExcitado(false);

						addDestello(ptChoque.x, ptChoque.y);
						
						// add un nuevo neutron, y tal vez duplicar el nucleo
						// Calcular el punto para que no choque con el nucleo
						ofVec2f dirNucleo = pA->getVelocity();
						ofVec2f posNucleo = pA->getPosition();
						ofVec2f dirNeutron = pB->getVelocity();
						ofVec2f posNeutron = pB->getPosition();
						
						ofVec2f posNeutron2 = pB->getPosition();
						ofVec2f dirNeutron2 = pB->getVelocity();
						dirNeutron2.rotate( -2.0 * dirNeutron2.angle(dirNucleo) );
						
						ofVec2f distPBA = posNeutron - posNucleo;
						float angPBA_dirNucleo = distPBA.angle(dirNucleo);
						distPBA.rotate(-2*angPBA_dirNucleo);
						
						posNeutron2 = posNucleo + distPBA;
						
						// Da error al crear el objeto
						// Hay que a–adirlo a una lista de creacion de neutrones
//						addNeutron(posNeutron2.x, posNeutron2.y, 
//								   velocNeutronLanz*0.95, 
//								   atan2(dirNeutron2.y, dirNeutron2.x));
						
						addDestello(posNeutron2.x, posNeutron2.y);
						
					}
				} 
				else {
					if(pB->swExcitado) {
						pB->setExcitado(false);

						addDestello(ptChoque.x, ptChoque.y);
						// add un nuevo neutron, y tal vez duplicar el nucleo
						
					}
					
				}
			}
		}
		
//		
//		SoundData * aData = (SoundData*)e.a->GetBody()->GetUserData();
//		SoundData * bData = (SoundData*)e.b->GetBody()->GetUserData();
//		
//		if(aData) {
//			aData->bHit = false;
//		}
//		
//		if(bData) {
//			bData->bHit = false;
//		}
	}
}



//--------------------------------------------------------------
void testApp::update() {
	box2d.update();	

	// Revisar si hay que eliminar particulas
	// 
	// - updateParticula
	// - check algo y borrar si toca
	//
	
	// Update Destellos
	for(int i=destellos.size()-1; i>0; i--) {
		
		if(destellos[i].get()->isFinished) {
			// eliminar el destello de la lista
			destellos.erase(destellos.begin()+i);
		} else {
			// update
			destellos[i].get()->update();
		}
	}
	chispa.update();
	
	
	// Aplica fuerzas
	if(ofGetFrameRate()>0) 	fuerzaAng += fuerzaWAng/ofGetFrameRate();
	fuerza = ofPoint(fuerzaVal*cos(fuerzaAng), fuerzaVal*sin(fuerzaAng));
	
	if(swFuerza) {
		box2d.setGravity(fuerza.x, fuerza.y);
	}
	else box2d.setGravity(0,0); 
	
	
}

//--------------------------------------------------------------
void testApp::draw() {

	// Selector Fuerza: 
	// caja fuera de los dos circulos
	drawFuerzaSelector();
	
	// PAREDES
	ofPushStyle();
		ofEnableSmoothing();
		ofNoFill();
		ofSetLineWidth(1.0);
		ofSetColor(10,10,10);
		
		// PARED EXTERIOR
		circuloExt.draw();
		
		// PARED INTERIOR
		//box2d.drawGround();
		bordeLine.draw();
	ofPopStyle();
	
	for(int i=0; i<nucleos.size(); i++) {
		ofFill();
		ofSetHexColor(0xf6c738);
		nucleos[i].get()->draw();
	}
	for(int i=0; i<neutrones.size(); i++) {
//		ofFill();
		float veloc = neutrones[i].get()->getVelocity().length();
		if(veloc>velocNeutronLim) neutrones[i].get()->setColor(ofColor(255,0,0));
		else neutrones[i].get()->setColor(ofColor(200,200,200));
//		ofSetHexColor(0xf6c738);
		neutrones[i].get()->draw();
		ofDrawBitmapString(ofToString(veloc), neutrones[i].get()->getPosition().x, neutrones[i].get()->getPosition().y);
	}
	
	for(int i=0; i<boxes.size(); i++) {
		ofFill();
		ofSetHexColor(0xBF2545);
		boxes[i].get()->draw();
	}


	// dibuja valor fuerza
//	drawFuerza(bordeLine.getCentroid2D(), fuerza);
	
	
	// Destellos y Efectos
	for(int i=0; i<destellos.size(); i++) {
		destellos[i].get()->draw();
	}
	chispa.draw();
	
	
	// Mascara exterior
	// ...Con algun Shape...

	
	// Info
	string info = "";
	info += "Press [c] for nucleos\n";
	info += "Press [n] for neutron\n";
	info += "Press [t] + mouseX para Time Step: "+ofToString(fpsAct)+"\n";
	info += "time en millis: "+ ofToString(ofGetElapsedTimef()) +"\n";
	info += "Total Bodies: "+ofToString(box2d.getBodyCount())+"\n";
	info += "Total Joints: "+ofToString(box2d.getJointCount())+"\n\n";
	info += "FPS: "+ofToString(ofGetFrameRate(), 1)+"\n\n";
	info += "Aplica Fuerza [f]: "+ofToString(swFuerza)+"\n";
	info += "valor Fuerza [mouseY+Click+f]: "+ofToString(fuerzaVal)+" \n";
	info += "w Giro Fuerza [mouseX+Click+f]: "+ofToString(ofRadToDeg(fuerzaWAng)/360)+" Hz\n\n";
	ofSetHexColor(0x444342);
	ofDrawBitmapString(info, 30, 30);
}

void testApp::drawFuerzaSelector() {
	//	fuerzaWAng = ofDegToRad( ofMap(x, 0, ofGetWidth(), 30, 360*6) );
	//	fuerzaVal = ofDegToRad( ofMap(y, 0, ofGetHeight(), 0, 200) );
	ofPushStyle();
	ofPushMatrix();
	ofTranslate(marco.x, marco.y, 0);
	//		ofSetColor(0,0,100);
	ofSetColor(10,10,10);
	ofFill();
	ofRect(0,0,marco.width, marco.height);
	ofSetColor(255);
	ofNoFill();
	ofRect(0,0,marco.width, marco.height);
	
	ofSetColor(255, 200);
	// valor de la fuerza
	float posY = ofMap(fuerzaVal, 0, 200, 0, marco.height);
	ofLine(0, posY, marco.width, posY);
	// valor de la veloc de rotacion de la fuerza
	float posX = ofMap(ofRadToDeg(fuerzaWAng), 30, 360*6, 0, marco.width);
	ofLine(posX, 0, posX, marco.height);
	
	
	
	//marco.getCenter();
	drawFuerza(ofPoint(marco.width/2, marco.height/2), fuerza, 0.3);
	
	ofPopMatrix();
	ofPopStyle();
}

void testApp::drawFuerza(ofPoint p, ofPoint gravity, float esc) {
    float angle = (atan2(gravity.y, gravity.x) * 180 / PI) - 90;
//    float len   = MIN(100, gravity.length()*10); // scale it up a bit
    float len   = gravity.length()*esc; // scale it up a bit
    
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




//--------------------------------------------------------------
void testApp::addNucleo(){
	// Lo pone a una distancia dd del centro
	float dd = radioInt*0.3;
	float angTmp = ofRandom(TWO_PI);
	ofPoint ptoOut = bordeLine.getCentroid2D();
	addNucleo(bordeLine.getCentroid2D().x + dd*cos(angTmp), 
			  bordeLine.getCentroid2D().y + dd*sin(angTmp),
			  rNucleo);
}

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
	
	// test de Exitado o no
	nucleos.back().get()->setExcitado(true);
//	if(ofRandom(1)<0.5)	nucleos.back().get()->setExcitado(true);
//	else				nucleos.back().get()->setExcitado(false);
	
	nucleos.back().get()->setTipo(tipoNucleo);
	
	// * * * * * * *
	// A corregir:
	// http://forum.openframeworks.cc/t/box2d-contact-listening-and-userdata/3441/3
	//
	nucleos.back().get()->setData(nucleos.back().get());	
	// * * * * * * *
	
}

void testApp::addNeutron(int xx, int yy){
	
	// Lanzarla con velocidad
	float vVal = velocNeutronLanz;
	float vAng = -HALF_PI;//ofRandom(TWO_PI);
	
	addNeutron(xx,yy,vVal,vAng);
}

void testApp::addNeutron(int xx, int yy, float vVal, float vAng){

	neutrones.push_back(ofPtr<Particula>(new Particula));
	neutrones.back().get()->setPhysics(50.5, 0.85, 0.1); // setPhysics(float density, float bounce, float friction);
	neutrones.back().get()->setup(box2d.getWorld(), xx, yy, rNeutron);		// pos, rr
	neutrones.back().get()->setRotation(ofRandom(360));
	neutrones.back().get()->setTexture(texPartic);
	neutrones.back().get()->setColor(ofColor(0x5231c9));

	neutrones.back().get()->setVelocity(vVal*cos(vAng),vVal*sin(vAng));
	
	neutrones.back().get()->setExcitado(true);
	
	neutrones.back().get()->setTipo(tipoNeutron);
	
	// * * * * * * *
	// A corregir:
	// http://forum.openframeworks.cc/t/box2d-contact-listening-and-userdata/3441/3
	//
	neutrones.back().get()->setData(neutrones.back().get());
	// * * * * * * *
	
}

void testApp::addDestello(float px, float py) {
	
	ofPtr<Destello> destPtr(new Destello(px,py,40, 0.3));
	destPtr->start();
	destellos.push_back(destPtr);
	
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
void testApp::keyPressed(int key) {
	
	if(key == 'c') {
//		addNucleo(mouseX, mouseY, rNucleo);
		addNucleo();
	}
	
	if(key == 'n') {
		addNeutron(mouseX, mouseY);
	}
	
	if(key=='q') {
		// Quitar particulas
		nucleos.clear();
		neutrones.clear();
	}
	
	if(key=='f') swFuerza = !swFuerza;
	
	if(key=='t') {
		fpsAct = ofMap(mouseX,0,ofGetWidth(), 20.0, 600.0);
		box2d.setFPS(fpsAct);
	}
	
	if(key == 's') ofToggleFullscreen();
}

//--------------------------------------------------------------
void testApp::keyReleased(int key) {
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ) {
	
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button) {
	if(marco.inside(x,y)) {
	   fuerzaWAng = ofDegToRad( ofMap(x-marco.x, 0, marco.width, 30, 360*6) );
	   fuerzaVal = ( ofMap(y-marco.y, 0, marco.height, 0, 200) );
	}
//	if(swFuerza) {
//		fuerzaWAng = ofDegToRad( ofMap(x, 0, ofGetWidth(), 30, 360*6) );
//		fuerzaVal = ( ofMap(y, 0, ofGetHeight(), 0, 200) );
//	}
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button) {
	if(marco.inside(x,y)) {
		fuerzaWAng = ofDegToRad( ofMap(x-marco.x, 0, marco.width, 30, 360*6) );
		fuerzaVal = ( ofMap(y-marco.y, 0, marco.height, 0, 200) );
	}
	//	if(swFuerza) {
//		fuerzaWAng = ofDegToRad( ofMap(x, 0, ofGetWidth(), 30, 360*6) );
//		fuerzaVal = ( ofMap(y, 0, ofGetHeight(), 0, 200) );
//	}
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button) {
	if(!swFuerza) {
		chispa.pos = ofPoint(x,y);
		chispa.start();
		
		addDestello(x,y);
	}
}

//--------------------------------------------------------------
void testApp::resized(int w, int h){
}

