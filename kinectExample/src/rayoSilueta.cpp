//
//  rayoSilueta.cpp
//  kinectExample
//
//  Created by Sergio Galan on 9/16/14.
//
//

#include "rayoSilueta.h"


#include "utilsGeom.cpp"

//--------------------------------------------------------------
void rayoSilueta::setup(){
    
	ofSetVerticalSync(true);
	zentro = ofVec3f(ofGetWidth()/2.0,ofGetHeight()/2.0,0);
	swDifraccion = false;
	ratePartic = 20;
    vel=20;
	bDrawCaminos = true;
	bDrawPtosChoque = true;
	bTiltCamino = false;
		
	
	testPath.setFilled(false);
	testPath.setStrokeColor(ofColor::blueSteel);
	testPath.setColor(ofColor::blueSteel);
	
    bAddParticles=false;
    glowShader.load("shaders/glow.vert","shaders/glow.frag");
    fboGeneral.allocate(ofGetWidth()/3,ofGetHeight()/3);
    fboGeneral.begin();
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    fboGeneral.end();
    
   // postp1.init(ofGetWidth(), ofGetHeight());
   // postp1.createPass<BloomPass>()->setEnabled(true);
    
    fboRayos.allocate(ofGetWidth()/3,ofGetHeight()/3,GL_RGBA);
    fboRayos.begin();
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    fboRayos.end();
    
}

void rayoSilueta::setSilueta(ofPolyline p){
    camino=p;
}

//--------------------------------------------------------------
void rayoSilueta::update(contours &cont ){
    mExplosionEfect.update();
	// add alguna particula desde un lateral
    
    if(ofGetElapsedTimeMillis() > stopParticlesAt ) bAddParticles=false;
    if (guiAddParticles) bAddParticles =true;
    
    if(bAddParticles)
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
                    mExplosionEfect.addEmitter(ptChoque.x, ptChoque.y);
					if(ofRandom(0,1)>0.5)    cont.addImpact(ptChoque.x, ptChoque.y);
                    
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
		if(particulas[i].position.y>ofGetHeight()+50) particulas.erase(particulas.begin()+i);
	}
    
}

void rayoSilueta::tiltCamino() {
	ofPolyline pl = camino;
	camino.clear();
	
	float maxDespl = 2.0;
	for(int i=0; i<pl.size(); i++) {
		camino.addVertex( pl[i]+ofVec2f(ofRandom(-maxDespl, maxDespl), ofRandom(-maxDespl, maxDespl)) );
	}
	camino.close();
	
	
}


void rayoSilueta::addParticleLateral() {
	//	float rnd = ofRandom(1.0);
	//	if(rnd<0.15) {
	if(ofGetFrameNum()%ratePartic==0) {
		//		ParticleX( ofVec3f _position , ofColor _color, float mass = 1.0, float charge = 0.0 )
		//		ParticleX( ofVec3f _position , ofVec3f _vel , ofColor _color, float mass = 1.0, float charge = 0.0 )
        for(int ii=0; ii<10; ii++){
            ParticleX p = ParticleX(ofVec3f(ofRandom(ofGetWidth()/3),0), ofVec3f(0,vel,0), ofColor(255,255,255) );
            particulas.push_back(p);
        }	
		// pos, vel, color, mass, charge
        //		ParticleX pTmp = ParticleX(ofVec3f(0,ofRandom(ofGetHeight())) , ofVec3f(ofRandom(10.0, 30.0), 0) , ofColor::white, 1.0, 0.0);
	}
}

void rayoSilueta::addParticlesFor(int msecs){
    bAddParticles=true;
    stopParticlesAt=ofGetElapsedTimeMillis()+msecs;
}

void rayoSilueta::triggerParticles(float ang){
    if(SCREEN_ID==1){
        if(ang>angTV1 && ang<angTV1+0.3) addParticlesFor(500);
    }
    if(SCREEN_ID==2){
        if(ang>angTV2 && ang<angTV2+0.2)addParticlesFor(500);
    }
    if(SCREEN_ID==3){
        if(ang>angTV3 && ang<angTV3+0.2)addParticlesFor(500);
    }
}
//--------------------------------------------------------------
void rayoSilueta::draw(){
  //  fboGeneral.begin();
  //  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    ofPushMatrix();
    //ofTranslate(ofGetWidth(), ofGetHeight());
    //ofRotateZ(180);
    ofScale(1,1,1);
	
	//
	//
	// PARTICULAS
	//
	
	ofEnableSmoothing();
	ofEnableAntiAliasing();
    
       ofEnableAlphaBlending();
       ofSetColor(0 ,0,0,10);
       ofFill();
       ofRect(0, 0, ofGetWidth(), ofGetHeight());
    ofSetColor(255,255,255,255);
		ofPushStyle();
		ofSetColor(ofColor::lime, 120);
		ofSetLineWidth(2);
		camino.draw();

        
		ofSetColor(ofColor::white, 200);
		ofSetLineWidth(1);
		camino1.draw();
		ofPopStyle();
		testPath.draw();


	fboGeneral.begin();
    glEnable(GL_POINT_SMOOTH);
    ofEnableAlphaBlending();
    ofSetColor(0 ,0,0,10);
    ofFill();
    ofRect(0, 0, ofGetWidth(), ofGetHeight());
    ofSetColor(255,255,255,255);
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
    mExplosionEfect.draw();
	fboGeneral.end();
    //postp1.begin();
      //  glPushMatrix();
       // glViewport(0, 0, postp1.getRawRef().getWidth()*3,  postp1.getRawRef().getHeight()*3);
    fboGeneral.draw(0,0);
       // glPopMatrix();
    //postp1.end();


	
	ofPopMatrix();
   // fboGeneral.end();
    
    /*glowShader.begin();
    glowShader.setUniform1f("glowSize",0.5);
    
    glowShader.setUniformTexture("tex1", fboGeneral.getTextureReference(), 1 );
        ofSetColor(255);
        fboGeneral.draw(0, 0);
    glowShader.end();*/
	
	
	
}

void rayoSilueta::setUI(ofxUITabBar *guiTabBar ){
    gui = new ofxUICanvas(0,100,400,800);
    gui->setName("RayoSilueta" );
    gui->addIntSlider("ratePartic", 1, 30,  &(ratePartic));
    gui->addSlider("vel", 0.1f, 60,  &(vel));
    gui->addToggle("swDifraccion", &swDifraccion);
    gui->addToggle("bDrawPtosChoque", &bDrawPtosChoque);
    gui->addToggle("bDrawCaminos", &bDrawCaminos);
    gui->addToggle("bTiltCamino", &bTiltCamino);
    gui->addToggle("addParticles", &guiAddParticles);
    gui->addButton("clear", false);
    gui->addButton("partClear", false);
    
    ofAddListener(gui->newGUIEvent,this,&rayoSilueta::guiEvent);
    guiTabBar->addCanvas(gui);
    
}

void rayoSilueta::guiEvent(ofxUIEventArgs &e)
{
    string name = e.widget->getName();
    int kind = e.widget->getKind();
    
    if(name == "clear")
    {
        camino.clear();
		camino1.clear();
		ptsChoque.clear();
		
		testPath.clear();
    }
    else if(name== "partClear"){
		particulas.clear();
    }
    
}


