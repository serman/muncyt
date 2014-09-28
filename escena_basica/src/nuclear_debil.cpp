#include "nuclear_debil.h"

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
void nuclear_debil::setup() {
    
//    ofDisableAntiAliasing();
	ofSetVerticalSync(true);
	ofBackgroundHex(0xfdefc2);
//	ofSetLogLevel(OF_LOG_NOTICE);
	ofSetLogLevel(OF_LOG_NOTICE);

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
//	box2d.setFPS(fpsAct);
//	box2d.registerGrabbing();	// Esto creo que es para interaccion con mouse

	// Lo paso a init_Escena()
	// EVENTOS
	// register the listener so that we get the events
//    box2d.enableEvents();
//	ofAddListener(box2d.contactStartEvents, this, &nuclear_debil::contactStart);
//	ofAddListener(box2d.contactEndEvents, this, &nuclear_debil::contactEnd);
	
	
	// Poner Muro circular
	float radioMuro = MIN(W_WIDTH, W_HEIGHT) / 2.0*0.85;
	int resol = 360;

	radioInt = radioMuro;
	radioExt = MIN(W_WIDTH, W_HEIGHT) / 2.0*0.98;
    cout<< radioInt << " exterior " << radioExt << endl;
	// Preparo el polyline
	ofPoint centro = ofPoint(W_WIDTH/2.0, W_HEIGHT/2.0);
	bordeLine.arc(centro, radioMuro, radioMuro, 0.0, 360.0, true, resol);
	
	borde.clear();
    borde.addVertexes(bordeLine);
    borde.create(box2d.getWorld());
	
	// Paredes camara
//	circuloInt.arc(centro, radioMuro, radioMuro, 0.0, 360.0, true, resol);
	circuloExt.arc(centro, radioExt, radioExt, 0.0, 360.0, true, resol);
    entrada1.arc(ofPoint(centro.x,centro.y-radioExt+100), 100, 100, 125, 270,true,resol);
    entrada2.arc(ofPoint(centro.x,centro.y-radioExt+100), (radioExt-radioInt), (radioExt-radioInt), 125, 270,true,resol);
	
	// Fuerza que afecta a todas las particulas
	fuerzaVal = 100.0;
	fuerzaAng = 0.0;
	fuerzaWAng = ofDegToRad(90.0);

//	fuerzaAng += fuerzaWAng/ofGetFrameRate();
	fuerza = ofPoint(fuerzaVal*cos(fuerzaAng), fuerzaVal*sin(fuerzaAng));
	
	// Lo paso a init_Escena
//	swFuerza = false;	// Lo paso a init_Escena
	
	// radios
	rNucleo	= 10;
	rNeutron = 3;
	
	velocNeutronLim = 80;
	velocNeutronDestroy = velocNeutronLim*0.7;
	velocNeutronLanz = 120;

	
	cargaSounds();
	
	
	//GUI Fuerza
	float ladoMarco = 90;
	float margenMarco = 10;
	marco = ofRectangle(W_WIDTH-ladoMarco-margenMarco,margenMarco,ladoMarco,ladoMarco);
	
	
	// Efecto
	chispa = Destello(0,0, 80, 0.3);
	

// Acelerador Exterior - - - 
	anillo = Anillo(ofVec2f(centro.x,centro.y), radioInt, radioExt);
	anillo.setTexture(texPartic);
	
	// Control del anillo
//	ofRectangle anilloUI_L = ofRectangle(0,ofGetHeight()-40, 40,40);
//	ofRectangle anilloUI_R = ofRectangle(W_WIDTH-40,ofGetHeight()-40, 40,40);
	float dRad = radioExt-radioInt;
	float rMed = radioInt+dRad/2;
	//buttonSpeed1.set(centro.x-rMed-(dRad*0.9/2),centro.y-(dRad*0.9/2), dRad*0.9,dRad*0.9);
    buttonSpeed1.set(5, 314,57,143);
    buttonSpeed1.setup();
//	buttonSpeed2.set(centro.x+rMed-(dRad*0.9/2),centro.y-(dRad*0.9/2), dRad*0.9,dRad*0.9);
    buttonSpeed1.btype=TYPE_ACC;

    buttonSpeed2.set(703,314,57,143);
    buttonSpeed2.setup();
    buttonSpeed2.btype=TYPE_ACC;
    
    //buttonCollide.set(centro.x-(dRad*0.9/2),centro.y-rMed-(dRad*0.9/2), dRad*0.9,dRad*0.9);
    buttonCollide.set(309,5,143,57);
    buttonCollide.btype=TYPE_CRASH;
    buttonCollide.setup();

    touchElements.addObject(buttonSpeed1);
    touchElements.addObject(buttonSpeed2);
    touchElements.addObject(buttonCollide);

	
// Otros - - - - - 
	swBlendModeADD = false;
	
	frecFondo = 4.0;

	// Esto deber’a ser comœn a todas las escenas
//	myComm.setup();
	
	//setupTuio();
	
	//init_Escena();
}

