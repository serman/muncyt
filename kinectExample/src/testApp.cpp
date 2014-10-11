#include "testApp.h"
#include <math.h>

int  SCREEN_ID=0;

//--------------------------------------------------------------
void testApp::setup() {
    //particleCloud(oniCamGrabber,depthGenerator);
	ofSetLogLevel(OF_LOG_VERBOSE);
	
    ofSetFullscreen(true);
    loadScreenId();
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
#ifdef TESTMODE
    boolDrawNoise = false;
    boolDrawNoise = false
#endif
    
	particleCloud.cloudState=particleClouds::ESPEJO;
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
	camera.setCursorWorld(ofVec3f(0,0,-2000));

#endif
    
	setupStatus();
	setupGUI();
	setupShader();
    
    mcontour.setup(&oniCamGrabber);
    myOSCrcv=new cheapCommRcv();
    myOSCrcv->setup();
    sender.setup();
    rcvCont.setup();
    mrayoSil.setup();
    mgrid.setup(oniSettings.width, oniSettings.height, &zMin, &zMax, &oniCamGrabber, &depthGenerator);
    particleCloud.setup(oniSettings.width, oniSettings.height, &zMin, &zMax, &oniCamGrabber, &depthGenerator,&camera);
    mdela.setup(oniSettings.width, oniSettings.height, &zMin, &zMax, &oniCamGrabber, &depthGenerator,&camera);
    
    //gui1->loadSettings("./config/gui/gui_kinect.xml");
    guiTabBar->loadSettings("./config/gui/","espejo_");
#ifndef EASYCAM
    loadCameraPose();
#endif
    
    
    //http://stackoverflow.com/questions/12018710/calculate-near-far-plane-vertices-using-three-frustum
 
    light.setup();
   
    light.setAmbientColor(ofColor(230, 230, 250));
    light.setSpotlight();
    light.setPosition(0, 100, 0);
    // light.enable();
    ofDisableLighting();
    post2D.init(ofGetWidth()/3, ofGetHeight()/3);
    post2D.createPass<ZoomBlurPass>()->setEnabled(false);
    post2D.createPass<BloomPass>()->setEnabled(false);
    post2D.setFlip(false);
    
    
    post.init(ofGetWidth(), ofGetHeight());
    post.createPass<ZoomBlurPass>()->setEnabled(false);
    post.createPass<BloomPass>()->setEnabled(false);
    post.createPass<VerticalTiltShifPass>()->setEnabled(false);
    post.createPass<ContrastPass>()->setEnabled(false);
    post.createPass<BleachBypassPass>()->setEnabled(false);
    post.createPass<GodRaysPass>()->setEnabled(false);
    post.createPass<LimbDarkeningPass>()->setEnabled(false);
    post.createPass<ConvolutionPass>()->setEnabled(false);
    post.setFlip(false);
    mtunnel.setup();
    mmenu.setup();
    
  //  pfbo.allocate(ofGetWidth(),ofGetHeight(),GL_RGBA);
    ofFbo::Settings s;
    s.width = ofNextPow2(ofGetWidth());
    s.height = ofNextPow2(ofGetHeight());
    s.textureTarget = GL_TEXTURE_2D;
    s.useDepth = true;
    s.depthStencilInternalFormat = GL_DEPTH_COMPONENT24;
    s.depthStencilAsTexture = true;
    pfbo.allocate(s);
    pfbo.begin();
    ofBackground( 0, 0, 0 );
    pfbo.end();
    

    

    
}

void testApp::setupStatus(){
     appStatuses["escena"]=NUCLEAR_DEBIL;
     appStatuses["em_ruido"]=true;
     appStatuses["alpha_ruido"]=255;
    
   //  appStatuses["escena"]=escenas.EM;
}




