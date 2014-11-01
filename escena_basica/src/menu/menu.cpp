#include "menu.h"

//--------------------------------------------------------------
void menu::setup() {
	ofLogNotice("menu - SETUP - inicio");
	
	ofSetVerticalSync(true);
	ofBackgroundHex(0x000000);
	ofSetLogLevel(OF_LOG_NOTICE);
	ofEnableAntiAliasing();
	ofEnableSmoothing();

	box2d.init();
	box2d.setGravity(0, 0);
	box2d.setFPS(30.0);

	bola.loadImage("images/dot.png");
	ladoPart1 = 5;
	ladoPart2 = 10;
	
	// Cargar colores
	coloresBN.push_back(ofColor(0));
	coloresBN.push_back(ofColor(150));
	coloresBN.push_back(ofColor(255));

	// Modos dibujo
	bDraw4Forces = true;
	fRed = true;

	// centro
	centroScreen = ofVec2f(ofGetWidth()/2, W_HEIGHT/2); // ofGetHeight()/2);
	
	// Borde Negro circular
	borde.clear();
	ofColor ctmp = ofColor::black;
	borde.setFillColor(ctmp);
	//http://www.openframeworks.cc/documentation/graphics/ofPath.html#show_setPolyWindingMode
	borde.setPolyWindingMode(OF_POLY_WINDING_ODD);
	// rectangulo 
	borde.rectangle(0,0,ofGetWidth(),ofGetHeight());
	borde.setCircleResolution(60);
	// Paso a posicion de la pantall centrada en ancho y ajustada al borde superior
//	borde.circle(ofGetWidth()/2,ofGetHeight()/2,ofGetHeight()/2*0.95);
//	centro = ofVec2f(ofGetWidth()/2.0, ofGetHeight()/2.0);
//	distConf = ofGetHeight()/2.0*0.9;
	borde.circle(ofGetWidth()/2, W_HEIGHT/2,W_HEIGHT/2*0.95);	
	centro = ofVec2f(ofGetWidth()/2.0, W_HEIGHT/2.0);
	distConf = W_HEIGHT/2.0*0.9;
	
	minDisInt = 25;
	
	// fbos
	fbo1.allocate(ofGetHeight()/2,W_HEIGHT/2);// ofGetHeight()/2);
	fbo2.allocate(ofGetHeight()/2,W_HEIGHT/2);// ofGetHeight()/2);
	fbo3.allocate(ofGetHeight()/2,W_HEIGHT/2);// ofGetHeight()/2);
	fbo4.allocate(ofGetHeight()/2,W_HEIGHT/2);// ofGetHeight()/2);	
	
	
	//
	// crear Botones TUIO
	//

	//buttonSpeed1.set(centro.x-rMed-(dRad*0.9/2),centro.y-(dRad*0.9/2), dRad*0.9,dRad*0.9);
    menuButton button1;
	button1.set(0, 0, centro.x, centro.y);
//	button1.setup("E·M", ofColor::fromHex(0xF70D1A) );	// ferrary red
	button1.setup(EM, ofColor::fromHex(0xF70D1A) , &mdisplay);	// ferrary red
    buttons.push_back(button1);
	
    menuButton button2;
	button2.set(centro.x, 0, centro.x, centro.y);
	button2.setup(FUERTE, ofColor::chartreuse, &mdisplay);
    buttons.push_back(button2);
    menuButton button3;
	button3.set(centro.x,centro.y, centro.x, centro.y);
	button3.setup(GRAVEDAD, ofColor::skyBlue, &mdisplay);//	fromHex(0x64E986) );	// Algae Green	// Emerald
    buttons.push_back(button3);
    menuButton button4;
	button4.set(0, centro.y, centro.x, centro.y);
	button4.setup(DEBIL, ofColor::pink , &mdisplay);
     buttons.push_back(button4);
	touchElements.addObject(buttons[0]);
	touchElements.addObject(buttons[1]);
	touchElements.addObject(buttons[2]);
	touchElements.addObject(buttons[3]);

	
//	init_Escena();
	ofLogNotice("menu - SETUP - fin");
	
}


