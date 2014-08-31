#include "testApp.h"
#include <math.h>

#include "cvFunctions.h"

//--------------------------------------------------------------
void testApp::setup() {
    //particleCloud(oniCamGrabber,depthGenerator);
	ofSetLogLevel(OF_LOG_VERBOSE);
	
    ofSetFullscreen(true);

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
	
	ofSetFrameRate(60);
	
	colorfondo=ofColor::black;
	
	// start from the front
	bDrawPointCloud = true;

	// init variables
	explosion = false;
    
	particleCloud.particleMode=ESPEJO;
    boolDrawNoise = false;
	alphaNoise=255;
	debug = true;
	
    low=0.25;
    zMin = 200;
    zMax = 1500;

    alphaLines = 255;
	stepLines = 5;
	
    incrDistance=0;
	
    particleCloud.setup(oniSettings.width, oniSettings.height, zMin, zMax, &oniCamGrabber, &depthGenerator);
    
	
	// setupCam
	// fijar posicion y orientacion (ofNode, Target, lookAt...)
#ifdef EASYCAM	
	easyCam.setAutoDistance(true);
#else
	camera.setCursorWorld(ofVec3f(0,0,-2000));
#endif
	setupStatus();	
    cargaColores();
	bFill = false;
    bSoloEnContorno = true;
	bDrawOld = false;

	setupGUI();
	setupShader();
    

    myOSCrcv.setup();
    
    sender.setup();
    
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
        particleCloud.particleMode=ESPEJO;
       // cout << appStatuses["alpha_ruido"] <<endl;
        
    }
    if(appStatuses["em_ruido"]==true){
        appStatuses["alpha_ruido"]=255;
    }
	

        if (isReady)
        {
            oniCamGrabber.update();
            grayImage.setFromPixels( oniCamGrabber.getDepthPixels() );
            grayImage.updateTexture();
            
// IMAGEN DE PRODUNFIDAD
            //oniCamGrabber.
            if(depthGenerator.isUpdated==true){
	            particleCloud.updateParticles();
                if(bDrawContours){
                    depthImg.setFromPixels( oniCamGrabber.getDepthPixels());
                    depthImg.update();                    
                    vector<ofPolyline> v= getSmoothSilhouete1(contourFinder, depthImg, (float)0.1/10);
                    if(v.size()>0){
                        resampledContour =v[0];
                        sender.send(v[0]);
                    }
                   // msgpack::pack(sbuf, v[0]);
                }
              	depthGenerator.isUpdated=false;
            }
            rgbGenerator.update();
            
        }

	
    
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
        particleCloud.explosionParticles();
        
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

	    if(bDrawPoints) particleCloud.drawParticles();
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


void testApp::showDebug(){
    ofDrawBitmapString("Framerate " + ofToString(ofGetFrameRate()), 20, 20);
    if(particleCloud.particleMode==NUBE)
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

            int distance=depthGenerator.currentRawPixels->getPixels()[y * depthGenerator.width +x];
            if(distance> zMin && distance < zMax) {
                ofVec3f vtmp=oniCamGrabber.convertDepthToWorld(x,y);

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
    

    int w= oniSettings.width;
    int h= oniSettings.height;
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

            int distance=depthGenerator.currentRawPixels->getPixels()[y * depthGenerator.width +x];
            if(distance> zMin && distance < zMax) {
	            ofVec3f vtmp=oniCamGrabber.convertDepthToWorld(x,y);


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

    oniCamGrabber.close();
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

