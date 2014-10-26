#include "testApp.h"
#include <math.h>


int  SCREEN_ID=0;

//--------------------------------------------------------------
void testApp::setup() {
  //  cout << "ruta directorio:" << getenv("HOME");
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
	oniSettings.doRegisterDepthToColor = false;
#ifndef USEFILE
    oniSettings.useOniFile=false;
#else
    oniSettings.useOniFile = true;
	oniSettings.oniFilePath = "/Users/sergiogalan/MultimediaProgramming/of_v0.8.4_osx_release/apps/muncyt/kinectExample/bin/data/test_museo.oni";

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
            //oniCamGrabber.depthSource.setDepthClipping(300,6000);
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
    mPuntosFormas.setup(oniSettings.width, oniSettings.height, &zMin, &zMax, &oniCamGrabber, &depthGenerator,&camera);
    mvideoMask.setup();
    //gui1->loadSettings("./config/gui/gui_kinect.xml");
    guiTabBar->loadSettings("./config/gui/","espejo_");
    cameraMoves::getInstance()->setup(&camera);
    loadCameraPose();

    
    //http://stackoverflow.com/questions/12018710/calculate-near-far-plane-vertices-using-three-frustum
 
    light.setup();
   
    light.setAmbientColor(ofColor(230, 230, 250));
    light.setSpotlight();
    light.setPosition(0, 100, 0);
    // light.enable();
    ofDisableLighting();
   //Este es un shader a 1/3 de tamaño de pantalla que sólo uso en la silueta 2d de la nuclear debil
    post2D.init(ofGetWidth()/3, ofGetHeight()/3);
    post2D.createPass<ZoomBlurPass>()->setEnabled(false);
    post2D.createPass<BloomPass>()->setEnabled(false);
    post2D.setFlip(false);
    
    
    post.init(ofGetWidth(), ofGetHeight());
    post.createPass<ZoomBlurPass>()->setEnabled(false);
    post.createPass<highBloomPass>()->setEnabled(false);
    post.createPass<VerticalTiltShifPass>()->setEnabled(false);
    post.createPass<ConvolutionPass>()->setEnabled(false);
    post.createPass<ContrastPass>()->setEnabled(false);
    post.createPass<BleachBypassPass>()->setEnabled(false);
    post.createPass<GodRaysPass>()->setEnabled(false);
    post.createPass<LimbDarkeningPass>()->setEnabled(false);

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
     appStatuses["escena"]=EM;
     appStatuses["em_ruido"]=true;
     appStatuses["alpha_ruido"]=255;
}




//--------------------------------------------------------------
void testApp::update() {
    if (isReady)
    {
        oniCamGrabber.update();
        if( appStatuses["escena"]==TESTING) return;
/// ACTUALIZACION CONTINUA
        switch(appStatuses["escena"]){
            case EM:
                //El modo electromagnetico solo usa la nube de particulas
                particleCloud.updateParticles();
                break;
                
            case GRAVEDAD:
                //la gravedad usa en principio el grid
                mgrid.update();
                break;
                
            case NUCLEAR_DEBIL:
                //la nuclear debil utiliza la silueta en 2D
                        mcontour.update(); //procesa la silueta
                        if( mcontour.getMainSilhouette()!=NULL )
                            mrayoSil.setSilueta( *mcontour.getMainSilhouette() ); //Detecta colisiones y genera los rayos
                
                        mrayoSil.update(mcontour);
                break;
                
            case NUCLEAR_FUERTE:
                //la nuclear fuerte usa un delaunay en 3D muy colorido
                //mdela.update();
                break;
                
        }
//OJO: ACTUALIZACION SÓLO CUANDO HAY IMAGEN NUEVA
        if(depthGenerator.isUpdated==true){
            switch(appStatuses["escena"]){
                case EM:
                   // particleCloud.updateParticles();
                break;
                
                case GRAVEDAD:
                    if(mgrid.status==mgrid.BLACKHOLE){
                        mvideoMask.update();
                        mcontour.update();
                //envío la imagen local a donde toque.
                        if( mcontour.getMainSilhouette()!=NULL )
                            sender.send( *mcontour.getMainSilhouette());
                        //Si no está puesto activo el thread para leer el contorno remoto
                        if(rcvCont.isThreadRunning()==false){
                            cout << "receiver start" <<endl;
                            rcvCont.start();
                        }
                    }else{ //Si no estamos en modo "black hole" paramos el thread de leer contorno remoto
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
                    
                break;
                    
            }
        }
            depthGenerator.isUpdated=false;
        }
        rgbGenerator.update();
        myOSCrcv->update();
   // parseOSC(r);

}
void testApp::startFBO(){
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
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);  //cambiar esta linea por la de arriba para que no se borre el fbo en cada frame
    ofPushStyle();
    glPushAttrib(GL_ENABLE_BIT);
}

