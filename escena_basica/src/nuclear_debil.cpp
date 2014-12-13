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
    
	// Sustituye a 
	//    static enum states_nuclear_w{NORMAL, EXPLOSION};
    NORMAL = 0;
	EXPLOSION = 1;
	
	
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
	ofEnableArbTex();
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
	mas_particulas_img.loadImage("mas_particulas.png");
    mas_acel_img.loadImage("mas_aceleracion.png");
	
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
    buttonSpeed1.set(0, 290,100,143);
    buttonSpeed1.setup();
//	buttonSpeed2.set(centro.x+rMed-(dRad*0.9/2),centro.y-(dRad*0.9/2), dRad*0.9,dRad*0.9);
    buttonSpeed1.btype=TYPE_ACC;

    buttonSpeed2.set(668,290,100,143);
    buttonSpeed2.setup();
    buttonSpeed2.btype=TYPE_ACC;
    
    //buttonCollide.set(centro.x-(dRad*0.9/2),centro.y-rMed-(dRad*0.9/2), dRad*0.9,dRad*0.9);
    buttonCollide.set(309,0,143,100);
    buttonCollide.btype=TYPE_CRASH;
    buttonCollide.setup();

    touchElements.addObject(buttonSpeed1);
    touchElements.addObject(buttonSpeed2);
    touchElements.addObject(buttonCollide);

	
// Otros - - - - - 
	swBlendModeADD = false;
	
	frecFondo = 4.0;

	// Esto deber�a ser com�n a todas las escenas
//	myComm.setup();
	
	//setupTuio();
	
	//init_Escena();
    
    spriteExp = new ofxSpriteSheetRenderer(1, 1000, 0, 100); //declare a new renderer with 1 layer, 10000 tiles per layer, default layer of 0, tile size of 32
	spriteExp->loadTexture("exp__Spritesheet.png", 2800, GL_NEAREST); // load the spriteSheetExample.png texture of size 256x256 into the sprite sheet. set it's scale mode to nearest since it's pixel art

}
// comparison routine for sort...

bool sortVertically(  basicSprite * a, basicSprite * b ) {
	return a->pos.y > b->pos.y;
}



void nuclear_debil::init_Escena() {
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	// init variables
	fpsAct = 370.0;
	box2d.setFPS(fpsAct);

	swFuerza = false;
	
	modoDrawParticulas = MODO_PARTIC;	
	swDrawTRAILS = true;

	anillo.reset_wAng();
    anillo.setFadeInMode();
	
    status=nuclear_debil::NORMAL;
	ofPoint centro = ofPoint(W_WIDTH/2.0, W_HEIGHT/2.0);

	// crear cosas
	// add some nucleos
	nucleos.clear();
    destellos.clear();
	borrar_neutrones();
    borrar_nucleos();
    timeLastColision=0;
    
    nCircs = 60 + floor(  ofRandom(10) );
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
    sent_changeScene_message=false;
	ofEnableAlphaBlending();
    bshowdebug=false;
    display_rotation_angle=0;
    show_info_img_until=0;
    

}

