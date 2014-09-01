#include "testApp.h"
#include <math.h>


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
//	bDrawPointCloud = true;

	// init variables
	//explosion = false;
    
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
	setupGUI();
	setupShader();
    
    mcontour.setup(&oniCamGrabber);
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
           }
    else{
            }
}


//--------------------------------------------------------------
void testApp::update() {
    
	

    if (isReady)
    {
        oniCamGrabber.update();
        if(depthGenerator.isUpdated==true){ // OJO ESTA ACTUALIZACION SOLO OCURRE CUANDO HAY IMAGEN NUEVA
            switch(appStatuses["escena"]){
                case EM:
                    particleCloud.updateParticles();
                break;
                
                case GRAVEDAD:
                        mcontour.update();
                        sender.send(mcontour.v[0]);
                break;
                      
                case NUCLEAR_DEBIL:
                        mcontour.update();
                        sender.send(mcontour.v[0]);
                break;
                    
                case NUCLEAR_FUERTE:

                break;
                    
            }
        }
            depthGenerator.isUpdated=false;
        }
        rgbGenerator.update();
    
    
    int r=myOSCrcv.update();
    parseOSC(r);

}


void testApp::draw() {
    fadeBG();
	ofBackground(colorfondo);
    
    ofEnableAlphaBlending();
	
#ifdef EASYCAM
	easyCam.begin();
#else
	camera.begin();
#endif

    
    //Things to be drawn in 3D
	ofPushMatrix();
        ofScale(1, 1, -1);
		// the projected points are 'upside down' and 'backwards'
        drawAxis();

		
		// Superponemos modos de dibujo en 3D
#ifdef TESTMODE
    
	    if(bDrawPoints) particleCloud.drawParticles();
		if(bDrawLinesH) drawLinesH();
		if(bDrawLinesV) drawLinesV();
    	if(bDrawNativePointCloud) drawPointCloud();
#else
        switch(appStatuses["escena"]){
            case EM:
                particleCloud.drawParticles();
            break;
                
            case GRAVEDAD:
                drawLinesH();
                drawLinesV();
            break;
                
            case NUCLEAR_DEBIL:

            break;
                
            case NUCLEAR_FUERTE:
                
                break;
        }
#endif
	ofPopMatrix();
    
   
#ifdef EASYCAM
	easyCam.end();
#else
	camera.end();
#endif
    
 /** DIBUJADO EN 2D  **/
    #ifdef TESTMODE
        if(boolDrawNoise)  	drawNoise();
        if(bDrawContours){
            mcontour.draw();
        }
        if(debug) showDebug();
    #else
        switch(appStatuses["escena"]){
            case EM:

                break;
                
            case GRAVEDAD:

                break;
                
            case NUCLEAR_DEBIL:
                mcontour.draw();
                break;
                
            case NUCLEAR_FUERTE:
                
                break;
        }
    #endif
	
}


void testApp::parseOSC(int response){
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




void testApp::setupShader(){
	shader.load("", "shaders/whitenoise.frag");
}


void testApp::showDebug(){
    ofPushMatrix();
    ofTranslate(ofGetWidth()-300,0);
    ofDrawBitmapString("Framerate " + ofToString(ofGetFrameRate()), 20, 20);
    if(particleCloud.particleMode==NUBE)
	    ofDrawBitmapString("MODO NUBE " , 20, 50);
    else
    	ofDrawBitmapString("MODO ESPEJO " , 20, 50);
	ofPopMatrix();
}

//--------------------------------------------------------------
void testApp::exit() {
//	kinect.setCameraTiltAngle(0); // zero the tilt on exit
    oniCamGrabber.close();
	//gui1->saveSettings("gui_kinect.xml");
	delete gui1;
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

