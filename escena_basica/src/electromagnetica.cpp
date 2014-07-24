//
//  electromagnetica.cpp
//  escena_basica
//
//  Created by Sergio Galan on 7/11/14.
//
//

#include "electromagnetica.h"


void electromagnetica::setup(){

    alpha=0.2;
    options_sampling=3;

    setupShader();
    setupGUI();
    wavesm.setup();
    //tuioClientEm.start(3333);

    gui1->disable();
    
    colours.push_back(ofColor(63,184,175));
    colours.push_back(ofColor(127,199,175));
    colours.push_back(ofColor(218,216,167));
    colours.push_back(ofColor(255,158,157));
    colours.push_back(ofColor(255,61,127));
    colours.push_back(ofColor(0,223,252));
    
    freq=-0.11;
    wave_amp=50;
    n_ciclos=1;
    ofAddListener(gui1->newGUIEvent,this,&electromagnetica::guiEvent);
    
    //magneticFieldSvg.load("magneticfield.svg");
    
    //updateMagneticField(100,0,0);
}

void electromagnetica::update(float d1){
    hands.update(); //actualiza la sombra de las manos
    int singleWaveId= wavesm.getSingleWaveId();
    int singleWaveIndex=-1;
    if(singleWaveId!=-1){
        for (int i=0; i< hands.objectsCol.size(); i++){
            if(hands.objectsCol[i]->cursor_id== singleWaveId){
                singleWaveIndex=i;
            }
        }
    }
    //tuioClientEm.getMessage();
    
    wavesm.update(); //actualiza la onda  matematica

  /*  if(singleWaveIndex!=-1){
        //muevo particulas cerca de la mano: orbita
        // oculto el resto
        for(int i=0; i< meshParticles.getVertices().size(); i++){
            if( particles[i].position.distanceSquared( ofPoint( hands.objectsCol[singleWaveIndex]->x, hands.objectsCol[singleWaveIndex]->y ) ) >10000 ){
                if(ofRandomf()>0.5)
                    meshParticles.getColors()[i].set(0);
                else
                    meshParticles.getColors()[i].set(255);
                
                ofPoint p= ofPoint(particles[i]._x,particles[i]._y,particles[i].position.z);
                particles[i].steer( p, true, 3, 8);
                particles[i].update();
                
            }else{ //color de ruido
                ofPoint p1=((noiseShadow *)hands.objectsCol[singleWaveIndex])->getDstPoint(particles[i]._x, particles[i]._y);
                particles[i].steer( p1, true, 4, 10);
                
                particles[i].update();
                //ofPoint p= ofPoint(particles[i]._x,particles[i]._y,particles[i].position.z);
                //particles[i].gravityTowards(p, 30, 10);
                //particles[i].update();
                meshParticles.getColors()[i].set(255);
                }
            meshParticles.getVertices()[i].set(particles[i].position);

        }
        
    }else*/
    if(wavesm.howManyWaves()==0){
     //ruido puro
        for(int i=0; i< meshParticles.getVertices().size(); i++){
            //ofSeedRandom();
            if(ofRandomf()>0.5)
                meshParticles.getColors()[i].set(0);
            else
                meshParticles.getColors()[i].set(255);
         
            ofPoint p= ofPoint(particles[i]._x,particles[i]._y);
            
            if(singleWaveIndex!=-1 &&
               p.distanceSquared( ofPoint( hands.objectsCol[singleWaveIndex]->x, hands.objectsCol[singleWaveIndex]->y ) ) <10000 ){
                ofPoint p1=((noiseShadow *)hands.objectsCol[singleWaveIndex])->getDstPoint(particles[i]._x, particles[i]._y);
                particles[i].steer( p1, true, 4, 10);
            }else{
                particles[i].steer( p, true, 7, 10);
            }
            particles[i].update();
            meshParticles.getVertices()[i].set(particles[i].position);
        }
    }
    
    else if(wavesm.howManyWaves()>=1){ // Dependiendo de numero de ondas mas o menos particulas irán a cada onda.
        //ondas entre 2 puntos
        // oculto el resto
        int puntos_ondas=0;
        
        int ondasAct=wavesm.howManyWaves();
        for(int i=0; i<wavesm.howManyWaves(); i++){
            puntos_ondas+=wavesm.waveslist[i].AbsPoints.size();
        }
        int rows=W_WIDTH/options_sampling;
        
        //check if there is any single cursor where we need to draw the magnetic field
        ofPoint positionCursor;
        if(singleWaveIndex!=-1){
             positionCursor=ofPoint( hands.objectsCol[singleWaveIndex]->x, hands.objectsCol[singleWaveIndex]->y );
        }
        else  positionCursor=ofPoint(0,0);
        
        for(int i=0; i< meshAux.getVertices().size(); i++){
            if(singleWaveIndex!=-1){
                ofPoint p= ofPoint(particlesAux[i]._x,particlesAux[i]._y);
                ofPoint p1=((noiseShadow *)hands.objectsCol[singleWaveIndex])->getDstPoint2(particlesAux[i]._x, particlesAux[i]._y);
                particlesAux[i].steer( p1, true, 1, 10);
                if(meshAux.getColors()[i].a<=250)
                    meshAux.getColors()[i].a+=5;
                particlesAux[i].update();
                meshAux.getVertices()[i].set(particlesAux[i].position);
            }
            else{
                if(meshAux.getColors()[i].a>=5)
                    meshAux.getColors()[i].a-=5;
            }
        }
        
        for(int i=0; i< meshParticles.getVertices().size(); i++){

            int index=i % puntos_ondas;
            trIndices num_onda= wavesm.num_onda(index);
            //particles[i]._x;
            ofPoint p= wavesm.waveslist[num_onda.n_wave].AbsPoints[num_onda.new_index];
            particles[i].steer(p , true, 5, 10);
            int len=wavesm.waveslist[num_onda.n_wave].npuntos;
            ofColor color1=ofColor::fromHsb( ofMap(len, 0, 768, 0,360),255,255) ;
            color1.a=200;
            meshParticles.getColors()[i].set(color1);
            particles[i].update();
            meshParticles.getVertices()[i].set(particles[i].position);
        }
    }
  /*  for(int i =0; i<hands.objectsCol.size(); i++){
        int mx= hands.objectsCol[i]->x;
        int my= hands.objectsCol[i]->y;
        
        ofPoint p= ofPoint(mx,my);
        for(int j=1; j<100; j++){
            int jj=(int) (numParticles/j);
            particles[jj].steer(p , true, 5, 10);
            particles[jj].update();
            meshParticles.getColors()[jj].set(colours[(int)( hands.objectsCol[i]->cursor_id % colours.size()-1) ]);
            meshParticles.getVertices()[jj].set(particles[jj].position);
        }
        
    }*/
    
    }

