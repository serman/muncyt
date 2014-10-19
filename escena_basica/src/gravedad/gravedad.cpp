//
//  gravedad::.cpp
//  escena_basica
//
//  Created by Sergio Galan on 7/11/14.
//
//

#include "gravedad.h"


void gravedad::setup(){
    cout << "setupGravedad" <<endl;
    
	ofEnableAlphaBlending();
	imgTex.loadImage("imagen.jpg");
	
	
	//we load a font and tell OF to make outlines so we can draw it as GL shapes rather than textures
	font.loadFont("type/verdana.ttf", 100, true, false, true, 0.4, 72);
	
	// Meshes
	// superficie
	setupMeshSuperf();

    //	ofFloatColor ambient_color(0.0,0.0,0.0,1.0);
    //	ofFloatColor diffuse_color(1.0,0.0,0.0, 0.2);
    //	ofFloatColor specular_color(1.0,1.0,1.0, 0.1);
    //	light.setAmbientColor(ambient_color);
    //	light.setDiffuseColor(diffuse_color);
    //	light.setSpecularColor(specular_color);
	swLight = true;
	light.enable();
	
	swOrtho = false;
	
	swWeb = false;
	
	float wFBOs = W_HEIGHT;
	
	// FBO
	imgMix.allocate(wFBOs,wFBOs, GL_RGBA);
	imgMix.begin();
	ofClear(0,0,0, 0);
	imgMix.end();
	
	ofPushStyle();
	imgDyn.allocate(wFBOs,wFBOs, GL_RGBA);
	imgDyn.begin();
    ofClear(0,0,0, 0);
	imgDyn.end();
	
	
	imgWeb.allocate(wFBOs,wFBOs, GL_RGBA);
	imgWeb.begin();
    ofClear(0,0,0, 0);
	imgWeb.end();
    
	// Dibujar malla
	imgWeb.begin();
    ofSetColor(255,255,255);
    //		ofEnableAntiAliasing();
    ofEnableSmoothing();
    ofSetLineWidth(1.5);
    
    for(int i=0; i<50; i++) {
        float yAct = (float)i*imgDyn.getHeight()/50.0;
        float xAct = (float)i*imgDyn.getWidth()/50.0;
        ofLine(xAct,0, xAct, imgDyn.getHeight());
        ofLine(0, yAct, imgDyn.getWidth(), yAct);
    }
    
    ofDisableSmoothing();
    //		ofDisableAntiAliasing();
	imgWeb.end();
	ofPopStyle();
    
	
	// control fuerza atraccion
	kk = 100.0;
	gg = 10000.0;
	
	
	masaTUIO = 10000;
    
	
	nivelNoise = 7;// 60.0;
	
	zCam = 446.81;
    //cam.setAutoDistance(false);
    
	ratePartic = 30;
    
    setupGUI();
    gui1->disable();
    ofAddListener(gui1->newGUIEvent,this,&gravedad::guiEvent);
    gui1->loadSettings("guiSettings_gravedad.xml");
	
    ofDisableAlphaBlending();
    blackHoleShader.load("", "shaders/blackHole.frag");
  
    
}

void gravedad::init_Escena() {
    
    cheapComm::getInstance()->sendAudio0("/audio/gravity/start_event");
    
        ofSetBackgroundAuto(true);
	    cout << "initScena gravedad" <<endl;
    ofBackground(34, 34, 34);
	ofSetVerticalSync(false);
	
	ofSetCircleResolution(60);
	zentro = ofVec2f(ofGetWidth()/2.0, W_HEIGHT/2.0);
    
    
    bUseTexture = true;
	swWireFrame = false;
	
	// Light
	light.setDirectional();
	light.setPosition(0,0,50);
	light.setOrientation( ofVec3f(180,0, 0) );
    
    
    gui1->enable();
    initSol(INIT_MASA_SOL);
	
	cam.setDistance(zCam);
    state=GROWING;
    status_sent_EOS_Sent=false;
    // Crear las particulas
	
	
	// addListeners
	// Aqui no hacemos nada (por ahora) con las colisiones y otros eventos
	//    box2d.enableEvents();
	//	ofAddListener(box2d.contactStartEvents, this, &gravedad::contactStart);
	//	ofAddListener(box2d.contactEndEvents, this, &gravedad::contactEnd);
	
	// Listeners para detectar que se esta tocando en una zona u otra
//    ofAddListener(button1.buttonEvent ,this, &gravedad::onButtonPressed);
//    ofAddListener(button2.buttonEvent ,this, &gravedad::onButtonPressed);
//    ofAddListener(button3.buttonEvent ,this, &gravedad::onButtonPressed);
//    ofAddListener(button4.buttonEvent ,this, &gravedad::onButtonPressed);
	
	ofSetBackgroundAuto(true);
}