void nuclear_debil::exit_Escena() {

	// borrar objetos
	// Eliminar neutrones
	borrar_neutrones();
	
	// Eliminar nucleos
	borrar_nucleos();
	
	// Eliminar los destellos que queden
	destellos.clear();
	//quito manos
    hands.reset();
	
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
	if(ofGetFrameNum()%3 ==0){
        float ang=anillo.angT;
        ang-=PI/4;
        
        cheapComm::getInstance()->sendAudio2("/audio/weak_nuclear/ball", ofMap(ofWrapRadians(ang,0,2.0*PI),0,2.0*PI,0,1),
                                             ofMap(anillo.wAng,0,-anillo.wAngMax,0,1));
        //cout << anillo.angT<< " " << -1*(anillo.angT)/(2*PI) <<endl;
       // cout << anillo.angT<< ofMap(ofWrapRadians(ang,0,2.0*PI),0,2.0*PI,0,1);
        
        cheapComm::getInstance()->sendAudio2("/audio/weak_nuclear/number_particles", ofMap(nucleos.size(),nCircs,nCircs+50,0,1), ofMap(anillo.angT,0,2*PI,0,1));
        //cout << nucleos.size()<< " ";
//        CUAL ES EL MAXIMO DE NUCLEOS AQUI?
    
         ang=anillo.angT;
        ang+=PI/2;
        cheapComm::getInstance()->sendSync3("/sync/weak_nuclear/ball", ofWrapRadians(ang,0,2.0*PI),
                                            ofMap(abs(anillo.wAng),0,abs(anillo.wAngMax),0,1),
                                            ofSign(anillo.wAng) );
   }
    if ( ofGetElapsedTimeMillis() < (timeLastColision+2000)){
        cheapComm::getInstance()->sendSync0("/sync/weak_nuclear/colisiones");

    }
    else
        cheapComm::getInstance()->sendSync0("/sync/weak_nuclear/nocolisiones");
    
    /********/

//TUIOS
    buttonCollide.update_prev(abs(anillo.wAng/(anillo.wAngMax*0.95)),(float)numNucleosActivos/NUCLEOS_TO_EXPLOSION);
    buttonSpeed1.update_prev(abs(anillo.wAng/(anillo.wAngMax*0.95)),(float)numNucleosActivos/NUCLEOS_TO_EXPLOSION);
    buttonSpeed2.update_prev(abs(anillo.wAng/(anillo.wAngMax*0.95)),(float)numNucleosActivos/NUCLEOS_TO_EXPLOSION);
    
	touchElements.update();
    hands.update();
	
	// recorrer el vector de cursores que hay en hands (tangiblesHandler) e interactuar si esta dentro del c�rculo
	int ttAct = ofGetElapsedTimeMillis();
	float sqLim = (radioInt*radioInt)*0.9;
	int limTime = floor(1000/ofGetFrameRate())+20;
    
    
    //ANIADIMOS NUCLEOS SOLO SI NO HAY YA DEMASIADOS
    if(numNucleosActivos<NUCLEOS_TO_EXPLOSION*1.3){
        for(int i=0; i<hands.objectsCol.size(); i++) {
            handShadow * h = (handShadow *) hands.objectsCol[i];
            // si esta en la parte central del c�rculo
            ofVec2f vHand = ofVec2f(h->x,h->y);
    //		ofVec2f vHandCentro = ofVec2f(x-W_WIDTH/2.0,y-W_HEIGHT/2.0);
            if(vHand.squareDistance(ofVec2f(W_WIDTH/2.0,W_HEIGHT/2.0)) < sqLim ) {
                int dtt = ttAct-h->age;
                if(dtt%1000<limTime) {
                    addNucleo(h->x, h->y);
                }
            }
        }
    }
    
    /*** Sprite update ***/
    spriteExp->clear(); // clear the sheet
	spriteExp->update(ofGetElapsedTimeMillis()); //update the time in the renderer, this is necessary for animations to advance
    sort( sprites.begin(), sprites.end(), sortVertically ); // sorts the sprites vertically so the ones that are lower are drawn later and there for in front of the ones that are higher up
    
    if(sprites.size()>0) // if we have sprites
	{
		for(int i=sprites.size()-1;i>=0;i--) //go through them
		{		
			if(sprites[i]->animation.frame>= sprites[i]->animation.total_frames-1) //if they are past the bottom of the screen
			{
                
				//delete sprites[i]; //delete them
				//sprites.erase(sprites.begin()+i); // remove them from the vector
			}
			else //otherwise //int layer = -1,                        flipDirection f = F_NONE, float scale = 1.0,          int r=255, int g=255, int b=255,
				//spriteExp->addCenteredTile(&sprites[i]->animation, sprites[i]->pos.x, sprites[i]->pos.y);//,-1,F_NONE,1-0,0,0,120); // add them to the sprite renderer (add their animation at their position, there are a lot more options for what could happen here, scale, tint, rotation, etc, but the animation, x and y are the only variables that are required)
                spriteExp->addCenteredTile(&sprites[i]->animation, sprites[i]->pos.x, sprites[i]->pos.y,-1,F_NONE,0.5);
		}
	}
    cout << "num nucleos" << nucleos.size()<<endl;
    
}