void electromagnetica::draw(){
    
    ofBackground(0);
    ofPushMatrix(); //colocamos el canvas en su posicion centrada
        ofTranslate((ofGetWidth()-W_WIDTH)/2, 0);
        drawNoise();
        ofNoFill();
        //ofEllipse(W_WIDTH/2, W_WIDTH/2, W_WIDTH, W_WIDTH);
       // ofEnableAlphaBlending();
        ofPushMatrix();
            ofSetColor(255);
            meshParticles.setMode(OF_PRIMITIVE_POINTS);
                meshAux.setMode(OF_PRIMITIVE_POINTS);
            glPointSize(2);
            glEnable(GL_POINT_SMOOTH);	// Para que sean puntos redondos
            ofEnableDepthTest();
            meshParticles.draw();
            meshAux.draw();
            ofDisableDepthTest();
            ofSetColor(255,0,0);
            ofFill();
        if(drawlines) wavesm.draw();
        ofPopMatrix();
        hands.draw();
        ofPushMatrix();
            //ofTranslate(200,200);
       // drawEM();
        ofPopMatrix();
    ofPopMatrix();

    showDebug();

    //wavesm.debugInfo();
}

void electromagnetica::mousePressed( int x, int y, int button ){

//    mwave.addPoint(X_(x),y,0);
}

void electromagnetica::setupShader(){
	shader.load("", "shaders/whitenoise.frag");
    setupParticles();
    
}

void electromagnetica::setupGUI() {
	gui1 = new ofxUICanvas(0,50,300,400);
    gui1->addSlider("alpha", 0.0, 1.0,&alpha);
   // gui1->addToggle("noise or wave", &noiseMode);
    gui1->addSlider("freq", -3, 3,&freq);
    gui1->addSlider("amplitude", 1, 150,&wave_amp);
        gui1->addSlider("n_ciclos", 0.5, 4,&n_ciclos);
 //   gui1->addSlider("freq", 0.0, 100.0,&(mwave.freq2));
}
void electromagnetica::showDebug(){
    ofDrawBitmapString("Framerate " + ofToString(ofGetFrameRate()), 20, 300);

}

void electromagnetica::keyPressed(int key){
    switch(key){
        case 'v':
			gui1->saveSettings("/config/gui/gui_EM.xml");
			break;
        case ' ':
            if(gui1->isEnabled() )
	            gui1->disable();
            else
				gui1->enable();
        case 'l':
            drawlines=!drawlines;
            
    }
}

