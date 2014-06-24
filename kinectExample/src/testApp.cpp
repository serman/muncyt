#include "testApp.h"
#include <math.h>

//--------------------------------------------------------------
void testApp::setup() {
	ofSetLogLevel(OF_LOG_VERBOSE);
	
    ofSetFullscreen(true);
    #ifndef ASUS
	// enable depth->video image calibration
	kinect.setRegistration(true);
    
	kinect.init();
	//kinect.init(true); // shows infrared instead of RGB video image
	//kinect.init(false, false); // disable video image (faster fps)
	
	kinect.open();		// opens first available kinect
	//kinect.open("A00362A08602047A");	// open a kinect using it's unique serial #
	
	// print the intrinsic IR sensor values
	if(kinect.isConnected()) {
		ofLogNotice() << "sensor-emitter dist: " << kinect.getSensorEmitterDistance() << "cm";
		ofLogNotice() << "sensor-camera dist:  " << kinect.getSensorCameraDistance() << "cm";
		ofLogNotice() << "zero plane pixel size: " << kinect.getZeroPlanePixelSize() << "mm";
		ofLogNotice() << "zero plane dist: " << kinect.getZeroPlaneDistance() << "mm";
	}
	
	colorImg.allocate(kinect.width, kinect.height);
	grayImage.allocate(kinect.width, kinect.height);
	grayThreshNear.allocate(kinect.width, kinect.height);
	grayThreshFar.allocate(kinect.width, kinect.height);
    // zero the tilt on startup
	angle = 0;
	kinect.setCameraTiltAngle(angle);
	
    #else
    oniSettings.width = IRCAMERAWIDTH;
	oniSettings.height = IRCAMERAHEIGHT;
	oniSettings.fps = 30;
	oniSettings.doDepth = true;
	oniSettings.doRawDepth = true;
	oniSettings.doColor = true;
	oniSettings.doIr = false;
    
    oniSettings.depthPixelFormat = PIXEL_FORMAT_DEPTH_1_MM;

	oniSettings.colorPixelFormat = PIXEL_FORMAT_RGB888;
	//oniSettings.irPixelFormat = PIXEL_FORMAT_GRAY16;
	oniSettings.doRegisterDepthToColor = true;
#ifdef USEFILE
	oniSettings.useOniFile = true;
	oniSettings.oniFilePath = "/Users/sergiogalan/MultimediaProgramming/of_v0.8.0_osx_release/apps/muncyt/kinectExample/bin/data/2014-06-24-18-17-54-693_depth_color_640w_480h_30fps.oni";
#else
    oniSettings.useOniFile=false;
#endif
    //will search this directory for an .oni file
	//if not found will use the first available camera
	ofDirectory currentONIDirectory(ofToDataPath("current", true));
    
	if (currentONIDirectory.exists())
	{
		currentONIDirectory.listDir();
		vector<ofFile> files = currentONIDirectory.getFiles();
		if (files.size()>0)
		{
			oniSettings.useOniFile = true;
			oniSettings.oniFilePath = files[0].path();
			ofLogVerbose() << "using oniFilePath : " << oniSettings.oniFilePath;
		}
	}
	
	isReady = oniCamGrabber.setup(oniSettings);
	oniCamrecorder.setup(&oniCamGrabber);
    
 //configuracion
    for (std::vector<VideoStream*>::iterator it = oniCamGrabber.streams.begin() ; it != oniCamGrabber.streams.end(); ++it){
        (*it)->setMirroringEnabled(true);
    	CameraSettings *camset=    (*it)->getCameraSettings();
        if((*it)->isPropertySupported(STREAM_PROPERTY_AUTO_WHITE_BALANCE))
        	camset->setAutoWhiteBalanceEnabled(true);
        if((*it)->isPropertySupported(STREAM_PROPERTY_AUTO_EXPOSURE))
        	camset->setAutoExposureEnabled(true);
        
    }
    depthGenerator.setup(oniCamGrabber.deviceController);
    	rgbGenerator.setup(oniCamGrabber.deviceController);
    cout << "width: " <<  rgbGenerator.width << " hei: " <<rgbGenerator.height;
    
    //depthGenerator.videoStream.setMirroringEnabled(true);
	ofLogVerbose() << "testApp started";
    
    #endif

	
	ofSetFrameRate(60);
	
	colorfondo=ofColor::black;
	
	// start from the front
	bDrawPointCloud = true;

	// init variables
	explosion = false;
	numParticles = 0;
    speed = 1.0;
    stopUmbral = 10000;
    alphaParticles = 255;
	particleMode=ESPEJO;
    boolDrawNoise = false;
	
	debug = true;
	
    low=0.25;
    zMin = 200;
    zMax = 1500;

    alphaLines = 255;
	stepLines = 5;
	
    incrDistance=0;
	
	
	// setupCam
	// fijar posicion y orientacion (ofNode, Target, lookAt...)
#ifdef EASYCAM	
	easyCam.setAutoDistance(true);
#else 
//	camera.setMouseActions(false);
//	camera.removeListeners();
	camera.setCursorWorld(ofVec3f(0,0,-2000));
	
#endif
	
	
	setupParticles();
    
	setupGUI();

	setupShader();
    
    
    myOSCrcv.setup();
}



