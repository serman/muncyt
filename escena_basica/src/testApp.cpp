#include "testApp.h"

//
// HINTS para colisiones y para construccion de objetos con box2d:
// https://www.iforce2d.net/b2dtut/collision-anatomy
// http://forum.openframeworks.cc/t/box2d-contact-listening-and-userdata/3441/3
// 
// Filtrado: 
// http://www.amasso.info/?p=221
//

//
// Colores:
// https://kuler.adobe.com/nuclear-art-colors/
// http://colors-hex.com/
// http://www.workwithcolor.com/
// 

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
	

	fpsAct = 370.0;
	
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
	float radioMuro = MIN(ofGetWidth(), ofGetHeight()) / 2.0*0.85;
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
	velocNeutronDestroy = velocNeutronLim*0.7;
	velocNeutronLanz = 120;

	modoDrawParticulas = MODO_PARTIC;	
	
	ofLogVerbose("Add nucleos");
	// add some nucleos
	int nCircs = 60 + floor(  ofRandom(20) );
	for(int i =0; i<nCircs; i++) {
		addNucleo(centro.x+ofRandom(-1,1), centro.y+ofRandom(-1,1), rNucleo);
	}
	
	cargaSounds();
	
	
	//GUI Fuerza
	float ladoMarco = 90;
	float margenMarco = 10;
	marco = ofRectangle(ofGetWidth()-ladoMarco-margenMarco,margenMarco,ladoMarco,ladoMarco);
	
	
	// Efecto
	chispa = Destello(0,0, 80, 0.3);
	

// Acelerador Exterior - - - 
	// Anillo(ofVec2f _pos, float rInt, float rExt);
	anillo = Anillo(ofVec2f(centro.x,centro.y), radioInt, radioExt);
	
	// Control del anillo
//	ofRectangle anilloUI_L = ofRectangle(0,ofGetHeight()-40, 40,40);
//	ofRectangle anilloUI_R = ofRectangle(ofGetWidth()-40,ofGetHeight()-40, 40,40);
	float dRad = radioExt-radioInt;
	float rMed = radioInt+dRad/2;
	ofRectangle anilloUI_L = ofRectangle(centro.x-rMed-(dRad*0.9/2),centro.y-(dRad*0.9/2), dRad*0.9,dRad*0.9);
	ofRectangle anilloUI_R = ofRectangle(centro.x+rMed-(dRad*0.9/2),centro.y-(dRad*0.9/2), dRad*0.9,dRad*0.9);
	anillo.setUI(anilloUI_L, anilloUI_R);
	
	
// Otros - - - - - 
	swBlendModeADD = false;
	
	
	frecFondo = 4.0;
	
	swDrawTRAILS = false;
	
	myComm.setup();
	
	
	setupTuio();
}

void testApp::cargaSounds() {
	// load the 8 sfx soundfile
	ofLogVerbose("Cargar Sonidos");
	for (int i=0; i<N_SOUNDS; i++) {
		ofLogVerbose("media/sound/"+ofToString(i,0,2,'0')+".mp3");
		sounds[i].loadSound("media/sound/"+ofToString(i,0,2,'0')+".mp3");
		sounds[i].setMultiPlay(true);
		sounds[i].setLoop(false);
		sounds[i].setVolume(0.5);
		sounds[i].stop();
	}
}