void nuclear_debil::init_Escena() {
	// init variables
	fpsAct = 370.0;
	box2d.setFPS(fpsAct);

	swFuerza = false;
	
	modoDrawParticulas = MODO_PARTIC;	
	swDrawTRAILS = true;

	anillo.reset_wAng();
    anillo.setFadeInMode();
	
	ofPoint centro = ofPoint(W_WIDTH/2.0, W_HEIGHT/2.0);

	// crear cosas
	ofLogVerbose("Add nucleos");
	// add some nucleos
	nucleos.clear();
    nCircs = 60 + floor(  ofRandom(20) );
	for(int i =0; i<nCircs; i++) {
		addNucleo(centro.x+ofRandom(-1,1), centro.y+ofRandom(-1,1), rNucleo);
	}

	
	// addListeners
    box2d.enableEvents();
	ofAddListener(box2d.contactStartEvents, this, &nuclear_debil::contactStart);
	ofAddListener(box2d.contactEndEvents, this, &nuclear_debil::contactEnd);
	
    ofAddListener(buttonSpeed2.buttonEvent ,this, &nuclear_debil::onButtonPressed);
    ofAddListener(buttonSpeed1.buttonEvent ,this, &nuclear_debil::onButtonPressed);
    ofAddListener(buttonCollide.buttonEvent ,this, &nuclear_debil::onButtonPressed);
	
}

void nuclear_debil::exit_Escena() {

	// borrar objetos
	// Eliminar neutrones
	borrar_neutrones();
	
	// Eliminar nucleos
	borrar_nucleos();
	
	// Eliminar los destellos que queden
	destellos.clear();
	
	
	// quitar listeners
    box2d.disableEvents();
	ofRemoveListener(box2d.contactStartEvents, this, &nuclear_debil::contactStart);
	ofRemoveListener(box2d.contactEndEvents, this, &nuclear_debil::contactEnd);
	
    ofRemoveListener(buttonSpeed2.buttonEvent ,this, &nuclear_debil::onButtonPressed);
    ofRemoveListener(buttonSpeed1.buttonEvent ,this, &nuclear_debil::onButtonPressed);
    ofRemoveListener(buttonCollide.buttonEvent ,this, &nuclear_debil::onButtonPressed);	
	
	
}