//--------------------------------------------------------------
void testApp::update() {
    if (isReady)
    {
        oniCamGrabber.update();
/// ACTUALIZACION CONTINUA
        switch(appStatuses["escena"]){
            case EM:
                particleCloud.updateParticles();
                
                break;
                
            case GRAVEDAD:
                mgrid.update();
                break;
                
            case NUCLEAR_DEBIL:
                        mcontour.update();
                        mrayoSil.update(mcontour);
                        mrayoSil.setSilueta(mcontour.v[0]);
                break;
                
            case NUCLEAR_FUERTE:
                mdela.update();
                break;
                
        }
//ACTUALIZACION SÓLO CUANDO HAY IMAGEN NUEVA
        
        if(depthGenerator.isUpdated==true){ // OJO ESTA ACTUALIZACION SOLO OCURRE CUANDO HAY IMAGEN NUEVA
            switch(appStatuses["escena"]){
                case EM:
                   // particleCloud.updateParticles();
                break;
                
                case GRAVEDAD:
                    
                    if(mgrid.status==mgrid.BLACKHOLE){
                        mcontour.update();
                        sender.send(mcontour.v[0]);
                        if(rcvCont.isThreadRunning()==false){
                            cout << "receiver start" <<endl;
                            rcvCont.start();
                        }
                    }else{
                        if(rcvCont.isThreadRunning()==true){
                            rcvCont.stop();
                            cout << "receiver stop" <<endl;
                        }
                    }
                break;
                      
                case NUCLEAR_DEBIL:
                       // sender.send(mcontour.v[0]);
                break;
                    
                case NUCLEAR_FUERTE:
                       mcontour.update();
                break;
                    
            }
        }
            depthGenerator.isUpdated=false;
        }
        rgbGenerator.update();
    
    
    myOSCrcv->update();
   // parseOSC(r);

}


void testApp::draw() {
    //fadeBG();
	ofBackground(colorfondo);
    ofEnableAlphaBlending();
    camera.begin();
    camera.end();
    pfbo.begin(false);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    ofSetColor(0,0,0,255);
    ofRect(0,0,ofGetWidth(),ofGetHeight());
    
    glLoadMatrixf(camera.getProjectionMatrix(ofRectangle(0, 0, ofGetWidth(), ofGetHeight())).getPtr());
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadMatrixf(camera.getModelViewMatrix().getPtr());
    
    glViewport(0, 0, pfbo.getWidth(), pfbo.getHeight());
    
    // glClear(GL_DEPTH_BUFFER_BIT );
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    ofPushStyle();
    glPushAttrib(GL_ENABLE_BIT);

    //ofSetColor(255,255);
    
    //Things to be drawn in 3D
	ofPushMatrix();
        ofScale(-1, 1, -1);
		// the projected points are 'upside down' and 'backwards'
        //drawAxis();
		// Superponemos modos de dibujo en 3D
#ifdef TESTMODE
    
	    if(bDrawPoints) particleCloud.drawParticles();
		if(bDrawLinesH) drawLinesH();
		if(bDrawLinesV) drawLinesV();
    	if(bDrawNativePointCloud) drawPointCloud();
#else
  //  post.begin();
    light.draw();
        switch(appStatuses["escena"]){
                
            case EM:
                //particleCloud.drawWithRectangles();
                post[0]->setEnabled(false);
                post[1]->setEnabled(false);
                if(post[2]->getEnabled()==false) post[1]->setEnabled(true);
                    particleCloud.drawParticles();

            break;
                
            case GRAVEDAD:
                
                if(mgrid.status==mgrid.GRID){
                    //glow Shader
                    post[0]->setEnabled(false);
                    post[2]->setEnabled(false);
                    if(post[1]->getEnabled()==false) post[1]->setEnabled(true);
                    mgrid.draw(&camera);
                }
                else if(mgrid.status==mgrid.BLACKHOLE){
                    mtunnel.draw();
                }
            break;
                
            case NUCLEAR_DEBIL:
                    //el dibujado aquí es en 2D asi que no viene nada
            break;
                
            case NUCLEAR_FUERTE:
                //apago los shdaers que no tocan Dejo el glow
                post[0]->setEnabled(false);
                post[2]->setEnabled(false);
                if(post[1]->getEnabled()==false) post[1]->setEnabled(true);
                mdela.draw();
                break;
        }


#endif
	ofPopMatrix();

	//camera.end();
//    post.end();
    //pfbo.end();
    glPopAttrib();
    ofPopStyle();
    glViewport(0, 0, ofGetWidth(), ofGetHeight());
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    pfbo.end();/*
                
    ofPushStyle();
    glPushAttrib(GL_ENABLE_BIT);
    glDisable(GL_LIGHTING);
    ofSetColor(255, 255, 255);
    post.process(pfbo,true);
    */
    post.begin();
    ofSetColor(255, 255, 255);
    pfbo.draw(0,0,ofGetWidth(),ofGetHeight());
    post.end();


    
 /** DIBUJADO EN 2D  **/
    #ifdef TESTMODE
        if(boolDrawNoise)  	drawNoise();
        if(bDrawContours){
            mcontour.draw();
        }
    
    #else
        switch(appStatuses["escena"]){
            case EM:

                break;
                
            case MENU:
                //post[1]->setEnabled(true);
                pfbo.begin();
                    //glMatrixMode(GL_PROJECTION);
                    //glPushMatrix();
                    ofEnableAlphaBlending();
                    ofSetColor(0,0,0,2);
                    ofRect(0,0,ofGetWidth(),ofGetHeight());
                    mmenu.draw();
                pfbo.end();
                post.begin();
                    ofSetColor(255, 255, 255);
                    pfbo.draw(0,0);
                post.end();
                //post[1]->setEnabled(false);
                break;
                
            case GRAVEDAD:
                if(mgrid.status==mgrid.BLACKHOLE){
                    ofPolyline v=rcvCont.getRemoteContour();
                    if( v.size()>0 && v.size()>0){
                        mcontour.draw(&(v));
                    }
                }
                else{
                    mgrid.draw2D();
                }
            break;
                
            {case NUCLEAR_DEBIL:
                int ww=ofGetWidth();
                int hh=ofGetHeight();
                if(post2D[1]->getEnabled()==false) post2D[1]->setEnabled(true);
                if(mcontour.bFill==true){
                    if(post2D[0]->getEnabled()==false) post2D[0]->setEnabled(true);
                }else{
                    post2D[0]->setEnabled(false);
                }
                post2D.begin();
                glPushMatrix();
                 glViewport(0, 0, post2D.getRawRef().getWidth()*3,  post2D.getRawRef().getHeight()*3);
                //Deshago el cambio de viewport
             
                /*Post processing shader is set to 1/3 of the screen.    When you start it it "reset" the screen to its own size so the following code will think that its size is the real size.  In this case I need to set the real size of the screen again. By working with 1/3 resolution, Everything is faster and actually the kinect real resolution is around 1/3 of the screen. So together with the cool shaders the scalation doesn't matter at the end */
                mcontour.draw(&mrayoSil.camino);
                mrayoSil.draw();
                glPopMatrix();
                post2D.end(false);
                ofSetColor(255);
                post2D.draw(0, 0, ofGetWidth(), ofGetHeight());
                //post.getProcessedTextureReference().draw(0, 0, ofGetWidth(), ofGetHeight());
            break;
            }
            {case NUCLEAR_FUERTE:

                break;}
        }
    #endif
	if(debug) showDebug();
}


