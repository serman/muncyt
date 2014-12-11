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
	

	zentro = ofVec2f(ofGetWidth()/2.0, W_HEIGHT/2.0);
	//
	// Borde
	//
	radioEscena = W_HEIGHT/2;
	// Borde Negro circular
	borde.clear();
	ofColor ctmp = ofColor::black;//red;//black;
	borde.setFillColor(ctmp);
	//http://www.openframeworks.cc/documentation/graphics/ofPath.html#show_setPolyWindingMode
	borde.setPolyWindingMode(OF_POLY_WINDING_ODD);
	// rectangulo 
	borde.rectangle(0,0,ofGetWidth(),ofGetHeight());
	borde.setCircleResolution(60);
	borde.circle(zentro.x, zentro.y, radioEscena);	
	
	
	imgTex.loadImage("imagen.jpg");
	
	//we load a font and tell OF to make outlines so we can draw it as GL shapes rather than textures
	font.loadFont("type/verdana.ttf", 100, true, false, true, 0.4, 72);

	ofLogNotice() << 1;
	
	// Meshes
	// superficie
	setupMeshSuperf();

	// Luces
	swLight = true;
//	light.enable();
//	light1.enable();
	
	// Camara
	swOrtho = false;
	
	// Textura
	swWeb = false;
	
	ofLogNotice() << 2;
	
	// FBO's
	float wFBOs = W_HEIGHT;	
	imgMix.allocate(wFBOs,wFBOs, GL_RGBA);
	imgMix.begin();
	ofClear(0,0,0, 0);
	imgMix.end();
	
	ofPushStyle();
	imgDyn.allocate(wFBOs,wFBOs, GL_RGBA);
	imgDyn.begin();
    ofClear(0,0,0, 0);
	imgDyn.end();
	
	ofLogNotice() << 3;
	
	
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
    
    for(int i=0; i<350; i++) {
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
    
	
	zCam = 446.81;
    //cam.setAutoDistance(false);
    
	ratePartic = 30;

    setupGUI();	
    gui1->disable();
	
//	ofLogNotice() << "YA Shader";
    blackHoleShader.load("", "shaders/blackHole.frag");
//	ofLogNotice() << "YA Fin Shader";
  
}

void gravedad::init_Escena() {
	ofLogNotice() << "gravedad::init_Escena: " << 0;
    
    cheapComm::getInstance()->sendAudio0("/audio/gravity/start_event");
    
	ofSetBackgroundAuto(true);
	cout << "initScena gravedad" <<endl;
	
    ofBackground(34, 34, 34);
	ofSetVerticalSync(false);
	
	ofSetCircleResolution(60);

	nivelNoise = 7;// 60.0;
	noiseAuto = true;
	
	// Meshes
	// superficie
	setupMeshSuperf();

	// Camara
	swOrtho = false;
	
	// Textura
	swWeb = false;
	
	
	// Luces
	ofEnableLighting();
	ofSetSmoothLighting(true);
	swLight = true;
	light.enable();
	//	light1.enable();
	
    bUseTexture = true;
	swWireFrame = false;
	
	// Light
	light.setDirectional();
	light.setPosition(0,0,450);
	light.setOrientation( ofVec3f(180,0,0) );
    
//	ofLogNotice() << "gravedad::init_Escena: " << 1;
	
    
	// FBO's
	float wFBOs = W_HEIGHT;
	imgMix.allocate(wFBOs,wFBOs, GL_RGBA);
	imgMix.begin();
	ofClear(0,0,0, 0);
	imgMix.end();
	
	ofPushStyle();
	imgDyn.allocate(wFBOs,wFBOs, GL_RGBA);
	imgDyn.begin();
    ofClear(0,0,0, 0);
	imgDyn.end();
    ofPopStyle();
    
    
    
//    gui1->enable();
//    initSol(INIT_MASA_SOL);
	sol.init(INIT_MASA_SOL, ofVec3f(0,0,0), ofVec2f(W_WIDTH/2.0, W_HEIGHT/2.0) );
	ofLogNotice() << "gravedad::init_Escena: " << 2;	
	
	cam.setDistance(zCam);
    state=GROWING;
    status_sent_EOS_Sent=false;
    
	// Limpiar las particulas
	particulas.clear();
	
	gui1->disable();
//	ofLogNotice() << "gravedad::init_Escena: " << 3;
	
	// addListeners
	bshowdebug=false;
	ofSetBackgroundAuto(true);
}