void gravedad::exit_Escena() {
    cheapComm::getInstance()->sendAudio0("/audio/gravity/end_event");
	// borrar objetos
	
	
	// quitar listeners
	//    box2d.disableEvents();
	//	ofRemoveListener(box2d.contactStartEvents, this, &gravedad::contactStart);
	//	ofRemoveListener(box2d.contactEndEvents, this, &gravedad::contactEnd);
	
//    ofRemoveListener(button1.buttonEvent ,this, &gravedad::onButtonPressed);
//    ofRemoveListener(button2.buttonEvent ,this, &gravedad::onButtonPressed);
//    ofRemoveListener(button3.buttonEvent ,this, &gravedad::onButtonPressed);
//    ofRemoveListener(button4.buttonEvent ,this, &gravedad::onButtonPressed);
	//ofRemoveListener(gui1->newGUIEvent,this,&gravedad::guiEvent);
    
	gui1->saveSettings("guiSettings_gravedad.xml");
    gui1->disable();
	//delete gui1;

}

void gravedad::setupMeshSuperf() {
    
	int skip = 10/2;	// this controls the resolution of the mesh
	
	superfW = W_WIDTH;
	superfH = W_HEIGHT;
	
	nW = floor(superfW/skip);
	nH = floor(superfH/skip);
	
	//Set up vertices and colors
	for (int y=0; y<nH; y++) {
		for (int x=0; x<nW; x++) {
			
			superf.addVertex(ofPoint( (x - nW/2) * skip, (y - nH/2) * skip, 0 ) );
			superf.addColor( ofColor( 255,255,255 ) );
            
			// add texture points
			float xn = (float)x*skip/superfW;
			float yn = (float)y*skip/superfH;
			superf.addTexCoord(ofVec2f(xn, yn));
			
		}
	}
	
	//Set up triangles' indices
	for (int y=0; y<nH-1; y++) {
		for (int x=0; x<nW-1; x++) {
			int i1 = x + nW * y;
			int i2 = x+1 + nW * y;
			int i3 = x + nW * (y+1);
			int i4 = x+1 + nW * (y+1);
			superf.addTriangle( i1, i2, i3 );
			superf.addTriangle( i2, i4, i3 );
		}
	}
	
	setNormals( superf ); //Set normals
	
	
	// Guardamos una copia del mesh original
	superfOrig = superf;
	
	
	// Material
	mat1.setShininess(64);
	
    mat1Color.setBrightness(250.f);
    mat1Color.setSaturation(200);
}



void gravedad::update(float d1){
	// aplicar noise y deformaciones por sol y hands
    if(masaSol>MAX_MASA_SOL && state==GROWING){
        initExplosion();
    }
    if(( (initExplosionTime+DURACION_BLACK_HOLE)  < ofGetElapsedTimeMillis()) && state==EXPLOSION){
        if(status_sent_EOS_Sent==false){
            cheapComm::getInstance()->sendAudio0("/audio/gravity/collapse_end_event");
            cheapComm::getInstance()->sendSync0("/sync/gravity/collapse_end_event");
            ofSendMessage("changeScene" + ofToString(MENU) );
            status_sent_EOS_Sent=true;
        }
        
    }
    
	updateMeshSuperf();
    
	// particulas
	updateParticlesX();
	
	// dibujar particulas en textura
	// preparar mezcla de imagenes
	imgMix.begin();
	{
		ofClear(0);
		//		ofEnableBlendMode( OF_BLENDMODE_ADD );
		ofDisableDepthTest();
        
		// rejilla
		if(swWeb) {
			imgWeb.draw(0,0);
		}
		else {
            // fondo blanco
            ofSetColor(255);
            ofRect(0,0, imgMix.getWidth(), imgMix.getHeight());
		}
		
		// test draw
		ofPushStyle();
		ofSetColor(0,0,255);
		ofLine(0,0, imgMix.getWidth(), imgMix.getHeight());
		ofPopStyle();
		
		
		ofEnableDepthTest();
		
		// dibujar particulas y cosas
		updateFBO();
		
		drawParticlesX();
		ofDisableDepthTest();
		//		ofDisableBlendMode();
	}
	imgMix.end();
    
 /***************/
	// eliminar las particulas que ya han chocado contra el sol o que se han ido muy lejos
	if(ofGetFrameNum() % 10==0){        
        cheapComm::getInstance()->sendAudio1("/audio/gravity/collapse_proximity",ofMap(masaSol, INIT_MASA_SOL, MAX_MASA_SOL, 0, 1));
        cheapComm::getInstance()->sendSync1("/sync/gravity/collapse_proximity",ofMap(masaSol, INIT_MASA_SOL, MAX_MASA_SOL, 0, 1));
        
#ifdef DEBUGOSC
        ofLogNotice() << "/audio/gravity/collapse_proximity" << "masa sol: " << ofMap(masaSol, INIT_MASA_SOL, MAX_MASA_SOL, 0, 1) << "real"<<  masaSol;
#endif
        sendSunParticlesOsc();
        
    }
}