void menu::init_Escena() {
	ofLogNotice("- - - - - - - init escena() - - - - - - - ");
	
	ofPoint centro = ofPoint(W_WIDTH/2.0, W_HEIGHT/2.0);

	isKeyPressed = false;
	isMousePressed = false;
	
	// Crear las particulas
	initParticles();
	
	bDraw4Forces = true;
	fRed = false;

	swFuerzaDensidad = false;
    ofEnableAlphaBlending();
	// fbos:
	// limpiarlas
	fbo1.begin();	ofClear(255,255,255,0);	fbo1.end();
	fbo2.begin();	ofClear(255,255,255,0);	fbo2.end();
	fbo3.begin();	ofClear(255,255,255,0);	fbo3.end();
	fbo4.begin();	ofClear(255,255,255,0);	fbo4.end();	
	
	bDrawFbos = false;
	
	// rangos angulares de los sectores
//	delete[] rangosAngDeg;
	rangosAngDeg [0] = 0;
	rangosAngDeg [1] = 90;
	rangosAngDeg [2] = 180;
	rangosAngDeg [3] = 270;
	ofDisableAlphaBlending();
	
	// addListeners
	// Aqui no hacemos nada (por ahora) con las colisiones y otros eventos
//    box2d.enableEvents();
//	ofAddListener(box2d.contactStartEvents, this, &menu::contactStart);
//	ofAddListener(box2d.contactEndEvents, this, &menu::contactEnd);
	
	// Listeners para detectar que se esta tocando en una zona u otra
 /*   ofAddListener(button1.buttonEvent ,this, &menu::onButtonPressed);
    ofAddListener(button2.buttonEvent ,this, &menu::onButtonPressed);
    ofAddListener(button3.buttonEvent ,this, &menu::onButtonPressed);
    ofAddListener(button4.buttonEvent ,this, &menu::onButtonPressed);*/
		
	ofLogNotice("menu - init_escena - fin");
   
	currentButton=-1;
    for(int i=0; i<buttons.size();i++){
        buttons[i].touchUpAll();
    }
    hands.objectsCol.clear();
}

void menu::exit_Escena() {
	ofLogNotice("menu - exit_escena - ini");
	
	// borrar objetos
	
	
	
	
	// quitar listeners
//    box2d.disableEvents();
//	ofRemoveListener(box2d.contactStartEvents, this, &menu::contactStart);
//	ofRemoveListener(box2d.contactEndEvents, this, &menu::contactEnd);
	
    ofRemoveListener(buttons[0].buttonEvent ,this, &menu::onButtonPressed);
    ofRemoveListener(buttons[1].buttonEvent ,this, &menu::onButtonPressed);
    ofRemoveListener(buttons[2].buttonEvent ,this, &menu::onButtonPressed);
    ofRemoveListener(buttons[3].buttonEvent ,this, &menu::onButtonPressed);
	
	ofLogNotice("menu - exit_escena - fine");
	
}


void menu::initParticles() {
	ofLogNotice("oooooooooo ----- initParticles()");
	// add objects
	removeParticles();	// primero borrar las que pudiera haber, porsiaca.
	for (int i=0; i<150; i++) {
		// circulos
//		addCircle(ofPoint(ofGetWidth()/2+ofRandom(100), ofGetHeight()/2+ofRandom(100)));
		addCircle(ofPoint(ofGetWidth()/2+ofRandom(200)-100, W_HEIGHT/2+ofRandom(200)-100));
		
		// rectangulos
//		addBox(ofPoint(ofGetWidth()/2+ofRandom(100), ofGetHeight()/2+ofRandom(100)));
		addBox(ofPoint(ofGetWidth()/2+ofRandom(200)-100, W_HEIGHT/2+ofRandom(200)-100));
	}
	ptoMed_circles = ptoMedio(circles);
	ptoMed_boxes = ptoMedio(boxes);
	
	bAddCircle = false;
	bAddBox = false;

}