//--------------------------------------------------------------
void testApp::update() {
	
	// 
	box2d.update();	

	// Revisar si hay que eliminar particulas
	// 
	// - updateParticula
	// - check algo y borrar si toca
	//
	
	// revisar si hay que add nuevas particulas
	for(int i=0; i<nuevasPartics.size(); i++) {
		CineticData cd = nuevasPartics[i];
		if(cd.tipoPart==tipoNeutron) {
			addNeutron(cd.x, cd.y, velocNeutronLanz, cd.vAng);
			ofLogVerbose("addNeutron en escena************************** n n n ********");
		}
	}
	nuevasPartics.clear();	// **** BORRADO ****

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

//	// Actualizar estado de neutrones y eliminar si procede
	for(int i=0; i<neutrones.size(); i++) {
		//		ofFill();
		float veloc = neutrones[i].get()->getVelocity().length();
		if(veloc>velocNeutronDestroy) {
			neutrones[i].get()->setExcitado(veloc>velocNeutronLim);
		}
		else {
			// destroy
			neutrones[i].get()->destroy();
			neutrones.erase(neutrones.begin()+i);
		}
	}
	
	// Aplica fuerzas
	if(ofGetFrameRate()>0) 	fuerzaAng += fuerzaWAng/ofGetFrameRate();
	fuerza = ofPoint(fuerzaVal*cos(fuerzaAng), fuerzaVal*sin(fuerzaAng));
	
	if(swFuerza) {
		box2d.setGravity(fuerza.x, fuerza.y);
	}
	else box2d.setGravity(0,0); 
	
	// Anillo Aceleracion
	anillo.update();
	
	
	// Update Particulas
	for(int i=0; i<neutrones.size(); i++) {
		neutrones[i].get()->update();		
	}
	for(int i=0; i<nucleos.size(); i++) {
		nucleos[i].get()->update();
	}
	
    //Envio OSC
	if(ofGetFrameNum()%10 ==0){
        myComm.sendPosition(anillo.angT, anillo.wAng);
    }
	
	
	// - - - - TUIOS - - - - 
    tuioClient.getMessage();

	// Interaccion de los blobs recibidos	
	list<ofxTuioCursor*>cursorList = tuioClient.getTuioCursors();
	for(list<ofxTuioCursor*>::iterator it=cursorList.begin(); it != cursorList.end(); it++) {
		ofxTuioCursor *tcur = (*it);

//		ofPoint ptCursor = ofPoint(tcur->getX(),tcur->getY());
		ofPoint ptCursor = tuioPoint2Screen(*tcur);

//		ofLogNotice("nuevo TUIO cursor: " + ofToString(ptCursor));
		
		// Evaluar si esta en posicion de interaccion:		
		if(bordeLine.inside(ptCursor)) {
			addDestello(ptCursor.x, ptCursor.y);
		}
		
		else anillo.interaccionAnillo(ptCursor);
		
    }
	
}