void gravedad::sendSunParticlesOsc(){
    /*************/
    //ofVec3f pTUIO = ofVec3f(ofGetMouseX()-ofGetWidth()/2, -(ofGetMouseY()-ofGetHeight()/2), 0);
    for (int j=0; j<hands.objectsCol.size(); j++){
        ofVec3f pTUIO(hands.objectsCol[j]->x,hands.objectsCol[j]->y);
        int minimun_dist;
        int minimun_dist_index;
        for(int i=0; i<particulas.size(); i++) {
            int dist=(pTUIO-particulas[i].position).length();
            if(i==0) { minimun_dist=dist; minimun_dist_index=0;}
            else{
                if(dist<minimun_dist){
                    minimun_dist=dist;
                    minimun_dist_index=i;
                }
            }
        }
        if(particulas.size()>0){
			float angZentro = particulas[minimun_dist_index].getAngleRad(zentro);
            cheapComm::getInstance()->sendAudio3("/audio/gravity/hand_particle",
                                                 j,
                                                 ofMap(angZentro,0,2*PI,0,1),
                                                 ofMap(sqrt(minimun_dist),0,768,0,1)
                                                 );
#ifdef DEBUGOSC
            ofLogNotice()  << "hand_particle: " << j << " " << angZentro << "(" <<  ofMap(angZentro,0,2*PI,0,1) << ")"<<
            "dist: " << sqrt(minimun_dist)<< "("<< ofMap(sqrt(minimun_dist),0,768,0,1) <<")"<< endl ;
#endif
            
        }
        
        
    }
    /****************************/
    ////DISTANCIA AL SOL
    for(int i=0; i<particulas.size(); i++) {
        ofVec3f pSol = ofVec3f(W_WIDTH/2.0, W_HEIGHT/2.0,0);
        ofVec3f diff =  particulas[i].position - pSol;
        float sqdistance =  diff.lengthSquared();
		if(sqdistance<W_HEIGHT*W_HEIGHT/4) {
			float angZentro = particulas[i].getAngleRad(zentro);
			cheapComm::getInstance()->sendAudio3("/audio/gravity/sun_particle",
                                             i,
                                             ofMap(angZentro,0,2*PI,0,1),
                                             ofMap(sqdistance,0,W_HEIGHT*W_HEIGHT/4,0,1)
                                             );
        
#ifdef DEBUGOSC
        ofLogNotice()  << "sun_particle: " << i << " " << angZentro << "(" << ofMap(angZentro,0,2*PI,0,1) << ")" <<
        "dist" << sqdistance<< "("<< ofMap(sqdistance,0,W_HEIGHT*W_HEIGHT/4,0,1)  << ")"<< endl ;
#endif
		}
    }

}


void gravedad::initExplosion(){
    state=EXPLOSION;
    cheapComm::getInstance()->sendAudio0("/audio/gravity/collapse_start_event");
    cheapComm::getInstance()->sendSync0("/sync/gravity/collapse_start_event");
    initExplosionTime=ofGetElapsedTimeMillis();
}