void electromagnetica::drawNoise(){
    ofPushStyle();
    ofFill();
     ofSetColor(255,255,255,40);
    
    /**ofTexture mtexture;
     mtexture.allocate(200, 200, GL_RGB);
     ofImage mimage;mimage.loadImage("Vendeta.jpg");
     mimage.getTextureReference().bind();**/

    shader.begin();
	
    shader.setUniform3f("iResolution", W_WIDTH, W_HEIGHT,0);
    shader.setUniform1f("iGlobalTime", ofRandomf());
    shader.setUniform1f("alpha", alpha);
    shader.setUniform2f("iMouse",ofGetMouseX(),ofGetMouseY());
    
    //mtexture.loadScreenData(600,600,200,200);
    //mtexture.loadData(mimage.getPixelsRef());
    ///shader.setUniformTexture("iChannel0",mtexture,2 );
    
    ofRect(0, 0, W_WIDTH, W_HEIGHT);
    shader.end();
    ofPopStyle();
    //mimage.getTextureReference().unbind();
}

void electromagnetica::setupParticles(){

    int w= W_WIDTH;
    int h= W_HEIGHT;

    //Loop through all the rows
    
	numParticles = 0;
    //Loop through all the columns
	// Distribuir las particulas por la escena
    ofSeedRandom();
    for ( int y = 0 ; y < h ; y+=options_sampling ){
        for ( int x = 0 ; x < w ; x+=options_sampling ){
            //			particles.push_back(Particle(ofVec3f(0,0,0),ofColor(255,255,255) ,x,y));
            //			float px = ofRandom(-1000,1000);
            //			float py = ofRandom(-1000,1000);
            //			float pz = ofRandom(0,5000);
            //			particles.push_back(Particle(ofVec3f(px,py,pz) ,ofColor(255,255,255) ,x,y));
			
            if(ofRandomf()>0.5)
            {
                Particle mparticle=Particle(ofVec3f(x,y,3) ,ofColor(255,255,255) ,x,y);
                particles.push_back(mparticle);
                meshParticles.addVertex(mparticle.position );
                meshParticles.addColor(mparticle.color);
            }
            else{
                Particle mparticle=Particle(ofVec3f(x,y,3) ,ofColor(0,0,0) ,x,y);
                particles.push_back(mparticle);
                meshParticles.addVertex(mparticle.position );
                meshParticles.addColor(mparticle.color);
            
            }
			numParticles++ ;
        }
    }
    
    cout<<"particulas"<<numParticles;
    
    for ( int y = -75 ; y < 70 ; y+=options_sampling ){
        for ( int x = -75 ; x < 75 ; x+=options_sampling ){
            //			particles.push_back(Particle(ofVec3f(0,0,0),ofColor(255,255,255) ,x,y));
            //			float px = ofRandom(-1000,1000);
            //			float py = ofRandom(-1000,1000);
            //			float pz = ofRandom(0,5000);
            //			particles.push_back(Particle(ofVec3f(px,py,pz) ,ofColor(255,255,255) ,x,y));

            if(ofRandomf()>0.5)
            {
                Particle mparticle=Particle(ofVec3f(x+300,y+300,3) ,ofColor(255,255,255,0) ,x,y);
                particlesAux.push_back(mparticle);
                meshAux.addVertex(mparticle.position );
                meshAux.addColor(mparticle.color);
            }
            else{
                Particle mparticle=Particle(ofVec3f(x+300,y+300,3) ,ofColor(0,0,0,0) ,x,y);
                particlesAux.push_back(mparticle);
                meshAux.addVertex(mparticle.position );
                meshAux.addColor(mparticle.color);
                
            }
			numParticles++ ;
        }
    }

    
}



void electromagnetica::resetParticles(){
    particles.clear();
    setupParticles();
}

//scene notifications
void electromagnetica::sceneWillAppear( ofxScene * fromScreen ){  // reset our scene when we appear
    gui1->enable();
   /* ofAddListener(tuioClientEm.cursorAdded,this,&electromagnetica::tuioAdded);
	ofAddListener(tuioClientEm.cursorRemoved,this,&electromagnetica::tuioRemoved);
	ofAddListener(tuioClientEm.cursorUpdated,this,&electromagnetica::tuioUpdated);*/

   
};


//scene notifications
void electromagnetica::sceneWillDisappear( ofxScene * toScreen ){
    gui1->disable();
   /* ofRemoveListener(tuioClientEm.cursorAdded,this,&electromagnetica::tuioAdded);
	ofRemoveListener(tuioClientEm.cursorRemoved,this,&electromagnetica::tuioRemoved);
	ofRemoveListener(tuioClientEm.cursorUpdated,this,&electromagnetica::tuioUpdated);*/

}