void gravedad::exit_Escena() {
    cheapComm::getInstance()->sendAudio0("/audio/gravity/end_event");
    hands.reset();
	// borrar objetos
	particulas.clear();
	
//	gui1->saveSettings("guiSettings_gravedad.xml");
    gui1->disable();
	//delete gui1;
}

void gravedad::setupMeshSuperf() {
//	ofLogNotice() << "setupMeshSuperf " << 1;
    
	int skip = 10;// /2;	// Resolucion del mesh
	
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
	mat1.setAmbientColor(mat1Color);
	
}



void gravedad::update(float d1){
//	ofLogNotice() << "gravedad::update: " << 0;
	// Explosion
    if(sol.masa>MAX_MASA_SOL && state==GROWING){
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
	
	// particulas
	updateParticlesX();

	// Sol
	sol.update();	
	
	// aplicar noise y deformaciones por sol y hands
	updateMeshSuperf();
    
	// dibujar particulas en textura
	// preparar mezcla de imagenes
	imgMix.begin();
	{
		ofClear(0);
//		ofEnableBlendMode( OF_BLENDMODE_ADD );
//		ofDisableDepthTest();
        
		// rejilla
		if(swWeb) {
			imgWeb.draw(0,0);
		}
		else {
            // fondo blanco
            ofSetColor(150);
            ofRect(0,0, imgMix.getWidth(), imgMix.getHeight());
		}
	
		// dibujar particulas y cosas
		updateFBO();
		
		drawParticlesX();
		
	}
	imgMix.end();
    
 /***************/
	// eliminar las particulas que ya han chocado contra el sol o que se han ido muy lejos
	
	
	// Envia datos de la posicion de las particulas respecto al sol
	if(ofGetFrameNum() % 10==0){        
        cheapComm::getInstance()->sendAudio1("/audio/gravity/collapse_proximity",ofMap(sol.masa, INIT_MASA_SOL, MAX_MASA_SOL, 0, 1));
        cheapComm::getInstance()->sendSync1("/sync/gravity/collapse_proximity",ofMap(sol.masa, INIT_MASA_SOL, MAX_MASA_SOL, 0, 1));
        
#ifdef DEBUGOSC
        ofLogNotice() << "/audio/gravity/collapse_proximity" << "masa sol: " << ofMap(sol.masa, INIT_MASA_SOL, MAX_MASA_SOL, 0, 1) << "real"<<  sol.masa;
#endif
        sendSunParticlesOsc();
        
    }
}

void gravedad::sendSunParticlesOsc(){
    /*************/
	// DISTANCIAS A HANDS
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
            angZentro-=PI/4;
            angZentro=ofWrapRadians(angZentro,0,2*PI);
            cheapComm::getInstance()->sendAudio3("/audio/gravity/hand_particle",
                                                 j,
                                                 ofMap(angZentro,0,2*PI,0,1),
                                                 ofMap(sqrt(minimun_dist),0,W_WIDTH,0,1)
                                                 );
#ifdef DEBUGOSC
            ofLogNotice()  << "hand_particle: " << j << " " << angZentro << "(" <<  ofMap(angZentro,0,2*PI,0,1) << ")"<<
            "dist: " << sqrt(minimun_dist)<< "("<< ofMap(sqrt(minimun_dist),0,W_WIDTH,0,1) <<")"<< endl ;
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
            angZentro-=PI/4;
            angZentro=ofWrapRadians(angZentro,0,2*PI);
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
	
	if(noiseAuto) nivelNoise = ofMap(sol.masa, INIT_MASA_SOL, MAX_MASA_SOL, 0,100);
	
	float time = ofGetElapsedTimef();	//Get time
	
	superf = superfOrig;
	
	for (int y=0; y<nH; y++) {
		for (int x=0; x<nW; x++) {
			int i = x + nW * y;	//Vertex index
			ofPoint p = superf.getVertex( i );

			ofPoint pNew = calculaPosicionMesh(p, time);			
			
			// asignar color segun la altura
			ofFloatColor cc;
			float ccv = ofMap(pNew.z,0,-500, 1.0, 0.0);
			cc = ofFloatColor(abs(1-ccv),abs(1-ccv),ccv, 1.0);
			
			//
			// SET VERTEX
			superf.setVertex( i, pNew );
			superf.setColor(i, cc);
			
		}
	}
	
	setNormals( superf ); // Update normals
}

ofPoint gravedad::calculaPosicionMesh(ofPoint pOrig, float t) {
	ofVec3f p;
	
	p = pOrig;
	
	//
	// PERLIN NOISE
	//Get Perlin noise value
	float value = ofNoise( p.x * 0.005, p.y * 0.005, t * 0.5 );
	//Change z-coordinate of vertex
	p.z = 0;
	p.z -= value * nivelNoise;
	//p.z = -value; //37.0;
	
	//
	// - - - - ATRACCION SOL - - - - 
	//
	float rho2 = p.x*p.x+p.y*p.y;	//dist al cuadrado
	float var = 20+5*sin(t*0.5);
	//			float var = 20;
	//			p.z += 100.0*exp(-rho2/(2*var*var));
	
	// asignar altura
	float rho = sqrt(rho2);
	if(rho>=10) {
		p.z -= gg/rho;
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
		ofVec2f ptInteract(hands.objectsCol[j]->x,-hands.objectsCol[j]->y);
		//ofVec2f ptInteract = ofVec2f(ofGetMouseX()-zentro.x, ofGetMouseY()-zentro.y);
		
		float dx = p.x-ptInteract.x;
		float dy = p.y-ptInteract.y;
		
		// Varianza sigma
		var = 100;
		
		float rhoZ2 = dx*dx+dy*dy;
		p.z -= 150.0*exp(-rhoZ2/(2*var*var));
		
		// un poquito de fuerza de resistencia tpo muelle
		float dxO = pOrig.x-ptInteract.x;
		float dyO = pOrig.y-ptInteract.y;
		
		float d2 = dxO*dxO+dyO*dyO;
		float angDir = atan2(dyO, dxO);	// radianes
		
		float dd = sqrt(d2);
		if(dd>10) {
			if(ofGetMousePressed()) {
				p.x -= kk / dd * cos(angDir);
				p.y -= kk / dd * sin(angDir);
			}
			else {
//				p.x = pOrig.x - kk / dd * cos(angDir);
//				p.y = pOrig.y - kk / dd * sin(angDir);
				p.x -= kk / dd * cos(angDir);
				p.y -= kk / dd * sin(angDir);
			}
		}
	}     
	
	return p;
	
}

void gravedad::updateFBO() {
	
	ofPushStyle();
	{
		// lineas alrededor de la bola central.
		// Varian segun el radio que tenga el circulo == radio^2 / dist
		
		ofColor cLin = ofColor(39,81,227);
		//		ofLogNotice() << cLin.getBrightness();
		//		ofLogNotice() << cLin.getSaturation();
		cLin.setBrightness(255);
		cLin.setSaturation(250);
		ofSetColor(cLin);	// 0x2751E3 (AZUL GUAY)
		ofSetLineWidth(3.0);
		ofNoFill();
		
		float baseSol = sol.radio/10000.0;
		ofPushMatrix();
		
		ofTranslate(imgDyn.getWidth()/2.0, imgDyn.getHeight()/2.0, 0 );
		ofEnableSmoothing();
		//		int lim = ofGetHeight()/2.0*floor(
		for(float i=0.1; i<10.0; i+=0.1) {
			float vPot = i*baseSol;
			float rPot = 1/vPot;
			
			ofCircle(0,0, rPot);
		}
		
		// Líneas Radiales
		for(int i=0; i<16; i++) {
			float ang = (float) TWO_PI/32.0*i;
			float cang = cos(ang)*imgDyn.getWidth();
			float sang = sin(ang)*imgDyn.getWidth();
			ofLine(-cang, -sang, cang, sang);
		}
		
		ofDisableSmoothing();
		
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
    
//	ofLogNotice() << 1;
	
	// De ejemplo de OF MeshFromCamera
	//	float rotateAmount = ofMap(ofGetMouseY(),0,ofGetHeight(), -90,90);
	//	ofVec3f rotateDir = ofVec3f(1,0,0);
	float rotateAmount = 0;
	ofVec3f rotateDir = ofVec3f(1,0,0);

	if(hands.objectsCol.size()>0 || ofGetMousePressed(0)) {
		
		rotateAmount = 0; //ofMap(ofGetMouseY(),0,ofGetHeight(), -90,90);
		ofVec3f posTouch;
		if(hands.objectsCol.size()>0) {
			posTouch = ofVec3f(hands.objectsCol[0]->x, hands.objectsCol[0]->y, 0.0);
//ofLogNotice() << "updateParticlesX() · TUIO: " << ofToString(posTouch);
		}
		else {
			posTouch = ofVec3f(ofGetMouseX()-zentro.x, ofGetMouseY()-zentro.y, 0.0);
		}
		posTouch.normalize();
		rotateDir = ofVec3f(0,0,1).cross(posTouch);
	}
	
	float extrusionAmount = 300.0;
	
	// Situar la camara
	ofVec3f camDirection(0,0,1);
	ofVec3f centre = sol.pos3D;
	//	ofVec3f camDirectionRotated = camDirection.rotated(rotateAmount, ofVec3f(1,0,0));
	ofVec3f camDirectionRotated = camDirection.rotated(rotateAmount, rotateDir);
	ofVec3f camPosition = centre + camDirectionRotated * extrusionAmount;
	
	camera.setPosition(camPosition);
	camera.lookAt(centre);
	
	//	if(swOrtho) {
	//		ofEnableDepthTest();
	//	}
	

	
	//	cam.begin();
	camera.begin();
	
	
	//	if(!swOrtho) cam.setDistance(zCam);
	//	else	cam.enableOrtho();
	if(swOrtho) camera.enableOrtho();
	else camera.disableOrtho();
	
	
    ofEnableAlphaBlending();
    ofEnableDepthTest();
    if(swLight) ofEnableLighting();

	light.setPosition(0,0,ofMap(ofGetMouseY(),0,ofGetHeight(), 0,1000));
	float euler1 = 135.0;
	float euler2 = 0.0;
	float euler3 = ofGetElapsedTimef()*100.0;//ofMap(ofGetMouseX(),0,ofGetWidth(), 0,720)
//	float euler3 = ofMap(ofGetMouseX(),0,ofGetWidth(), 0,720);
//	ofLogNotice() << euler3;
	light.setOrientation(ofVec3f(euler1, euler2, euler3));
	
	ofPushMatrix();
	if(swOrtho) {
		ofTranslate(zentro.x, zentro.y,0);
	}
	else {
		ofTranslate(0,0, -260);			//¿?¿??¿?
	}
	    
    mat1.begin();
    
//    if(swOrtho) ofTranslate(zentro.x, zentro.y,0);
    
    //img.bind();
	if(bUseTexture) {
//        ofLogNotice() << "bind";
		ofEnableNormalizedTexCoords();
		imgMix.getTextureReference().bind();
	}
    
	if(swLight) {
		light.enable();
	}
	if(swWireFrame) {
		superf.drawWireframe();
	}
	else {
		superf.draw();
	}
	//img.unbind();
	if(bUseTexture) {
		imgMix.getTextureReference().unbind();
		ofDisableNormalizedTexCoords();
	}
	
	
	// Dibujar esfera central
	//	drawSol();
	ofDisableAlphaBlending();
	sol.draw();
	ofEnableAlphaBlending();

	
	// Dibujar bolas de las partículas:
	ofPushStyle();
		ofPushMatrix();
		//
//		ofTranslate(-(W_WIDTH)/2.0, -ofGetHeight()/2.0, -60);
		ofTranslate(0,0, -60);
		//
		ofDisableDepthTest();
		for(int i=0; i<particulas.size(); i++) {
			particulas[i].draw3D();
		}
		ofEnableDepthTest();
		ofPopMatrix();
	ofPopStyle();	

	
	mat1.end();
	if(swLight) {
		light.disable(); 
		ofDisableLighting();
	}
	ofDisableAlphaBlending();
	
	ofPopMatrix();
	
//	cam.end();
	camera.end();

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
//            cout << "parte final"<<endl;
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
	
	ofDisableDepthTest();
	
	borde.draw();
	
	ofPushStyle();
	ofSetColor(255,0,0);
	
	
	
	
	
	
    if(bshowdebug){	
		if(hands.objectsCol.size()>0) {
			ofVec2f ptInteract(hands.objectsCol[0]->x,-hands.objectsCol[0]->y);
			ofDrawBitmapString("TUIO 0: "+ofToString(ptInteract.x), ptInteract.x, ptInteract.y);
			ofSetColor(255,255,0);
			ofDrawBitmapString("TUIO 0: "+ofToString(ptInteract.x)+","+ofToString(ptInteract.y), zentro.x+ptInteract.x, zentro.y-ptInteract.y);
			
		}
        ofDrawBitmapString("fr: " + ofToString(ofGetFrameRate()), 10, ofGetHeight()-90);
        ofDrawBitmapString("masa Sol: " + ofToString(sol.masa), 10, ofGetHeight()-70);
        ofDrawBitmapString("num Partics: " + ofToString(particulas.size()), 10, ofGetHeight()-50);
    }
	ofPopStyle();
//    ofDisableDepthTest();
}

void gravedad::mousePressed( int x, int y, int button ){}


//scene notifications
void gravedad::sceneWillAppear( ofxScene * fromScreen ){  // reset our scene when we appear
    //gui1->enable();
    cheapComm::getInstance()->sendAudio0("/audio/gravity/start_event");
    cheapComm::getInstance()->sendSync0("/sync/gravity/start_event");
    init_Escena();
};


//scene notifications
void gravedad::sceneWillDisappear( ofxScene * toScreen ){
	// gui1->disable();

    cheapComm::getInstance()->sendAudio0("/audio/gravity/end_event");
    cheapComm::getInstance()->sendSync0("/sync/gravity/end_event");
	exit_Escena();
};


ofPoint gravedad::transformTUIO(ofxTuioCursor &tuioCursor) {
	return ofPoint((tuioCursor.getX()-0.5)*W_WIDTH,(tuioCursor.getY()-0.5)*W_HEIGHT);														   
}

void gravedad::tuioAdded(ofxTuioCursor &tuioCursor){
//	ofPoint loc = ofPoint(tuioCursor.getX()*W_WIDTH-300,tuioCursor.getY()*W_HEIGHT-300);
	ofPoint loc = transformTUIO(tuioCursor);
    
    handShadow *h1 = new handShadow();
    h1->setup();
    hands.addObject(*h1);
    hands.notifyTouch(loc.x, loc.y,tuioCursor.getSessionId());
    
    cheapComm::getInstance()->sendAudio0("/audio/gravity/hand_on_event");
}

void gravedad::tuioUpdated(ofxTuioCursor &tuioCursor){
	//	ofPoint loc = ofPoint(tuioCursor.getX()*W_WIDTH-300,tuioCursor.getY()*W_HEIGHT-300);
	ofPoint loc = transformTUIO(tuioCursor);
        hands.notifySlide(loc.x, loc.y,tuioCursor.getSessionId(),tuioCursor.getMotionAccel());
}

void gravedad::tuioRemoved(ofxTuioCursor &tuioCursor){
	//	ofPoint loc = ofPoint(tuioCursor.getX()*W_WIDTH-300,tuioCursor.getY()*W_HEIGHT-300);
	ofPoint loc = transformTUIO(tuioCursor);
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
	gui1->addToggle("Noise Auto", &noiseAuto);
	gui1->addSlider("nivel_Noise", 0, 100.0, &nivelNoise);
	
	gui1->addSpacer();
	gui1->addLabel("Sol");
	gui1->addSlider("radio_Sol", 10, 250.0, &sol.radio);
	gui1->addSlider("masa_Sol", 1000, MAX_MASA_SOL*1.5, &sol.masa);
    
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
    if(key=='g'){
        if(gui1->isEnabled())
            gui1->disable();
        else
            gui1->enable();
    }
    
}
void gravedad::guiEvent(ofxUIEventArgs &e) {
	string name = e.getName();
	int kind = e.getKind();
    //	cout << "got event from: " << name
	if(name == "radio_Sol")
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		sol.setRadio(slider->getScaledValue());		
//		rSol = slider->getScaledValue();
//		sole.setRadius(rSol);
//		setMfromR();
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
		if(swOrtho) {
			cam.enableOrtho();
			camera.enableOrtho();			
		}
		else {
			cam.disableOrtho();
			camera.disableOrtho();
		}
	}
	
}


