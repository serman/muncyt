//
//  electromagnetica.cpp
//  escena_basica
//
//  Created by Sergio Galan on 7/11/14.
//
//

#include "electromagnetica.h"


void electromagnetica::setup(){
    alpha=1.0;
    setupShader();
    setupGUI();
    mwave.setup();
}

void electromagnetica::update(float d1){
    
    for(int i=0; i< meshParticles.getVertices().size(); i++){
        //ofSeedRandom();
        if(ofRandomf()>0.5)
            meshParticles.getColors()[i].set(0);
        else
            meshParticles.getColors()[i].set(255);
//        meshParticles.getColors()[i].set(255);
        if( particles[i].position.distance(ofPoint(X_(ofGetMouseX()),ofGetMouseY())) >100){
            meshParticles.getColors()[i].set(0);
        }

    }
    mwave.update();
    
}

void electromagnetica::draw(){
    ofBackground(0);
    ofPushMatrix(); //colocamos el canvas en su posicion centrada
    ofTranslate((ofGetWidth()-W_WIDTH)/2, 0);
   // ofEnableAlphaBlending();
    
    ofSetColor(255);
    meshParticles.setMode(OF_PRIMITIVE_POINTS);
    glPointSize(2);
   	glEnable(GL_POINT_SMOOTH);	// Para que sean puntos redondos
	ofEnableDepthTest();
    meshParticles.draw();
    ofDisableDepthTest();
    //drawNoise();
   //     ofDisableAlphaBlending();
    ofSetColor(255,0,0);
    ofFill();
    mwave.draw();
    ofPopMatrix();

    showDebug();
}

void electromagnetica::mousePressed( int x, int y, int button ){}

void electromagnetica::setupShader(){
	shader.load("", "shaders/whitenoise.frag");
    setupParticles();
    
}

void electromagnetica::setupGUI() {
	gui1 = new ofxUICanvas(0,100,400,800);
    gui1->addSlider("alpha", 0.0, 1.0,&alpha);
		//gui1->addToggle("noise", &boolDrawNoise);
}
void electromagnetica::showDebug(){
    ofDrawBitmapString("Framerate " + ofToString(ofGetFrameRate()), 20, 20);

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
	int    sampling=3;
    //Loop through all the rows
    
	numParticles = 0;
    //Loop through all the columns
	// Distribuir las particulas por la escena
    ofSeedRandom();
    for ( int y = 0 ; y < h ; y+=sampling ){
        for ( int x = 0 ; x < w ; x+=sampling ){
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