void menu::removeParticles() {
	ofLogNotice("oooooooooo ----- removeParticles()");
	for(int i=circles.size()-1; i>0; i--) {
		circles[i].get()->destroy();
	}
	circles.clear();

	for(int i=boxes.size()-1; i>0; i--) {
		boxes[i].get()->destroy();
	}
	boxes.clear();
}

ofVec2f menu::ptoMedio(vector <ofPtr<ofxBox2dCircle> > shapes) {
	ofVec2f ptMed;
	for(int i=0; i<shapes.size(); i++) {
		ptMed+=shapes[i].get()->getPosition();
	}
	ptMed/=shapes.size();
	return ptMed;
}
ofVec2f menu::ptoMedio(vector <ofPtr<ofxBox2dRect> > shapes) {
	ofVec2f ptMed;
	for(int i=0; i<shapes.size(); i++) {
		ptMed+=shapes[i].get()->getPosition();
	}
	ptMed/=shapes.size();
	return ptMed;
}

void menu::setupSectores() {
//	ofVec2f centroScreen = ofVec2f(ofGetWidth()/2, ofGetHeight()/2);
//	float rrr = ofGetHeight()/2.0;
	float rrr = W_HEIGHT/2.0;
	
	for(int i=0; i<4; i++) {
		float ang = i*HALF_PI;
		ofPolyline pl;
		pl.addVertex(centroScreen.x,centroScreen.y,0);
		pl.addVertex(rrr*cos(ang),rrr*sin(ang),0);
		pl.arc(centroScreen, rrr, rrr, DEG_TO_RAD*ang, DEG_TO_RAD*(ang+HALF_PI), true);
		pl.close();
		
	}
	
}

void menu::addCircle(ofPoint _pos) {
	float r = ofRandom(3, 6);		
	ofPtr<ofxBox2dCircle> circle = ofPtr<ofxBox2dCircle>(new ofxBox2dCircle);
	circle.get()->setPhysics(3.0, 0.53, 0.9);
	circle.get()->setup(box2d.getWorld(), _pos.x, _pos.y, r);
	circle.get()->setFriction(0.9);
	circles.push_back(circle);
}

void menu::addBox(ofPoint _pos) {
	float w = ofRandom(4, 8);	
	float h = ofRandom(w, 12);	
	ofPtr<ofxBox2dRect> rect = ofPtr<ofxBox2dRect>(new ofxBox2dRect);
	rect.get()->setPhysics(1.5, 0.53, 0.9);
	rect.get()->setup(box2d.getWorld(), _pos.x, _pos.y, w, h);
	boxes.push_back(rect);
}
	