void gravedad::updateMeshSuperf(){
	
	float time = ofGetElapsedTimef();	//Get time
	
	superf = superfOrig;
	
	for (int y=0; y<nH; y++) {
		for (int x=0; x<nW; x++) {
			int i = x + nW * y;	//Vertex index
			ofPoint p = superf.getVertex( i );
			ofPoint pO = superfOrig.getVertex( i );
			
			//Get Perlin noise value
			float value = ofNoise( x * 0.05, y * 0.05, time * 0.5 );
			//Change z-coordinate of vertex
			p.z = 0;
			p.z -= value * nivelNoise;
			//p.z = -value; //37.0;
			
			
			//
			// - - - - ATRACCION SOL - - - -
			//
            //			p.z = value * 100;
			float rho2 = p.x*p.x+p.y*p.y;
            //			float var = 20+10*sin(time*0.5);
			float var = 20;
            //			p.z += 100.0*exp(-rho2/(2*var*var));
			
			
			// asignar altura
			// y color segun la altura
			ofFloatColor cc;
			float rho = sqrt(rho2);
			if(rho>=10) {
                //				p.z -= 10000/rho;
				p.z -= gg/rho;
				float ccv = ofMap(p.z,0,-500, 1.0, 0.0);
                //				cc = ofFloatColor(ccv,1-ccv,ccv, 1.0);
				cc = ofFloatColor(ccv,ccv,ccv, 1.0);
				
                //				float angDir = atan2(p.y, p.x);	// radianes
                //				p.x -= 100*kk / rho * cos(angDir);
                //				p.y -= 100*kk / rho * sin(angDir);
			}
			else {
				p.z -= gg/10;
			}
			
			
			//
			// - - - - - INTERACCIONES - - - - -
			//
			// Evaluar el desplazamiento por los ptos de interaccion (mouse, TUIO...)
			
			// MOUSE
			// Pasamos las coordenadas del pto de interaccion al marco de referencia de la malla
            for (int j=0; j<hands.objectsCol.size(); j++){
                ofVec2f ptInteract(hands.objectsCol[j]->x,hands.objectsCol[j]->y);
                //ofVec2f ptInteract = ofVec2f(ofGetMouseX()-zentro.x, ofGetMouseY()-zentro.y);
                
                float dx = p.x-ptInteract.x;
                float dy = p.y+ptInteract.y;
                
                float rhoZ2 = dx*dx+dy*dy;
                p.z -= 150.0*exp(-rhoZ2/(2*var*var));
                
                float dxO = pO.x-ptInteract.x;
                float dyO = pO.y+ptInteract.y;
                
                float d2 = dxO*dxO+dyO*dyO;
                float angDir = atan2(dyO, dxO);	// radianes
                
                float dd = sqrt(d2);
                if(dd>10) {
                    p.x -= kk / sqrt(d2) * cos(angDir);
                    p.y -= kk / sqrt(d2) * sin(angDir);
                }
           }     
                // SET VERTEX
                superf.setVertex( i, p );
                superf.setColor(i, cc);
                //Change color of vertex
                //			mesh.setColor( i, ofColor( value*255, value * 255, 255 ) );
			
		}
	}
	
	setNormals( superf ); // Update normals
}

void gravedad::updateFBO() {
	
	ofPushStyle();
	{
		// lineas alrededor de la bola central.
		// Varian segun el radio que tenga el circulo == radio^2 / dist
		
		ofSetColor(255,0,0);
		ofSetLineWidth(1.0);
		ofNoFill();
		
		float baseSol = rSol/10000.0;
		ofPushMatrix();
		
		ofTranslate(imgDyn.getWidth()/2.0, imgDyn.getHeight()/2.0, 0 );
		//		int lim = ofGetHeight()/2.0*floor(
		for(float i=0.1; i<10.0; i+=0.3) {
			float vPot = i*baseSol;
			float rPot = 1/vPot;
			
			ofEnableSmoothing();
			ofCircle(0,0, rPot);
			ofDisableSmoothing();
		}
		ofPopMatrix();
		
		
		// lineas alrededor de los pts de interaccion (mouse, TUIOs):	vector<ofVec2f> ptsInter
		//		for(int i=0; i<10; i++) {
		//
		//
		//		}
		
		
		
	}
	
	ofPopStyle();
	
}