void testApp::draw() {
    if(appStatuses["escena"]==TESTING){
        ofDisableAlphaBlending();
        ofBackground(0,40,100);
        ofTexture& depth = oniCamGrabber.getDepthTextureReference();
        ofSetColor(0, 200, 0);
        depth.draw(0, 0,640,480);
        
        ofMesh m;
        for(int y = 0; y < depth.getHeight(); y++){
            for(int x = 0; x < depth.getWidth(); x++){
                m.addVertex(oniCamGrabber.convertDepthToWorld(x, y));
            }
        }
        m.drawVertices();
        return;
    }
    
    
    if(cameraMoves::getInstance()->shouldRestartCamera){
        loadCameraPose();
        cout << "load camera pose" <<endl;
        cameraMoves::getInstance()->shouldRestartCamera=false;
    }
    //fadeBG();

    ofSetColor(255);

    ofBackground(colorfondo);
    ofEnableAlphaBlending();
    //Estas lineas que vienen son una copia de lo que hace ofxpostprocessing
//    para dibujar en un fbo manteniendo la perspectiva de cámara. La idea de dibujar en el fbo es que podamos usar la táctica de no borrar el fondo para conseguir ciertas animaciones aunque no se está aplicando
    startFBO();
    //Things to be drawn in 3D
	ofPushMatrix();
    ofScale(-1, 1, -1);		// the projected points are 'upside down' and 'backwards'
  //  post.begin();
   // light.draw();
        switch(appStatuses["escena"]){
            case EM:
                //particleCloud.drawWithRectangles();
                setShaders(0);
                    particleCloud.drawParticles();

            break;
                
            case GRAVEDAD:
                if(mgrid.status==mgrid.GRID){
                    //solo glow Shader
                    setShaders(GLOW_ON);
                    mgrid.draw(&camera);
                }
                else if(mgrid.status==mgrid.BLACKHOLE){
                    setShaders(CONV_ON);
                    mtunnel.draw();
                }
            break;
                
            case NUCLEAR_DEBIL:
                    setShaders(0);
                    //el dibujado aquí es en 2D asi que no viene nada
            break;
                
            case NUCLEAR_FUERTE:
                //apago los shdaers que no tocan Dejo el glow
                setShaders(GLOW_ON);
                mPuntosFormas.drawWithRectangles();
                break;
        }
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
    pfbo.end(); //FIN DEL FBO
    /*
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
                setShaders(GLOW_ON);
                post.begin();
                    mmenu.draw();
                post.end();
                //post[1]->setEnabled(false);
                break;
          //EN EL CASO DE LA FUERZA GRAVITATORIA DIBUJO EN 2D LA LINEA DE "ESCANER" AL COMIENZO  Y TAMBIÉN LA LA SILETA REMOTA SI ESTAMOS EN MODO AGUJERO NEGRO
            case GRAVEDAD:
                if(mgrid.status==mgrid.BLACKHOLE){
                    /*if(post2D[1]->getEnabled()==false) post2D[1]->setEnabled(true);
                    if(post2D[0]->getEnabled()==true) post2D[0]->setEnabled(false);
                    post2D.begin();
                    glPushMatrix();
                    glViewport(0, 0, post2D.getRawRef().getWidth()*3,  post2D.getRawRef().getHeight()*3);
                    
                    
                     pfbo.draw(0,0,ofGetWidth()/3,ofGetHeight()/3);
                    ofPolyline v=rcvCont.getRemoteContour();
                    if( v.size()>0){
                        //Qué se dibuja.
                        //Aqui hay una niapa. Para que se vea el tunel lo vuelvo a dibujar detrás
                        //mcontour.drawSimple(&(v));
                    }
                    glPopMatrix();
                    post2D.end(false);
                    ofSetColor(255);
                    post2D.draw(0, 0, ofGetWidth(), ofGetHeight());*/
                    ofPolyline v=rcvCont.getRemoteContour();
                    if( v.size()>0){
                        ofPushMatrix();
                        ofScale(2, 2);
//                        mcontour.drawSimple(&(v));
                        mvideoMask.draw(v);
                        ofPopMatrix();
                    }
                }
                else{
                    mgrid.draw2D();
                }
            break;
                
            {case NUCLEAR_DEBIL:
                // En este caso en función del estado de la animación
                //se enciende o apaga un determinado shader.
                int ww=ofGetWidth();
                int hh=ofGetHeight();
                post2D[1]->setEnabled(true); //bloom
                if(mcontour.bFill==true){
                    post2D[0]->setEnabled(true); //zoomblur
                    //mrayoSil.bDrawTraza=true;
                }else{
                    post2D[0]->setEnabled(false);
                    //mrayoSil.bDrawTraza=false;
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

void testApp::setShaders(unsigned int shaderFlags){
    for( unsigned int i=0; i<7; i++){
        unsigned int test=(int)pow(2.0,(double)i);
        if((test&shaderFlags)!=0){
            post[i]->setEnabled(true);
        }else{
            post[i]->setEnabled(false);
        }
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

void testApp::loadScreenId(){
    ofFile fileRead(ofToString(getenv("HOME"))+"/id_pantalla_flab.txt");
    int id;
    fileRead >> id;
    
    SCREEN_ID=id;
    ofLogVerbose()<<"SCREEN ID:" << SCREEN_ID<<endl;
    if(id==32){
        ofLog() << "id_pantalla_flab.txt" << id;
        exit();
    }
}


#endif