//--------------------------------------------------------------
void menu::update(float d1) {
	
//	ofLogNotice("Update");
	if(bAddCircle) {
		ofLogNotice("Update-AddCircle");
		addCircle(ofPoint(ofGetMouseX(),ofGetMouseY()));
	}
	if(bAddBox) {
		addBox(ofPoint(ofGetMouseX(),ofGetMouseY()));
	}
	
	box2d.update();	
	
	if(!bAddCircle && !bAddBox) {
		ofVec2f mouse(ofGetMouseX(), ofGetMouseY());

		int ncircles = circles.size();
		int nboxes = boxes.size();
		int ntot = ncircles+nboxes;
		
		// = Fuerzas entre particulas
		// Se puede hacer estadistico por ahora
		// Tambien se puede hacer por densidades:
		// - calcular pto medio de cada tipo de particula y carga total
		// - calcular la fuerza ejercida sobre cada particula por todos los grupos de particulas
		ptoMed_circles = ptoMedio(circles);
		ptoMed_boxes = ptoMedio(boxes);
		if(swFuerzaDensidad) {
			for(int i=0; i<circles.size(); i++) {
				float dis1 = ptoMed_circles.distance(circles[i].get()->getPosition());
				float dis2 = ptoMed_boxes.distance(circles[i].get()->getPosition());
				if(dis1 > minDisInt) circles[i].get()->addRepulsionForce(ptoMed_circles, 2*(ncircles/ntot)/dis1);
				if(dis2 > minDisInt) circles[i].get()->addAttractionPoint(ptoMed_boxes, 2*(nboxes/ntot)/dis2);
			}
			for(int i=0; i<boxes.size(); i++) {
				float dis1 = ptoMed_boxes.distance(boxes[i].get()->getPosition());
				float dis2 = ptoMed_circles.distance(boxes[i].get()->getPosition());
				if(dis1 > minDisInt) boxes[i].get()->addRepulsionForce(ptoMed_boxes, 2*(nboxes/ntot)/dis1);
				if(dis2 > minDisInt) boxes[i].get()->addAttractionPoint(ptoMed_circles, 2*(ncircles/ntot)/dis2);
			}
		}
		
		
		// = Interaccion mouse/TUIOs:
		// Solo se da si el mouse esta en la zona de confinamiento
		// y depende de si el mouse is Pressed o no:
		
		// pressed: 
		//  - REPULSION circulo y ATRACCION box: si dist-mouse < distMinima
		//  - ATRACCION circulo y REPULSION box: si dist-mouse > distMaxima
		// 
		
		// o poner los tuios impares con atraccion y los impares con repulsion (<<== probemos esta)
		interaccion_point(mouse, !	isMousePressed);
		
		// interaccion TUIOS
		
		
		
		// = Fuerza de confinamiento
		for(int i=0; i<circles.size(); i++) {
			float dis = centro.distance(circles[i].get()->getPosition());
			if(dis > distConf) circles[i].get()->addAttractionPoint(centro, 0.5);//3, 9);
		}
		for(int i=0; i<boxes.size(); i++) {
			float dis = centro.distance(boxes[i].get()->getPosition());
			if(dis > distConf) boxes[i].get()->addAttractionPoint(centro, 0.5);//3, 9);
		}
	
	}
	
	bAddCircle=false;
	bAddBox=false;

	// - - - - TUIOS - - - - 
    //tuioClient.getMessage();

	// botones (tangibleHandler's ==> touchElements)
//    buttonCollide.update_prev(anillo.getParticlePosition());
//    buttonSpeed1.update_prev(anillo.getParticlePosition());
//    buttonSpeed2.update_prev(anillo.getParticlePosition());
    
//	touchElements.update();
	
	// cursores recibidos
    hands.update();
	
	// recorrer el vector de cursores que hay en hands (tangiblesHandler) e interactuar si esta dentro del círculo
//	int ttAct = ofGetElapsedTimeMillis();
//	float sqLim = (radioInt*radioInt)*0.9;
//	int limTime = floor(1000/ofGetFrameRate())+20;
	for(int i=0; i<hands.objectsCol.size(); i++) {
		//		ofLogNotice("handShadow num: " + ofToString(i));
		handShadow * h = (handShadow *) hands.objectsCol[i];
		//ofLogNotice("Id: " + ofToString(h->cursor_id) + "  x,y: " + ofToString(h->x)+"/"+ofToString(h->y)+ "   age: " + ofToString(h->age));

		bool bPar = false;
		if(h->cursor_id%2 == 0)		   bPar = true;
		   
		interaccion_point(ofPoint(h->x,h->y), bPar);
		
	}
    interactionUpdate();
	
	
}