//--------------------------------------------------------------
void testApp::draw() {

//	ofBackground(0);
	ofColor colorCentro = ofColor::fromHsb(60, 50+50*sin(ofGetElapsedTimef()/frecFondo*TWO_PI), 100);
	ofBackgroundGradient (colorCentro, ofColor::black, OF_GRADIENT_CIRCULAR);
//	ofBackgroundGradient(ofColor::gray, ofColor::black, OF_GRADIENT_CIRCULAR);
	
	
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
	
	if(swBlendModeADD) ofEnableBlendMode(OF_BLENDMODE_ADD);
	for(int i=0; i<nucleos.size(); i++) {
		ofFill();
		ofSetHexColor(0xf6c738);
		nucleos[i].get()->draw();
	}
	
	
	for(int i=0; i<neutrones.size(); i++) {
//		ofFill();
		float veloc = neutrones[i].get()->getVelocity().length();
		neutrones[i].get()->setExcitado(veloc>velocNeutronLim);
//		ofSetHexColor(0xf6c738);
		// Eliminar particulas
		if(veloc<velocNeutronLim*0.6) {
			neutrones[i].get()->destroy();
			neutrones.erase(neutrones.begin()+i);
		}
		else {
			neutrones[i].get()->draw();
			ofDrawBitmapString(ofToString(veloc), neutrones[i].get()->getPosition().x, neutrones[i].get()->getPosition().y);
		}
	}

	// dibuja valor fuerza
//	drawFuerza(bordeLine.getCentroid2D(), fuerza);
	if(swDrawTRAILS) {
		for(int i=0; i<nucleos.size(); i++) {
			nucleos[i].get()->drawTrail();
		}		
		for(int i=0; i<neutrones.size(); i++) {
			neutrones[i].get()->drawTrail();
		}
	}
	
	
	// Destellos y Efectos
	for(int i=0; i<destellos.size(); i++) {
		destellos[i].get()->draw();
	}
	chispa.draw();
	
	if(swBlendModeADD) ofDisableBlendMode();
	
	// Mascara exterior
	// ...Con algun Shape...
	anillo.draw();
	
	// GUI ANILLO
	anillo.drawControls();
	
	
	// TUIOS
//    tuioClient.drawCursors();	
	
	
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
	info += "BlendMode ADD [b]: "+ofToString(swBlendModeADD)+" \n";
	info += "aceleracion [click en esquinas inferiores - +] w/a: "+ofToString(anillo.wAng)+"/"+ofToString(anillo.accAng)+" \n\n";

	info += "nuevasPartics: " + ofToString(nuevasPartics.size()) + "\n";
	//	ofSetHexColor(0x444342);
	ofSetHexColor(0xCCCCCC);
	ofDrawBitmapString(info, 30, 30);
	
//	ofSetColor(255);
//	ofLine(ofGetMouseX()*ofGetWidth()/ofGetScreenWidth(),0,ofGetMouseX()*ofGetScreenWidth()/ofGetWidth(),ofGetHeight());

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
	nucleos.back().get()->setPhysics(50, 0.53, 0.1); //setPhysics(float density, float bounce, float friction);
	nucleos.back().get()->setup(box2d.getWorld(), xx, yy, r);		// pos, rr
	nucleos.back().get()->setRotation(ofRandom(360));
	
	nucleos.back().get()->setTexture(texPartic);
	
	// test de Exitado o no
	nucleos.back().get()->setExcitado(true);
//	if(ofRandom(1)<0.5)	nucleos.back().get()->setExcitado(true);
//	else				nucleos.back().get()->setExcitado(false);
	
	
	nucleos.back().get()->setEscala(4.0);
	nucleos.back().get()->setTipo(tipoNucleo);
	
	nucleos.back().get()->setColorExcitado(ofColor::fromHsb(60,250,250));
	nucleos.back().get()->setColor(ofColor::fromHsb(60,250,100));
	
	nucleos.back().get()->setModoDraw(modoDrawParticulas);
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

//	ofLogVerbose("Nuevo Neutron. INI. #neutrones: "+ofToString(neutrones));
	neutrones.push_back(ofPtr<Particula>(new Particula));
	neutrones.back().get()->setPhysics(50.5, 0.85, 0.1); // setPhysics(float density, float bounce, float friction);
	neutrones.back().get()->setup(box2d.getWorld(), xx, yy, rNeutron);		// pos, rr
	neutrones.back().get()->setRotation(ofRandom(360));
	neutrones.back().get()->setTexture(texPartic);
	neutrones.back().get()->setColorExcitado(ofColor::fromHsb(0,250,250));
	neutrones.back().get()->setColor(ofColor(0x5231c9));

	neutrones.back().get()->setModoDraw(modoDrawParticulas);

	neutrones.back().get()->setVelocity(vVal*cos(vAng),vVal*sin(vAng));
	
	neutrones.back().get()->setExcitado(true);
	neutrones.back().get()->setEscala(0.7f);
	neutrones.back().get()->setTipo(tipoNeutron);
	
	// * * * * * * *
	// A corregir:
	// http://forum.openframeworks.cc/t/box2d-contact-listening-and-userdata/3441/3
	//
	neutrones.back().get()->setData(neutrones.back().get());
	// * * * * * * *
//	ofLogVerbose("Nuevo Neutron. FIN. #neutrones: "+ofToString(neutrones));
	
}

void testApp::addDestello(float px, float py) {
	
	ofPtr<Destello> destPtr(new Destello(px,py,40, 0.3));
	destPtr->start();
	destellos.push_back(destPtr);
	
}



//--------------------------------------------------------------
void testApp::keyPressed(int key) {
	ofLogVerbose("keyPressed");
	
	if(key == 'c') {
//		addNucleo(mouseX, mouseY, rNucleo);
		addNucleo();
	}
	
	if(key == 'n') {
		addNeutron(mouseX, mouseY);
	}
	
	if(key=='q') {
		// Quitar particulas
		for(int i=0; i<nucleos.size(); i++) {
			nucleos[i].get()->destroy();
		}
		nucleos.clear();
		for(int i=0; i<neutrones.size(); i++) {
			neutrones[i].get()->destroy();
		}
		neutrones.clear();
	}
	
	if(key=='f') swFuerza = !swFuerza;
	
	if(key=='x') {
		myComm.sendExplosion();
	}
	
	if(key=='t') {
		fpsAct = ofMap(mouseX,0,ofGetWidth(), 20.0, 600.0);
		box2d.setFPS(fpsAct);
	}
	
	if(key == 's') ofToggleFullscreen();
	
	if(key=='b') swBlendModeADD=!swBlendModeADD;
	
	if(key=='1'){
		// cambiar particulas a modo PARTIC
		setModoParticulas(MODO_PARTIC);
	}
	else if(key=='2') {
		// cambiar particulas a modo PATH
		setModoParticulas(MODO_PATH);
	}
	else if(key=='4') {
		swDrawTRAILS = !swDrawTRAILS;
	}
}