void gravedad::draw(){
    
	//ofBackground(0);
    //		ofLogNotice("***"+ofToString(superf.getMode()));
    
    //		imgWeb.draw(ofGetWidth()-200,0,200,200);
    //		imgDyn.draw(ofGetWidth()-200,300,200,200);
    //		imgMix.draw(ofGetWidth()-200,ofGetHeight()-200,200,200);
	
	cam.begin();
    //			if(!swOrtho) cam.setDistance(zCam);
	//	cam.enableOrtho();
    ofEnableAlphaBlending();
    if(swLight) ofEnableLighting();
    ofEnableDepthTest();
    
    mat1.begin();
    
    if(swOrtho) ofTranslate(zentro.x, zentro.y,0);
    
    //img.bind();
	if(bUseTexture) {
		ofEnableNormalizedTexCoords();
        //			imgWeb.getTextureReference().bind();
        //			imgDyn.getTextureReference().bind();
		imgMix.getTextureReference().bind();
	}
    
	if(swLight) {
		light.enable();
		light.draw();
	}
	if(swWireFrame) {
		superf.drawWireframe();
	}
	else {
		superf.draw();
	}
	//img.unbind();
	if(bUseTexture) {
        //				imgWeb.getTextureReference().unbind();
        //				imgDyn.getTextureReference().unbind();
		imgMix.getTextureReference().unbind();
		ofDisableNormalizedTexCoords();
	}
	
	
    //		superfOrig.drawWireframe();
	
	// Dibujar esfera central
    //			ofSphere(0,0, radioEsfera);	// deprecated
	drawSol();
	
	
	if(swLight) 	light.disable();
	
	mat1.end();
	
	ofDisableAlphaBlending();
	if(swLight) ofDisableLighting();
	
	cam.end();
    
    //	imgWeb.draw(ofGetWidth()-200,0,200,200);
    //	imgDyn.draw(ofGetWidth()-200,300,200,200);
    //	imgMix.draw(ofGetWidth()-200,ofGetHeight()-200,200,200);
    if(state==EXPLOSION){

        
        int timeSinceExplosion=ofGetElapsedTimeMillis()-initExplosionTime;
        if(timeSinceExplosion <500 ) {
            int circleWidth=ofMap(ofGetElapsedTimeMillis()-initExplosionTime,0,500,0,ofGetWidth());
            ofSetColor(0);
            ofEllipse(ofGetWidth()/2, ofGetHeight()/2,circleWidth,circleWidth);
        }else if(timeSinceExplosion>500 && timeSinceExplosion<1300){
            blackHoleShader.begin();
            blackHoleShader.setUniform3f("iResolution", ofGetWidth(), ofGetHeight(),0);
            blackHoleShader.setUniform1f("iGlobalTime", (float) ofGetElapsedTimef());
            blackHoleShader.setUniform1f("BULB_R", (float) ofMap(ofGetElapsedTimeMillis()-(initExplosionTime+500),0,1300,1.1,1.0/6.0) ) ;
            blackHoleShader.setUniformTexture("iChannel1", imgMix.getTextureReference(), 1);
            ofSetColor(255);
            imgMix.draw(0,0,ofGetWidth(), ofGetHeight());
            blackHoleShader.end();
           // int circleWidth=ofMap(ofGetElapsedTimeMillis()-(initExplosionTime+500),0,1300,ofGetWidth()+200,0);
        }
        else{
            cout << "parte final"<<endl;
            blackHoleShader.begin();
            blackHoleShader.setUniform3f("iResolution", ofGetWidth(), ofGetHeight(),0);
            blackHoleShader.setUniform1f("iGlobalTime", (float) ofGetElapsedTimef());
            blackHoleShader.setUniform1f("BULB_R", (float) ofMap(ofGetElapsedTimeMillis()-(initExplosionTime+1300),0,DURACION_BLACK_HOLE-2000,1.0/6.0,0.7) );
            blackHoleShader.setUniformTexture("iChannel1", imgMix.getTextureReference(), 1);
            ofSetColor(255);
            imgMix.draw(0,0,ofGetWidth(), ofGetHeight());
            blackHoleShader.end();
            
        }
        
    }
    
	ofPushStyle();
	ofSetColor(255,0,0);
	ofDrawBitmapString("fr: " + ofToString(ofGetFrameRate()), 10, ofGetHeight()-90);
	ofDrawBitmapString("masa Sol: " + ofToString(masaSol), 10, ofGetHeight()-70);
	ofDrawBitmapString("num Partics: " + ofToString(particulas.size()), 10, ofGetHeight()-50);
	ofPopStyle();
    ofDisableDepthTest();
}

void gravedad::mousePressed( int x, int y, int button ){}


//scene notifications
void gravedad::sceneWillAppear( ofxScene * fromScreen ){  // reset our scene when we appear
    //gui1->enable();
    cheapComm::getInstance()->sendAudio0("/sync/gravity/start_event");
    cheapComm::getInstance()->sendSync0("/sync/gravity/start_event");
    init_Escena();
};