//--------------------------------------------------------------
void menu::draw() {
	ofBackground(ofColor::black);



	// clear fbos
	if(bDrawFbos) {
		ofSetColor(255,255,255);
		
		fbo1.begin();	ofBackground(0,0,0); ofEnableAlphaBlending();	fbo1.end();
		fbo2.begin();	ofBackground(0,0,0); ofEnableAlphaBlending();	fbo2.end();
		fbo3.begin();	ofBackground(0,0,0); ofEnableAlphaBlending();	fbo3.end();
		fbo4.begin();	ofBackground(0,0,0); ofEnableAlphaBlending();	fbo4.end();	
		
		draw4Forces_fbos();
		
		fbo1.begin();	ofDisableAlphaBlending();	fbo1.end();
		fbo2.begin();	ofDisableAlphaBlending();	fbo2.end();
		fbo3.begin();	ofDisableAlphaBlending();	fbo3.end();
		fbo4.begin();	ofDisableAlphaBlending();	fbo4.end();
		
		fbo1.draw(centroScreen.x, centroScreen.y);
		fbo2.draw(centroScreen.x-fbo2.getWidth(), centroScreen.y);
		fbo3.draw(centroScreen.x-fbo3.getWidth(), centroScreen.y-fbo3.getHeight());
		fbo4.draw(centroScreen.x, centroScreen.y-fbo4.getHeight());
		
	}	
	else {
		if(bDraw4Forces) {
            ofEnableAlphaBlending();
			draw4Forces();
            ofDisableAlphaBlending();
		}
		else {
             ofEnableAlphaBlending();
			draw2Colors();
              ofDisableAlphaBlending();
		}
	}


    // Botones para TUIO
    touchElements.draw();
	
	// TUIOS
	//    tuioClient.drawCursors();	
//    ofPushMatrix(); //colocamos el canvas en su posicion centrada
//	ofTranslate((ofGetWidth()-W_WIDTH)/2, 0);
    hands.draw();
//	ofPopMatrix();
	
	ofPushStyle();
	borde.draw();
	ofPopStyle();
	
	ofPushStyle();
	string info = "";
	info += "Press [c] for circles\n";
	info += "Press [b] for blocks\n";
	info += "(4) draw 4Forces: "+ofToString(bDraw4Forces)+"\n";
	info += "(r) Modo Fuerza Color: "+ofToString(fRed)+"\n";
	info += "(f) Modo FBOs: "+ofToString(bDrawFbos)+"\n";
	info += "(d) Fuerza Densidad: "+ofToString(swFuerzaDensidad)+"\n";
	info += "(m) mousePressed: " + ofToString(isMousePressed)+"\n";
	info += "Total Bodies: "+ofToString(box2d.getBodyCount())+"\n";
	info += "Total Joints: "+ofToString(box2d.getJointCount())+"\n\n";
	info += "FPS: "+ofToString(ofGetFrameRate(), 1)+"\n";
    info += "Button Dest" + ofToString(mdisplay.scene);
//	ofSetHexColor(0x444342);
	ofSetHexColor(0xAAAAAA);
	ofDrawBitmapString(info, 30, 30);
	ofPopStyle();
    
    mdisplay.draw();
}


void menu::draw2Colors() {
	ofEnableAlphaBlending();
	for(int i=0; i<circles.size()-1; i++) {
		ofFill();
		//		ofSetHexColor(0xf6c738);
		//		circles[i].get()->draw();
		
		ofVec2f pos = circles[i].get()->getPosition();
		float rr = 3*circles[i].get()->getRadius();
		ofPushMatrix();
		ofPushStyle();
		ofTranslate(pos.x,pos.y,0);
		ofSetColor(ofColor::royalBlue, 200);
		bola.draw(-rr,-rr,2*rr,2*rr);
		ofPopStyle();
		ofPopMatrix();
	}
	
	for(int i=0; i<boxes.size(); i++) {
		ofFill();
		//		ofSetHexColor(0xBF2545);
		//		boxes[i].get()->draw();
		
		ofVec2f pos = boxes[i].get()->getPosition();
		float rr = 3*boxes[i].get()->getHeight();
		ofPushMatrix();
		ofPushStyle();
		ofTranslate(pos.x,pos.y,0);
		ofSetColor(ofColor::red, 180);
		bola.draw(-rr,-rr,2*rr,2*rr);
		ofPopStyle();
		ofPopMatrix();
		
	}
	
	// ptos Medios
	ofPushStyle();
	ofSetLineWidth(4);
	ofSetColor(ofColor::royalBlue);
	ofLine(ptoMed_circles.x-10, ptoMed_circles.y, ptoMed_circles.x+10, ptoMed_circles.y);
	ofLine(ptoMed_circles.x, ptoMed_circles.y-10, ptoMed_circles.x, ptoMed_circles.y+10);
	ofSetColor(ofColor::red);
	ofLine(ptoMed_boxes.x-10, ptoMed_boxes.y, ptoMed_boxes.x+10, ptoMed_boxes.y);
	ofLine(ptoMed_boxes.x, ptoMed_boxes.y-10, ptoMed_boxes.x, ptoMed_boxes.y+10);
	ofPopStyle();
	
	// draw the ground
	//box2d.drawGround();
	
	ofDisableAlphaBlending();	
}