void testApp::parseOSC(int response){
  /*  if(response==myOSCrcv.position){
        if(ofGetElapsedTimeMillis()-lastExplosionTime>4000){
            ofLog() << "myOSCrcv.remotePosition"  << myOSCrcv.remotePosition << "\n";
        }
    }
    else if(response==myOSCrcv.explosion){
        ofLog() << "explosin"<< endl;
        lastExplosionTime=ofGetElapsedTimeMillis();
        setRandomBG();
        particleCloud.explosionParticles();
        
    }*/
}




void testApp::setupShader(){
	shader.load("", "shaders/whitenoise.frag");
}


void testApp::showDebug(){
    ofPushMatrix();
    ofTranslate(ofGetWidth()-300,0);
    ofSetColor(255, 0, 0);
    ofDrawBitmapString("Framerate " + ofToString(ofGetFrameRate()), 20, 20);

    for (unsigned i = 0; i < post.size(); ++i)
    {
        if (post[i]->getEnabled()) ofSetColor(0, 255, 255);
        else ofSetColor(255, 0, 0);
        ostringstream oss;
        oss << i << ": " << post[i]->getName() << (post[i]->getEnabled()?" (on)":" (off)");
        ofDrawBitmapString(oss.str(), 10, 20 * (i + 2));
    }
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

void testApp::loadScreenId(){
    ofFile fileRead("./config/id.txt");
    int id;
    fileRead >> id;
    SCREEN_ID=id;
    ofLogVerbose()<<"SCREEN ID:" << SCREEN_ID<<endl;
}

    
#endif