void electromagnetica::tuioAdded(ofxTuioCursor &tuioCursor){
	ofPoint loc = ofPoint(tuioCursor.getX()*W_WIDTH,tuioCursor.getY()*W_HEIGHT);
	//cout << "Point n" << tuioCursor.getSessionId() << " add at " << loc << endl;
    
    
    // o1.touch(loc.x, loc.y,tuioCursor.getSessionId());
    // h1.touch(loc.x, loc.y,tuioCursor.getSessionId());
    
    noiseShadow *h1 = new noiseShadow();
    h1->setup();
    //h1->setColor(colours[(int)( tuioCursor.getSessionId() % colours.size()-1) ]);
   // h1->setColor(ofColor::black);
    hands.addObject(*h1);
    hands.notifyTouch(loc.x, loc.y,tuioCursor.getSessionId());
    //mwave.addPoint(loc.x,loc.y,tuioCursor.getSessionId());
    wavesm.touch(loc.x, loc.y,tuioCursor.getSessionId());
      //  updateMagneticField(100,loc.x,loc.y);

}

void electromagnetica::tuioUpdated(ofxTuioCursor &tuioCursor){
    int mx = W_WIDTH*tuioCursor.getX();
    int my = W_HEIGHT*tuioCursor.getY();

    hands.notifySlide(mx, my,tuioCursor.getSessionId(),tuioCursor.getMotionAccel());
    wavesm.slide(mx,  my,tuioCursor.getSessionId(),tuioCursor.getMotionAccel());
  //  updateMagneticField(100,mx,my);
}

void electromagnetica::tuioRemoved(ofxTuioCursor &tuioCursor){
	ofPoint loc = ofPoint(tuioCursor.getX()*W_WIDTH,tuioCursor.getY()*W_HEIGHT);
    //cout << "Point n" << tuioCursor.getSessionId() << " remove at " << loc << endl;
    /*
     if(id_slider1==tuioCursor.getSessionId()) id_slider1=NULL;
     if(id_jumpingRect==tuioCursor.getSessionId()) id_jumpingRect=NULL;
     if(id_moverect==tuioCursor.getSessionId()) id_moverect=NULL;*/
    
    
    hands.removeObjectByTuioID(tuioCursor.getSessionId() );
    wavesm.touchUp(tuioCursor.getSessionId());
    
}

void electromagnetica::guiEvent(ofxUIEventArgs &e){
    string name = e.widget->getName();
	int kind = e.widget->getKind();
    if(name == "freq")
    {
		for(int i=0; i<wavesm.howManyWaves(); i++){
            wavesm.waveslist[i].setFreq(freq);
        }
    }
    else     if(name == "amplitude")
    {
       // cout << ofToString(wave_amp) << " ondas" << ofToString(wavesm.howManyWaves());
		for(int i=0; i<wavesm.howManyWaves(); i++){
            wavesm.waveslist[i].setAmplitude(wave_amp);
        }
    }
    else     if(name == "n_ciclos")
    {
       
		for(int i=0; i<wavesm.howManyWaves(); i++){
            wavesm.waveslist[i].setCiclos(n_ciclos);
        }
    }
}

/*void electromagnetica::updateMagneticField(int length,int x1, int y1){
    fieldLines.clear();
    ofSetColor(0,255,0);
    ofNoFill();
   // ofRect(0,0,length,10);
   // ofEllipse(length/2,-50, 80,100);

    int tra_y=(magneticFieldSvg.getHeight()*0.6)/2;
    int tra_x=(magneticFieldSvg.getWidth()*0.6)/2;
        ofPoint pto=ofPoint(x1-tra_x,y1-tra_y);
    for(int i=0; i< magneticFieldSvg.getNumPath(); i++){
        ofPath path1=magneticFieldSvg.getPathAt(i);
        
        path1.scale(0.6,0.6);
        path1.translate(pto);
        fieldLines.push_back( path1.getOutline()[0].getResampledByCount(15));
        //magneticFieldSvg.getPathAt(i).setColor(ofColor::red);
    }
    
    //cout << magneticFieldSvg.getNumPath();
   // magneticFieldSvg.draw();
    
}

ofPoint electromagnetica::getDstPoint(int x1,int y1){
    ofPoint mpoint=ofPoint(x1,y1);
    int minDist=1000000000;
    ofPoint returnPoint=ofPoint(0,0);
    ofPoint tmpPoint;
    for(int i=0; i<fieldLines.size();i++){
        tmpPoint=fieldLines[i].getClosestPoint(mpoint);
        if(tmpPoint.distance(mpoint)<minDist){
            minDist=tmpPoint.distance(mpoint);
            returnPoint=tmpPoint;
        }
    }
    return returnPoint;
}

void electromagnetica::drawEM(){
    ofSetColor(0,255,0);
    for(int i=0; i<fieldLines.size();i++){
        fieldLines[i].draw();
    }
}*/