void testApp::setupGUI() {
	gui1 = new ofxUICanvas(0,100,400,800);
	
	gui1->addSlider("speed", 0.0f, 200, &speed);
	gui1->addIntSlider("stopUmbral", 1, 300, &stopUmbral) ;
	gui1->addIntSlider("maxForce", 0, 20, &maxForce) ;
    gui1->addSlider("Acceleration", 1.05f, 3, &accTest);
	gui1->addSpacer();
	gui1->addButton("save",true);
	gui1->addButton("load",true);	
	gui1->addSpacer();
	
    vector<string> names;
	names.push_back("NUBE");
	names.push_back("ESPEJO");
	gui1->addRadio("MODO_Partics", names, OFX_UI_ORIENTATION_HORIZONTAL);
	gui1->addIntSlider("alpha Particles", 1, 255, &alphaParticles) ;
	gui1->addIntSlider("alpha Lines", 1, 255, &alphaLines) ;
	gui1->addIntSlider("Distance Lines", 50, 600, &distanciaLineasK) ;
	gui1->addToggle("noise", &boolDrawNoise);
	
	gui1->addSpacer();
	
	gui1->addIntSlider("stepPointCloud", 1,5, &stepCloudPoint);
	gui1->addIntSlider("stepLines", 2,10, &stepLines);
	
	gui1->addSpacer();
	
	gui1->addRangeSlider("RangoZ", 100, 4000, &zMin, &zMax);
	
	gui1->addSpacer();
	
	gui1->addToggle("DrawPoints", &bDrawPoints);
	gui1->addToggle("DrawLinesH", &bDrawLinesH);
	gui1->addToggle("DrawLinesV", &bDrawLinesV);
    gui1->addToggle("DrawNativePointCloud", &bDrawNativePointCloud);
	gui1->addToggle("Explosion", &explosion);
    gui1->addToggle("bDrawContours", &bDrawContours);
    gui1->addToggle("camera Colours", &bRealColors);
    
	gui1->addSpacer();
	
	gui1->addButton("reset",true);
	
	
	
	gui1->addSpacer();
	
	ofAddListener(gui1->newGUIEvent,this,&testApp::guiEvent);

}



//--------------------------------------------------------------
void testApp::update() {
	
	#ifndef ASUS
	kinect.update();
	
	// there is a new frame and we are connected
	if(kinect.isFrameNew()) {
		
		
        if(bDrawPoints) {
			// Si estamos utilizando particulas (dibujandolas), hacer update
	        updateParticles();

			/*
			if(ofGetElapsedTimeMillis() % 10000 > 2000){
				particleMode=ESPEJO;
			}
			else{
				particleMode=NUBE; //NUBE
			} 
			*/
		}
		
	}
    #else
        if (isReady)
        {
            
            oniCamGrabber.update();
            //oniCamGrabber.
            if(depthGenerator.isUpdated==true){
	            updateParticles();
            	depthGenerator.isUpdated=false;
            }
            rgbGenerator.update();
            
        }
    #endif
	
    
    int response=myOSCrcv.update();
    if(response==myOSCrcv.position){
        if(ofGetElapsedTimeMillis()-lastExplosionTime>4000){
			ofLog() << "myOSCrcv.remotePosition"  << myOSCrcv.remotePosition << "\n";
        }
    }
    else if(response==myOSCrcv.explosion){
        ofLog() << "explosin"<< endl;
        lastExplosionTime=ofGetElapsedTimeMillis();
        setRandomBG();
        explosionParticles();
        
    }

}