void nuclear_debil::cargaSounds() {
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
void nuclear_debil::update(float dt) {
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
		nucleos[i].get()->addForce(ofVec2f(nucleos[i].get()->pos.x-W_WIDTH/2.0,nucleos[i].get()->pos.y-W_HEIGHT/2.0), -2.7);
		nucleos[i].get()->update();
	}
	
    //Envio OSC
	if(ofGetFrameNum()%2 ==0){
        cheapComm::getInstance()->sendAudio2("/audio/weak_nuclear/ball", -1*(anillo.angT)/(2*PI),ofMap(anillo.wAng,0,-anillo.wAngMax,0,1));
        cout << anillo.angT<< " " << -1*(anillo.angT)/(2*PI) <<endl;
        
        
        cheapComm::getInstance()->sendAudio2("/audio/weak_nuclear/number_particles", ofMap(nucleos.size(),nCircs,nCircs+50,0,1), ofMap(anillo.angT,0,2*PI,0,1));
        cout << nucleos.size()<< " ";
        
//        CUAL ES EL MAXIMO DE NUCLEOS AQUI?
    }
	
	
	// - - - - TUIOS - - - - 
    //tuioClient.getMessage();

    buttonCollide.update_prev(anillo.getParticlePosition());
    buttonSpeed1.update_prev(anillo.getParticlePosition());
    buttonSpeed2.update_prev(anillo.getParticlePosition());
    
	touchElements.update();
    hands.update();
	
	// recorrer el vector de cursores que hay en hands (tangiblesHandler) e interactuar si esta dentro del c’rculo
	int ttAct = ofGetElapsedTimeMillis();
	float sqLim = (radioInt*radioInt)*0.9;
	int limTime = floor(1000/ofGetFrameRate())+20;
	for(int i=0; i<hands.objectsCol.size(); i++) {
//		ofLogNotice("handShadow num: " + ofToString(i));
		handShadow * h = (handShadow *) hands.objectsCol[i];
//		ofLogNotice("x,y: " + ofToString(h->x)+"/"+ofToString(h->y)+ "   age: " + ofToString(h->age));
		// si esta en la parte central del c’rculo
		ofVec2f vHand = ofVec2f(h->x,h->y);
//		ofVec2f vHandCentro = ofVec2f(x-W_WIDTH/2.0,y-W_HEIGHT/2.0);
		if(vHand.squareDistance(ofVec2f(W_WIDTH/2.0,W_HEIGHT/2.0)) < sqLim ) {
			int dtt = ttAct-h->age;
			if(dtt%500<limTime) {
				addNucleo(h->x, h->y);
			}
		}
	}
	

	
}

//--------------------------------------------------------------
void nuclear_debil::draw(){
    ofColor colorCentro = ofColor::fromHsb(0, 0, 30+30*sin(ofGetElapsedTimef()/frecFondo*TWO_PI));
    ofBackgroundGradient (colorCentro, ofColor::black, OF_GRADIENT_CIRCULAR);
 //   ofBackground(colorCentro);
    ofPushMatrix(); //colocamos el canvas en su posicion centrada
    ofTranslate((ofGetWidth()-W_WIDTH)/2, 0);
    
//	ofBackground(0);

//	ofBackgroundGradient(ofColor::gray, ofColor::black, OF_GRADIENT_CIRCULAR);
	
	
	// Selector Fuerza: 
	// caja fuera de los dos circulos
	drawFuerzaSelector();
	
	// PAREDES
	ofPushStyle();
		ofEnableSmoothing();
		ofNoFill();
		ofSetLineWidth(1.0);
		ofSetColor(220,220,200);
		
		// PARED EXTERIOR
    	ofPoint centro = ofPoint(W_WIDTH/2.0, W_HEIGHT/2.0);
        circuloExt.clear();
        circuloExt.arc(centro, radioExt, radioExt, 0.0, 360.0, true, 180);
        bordeLine.clear();
    	float radioMuro = MIN(W_WIDTH, W_HEIGHT) / 2.0*0.85;
    	bordeLine.arc(centro, radioMuro, radioMuro, 0.0, 360.0, true, 180);
		//circuloExt.draw();
	
		entrada1.clear(); 
		entrada2.clear();
	
		entrada1.arc(ofPoint(centro.x,centro.y-radioExt+100), 100, 100, 125, 270,true,180);
		entrada2.arc(ofPoint(centro.x,centro.y-radioExt+100), (radioExt-radioInt), (radioExt-radioInt), 125, 270,true,180);

		//entrada1.draw();
		//entrada2.draw();
		// PARED INTERIOR
		//box2d.drawGround();
		//bordeLine.draw();
	ofPopStyle();
    // Mascara exterior
	// ...Con algun Shape...
	anillo.draw();

    
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

    // Botones para TUIO
    
    touchElements.draw();
	
	
	
	// TUIOS
//    tuioClient.drawCursors();	
    hands.draw();
	
    // Circulo de info
    
    ofPushStyle();
    
        ofSetColor(ofColor::orange, 100);
        ofNoFill();
        ofSetLineWidth(3);
        ofCircle(W_WIDTH/2,W_HEIGHT/2, 70);
        ofSetLineWidth(1);
        float rr = 40;
        ofFill();
        ofCircle(W_WIDTH/2,W_HEIGHT/2, rr);
    
    
    
    
    ofPopStyle();

    
    
    ofPopMatrix();  // FIn translate para centrar contenidos
    
    
	// Info
	drawInfo();

//	ofSetColor(255);
//	ofLine(ofGetMouseX()*W_WIDTH/ofGetScreenWidth(),0,ofGetMouseX()*ofGetScreenWidth()/W_WIDTH,ofGetHeight());

}