void menu::draw4Forces_fbos() {
	
	//	ofEnableAlphaBlending();
	for(int i=0; i<circles.size()-1; i++) {		
		ofVec2f pos = circles[i].get()->getPosition();
		float rr = circles[i].get()->getRadius();
		float ang = circles[i].get()->getRotation();
		drawBola4_fbos(pos, rr, ang*RAD_TO_DEG);		
	}
	
	for(int i=0; i<boxes.size(); i++) {
		ofVec2f pos = boxes[i].get()->getPosition();
		float rr = boxes[i].get()->getHeight();
		float ang = boxes[i].get()->getRotation();
		drawBola4_fbos(pos, rr, ang*RAD_TO_DEG);		
	}	
	
	//	ofDisableAlphaBlending();
}


void menu::draw4Forces() {
//	ofLogNotice("draw4Forces-->Inicio");
	ofEnableAlphaBlending();
	
	for(int i=0; i<circles.size()-1; i++) {		
		ofVec2f pos = circles[i].get()->getPosition();
		float rr = circles[i].get()->getRadius();
		float ang = circles[i].get()->getRotation();
		drawBola4(pos, rr, ang*RAD_TO_DEG);		
	}
	
	for(int i=0; i<boxes.size(); i++) {
		ofVec2f pos = boxes[i].get()->getPosition();
		float rr = boxes[i].get()->getHeight();
		float ang = boxes[i].get()->getRotation();
		drawBola4(pos, rr, ang*RAD_TO_DEG);		
	}	
	
	ofDisableAlphaBlending();
//	ofLogNotice("draw4Forces-->Exit");
}

