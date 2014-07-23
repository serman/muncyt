#include "testApp.h"
#include <math.h>

#include "cvFunctions.h"

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
	
	colorImg.allocate(IRCAMERAWIDTH, IRCAMERAHEIGHT);
	//grayImage.allocate(IRCAMERAWIDTH, IRCAMERAHEIGHT);
	grayThreshNear.allocate(IRCAMERAWIDTH, IRCAMERAHEIGHT);
	grayThreshFar.allocate(IRCAMERAWIDTH, IRCAMERAHEIGHT);
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
#ifndef USEFILE
    oniSettings.useOniFile=false;
#else
    oniSettings.useOniFile = true;
	oniSettings.oniFilePath = "/Users/sergiogalan/MultimediaProgramming/of_v0.8.0_osx_release/apps/muncyt/kinectExample/bin/data/2014-06-26-14-33-34-445_depth_color_640w_480h_30fps.oni";

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
#endif
	isReady = oniCamGrabber.setup(oniSettings);

#ifndef USEFILE
	oniCamrecorder.setup(&oniCamGrabber);
 #endif
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
	alphaNoise=255;
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
	setupStatus();
	
	setupParticles();
    cargaColores();
	bFill = false;
    bSoloEnContorno = true;
	bDrawOld = false;

	setupGUI();
	setupShader();
    

    myOSCrcv.setup();
    
    
    
}

void testApp::setupStatus(){
     appStatuses["escena"]=EM;
     appStatuses["em_ruido"]=true;
     appStatuses["alpha_ruido"]=255;
    
   //  appStatuses["escena"]=escenas.EM;
}

void testApp::cambioEscena(){
    if( appStatuses["escena"]==EM){
        boolDrawNoise=true;
        bDrawLinesH=false;
        bDrawLinesV=false;
        bDrawNativePointCloud=false;
        bDrawContours=false;
        bDrawPoints=true;
    }else{
        bDrawContours=true;
        appStatuses["em_ruido"]=true;
        boolDrawNoise=false;
        bDrawNativePointCloud=false;
        bDrawPoints=false;
    }
}


//--------------------------------------------------------------
void testApp::update() {
    
    if (appStatuses["em_ruido"]==false && appStatuses["escena"]==EM){
        appStatuses["alpha_ruido"]-=5;
        if(appStatuses["alpha_ruido"]<0) appStatuses["alpha_ruido"]=0;
        particleMode=ESPEJO;
       // cout << appStatuses["alpha_ruido"] <<endl;
        
    }
    if(appStatuses["em_ruido"]==true){
        appStatuses["alpha_ruido"]=255;
    }
	
	#ifndef ASUS
	kinect.update();
	// there is a new frame and we are connected
	if(kinect.isFrameNew()) {
        if(bDrawPoints) {
	        updateParticles();
		}
	}
    #else
        if (isReady)
        {
            oniCamGrabber.update();
            grayImage.setFromPixels( oniCamGrabber.getDepthPixels() );
            grayImage.updateTexture();
            
// IMAGEN DE PRODUNFIDAD

            //oniCamGrabber.
            if(depthGenerator.isUpdated==true){
	            updateParticles();
                if(bDrawContours){
                    depthImg.setFromPixels( oniCamGrabber.getDepthPixels());
                    depthImg.update();
                    
                    vector<ofPolyline> v= getSmoothSilhouete1(contourFinder, depthImg, (float)speed/10);
                    if(v.size()>0)
                        resampledContour =v[0];
                }
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
        if(false){
           /* int px(p->_x*(1920/640));
            int py(p->_y*(1080/480));
            ofVec3f dst= ofVec3f(px,py,-1);
//            dst=camera.screenToWorld((dst));
            dst=dst * camera.getModelViewMatrix();

            */

 //           p->steer(dst, true, speed, stopUmbral );
//            p->update();
        //    p->position=dst;

            
            if(ofRandomf()>0.5)
                p->color.set(0);
            else
                p->color.set(255);
            
            
                p->color=ofColor(255,0,0,255);
                meshParticles.addVertex(p->position);
                meshParticles.addColor(p->color);
            
        }else{
    /**#ifndef ASUS
            if( kinect.getDistanceAt(p->_x, p->_y) > zMin  &&  kinect.getDistanceAt(p->_x, p->_y) < zMax ) {if(particleMode==ESPEJO){ p->stopAcc();      	mdestPoint= kinect.getWorldCoordinateAt(p->_x, p->_y);
    #else**/
            int distance=depthGenerator.currentRawPixels->getPixels()[p->_y * depthGenerator.width + p->_x];
                      //cout << zMax;
            if(distance> zMin && distance < zMax) {
                pp++;
                p->recentlyUsed=5;
                if(particleMode==ESPEJO){
                   mdestPoint=  oniCamGrabber.convertDepthToWorld(p->_x, p->_y);
    //#endif
                    //diff=ofVec3f(0,0,0);
                    p->steer(mdestPoint, true, speed, stopUmbral );
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
            }else{ // si las particulas est‡n mas lejos
               // meshParticles.addVertex(p->position);
                if(!p->recentlyUsed>0){
                    p->color=ofColor(255,0,0,40);
                }else{
                    p->color=ofColor(255,0,0,255);
                    meshParticles.addVertex(p->position);
                    meshParticles.addColor(p->color);
                    p->recentlyUsed--;
                }
            }//else modo ruido NO

        }
    }//end for all points within vector

}

// - - - - - - - - - - - - - - - - - - -- -- -- -- -- -- -- -- -- -- -- -- -- --- --- --- --- --- ---

void testApp::draw() {

    fadeBG();
	ofBackground(colorfondo);
//	ofBackground(10,10,10,100);

	
    
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

	    if(bDrawPoints) drawParticles();
		if(bDrawLinesH) drawLinesH();
		if(bDrawLinesV) drawLinesV();
    	if(bDrawNativePointCloud) drawPointCloud();
		 //ofEllipse(camera.getCursorProjected().x,camera.getCursorProjected().y,camera.getCursorProjected().z,100,100);

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
//#else
//    if(bDrawContours) drawCountours();
#endif
    
 /** DIBUJO SILUETA **/
    if(boolDrawNoise)  	drawNoise();
    if(bDrawContours){
        drawCVSilhouettes();
    }

    if(debug) showDebug();
	
}

void testApp::setupShader(){
	shader.load("", "shaders/whitenoise.frag");

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
    
void testApp::cargaColores() {
    // paleta de colores fluor
    // http://www.colourlovers.com/palette/970972/FLUOR
    //
    colorFluor.clear();
    //	colorFluor.push_back(ofColor::red);	// rojo
    //	colorFluor.push_back(ofColor::green);	// green
    //	colorFluor.push_back(ofColor::blue);	// blue
    colorFluor.push_back(ofColor::fromHex(0x0DE0FC));	// melting flowers
    colorFluor.push_back(ofColor::fromHex(0x38FC48));	// Dead Nuclear
    colorFluor.push_back(ofColor::fromHex(0xF938FC));	// Calcinha
    colorFluor.push_back(ofColor::fromHex(0xFA00AB));	// ow!
    colorFluor.push_back(ofColor::fromHex(0xDFFC00));	// Limei Green
    
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