void 	nuclear_debil::drawInfo(){
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
	info += "veloc Ang: " + ofToString(anillo.wAng) + "\n";

	info += "nuevasPartics: " + ofToString(nuevasPartics.size()) + "\n";
	//	ofSetHexColor(0x444342);
	ofSetHexColor(0xCCCCCC);
	ofDrawBitmapString(info, 30, 30);
}

void nuclear_debil::drawFuerzaSelector() {
	//	fuerzaWAng = ofDegToRad( ofMap(x, 0, W_WIDTH, 30, 360*6) );
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

void nuclear_debil::drawFuerza(ofPoint p, ofPoint gravity, float esc) {
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
void nuclear_debil::addNucleo(){
	// Lo pone a una distancia dd del centro
	float dd = radioInt*0.3;
	float angTmp = ofRandom(TWO_PI);
	ofPoint ptoOut = bordeLine.getCentroid2D();
	addNucleo(bordeLine.getCentroid2D().x + dd*cos(angTmp), 
			  bordeLine.getCentroid2D().y + dd*sin(angTmp),
			  rNucleo);
}

void nuclear_debil::addNucleo(int xx, int yy){
	float r = ofRandom(rNucleo-2,rNucleo+1);
	addNucleo(xx,yy,r);
}

void nuclear_debil::addNucleo(int xx, int yy, float r){
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

void nuclear_debil::addNeutron(int xx, int yy){
	
	// Lanzarla con velocidad
	float vVal = velocNeutronLanz;
	float vAng = -HALF_PI;//ofRandom(TWO_PI);
	
	addNeutron(xx,yy,vVal,vAng);
}

void nuclear_debil::addNeutron(int xx, int yy, float vVal, float vAng){
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

void nuclear_debil::addDestello(float px, float py) {
	
	ofPtr<Destello> destPtr(new Destello(px,py,40, 0.3));
	destPtr->start();
	destellos.push_back(destPtr);
	
}

void nuclear_debil::borrar_nucleos() {
	for(int i=nucleos.size()-1; i>0; i--) {
		nucleos[i].get()->destroy();
	}
	nucleos.clear();
	
}

void nuclear_debil::borrar_neutrones() {
	for(int i=neutrones.size()-1; i>0; i--) {
		neutrones[i].get()->destroy();
	}
	neutrones.clear();

}

//--------------------------------------------------------------
void nuclear_debil::resized(int w, int h){
}


// ------------------------- TUIO ----------------------





//scene notifications
void nuclear_debil::sceneWillAppear( ofxScene * fromScreen ){  // reset our scene when we appear
    //gui1->enable();
   /* ofAddListener(tuioClient.cursorAdded,this,&nuclear_debil::tuioAdded);
	ofAddListener(tuioClient.cursorRemoved,this,&nuclear_debil::tuioRemoved);
	ofAddListener(tuioClient.cursorUpdated,this,&nuclear_debil::tuioUpdated);
    */
    init_Escena();
    cheapComm::getInstance()->sendAudio0("/audio/weak_nuclear/start_event");
};


//scene notifications
void nuclear_debil::sceneWillDisappear( ofxScene * toScreen ){
   // gui1->disable();
  /*  ofRemoveListener(tuioClient.cursorAdded,this,&nuclear_debil::tuioAdded);
	ofRemoveListener(tuioClient.cursorRemoved,this,&nuclear_debil::tuioRemoved);
	ofRemoveListener(tuioClient.cursorUpdated,this,&nuclear_debil::tuioUpdated);
   */
	exit_Escena();
    cheapComm::getInstance()->sendAudio0("/audio/weak_nuclear/end_event");
};





