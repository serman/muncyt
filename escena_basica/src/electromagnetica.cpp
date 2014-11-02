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
    
    borde.clear();
	ofColor ctmp = ofColor::black;//black;
	borde.setFillColor(ctmp);
	//http://www.openframeworks.cc/documentation/graphics/ofPath.html#show_setPolyWindingMode
	borde.setPolyWindingMode(OF_POLY_WINDING_ODD);
	// rectangulo
	borde.rectangle(0,0,ofGetWidth(),ofGetHeight());
	borde.setCircleResolution(100);
	// Paso a posicion de la pantall centrada en ancho y ajustada al borde superior
    //	borde.circle(ofGetWidth()/2,ofGetHeight()/2,ofGetHeight()/2*0.95);
    //	centro = ofVec2f(ofGetWidth()/2.0, ofGetHeight()/2.0);
    //	distConf = ofGetHeight()/2.0*0.9;
	borde.circle(ofGetWidth()/2, W_HEIGHT/2,W_HEIGHT/2);
    
    /*post.init(ofGetWidth(), ofGetHeight());

    post.createPass<FxaaPass>()->setEnabled(false);
    post.createPass<BloomPass>()->setEnabled(false);
    post.createPass<DofPass>()->setEnabled(false);
    post.createPass<ConvolutionPass>()->setEnabled(false);
    post.createPass<RimHighlightingPass>()->setEnabled(false);
    post.createPass<ZoomBlurPass>()->setEnabled(false);
    post.createPass<EdgePass>()->setEnabled(false);
    post.createPass<VerticalTiltShifPass>()->setEnabled(false);
    post.createPass<ContrastPass>()->setEnabled(false);
    post.setFlip(false);*/
    drawlines=true;
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


    if(wavesm.howManyWaves()==0){
     //ruido puro
        int tmpIndex=0;
        for(int i=0; i< meshParticles.getVertices().size(); i++){
            //ofSeedRandom();
            if(ofRandomf()>0.3)
                meshParticles.getColors()[i].set(ofRandom(0,200));
            else
                meshParticles.getColors()[i].set(0);
         
            ofPoint p= ofPoint(particles[i]._x,particles[i]._y);
            //si no hay ninguna onda, sólo ruido, las partículas de alrededor de la mano son las que forman la onda.
            if(singleWaveIndex!=-1 &&
               p.distanceSquared( ofPoint( hands.objectsCol[singleWaveIndex]->x, hands.objectsCol[singleWaveIndex]->y ) ) <10000 ){
                    ofPoint p1=((noiseShadow *)hands.objectsCol[singleWaveIndex])->getDstPoint(particles[i]._x, particles[i]._y,tmpIndex);
                    particles[i].steer( p1, true, 7, 10);
                    tmpIndex++;
                    meshParticles.getColors()[i].set(ofColor(0,160,176,255));
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
      /*  int puntos_ondas=0;
        
        int ondasAct=wavesm.howManyWaves();
        for(int i=0; i<wavesm.howManyWaves(); i++){
            puntos_ondas+=wavesm.waveslist[i].AbsPoints.size();
        }*/
        int rows=W_WIDTH/options_sampling;
        
        //check if there is any single cursor where we need to draw the magnetic field
        ofPoint positionCursor;
        if(singleWaveIndex!=-1){
             positionCursor=ofPoint( hands.objectsCol[singleWaveIndex]->x, hands.objectsCol[singleWaveIndex]->y );
        }
        else  positionCursor=ofPoint(0,0);
    
    // Aquí se mandan las particulas que son parte de dibujar el campo Magnetico
        for(int i=0; i< meshAux.getVertices().size(); i++){
            if(singleWaveIndex!=-1){
                //ofPoint p= ofPoint(particlesAux[i]._x,particlesAux[i]._y);
                ofPoint p1=((noiseShadow *)hands.objectsCol[singleWaveIndex])->getDstPoint2(particlesAux[i]._x, particlesAux[i]._y,i);
                particlesAux[i].steer( p1, true, 7,20);
                if(meshAux.getColors()[i].a<=1)
                    meshAux.getColors()[i].a+=0.5;
                particlesAux[i].update();
                meshAux.getVertices()[i].set(particlesAux[i].position);
            }
            else{ // si ya ha retirado la mano le quitamos opacidad hasta que desaparezca
                ofPoint p= ofPoint(particlesAux[i]._x,particlesAux[i]._y);
                particlesAux[i].steer( p, true, 7,20);
                particlesAux[i].update();
                meshAux.getVertices()[i].set(particlesAux[i].position);
                if(meshAux.getColors()[i].a>=0)
                    meshAux.getColors()[i].a-=0.05;
            }
        }
   // dibujado de las ondas normales
        for(int i=0; i< meshParticles.getVertices().size(); i++){
            int num_onda=i % MAX_ONDAS; //num_onda es la onda a la que se va a asignar
            trIndices index= wavesm.num_onda(num_onda,i); //Aquí se asigna
            
            ofPoint p= wavesm.waveslist[index.n_wave].AbsPoints[index.new_index];
            particles[i].steer(p , true, 15, 20);
            
            int len=wavesm.waveslist[index.n_wave].npuntos;
            ofColor color1=ofColor::fromHsb( ofMap(len, 0, W_WIDTH, 0,170),255,255) ;
            color1.a=230;
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
    /*OSC SENDING UPDATES */
    if(ofGetFrameNum()%2==0){
        for(int i=0; i<wavesm.howManyWaves(); i++){
            int waveLength=wavesm.waveslist[i].getLength();
            if(wavesm.waveslist[i].waveID!=-1){
            cheapComm::getInstance()->sendAudio2("/audio/electromagnetism/wave_length",wavesm.waveslist[i].waveID, ofMap(waveLength,0,W_WIDTH,1,0));
                
                cheapComm::getInstance()->sendSync2("/sync/electromagnetism/wave_length", wavesm.waveslist[i].waveID, waveLength);
            }
#ifdef OSCDEBUG
            cout << wavesm.waveslist[i].waveID << " ";
#endif
        }
        cheapComm::getInstance()->sendSync1("/sync/electromagnetism/number_waves",wavesm.howManyWaves());
    }
    
}

void electromagnetica::draw(){
        //
    
    ofBackground(0);
    
    ofEnableAlphaBlending();
    ofEnableBlendMode(OF_BLENDMODE_SCREEN);
    ofPushMatrix(); //colocamos el canvas en su posicion centrada
    //post.begin();
        ofTranslate((ofGetWidth()-W_WIDTH)/2, 0);
       // drawNoise();
    
        ofNoFill();
        //ofEllipse(W_WIDTH/2, W_WIDTH/2, W_WIDTH, W_WIDTH);
       //
        ofPushMatrix();

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
    //post.end();
    ofPopMatrix();
  

    ofDisableBlendMode();
    ofDisableAlphaBlending();
    borde.draw();
    if(bdrawDebug)    showDebug();
    
  /*  //wavesm.debugInfo();
    ofPushStyle();
	ofSetColor(255,255,255);
	ofDrawBitmapString("fr: " + ofToString(ofGetFrameRate()), 50, ofGetHeight()-90);
	ofPopStyle();
   
   
*/
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
    ofPushStyle();
	ofSetColor(255,255,255);
	ofDrawBitmapString("fr: " + ofToString(ofGetFrameRate()), 50, ofGetHeight()-150);
    
    ofSetColor(0, 255, 255);
    ofDrawBitmapString("Number keys toggle effects, mouse rotates scene", 150, ofGetHeight()-300);
    for (unsigned i = 0; i < post.size(); ++i)
    {
        if (post[i]->getEnabled()) ofSetColor(0, 255, 255);
        else ofSetColor(255, 0, 0);
        ostringstream oss;
        oss << i << ": " << post[i]->getName() << (post[i]->getEnabled()?" (on)":" (off)");
        ofDrawBitmapString(oss.str(), 150, ofGetHeight()-300 + 20 * (i + 2));
    }
    
	ofPopStyle();

}

void electromagnetica::keyPressed(int key){
    switch(key){
        case 'v':
			gui1->saveSettings("/config/gui/gui_EM.xml");
			break;
        case 'g':
            if(gui1->isEnabled() ){
	            gui1->disable();
                bdrawDebug=false;
            }
            else{
				gui1->enable();
                bdrawDebug=true;
            }
            
        case 'l':
            drawlines=!drawlines;
            
    }
    
    unsigned idx = key - '0';
    if (idx < post.size()) post[idx]->setEnabled(!post[idx]->getEnabled());
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
    
    cout<< "particulas mesa EM:" <<numParticles <<endl;
    resetExtraParticles();

}

void electromagnetica::resetExtraParticles(int x0, int y0){
    meshAux.clear();
    particlesAux.clear();
    for ( int y =0 ; y < W_WIDTH ; y+=4 ){
        for ( int x = -1 ; x < W_WIDTH ; x+=W_WIDTH ){
                Particle mparticle=Particle(ofVec3f(x,y,0) ,ofColor(0,160,176,0) ,x,y);
                particlesAux.push_back(mparticle);
                meshAux.addVertex(mparticle.position );
                meshAux.addColor(mparticle.color);

        }
    }
    
    for ( int x =0 ; x < W_WIDTH ; x+=4 ){
        for ( int y = -1 ; y < W_WIDTH ; y+=W_WIDTH ){
            Particle mparticle=Particle(ofVec3f(x,y,0) ,ofColor(0,160,176,0) ,x,y);
            particlesAux.push_back(mparticle);
            meshAux.addVertex(mparticle.position );
            meshAux.addColor(mparticle.color);
            
        }
    }
}



void electromagnetica::resetParticles(){
    particles.clear();
    setupParticles();
}

void electromagnetica::reset(){
   // resetParticles();
    wavesm.reset();
    hands.reset();
    ofLog() << "reset waves manager"<<endl;
}
//scene notifications
void electromagnetica::sceneWillAppear( ofxScene * fromScreen ){  // reset our scene when we appear
    gui1->disable();
    reset();
    cheapComm::getInstance()->sendAudio0("/audio/electromagnetism/start_event");
    cheapComm::getInstance()->sendSync0("/sync/electromagnetism/start_event");
};


//scene notifications
void electromagnetica::sceneWillDisappear( ofxScene * toScreen ){
    gui1->disable();
    cheapComm::getInstance()->sendAudio0("/audio/electromagnetism/end_event");
    cheapComm::getInstance()->sendSync0("/sync/electromagnetism/end_event");
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

    cheapComm::getInstance()->sendAudio0("/audio/electromagnetism/hand_on_event");
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
    cheapComm::getInstance()->sendAudio0("/audio/electromagnetism/hand_off_event");
    
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