void testApp::setRandomBG(){
    colorfondo = ofColor::fromHsb(ofRandom(360),255,255);
}

void testApp::fadeBG() {
    float bb = colorfondo.getBrightness();
	if(bb<2) colorfondo.setBrightness(0);
    else colorfondo.setBrightness(bb*0.95);

}

void testApp::updateParticles() {
    meshParticles.clear();
    ofVec3f mdestPoint;
    ofVec3f diff ;
    int pp=0;
    std::vector<Particle>::iterator p ;
	
    for ( p = particles.begin() ; p != particles.end() ; p++ )
    {
		
#ifndef ASUS
        if( kinect.getDistanceAt(p->_x, p->_y) > zMin  &&  kinect.getDistanceAt(p->_x, p->_y) < zMax ) {

            if(particleMode==ESPEJO){
                p->stopAcc();
             	mdestPoint= kinect.getWorldCoordinateAt(p->_x, p->_y);//getWorldCoordinateAt() returns the position in millimeters with kinect at the origin.
                
#else	
        int distance=depthGenerator.currentRawPixels->getPixels()[p->_y * depthGenerator.width + p->_x];
                  //cout << zMax;
        if(distance> zMin && distance < zMax) {
            pp++;
            p->recentlyUsed=5;
            if(particleMode==ESPEJO){
               mdestPoint=  oniCamGrabber.convertDepthToWorld(p->_x, p->_y);
#endif
               // diff =   mdestPoint - p->position;
                diff=ofVec3f(0,0,0);
                p->steer(mdestPoint, true, speed, stopUmbral );
                // p->rotateAround( diff, speed, stopUmbral*100 );
                //p->gravityTowards(mdestPoint, stopUmbral, speed);
                p->update();

            }
            else if(particleMode==NUBE) {

                
                    p->sandDown(accTest,-200);
                    p->update();
                
               // p->position+=ofNoise( p->position.x,p->position.y,p->position.z)*30;
            }
			
            if (bRealColors){
                #ifndef ASUS
                p->color=kinect.getColorAt((p->_x , p->_y));
                #else
                p->color= rgbGenerator.currentPixels->getColor(p->_x , p->_y) ;
				#endif
            }
            else{
            	p->color=ofColor(255,255,255,alphaParticles);
            }
            meshParticles.addVertex(p->position);
           // meshParticles.addColor(ofColor::fromHsb(ofMap(p->_x, p->_y, zMin, zMax, 1, 360) , 255, 255, 50));
            meshParticles.addColor(p->color);

            
            


            //[p->_y * depthGenerator.width + p->_x]:
            
            
        }else{
           // meshParticles.addVertex(p->position);
            if(!p->recentlyUsed>0){
            	p->color=ofColor(255,0,0,40);
            }else{
                p->color=ofColor(255,0,0,255);
                meshParticles.addVertex(p->position);
                meshParticles.addColor(p->color);
                p->recentlyUsed--;
                /*
                mdestPoint= ofVec3f(0,0,-10);
                diff = mdestPoint- p->position;
                if(diff.lengthSquared()<(stopUmbral)){
                    p->stopAcc();
                    p->velocity = ofVec3f(0,0,0);
                    p->recentlyUsed=false;
                }else{
                diff.normalize();
//                diff *= 2;
               // p->addForce( diff );
               // p->update();
                p->color=ofColor(255,0,0,255);
                meshParticles.addVertex(p->position);
                meshParticles.addColor(p->color);
				}*/
            }
            //meshParticles.addColor(ofColor(255,0,0,40));
        }
    }//end for all points within vector
           // cout <<"particulas dentro: " << pp << "\n";
    //if((ofGetFrameNum()%1==0))
    //cout << " \n ----------------------------------------------- \n";

}

// - - - - - - - - - - - - - - - - - - -- -- -- -- -- -- -- -- -- -- -- -- -- --- --- --- --- --- ---

void testApp::draw() {
    
    fadeBG();
	ofBackground(colorfondo);
//	ofBackground(10,10,10,100);

	ofSetColor(255, 255, 255);
    if(boolDrawNoise)  	drawNoise();
    ofEnableAlphaBlending();
	
#ifdef EASYCAM
	easyCam.begin();
#else
	camera.begin();
#endif
	
	
	ofPushMatrix();	
		// the projected points are 'upside down' and 'backwards'
		ofScale(1, 1, -1);
	    ofSetColor (ofColor::blue);
	    ofLine(0, 0, 2000, 0);
        ofSetColor (ofColor::white);
        ofLine(0, 0, 0, 2000);
        ofSetColor (ofColor::yellow);
    	ofLine(0, 0, 0, 0, 0, 3000);

   // ofTranslate(0, 0, 1000); //
		
		// Superponemos modos de dibujo en 3D
    	if(bDrawContours) drawCountours();
	    if(bDrawPoints) drawParticles();
		if(bDrawLinesH) drawLinesH();
		if(bDrawLinesV) drawLinesV();
    	if(bDrawNativePointCloud) drawPointCloud();
		
//		if(pulso==true){
//			//drawPointCloud(stepCloudPoint);
//			drawLinesV(stepLines);
//			drawParticles();
//		}
//		else {
////                drawPointCloud();
//			drawLinesV(stepLines);
//		}
	ofPopMatrix();
	
	
#ifdef EASYCAM
	easyCam.end();
#else
	camera.end();
#endif	
    
#ifndef ASUS
	// Dibujar imagen Profundidad
	float ww = 300;
	float hh = (float)300/kinect.width * kinect.height;
	ofNoFill();
	ofSetColor(0,0,255);
	ofRect(ofGetWidth()-ww,ofGetHeight()-hh,ww,hh);
	ofSetColor(255,255,255);
	kinect.drawDepth(ofGetWidth()-ww,ofGetHeight()-hh,ww,hh);
#endif
    if(debug) showDebug();
	
	
}

void testApp::drawCountours(){    
    ofTexture& depth = oniCamGrabber.getDepthTextureReference();
	depth.draw(0,0);
    rgbGenerator.texture.draw(600,600);
}



void testApp::drawPointCloud(int step) {
    mesh.clear();

	int w = IRCAMERAWIDTH;
	int h = IRCAMERAHEIGHT;
    
    incrDistance+=1;
	mesh.setMode(OF_PRIMITIVE_POINTS);
	for(int y = 0; y < h; y += step) { //recorro los puntos bajando por las columnas
		for(int x = 0; x < w; x += step) { //recorro columnas
#ifndef ASUS
			if(kinect.getDistanceAt(x, y) > zMin
               && kinect.getDistanceAt(x, y) < zMax) {
                	ofVec2f p2= ofVec2f(x,y);

                	mesh.addColor(ofColor(255,255,255,ofMap(mouseX,1,ofGetWidth(),100,255 ) ) );
                    ofVec3f vtmp = kinect.getWorldCoordinateAt(x , y);
                    mesh.addVertex(vtmp);

#else
            int distance=depthGenerator.currentRawPixels->getPixels()[y * depthGenerator.width +x];
            if(distance> zMin && distance < zMax) {
                    ofVec2f p2   = ofVec2f(x,y);
                    ofVec3f vtmp=oniCamGrabber.convertDepthToWorld(x,y);
                mesh.addColor(ofColor(255,255,255,ofMap(mouseX,1,ofGetWidth(),100,255 ) ) );
                	mesh.addVertex(vtmp);
                
#endif
                //}

               // lineMesh1.addColor(ofColor::yellowGreen );
               // lineMesh1.addVertex(vtmp);
            }//if
            
        } //for interior
        // cout << p1.distance(vtmp) << "\n";
                //vtmp.x +=20*sin(vtmp.y/10.0*PI+ofGetElapsedTimeMillis())*sin(ofGetElapsedTimeMillis()/1000.0*TWO_PI);
	}
	glPointSize(2);
	glEnable(GL_POINT_SMOOTH);	// Para que sean puntos redondos
	ofEnableDepthTest();
	mesh.draw();	
	ofDisableDepthTest();
}





void testApp::setupShader(){
	shader.load("", "shaders/myCrazyFragFile2.frag");

}

void testApp::drawNoise(){
   // ofSetColor(255);
    
    /**ofTexture mtexture;
    mtexture.allocate(200, 200, GL_RGB);
    ofImage mimage;mimage.loadImage("Vendeta.jpg");
    mimage.getTextureReference().bind();**/
    
    shader.begin();
	
    shader.setUniform3f("iResolution", ofGetWidth(), ofGetHeight(),0);
    shader.setUniform1f("iGlobalTime", ofRandomf());
    shader.setUniform2f("iMouse",ofGetMouseX(),ofGetMouseY());

    //mtexture.loadScreenData(600,600,200,200);
    //mtexture.loadData(mimage.getPixelsRef());
///shader.setUniformTexture("iChannel0",mtexture,2 );
    
    ofRect(0, 0, ofGetWidth(), ofGetHeight());
    shader.end();
    //mimage.getTextureReference().unbind();
}


void testApp::setupParticles(){
   //particles.clear() ;
#ifndef ASUS
    int w= kinect.width;
    int h= kinect.height;
#else
    int w= oniSettings.width;
    int h= oniSettings.height;
#endif
	int    sampling=2;
    //Loop through all the rows

	numParticles = 0;
    //Loop through all the columns
	// Distribuir las particulas por la escena
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
			
			particles.push_back(Particle(ofVec3f(rrho*cos(ang2),rrho*sin(ang2),-rr*cos(ang1)-1500) ,ofColor(255,255,255) ,x,y));
			numParticles++ ;
        }
    }
    
    cout<<"particulas"<<numParticles;
}

void testApp::resetParticles(){
    particles.clear();
    setupParticles();
}

    
void testApp::explosionParticles(){
    ofVec3f centroEscena = ofVec3f(0,0,2500);
	for(int i=0; i<particles.size(); i++) {
        ofVec3f dir = particles[i].position - centroEscena;
        dir.normalize();
        dir*=1000;
        particles[i].applyForce(dir);
    }
}

 
    
void testApp::drawParticles(){
    meshParticles.setMode(OF_PRIMITIVE_POINTS);
    glPointSize(2);
	glEnable(GL_POINT_SMOOTH);	// Para que sean puntos redondos
	ofEnableDepthTest();
	meshParticles.draw();
	ofDisableDepthTest();
}

void testApp::showDebug(){
    ofDrawBitmapString("Framerate " + ofToString(ofGetFrameRate()), 20, 20);
    if(particleMode==NUBE)
	    ofDrawBitmapString("MODO NUBE " , 20, 50);
    else
    	ofDrawBitmapString("MODO ESPEJO " , 20, 50);
	
	// ofEasyCam
//    ofDrawBitmapString("EasyCam - distance: " + ofToString(easyCam.getDistance()), ofGetWidth()-200, 20);
//    ofDrawBitmapString("drag: " + ofToString(easyCam.getDrag()), ofGetWidth()-200, 35);
//	
//	ofNode nodeCam = easyCam.getTarget();
////    ofDrawBitmapString("EasyCam - posCam: " + ofToString(nodeCam.getX()) + "," + ofToString(nodeCam.getY()) + "," + ofToString(nodeCam.getZ()), ofGetWidth()-200, 50);
//    ofDrawBitmapString("posCam: " + ofToString(nodeCam.getPosition()), ofGetWidth()-200, 50);
//    ofDrawBitmapString("globalposCam: " + ofToString(nodeCam.getGlobalPosition()), ofGetWidth()-200, 65);
//    ofDrawBitmapString("spitch: " + ofToString(nodeCam.getPitch()), ofGetWidth()-200, 80);
////    ofDrawBitmapString("EasyCam - distance: " + ofToString(easyCam.getDistance()), ofGetWidth()-200, 20);
////    ofDrawBitmapString("EasyCam - distance: " + ofToString(easyCam.getDistance()), ofGetWidth()-200, 20);
}

void testApp::drawLinesH(float step){

	int w = IRCAMERAWIDTH;
	int h = IRCAMERAHEIGHT;

    incrDistance+=1;
    std::vector<ofPolyline> lineMesh;
    ofColor col ;
    float _time = ofGetElapsedTimef() ;
    float theta = sin ( ofGetElapsedTimef() ) ;
    ofVec2f v1=ofVec2f(IRCAMERAWIDTH,IRCAMERAWIDTH);
    ofPolyline lineMesh1;
//    ofPoint lastPoint ;
    ofVec3f lastPoint ;
    
	for(int y = 0; y < h; y += step) { //recorro los puntos bajando por las columnas
        ofPath line ;
		bool bLastValid = false;
		int _xStep = step;
		for(int x = 0; x < w; x += step) { //recorro columnas
#ifndef ASUS
			if(kinect.getDistanceAt(x, y) > zMin && kinect.getDistanceAt(x, y) < zMax) {
                ofVec3f vtmp = kinect.getWorldCoordinateAt(x , y);
#else
            int distance=depthGenerator.currentRawPixels->getPixels()[y * depthGenerator.width +x];
            if(distance> zMin && distance < zMax) {
                ofVec3f vtmp=oniCamGrabber.convertDepthToWorld(x,y);
#endif
                ofPoint _lastPoint = vtmp ;
//                float dist = abs(vtmp.z - lastPoint.z) ;
//                if (  dist < 30  )
				float dist2 = vtmp.squareDistance(lastPoint);
                if (  dist2 < distanciaLineasK  )
                {
                    if ( bLastValid == false )
                    {
                        line.moveTo( vtmp ) ;
                    }
                    else
                    {
                        line.lineTo( vtmp ) ;//addVertex( vertex ) ;
                    }
                    bLastValid = true ;
                    
                }
                else
                {
                    bLastValid = false ;
                }
                lastPoint = vtmp ;
            }
        } 
        ofColor c =  ofColor( 35 , 255 , 24 , alphaLines) ;
		line.setColor( c ) ;
        line.setFilled( false ) ;
        line.setStrokeColor( c ) ;
        line.setStrokeWidth( 1);
        line.draw();
	}
}

void testApp::drawLinesV(float step){
    
#ifndef ASUS
    int w= kinect.width;
    int h= kinect.height;
#else
    int w= oniSettings.width;
    int h= oniSettings.height;
#endif
    incrDistance+=1;
    std::vector<ofPolyline> lineMesh;
    ofColor col ;
    float _time = ofGetElapsedTimef() ;
    float theta = sin ( ofGetElapsedTimef() ) ;
    ofVec2f v1=ofVec2f(w,h);
    ofPolyline lineMesh1;
//    ofPoint lastPoint ;
    ofVec3f lastPoint ;
    
	for(int x = 0; x < w; x += step) { // recorro la horizontal
        ofPath line;
		bool bLastValid = false;
//		int _xStep = step;
		for(int y = 0; y < h; y += step) { // recorro columnas
#ifndef ASUS
			if(kinect.getDistanceAt(x, y) > zMin && kinect.getDistanceAt(x, y) < zMax) {
                ofVec3f vtmp = kinect.getWorldCoordinateAt(x , y);
#else
            int distance=depthGenerator.currentRawPixels->getPixels()[y * depthGenerator.width +x];
            if(distance> zMin && distance < zMax) {
	            ofVec3f vtmp=oniCamGrabber.convertDepthToWorld(x,y);
#endif
//                ofPoint _lastPoint = vtmp ;
//                float dist = abs(vtmp.z - lastPoint.z) ;
//                if (  dist < 30  )
				float dist2 = vtmp.squareDistance(lastPoint);
                if (  dist2 < distanciaLineasK  )
                {
                    if ( bLastValid == false )
                    {
                        line.moveTo( vtmp ) ;
                    }
                    else
                    {
                        line.lineTo( vtmp ) ;//addVertex( vertex ) ;
                    }
                    bLastValid = true ;                    
                }
                else
                {
                    bLastValid = false ;
                }
                lastPoint = vtmp;
            }
        } 
		
        ofColor c =  ofColor( 35 , 255 , 24 , alphaLines) ;
		line.setColor( c ) ;
        line.setFilled( false ) ;
        line.setStrokeColor( c ) ;
        line.setStrokeWidth( 1);
        line.draw();
	}
}

//--------------------------------------------------------------
void testApp::exit() {
//	kinect.setCameraTiltAngle(0); // zero the tilt on exit
#ifndef ASUS
	kinect.close();
#else
    oniCamGrabber.close();
#endif
	//gui1->saveSettings("gui_kinect.xml");
    
	delete gui1;
	
	
}

//--------------------------------------------------------------
void testApp::keyPressed (int key) {
	switch (key) {
#ifndef ASUS
		case 'o':
			kinect.setCameraTiltAngle(angle); // go back to prev tilt
			kinect.open();
			break;
			
		case 'c':
			kinect.setCameraTiltAngle(0); // zero the tilt
			kinect.close();
			break;
			
		case '0':
			kinect.setLed(ofxKinect::LED_OFF);
			break;
#endif
		case 'm':
			bDrawPoints=!bDrawPoints;
			break;
		case 'n':
			explosion=!explosion;
			break;
        case 'g':
            debug=!debug;
			gui1->toggleVisible();
			break;
        case 'p':
            if(particleMode==NUBE)
                particleMode=ESPEJO;
            else particleMode=NUBE;
            break;
#ifndef ASUS            
		case OF_KEY_UP:
			angle++;
			if(angle>30) angle=30;
			kinect.setCameraTiltAngle(angle);
			break;
			
		case OF_KEY_DOWN:
			angle--;
			if(angle<-30) angle=-30;

			kinect.setCameraTiltAngle(angle);
            break;
#else 	
        case 'r':
            if(oniCamrecorder.isRecording==true)
                oniCamrecorder.stopRecording();
            else
                oniCamrecorder.startRecording();
            break;
            

#endif
			
            
        case 'e':
            zMax+=10;
            break;
        case 'd':
            zMax-=10;
            break;
        case 's':
			grabarScreen();
			break;
		case 'v':
			gui1->saveSettings("/config/gui/gui_kinect.xml");
			break;
        case ' ':
            if(gui1->isEnabled() )
	            gui1->disable();
            else
				gui1->enable();
			
	}
	
#ifndef EASYCAM
if(key=='1')	saveCameraPose();
else if(key=='2') 	loadCameraPose();
#endif
	
}


void testApp::grabarScreen() {
	Img.grabScreen( 0, 0, ofGetWidth(), ofGetHeight() );
	string nameImg = "./images/screens/pantalla_";
	nameImg+=ofToString(ofGetYear())+
			ofToString(ofGetYear(),4)+
			ofToString(ofGetMonth(),2,'0')+
			ofToString(ofGetDay(),2,'0')+
			ofToString(ofGetHours(),2,'0')+
			ofToString(ofGetMinutes(),2,'0')+
			ofToString(ofGetSeconds(),2,'0')+
			".png";
	cout << nameImg;
	Img.saveImage( nameImg );
}


//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button)
{}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button)
{}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button)
{}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h)
{}