void testApp::setModoParticulas(int _modo) {
	modoDrawParticulas = _modo;
	ofLogVerbose("setModoParticulas modo Draw: " + ofToString(_modo));
	for(int i=0; i<neutrones.size(); i++) {
		ofLogVerbose("neutron: " + ofToString(i));
		neutrones[i].get()->setModoDraw(_modo);
	}
	for(int i=0; i<nucleos.size(); i++) {
		nucleos[i].get()->setModoDraw(_modo);
	}
}

//--------------------------------------------------------------
void testApp::keyReleased(int key) {
	ofLogVerbose("keyReleased");

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
	
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button) {
	if(marco.inside(x,y)) {
		fuerzaWAng = ofDegToRad( ofMap(x-marco.x, 0, marco.width, 30, 360*6) );
		fuerzaVal = ( ofMap(y-marco.y, 0, marco.height, 0, 200) );
	}

	anillo.interaccionAnillo(x,y);
	
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


// ------------------------- TUIO ----------------------

void testApp::setupTuio() {
	
	tuioClient.start(3333);
    
    ofAddListener(tuioClient.cursorAdded,this,&testApp::tuioAdded);
	ofAddListener(tuioClient.cursorRemoved,this,&testApp::tuioRemoved);
	ofAddListener(tuioClient.cursorUpdated,this,&testApp::tuioUpdated);
	
}

void testApp::tuioAdded(ofxTuioCursor &tuioCursor){
	//	ofPoint loc = ofPoint(tuioCursor.getX()*ofGetWidth(),tuioCursor.getY()*ofGetHeight());
	ofPoint loc = tuioPoint2Screen(tuioCursor);

	// Si se quiere que solo interaccione cuando 
	anillo.interaccionAnillo(loc);
	
	cout << "Point n" << tuioCursor.getSessionId() << " add at " << loc << endl;
}

void testApp::tuioUpdated(ofxTuioCursor &tuioCursor){
	//	ofPoint loc = ofPoint(tuioCursor.getX()*ofGetWidth(),tuioCursor.getY()*ofGetHeight());
	ofPoint loc = tuioPoint2Screen(tuioCursor);
	cout << "Point n" << tuioCursor.getSessionId() << " updated at " << loc << "    x: " << tuioCursor.getX() << endl;
}

void testApp::tuioRemoved(ofxTuioCursor &tuioCursor){
	//	ofPoint loc = ofPoint(tuioCursor.getX()*ofGetWidth(),tuioCursor.getY()*ofGetHeight());
	ofPoint loc = tuioPoint2Screen(tuioCursor);
	cout << "Point n" << tuioCursor.getSessionId() << " remove at " << loc << endl;
}

ofPoint testApp::tuioPoint2Screen(ofxTuioCursor &tuioCursor) {
	ofLogNotice("ESTOY EN tuioPoint2Screen");
//	return ofPoint(tuioCursor.getX()*ofGetScreenWidth(),tuioCursor.getY()*ofGetScreenHeight());	
	cout << tuioCursor.getX() << ", " << tuioCursor.getY() << endl;
	return ofPoint(tuioCursor.getX()*1024/0.6,tuioCursor.getY()*768/0.7);
	
//	return ofPoint(tuioCursor.getX()*ofGetWidth()* 2,tuioCursor.getY()*ofGetHeight() * 2);
//	return ofPoint(tuioCursor.getX()*ofGetScreenWidth(),tuioCursor.getY()*ofGetScreenHeight());
}