void menu::drawBola4_fbos(ofVec2f pos, float radius, float rot) {
	ofPushStyle();
	
	ofVec2f vx = ofVec2f(1,0);
	ofVec2f posCentro =  pos-centroScreen;
	float angulo = vx.angle(posCentro);
	while(angulo<0) {angulo+=360.0;}
	
	//	ofPushMatrix();
	//	ofPushStyle();
	//	ofFill();
	//	ofTranslate(pos.x,pos.y,0);
	if(angulo>=0 && angulo<90) {
		float rr = 2*radius;
		fbo1.begin();
		
		ofPushMatrix();
		ofPushStyle();
		ofFill();
		ofTranslate(posCentro.x,posCentro.y,0);
		
		ofSetColor(ofColor::royalBlue, 200);
		bola.draw(-rr,-rr,2*rr,2*rr);
		ofTranslate(-posCentro.x,-posCentro.y,0);
		ofSetColor(ofColor::royalBlue, 80);
		ofSetLineWidth(0.2);
		ofLine(0,0,  posCentro.length()*cos(angulo*DEG_TO_RAD), posCentro.length()*sin(angulo*DEG_TO_RAD));
		ofPolyline arco;
		arco.arc(ofPoint(0,0), posCentro.length(), posCentro.length(),0, 90);
		arco.draw();
		
		ofPopStyle();
		ofPopMatrix();
		
		fbo1.end();
	}
	
	else if(angulo>=90 && angulo<180) {
		float rr = 6*radius;
		fbo2.begin();
		
		ofPushMatrix();
		ofPushStyle();
		ofFill();
		ofTranslate(fbo2.getWidth()+posCentro.x,posCentro.y,0);
		
		ofSetColor(ofColor::green, 200);
		bola.draw(-rr,-rr,2*rr,2*rr);
		
		ofPopStyle();
		ofPopMatrix();
		
		fbo2.end();
	}
	else if(angulo>=180 && angulo<270) {
		fbo3.begin();
		ofPushMatrix();
		ofPushStyle();
		ofFill();
		ofTranslate(fbo3.getWidth()+posCentro.x,fbo3.getHeight()+posCentro.y,0);
		
		//			ofSetColor(ofColor::deepPink, 200);
		//			bola.draw(-rr,-rr,2*rr,2*rr);
		ofSetColor(155 + ofRandom(100));
		ofCircle(0,0,radius/8);
		
		ofPopStyle();
		ofPopMatrix();
		
		fbo3.end();
	}
	else if(angulo>=270 && angulo<360) {
		fbo4.begin();
		ofPushMatrix();
		ofPushStyle();
		ofFill();
		ofTranslate(posCentro.x,fbo4.getHeight()+posCentro.y,0);
		
		ofEnableBlendMode(OF_BLENDMODE_ADD);
		
		if(fRed) {
			// bolas rojas, con simetria
			
			float rr = 2*radius;
			ofSetColor(ofColor::red, 150);
			//		ofSetColor(ofColor::darkBlue  , 150);
			bola.draw(-rr,-rr,2*rr,2*rr);
			
			ofPushMatrix();
			ofTranslate(-pos.x, -pos.y, 0);
			ofTranslate(ofGetWidth()/2, ofGetHeight()/2, 0);
			float rotSim = 2*(315-angulo);
			ofRotate(rotSim);
			ofTranslate(posCentro.x, posCentro.y,0);
			ofSetColor(ofColor::darkRed  , 150);
			rr = 2*radius;
			bola.draw(-rr,-rr,2*rr,2*rr);
			ofPopMatrix();
		}
		else {
			// 3 bolas color
			
			float rr = 0.7*radius;
			ofRotate(rot/20.0);
			ofSetColor(ofColor::red, 150);
			//		ofSetColor(ofColor::darkBlue  , 150);
			ofPushMatrix();
			ofTranslate(radius,0,0);
			bola.draw(-rr,-rr,2*rr,2*rr);
			ofPopMatrix();
			
			ofSetColor(ofColor::blue, 150);
			ofPushMatrix();
			ofRotateZ(120);
			ofTranslate(radius,0,0);
			bola.draw(-rr,-rr,2*rr,2*rr);
			ofPopMatrix();
			
			ofSetColor(ofColor::green, 150);
			ofPushMatrix();
			ofRotateZ(240);
			ofTranslate(radius,0,0);
			bola.draw(-rr,-rr,2*rr,2*rr);
			ofPopMatrix();
			
		}
		ofDisableBlendMode();
		
		ofPopStyle();
		ofPopMatrix();
		fbo4.end();
	}

	ofPopStyle();
	
}