//--------------------------------------------------------------
void nuclear_debil::draw(){
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
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

    //ofSetColor(0, 0, 255);

    
	numNucleosActivos = 0;
	if(swBlendModeADD) ofEnableBlendMode(OF_BLENDMODE_ADD);
	for(int i=0; i<nucleos.size(); i++) {
		ofFill();
		ofSetHexColor(0xf6c738);
		nucleos[i].get()->draw();
		
		if(nucleos[i].get()->swExcitado) numNucleosActivos++;
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
			if(!nucleos[i].get()->swExcitado)	nucleos[i].get()->drawTrail();
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
	ofEnableAlphaBlending();
	
	
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

    drawCenterDisplay();
    
    //spriteExp->draw();
//    if(sprites.size()>0) // if we have sprites
//MODO EXPLOSION FIN DE LA ESCENA
    if(status==EXPLOSION ){
        if(init_explosion_time+20000 < ofGetElapsedTimeMillis() ){ //la escena termina si ya ha pasado mucho tiempo desde la explosi�n
            if(sent_changeScene_message==false){
                ofSendMessage("changeScene" +ofToString(SCENE_MENU));
                status==OFF;
                sent_changeScene_message=true;
            }
        }
    }else{
        if(show_info_img_until>ofGetElapsedTimeMillis() && (show_info_img_until-5500)< ofGetElapsedTimeMillis()  ){
            drawMessage();
        }
        
    }
        
    ofPopMatrix();  // FIn translate para centrar contenidos
    
    
	// Info
	if(bshowdebug)drawInfo();


}

void nuclear_debil::addSprite(int x1, int y1){
    basicSprite * newSprite = new basicSprite(); // create a new sprite
    newSprite->pos.set(x1,y1); //set its position
    newSprite->speed=ofRandom(1,5); //set its speed
    newSprite->animation = walkAnimation; //set its animation to the walk animation we declared
    //newSprite->animation.frame_duration /= newSprite->speed; //adjust its frame duration based on how fast it is walking (faster = smaller)
    newSprite->animation.index = 0; //change the start index of our sprite. we have 4 rows of animations and our spritesheet is 8 tiles wide, so our possible start indicies are 0, 8, 16, and 24
    
    sprites.push_back(newSprite); //add our sprite to the vector

}

void 	nuclear_debil::drawInfo(){
	string info = "";
	info += "Press [c] for nucleos\n";
	info += "Press [n] for neutron\n";
	info += "Press [t] + mouseX para Time Step: "+ofToString(fpsAct)+"\n";
	info += "time en millis: "+ ofToString(ofGetElapsedTimef()) +"\n";
	info += "Total Bodies: "+ofToString(box2d.getBodyCount())+"\n";
	info += "FPS: "+ofToString(ofGetFrameRate(), 1)+"\n\n";
	info += "Aplica Fuerza [f]: "+ofToString(swFuerza)+"\n";
	info += "BlendMode ADD [b]: "+ofToString(swBlendModeADD)+" \n";
	info += "aceleracion [click en esquinas inferiores - +] w/a: "+ofToString(anillo.wAng)+"/"+ofToString(anillo.accAng)+" \n\n";
	info += "veloc Ang: " + ofToString(anillo.wAng) + "\n";

	info += "nuevasPartics: " + ofToString(nuevasPartics.size()) + "\n";
	info += "Num Nucleos: " + ofToString(nucleos.size()) + "\n";
	info += "Num Nucleos Activos: " + ofToString(numNucleosActivos) + "\n";
	info += "radianes enviados" + ofToString(ofWrapRadians(anillo.angT+PI/2,0,2.0*PI))+ "\n";
    info += "radianes enviados audio" + ofToString(ofMap(ofWrapRadians(anillo.angT-PI/4,0,2.0*PI),0,2.0*PI,0,1));
	//	ofSetHexColor(0x444342);
	ofSetColor(255,255);
	ofDrawBitmapString(info, 30, 30);
}

void nuclear_debil::drawCenterDisplay(){
    ofPushMatrix();
    ofPushStyle();
    ofTranslate(W_WIDTH/2, W_HEIGHT/2);
    int r=180;
    int n_nucleos=numNucleosActivos;
    ofSetColor(ofColor::fromHsb(ofMap(ofClamp(n_nucleos,0,NUCLEOS_TO_EXPLOSION),0,NUCLEOS_TO_EXPLOSION,100,250)
                                ,250, 250,
                                100+100*sin(ofGetElapsedTimef()/ofMap(ofClamp (n_nucleos,0,NUCLEOS_TO_EXPLOSION),0,NUCLEOS_TO_EXPLOSION,10.0,5.0)*TWO_PI)));
    
    // ofSetColor(27,117,187,200+50*sin(ofGetElapsedTimef()/2.0*TWO_PI));
    for(int i=0; i<ofMap(abs(n_nucleos),0,NUCLEOS_TO_EXPLOSION,0,360); i+=8){
            ofRotateZ(8);
      /*  if( (i>=0 && i<10) || (i>73 && i<100) || (i>166 && i<190) || (i>342 && i<360)){
            
        }else*/
            ofRect(-r,0,24,10);
    }
    ofPopStyle();
    ofPopMatrix();
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

void nuclear_debil::drawMessage(){
    ofImage * img;
    if(display_to_show==MAS_PART){
        img=&mas_particulas_img;
    }else if(display_to_show==MAS_ACE){
        img=&mas_acel_img;
    }
    else return;
    
    int RADIO = 130;
    ofPushMatrix();
        ofPushStyle();
       // ofCircle(W_WIDTH/2, W_HEIGHT/2, RADIO);
        ofSetColor(255);
        ofPushMatrix();
            ofTranslate(W_WIDTH/2, W_HEIGHT/2);
            ofRotateZ(display_rotation_angle);
            img->draw(-img->width/2,-img->height/2);
        ofPopMatrix();
        ofPopStyle(),
    ofPopMatrix();
    display_rotation_angle+=0.6;
    
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
    cheapComm::getInstance()->sendSync0("/sync/weak_nuclear/start_event");
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
     cheapComm::getInstance()->sendSync0("/sync/weak_nuclear/end_event");
};