void testApp::guiEvent(ofxUIEventArgs &e)
{
	string name = e.widget->getName();
	int kind = e.widget->getKind();
    
    if(name == "reset")
    {
		ofLogNotice("reset");
        resetParticles();
    }
	else if(name == "save")
    {
        cout << "save";
		gui1->saveSettings("./config/gui/gui_kinect.xml");
#ifndef EASYCAM
		saveCameraPose();
#endif
    }
	else if(name == "load")
    {
        cout << "load";
		gui1->loadSettings("./config/gui/gui_kinect.xml");
#ifndef EASYCAM
		loadCameraPose();
#endif
		
    }
	else if(name == "MODO_Partics")
    {
		ofxUIRadio *  wr = (ofxUIRadio *) e.widget;
		ofLogNotice("MODO_Partics. " + wr ->getActiveName() + " = " + ofToString(wr->getValue()));
		particleMode = wr->getValue();
//		gui1->loadSettings("./config/gui/gui_kinect.xml");
    }
}

// CameraPose
#ifndef EASYCAM
void testApp::saveCameraPose() {
	// grabar
	ofFile fileWrite("./config/cameraPose.txt",ofFile::WriteOnly);
	ofMatrix4x4 savedPose = camera.getGlobalTransformMatrix();
	fileWrite << savedPose;		
}

void testApp::loadCameraPose() {
	// leer
	ofFile fileRead("./config/cameraPose.txt");
	ofMatrix4x4 myMatrix;
	fileRead >> myMatrix;
	camera.setTransformMatrix(myMatrix);
}	
#endif