void menu::drawBola4(ofVec2f pos, float radius, float rot) {
	ofVec2f vx = ofVec2f(1,0);
	ofVec2f posCentro =  pos-centroScreen;
	float angulo = vx.angle(posCentro);
	while(angulo<0) {angulo+=360.0;}
	
	ofPushMatrix();
	ofPushStyle();
	ofFill();
	ofTranslate(pos.x,pos.y,0);
	if(angulo>=rangosAngDeg[0] && angulo<rangosAngDeg[1]) {
//		ofLogNotice("drawBola4-->UNO");
		float rr = 2*radius;
		ofSetColor(ofColor::royalBlue, 200);
		bola.draw(-rr,-rr,2*rr,2*rr);
		ofTranslate(-posCentro.x,-posCentro.y,0);
		ofSetColor(ofColor::royalBlue, 80);
		ofSetLineWidth(0.2);
//		ofLine(0,0,  posCentro.length()*cos(angulo*DEG_TO_RAD), posCentro.length()*sin(angulo*DEG_TO_RAD));
		ofLine(0,0,  posCentro.length()*cos(angulo*DEG_TO_RAD), posCentro.length()*sin(angulo*DEG_TO_RAD));
		ofPolyline arco;
		arco.arc(ofPoint(0,0), posCentro.length(), posCentro.length(),0, 90);
		arco.draw();
	}
	
	else if(angulo>=rangosAngDeg[1] && angulo<rangosAngDeg[2]) {
//		ofLogNotice("drawBola4-->DOS");
		float rr = 6*radius;
		ofSetColor(ofColor::green, 200);
		bola.draw(-rr,-rr,2*rr,2*rr);
	}
	
	else if(angulo>=rangosAngDeg[2] && angulo<rangosAngDeg[3]) {
//		ofLogNotice("drawBola4-->TRES");
		//			ofSetColor(ofColor::deepPink, 200);
		//			bola.draw(-rr,-rr,2*rr,2*rr);
		ofSetColor(150 + ofRandom(105));
		ofCircle(0,0,radius/8);
	}
	
	else if(angulo>=rangosAngDeg[3] && angulo<(rangosAngDeg[0]+360)) {
//		ofLogNotice("drawBola4-->CUATRO");
		ofEnableBlendMode(OF_BLENDMODE_ADD);

		if(fRed) {
			float rr = 2*radius;
			ofSetColor(ofColor::red, 150);
	//		ofSetColor(ofColor::darkBlue  , 150);
			bola.draw(-rr,-rr,2*rr,2*rr);
			
			ofPushMatrix();
			ofTranslate(-pos.x, -pos.y, 0);
//			ofTranslate(ofGetWidth()/2, ofGetHeight()/2, 0);
			ofTranslate(ofGetWidth()/2, W_HEIGHT/2, 0);
			float rot = 2*(315-angulo);
			ofRotate(rot);
			ofTranslate(posCentro.x, posCentro.y,0);
			ofSetColor(ofColor::darkRed  , 150);
			rr = 2*radius;
			bola.draw(-rr,-rr,2*rr,2*rr);
			ofPopMatrix();
		}
		else {
			float rr = 0.7*radius;
			ofRotate(rot);
			ofSetColor(ofColor::red, 150);
			//		ofSetColor(ofColor::darkBlue  , 150);
			ofPushMatrix();
			ofTranslate(radius,0,0);
			bola.draw(-rr,-rr,2*rr,2*rr);
			ofPopMatrix();
			
			ofSetColor(ofColor::blue, 150);
			ofPushMatrix();
			ofRotateZ(120);
			ofTranslate(radius,0,0);
			bola.draw(-rr,-rr,2*rr,2*rr);
			ofPopMatrix();
			
			ofSetColor(ofColor::green, 150);
			ofPushMatrix();
			ofRotateZ(240);
			ofTranslate(radius,0,0);
			bola.draw(-rr,-rr,2*rr,2*rr);
			ofPopMatrix();
			
		}
		ofDisableBlendMode();
	}
	
	ofPopStyle();
	ofPopMatrix();	
}



//scene notifications
void menu::sceneWillAppear( ofxScene * fromScreen ){  // reset our scene when we appear
    //gui1->enable();
	/* ofAddListener(tuioClient.cursorAdded,this,&menu::tuioAdded);
	 ofAddListener(tuioClient.cursorRemoved,this,&menu::tuioRemoved);
	 ofAddListener(tuioClient.cursorUpdated,this,&menu::tuioUpdated);
	 */
	ofLogNotice("menu - sceneWillAppear - ");
	
    cheapComm::getInstance()->sendAudio0("/audio/menu/start_event");
    cheapComm::getInstance()->sendSync0("/sync/menu/start_event");
    
    init_Escena();
};


//scene notifications
void menu::sceneWillDisappear( ofxScene * toScreen ){
	// gui1->disable();
	/*  ofRemoveListener(tuioClient.cursorAdded,this,&menu::tuioAdded);
	 ofRemoveListener(tuioClient.cursorRemoved,this,&menu::tuioRemoved);
	 ofRemoveListener(tuioClient.cursorUpdated,this,&menu::tuioUpdated);
	 */
    cheapComm::getInstance()->sendAudio0("/audio/menu/end_event");
    cheapComm::getInstance()->sendSync0("/sync/menu/end_event");
	exit_Escena();
    hands.objectsCol.clear();
    touchElements.notifyTouchUpAll( );
};