//scene notifications
void gravedad::sceneWillDisappear( ofxScene * toScreen ){
	// gui1->disable();

    cheapComm::getInstance()->sendAudio0("/sync/gravity/end_event");
    cheapComm::getInstance()->sendSync0("/sync/gravity/end_event");
	exit_Escena();
};


void gravedad::tuioAdded(ofxTuioCursor &tuioCursor){
	ofPoint loc = ofPoint(tuioCursor.getX()*W_WIDTH-300,tuioCursor.getY()*W_HEIGHT-300);
    
    handShadow *h1 = new handShadow();
    h1->setup();
    hands.addObject(*h1);
    hands.notifyTouch(loc.x, loc.y,tuioCursor.getSessionId());
    
    cheapComm::getInstance()->sendAudio0("/audio/gravity/hand_on_event");
}

void gravedad::tuioUpdated(ofxTuioCursor &tuioCursor){
        ofPoint loc = ofPoint(tuioCursor.getX()*W_WIDTH-300,tuioCursor.getY()*W_HEIGHT-300);
        hands.notifySlide(loc.x, loc.y,tuioCursor.getSessionId(),tuioCursor.getMotionAccel());
}

void gravedad::tuioRemoved(ofxTuioCursor &tuioCursor){
	ofPoint loc = ofPoint(tuioCursor.getX()*W_WIDTH-300,tuioCursor.getY()*W_HEIGHT-300);
    hands.removeObjectByTuioID(tuioCursor.getSessionId() );

    cheapComm::getInstance()->sendAudio0("/audio/gravity/hand_off_event");
    
}


// - - - - GUI - - - -
void gravedad::setupGUI() {
    
	gui1 = new ofxUISuperCanvas("GRAVITY F");
    gui1->addSpacer();
    //    gui1->addLabel("Press 'h' to Hide GUIs", OFX_UI_FONT_SMALL);
    
    //    gui1->addSpacer();
	gui1->addLabel("Noisy waves");
	gui1->addSlider("nivel_Noise", 0, 100.0, &nivelNoise);
	
	gui1->addSpacer();
	gui1->addLabel("Sol");
	gui1->addSlider("radio_Sol", 10, 250.0, &rSol);
    
	gui1->addSpacer();
	gui1->addSlider("fuerza_Sol", 0, 20000.0, &gg);
	gui1->addSlider("fuerza_User", 0, 1000.0, &kk);
	gui1->addSlider("masa TUIO", 0, 20000.0, &masaTUIO);
	gui1->addSpacer();
	
	gui1->addToggle( "TOGGLE_WireFrame", &swWireFrame);
	gui1->addToggle( "TOGGLE_Light", &swLight);
	gui1->addToggle( "TOGGLE_OrthoCamera", &swOrtho);
	gui1->addToggle( "TOGGLE_Texture", &bUseTexture);
	gui1->addToggle( "TOGGLE_Tex Web", &swWeb);
	
	gui1->addSpacer();
	gui1->addLabel("Camara");
	gui1->addSlider("cam_z", 100, 1000.0, &zCam);
    
	gui1->addSpacer();
	gui1->addIntSlider("Rate_partics", 1,120, &ratePartic);
	gui1->addSpacer();
	gui1->addLabel("fr: " + ofToString(ofGetFrameRate()));
	
    gui1->autoSizeToFitWidgets();
	
}

void gravedad::keyPressed(int key){
    if(key=='s'){
        cheapComm::getInstance()->sendAudio0("/audio/gravity/collapse_start_event");
        cheapComm::getInstance()->sendSync0("/sync/gravity/collapse_start_event");
        initExplosion();
    }
    
    if(key=='e'){
        cheapComm::getInstance()->sendAudio0("/audio/gravity/collapse_end_event");
        cheapComm::getInstance()->sendSync0("/sync/gravity/collapse_start_event");
        state=GROWING;
    }
    
}
void gravedad::guiEvent(ofxUIEventArgs &e) {
	string name = e.getName();
	int kind = e.getKind();
    //	cout << "got event from: " << name
	if(name == "radio_Sol")
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		rSol = slider->getScaledValue();
		sol.setRadius(rSol);
		setMfromR();
	}
	else if(name == "cam_z")
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		zCam = slider->getScaledValue();
		cam.setDistance(zCam);
	}
	else if(name == "TOGGLE_OrthoCamera")
	{	// swOrtho
		ofxUIToggle *toggle = (ofxUIToggle *) e.getToggle();
		swOrtho = toggle->getValue();
		if(swOrtho) cam.enableOrtho();
		else cam.disableOrtho();
	}
	
}


