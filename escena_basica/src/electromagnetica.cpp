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
    

    
}

void electromagnetica::update(float d1){
    hands.update(); //actualiza la sombra de las manos
    //tuioClientEm.getMessage();
    
    wavesm.update(); //actualiza la onda  matematica

   /* if(hands.objectsCol.size()==1){
        //muevo particulas cerca de la mano: orbita
        // oculto el resto
        for(int i=0; i< meshParticles.getVertices().size(); i++){
            if( particles[i].position.distanceSquared(ofPoint(hands.objectsCol[0]->x,hands.objectsCol[0]->y)) >10000){
                meshParticles.getColors()[i].set(0);
                
            }else{ //color de ruido
                if(ofRandomf()>0.5)
                    meshParticles.getColors()[i].set(0);
                else
                    meshParticles.getColors()[i].set(255);
                
                ofPoint p= ofPoint(particles[i]._x,particles[i]._y,particles[i].position.z);
                particles[i].gravityTowards(p, 30, 10);
                particles[i].update();
                meshParticles.getVertices()[i].set(particles[i].position);
            }            
        }
        
    }else */if(wavesm.howManyWaves()==0){
     //ruido puro
        for(int i=0; i< meshParticles.getVertices().size(); i++){
            //ofSeedRandom();
            if(ofRandomf()>0.5)
                meshParticles.getColors()[i].set(0);
            else
                meshParticles.getColors()[i].set(255);
         
            ofPoint p= ofPoint(particles[i]._x,particles[i]._y,particles[i].position.z);
            particles[i].steer( p, true, 7, 10);
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
        
        for(int i=0; i< meshParticles.getVertices().size(); i++){
            int index=i % puntos_ondas;
            trIndices num_onda= wavesm.num_onda(index);
            //particles[i]._x;
            ofPoint p= wavesm.waveslist[num_onda.n_wave].AbsPoints[num_onda.new_index];
            particles[i].steer(p , true, 5, 10);
            particles[i].update();
            if(ofRandomf()>0.5)
                meshParticles.getColors()[i].set(0);
            else
                meshParticles.getColors()[i].set(255);
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
        ofNoFill();
        ofEllipse(W_WIDTH/2, W_WIDTH/2, W_WIDTH, W_WIDTH);
       // ofEnableAlphaBlending();
        
        ofSetColor(255);
        meshParticles.setMode(OF_PRIMITIVE_POINTS);
        glPointSize(2);
        glEnable(GL_POINT_SMOOTH);	// Para que sean puntos redondos
        ofEnableDepthTest();

        meshParticles.draw();

        ofDisableDepthTest();
        ofSetColor(255,0,0);
        ofFill();
        wavesm.draw();
        ofPushStyle();
            ofSetColor(0,0,0,0);
            hands.draw();
        ofPopStyle();
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
	gui1 = new ofxUICanvas(0,100,400,800);
    gui1->addSlider("alpha", 0.0, 1.0,&alpha);
    gui1->addToggle("noise or wave", &noiseMode);
 //   gui1->addSlider("freq", 0.0, 100.0,&(mwave.freq));
 //   gui1->addSlider("freq", 0.0, 100.0,&(mwave.freq2));
}
void electromagnetica::showDebug(){
    ofDrawBitmapString("Framerate " + ofToString(ofGetFrameRate()), 20, 100);

}

void electromagnetica::keyPressed(int key){
    switch(key){
        case 'v':
			gui1->saveSettings("/config/gui/gui_kinect.xml");
			break;
        case ' ':
            if(gui1->isEnabled() )
	            gui1->disable();
            else
				gui1->enable();
            
    }
}

void electromagnetica::drawNoise(){
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
			
			float ang1 = ofRandom(PI);
			float ang2 = ofRandom(TWO_PI);
			float rr = 100;
			
			float rrho = rr*sin(ang1);
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
}

void electromagnetica::tuioUpdated(ofxTuioCursor &tuioCursor){
    int mx = W_WIDTH*tuioCursor.getX();
    int my =    W_HEIGHT*tuioCursor.getY();

    hands.notifySlide(mx, my,tuioCursor.getSessionId(),tuioCursor.getMotionAccel());
    wavesm.slide(mx,  my,tuioCursor.getSessionId(),tuioCursor.getMotionAccel());
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

