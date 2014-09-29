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
    mrayoSil.setup();
    mgrid.setup(oniSettings.width, oniSettings.height, &zMin, &zMax, &oniCamGrabber, &depthGenerator);
    particleCloud.setup(oniSettings.width, oniSettings.height, &zMin, &zMax, &oniCamGrabber, &depthGenerator,&camera);
    
    //gui1->loadSettings("./config/gui/gui_kinect.xml");
    guiTabBar->loadSettings("./config/gui/","espejo_");
#ifndef EASYCAM
    loadCameraPose();
#endif
    loadScreenId();
    
    //http://stackoverflow.com/questions/12018710/calculate-near-far-plane-vertices-using-three-frustum
 
    light.setup();
    light.setAmbientColor(ofColor(230, 230, 250));
    ofDisableLighting();
    
    post.init(ofGetWidth(), ofGetHeight());
    post.createPass<ZoomBlurPass>()->setEnabled(false);
    post.createPass<BloomPass>()->setEnabled(false);
    post.createPass<ContrastPass>()->setEnabled(false);
    post.createPass<BleachBypassPass>()->setEnabled(false);
    post.createPass<ToonPass>()->setEnabled(false);
    post.createPass<GodRaysPass>()->setEnabled(false);
    post.createPass<LimbDarkeningPass>()->setEnabled(false);
    post.createPass<VerticalTiltShifPass>()->setEnabled(false);
    post.createPass<ConvolutionPass>()->setEnabled(false);
    
    mtunnel.setup();
    mmenu.setup();
    

    
}

void testApp::setupStatus(){
     appStatuses["escena"]=GRAVEDAD;
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
                        mrayoSil.update();
                        mrayoSil.setSilueta(mcontour.v[0]);
                break;
                
            case NUCLEAR_FUERTE:
                
                break;
                
        }
//ACTUALIZACION SîLO CUANDO HAY IMAGEN NUEVA
        
        if(depthGenerator.isUpdated==true){ // OJO ESTA ACTUALIZACION SOLO OCURRE CUANDO HAY IMAGEN NUEVA
            switch(appStatuses["escena"]){
                case EM:
                   // particleCloud.updateParticles();
                break;
                
                case GRAVEDAD:
                    mcontour.update();
                    if(mgrid.status==mgrid.BLACKHOLE){
                        sender.send(mcontour.v[0]);
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
    fadeBG();
	ofBackground(colorfondo);
    
    ofEnableAlphaBlending();
	
#ifdef EASYCAM
	easyCam.begin();
#else
	//camera.begin();
    post.begin(camera);
#endif
    
    
    //Things to be drawn in 3D
	ofPushMatrix();
        ofScale(-1, 1, -1);
		// the projected points are 'upside down' and 'backwards'
        drawAxis();
    //ofDrawAxis(1000);
   // ofDrawGridPlane(100);
   // ofDrawGrid(300);

		
		// Superponemos modos de dibujo en 3D
#ifdef TESTMODE
    
	    if(bDrawPoints) particleCloud.drawParticles();
		if(bDrawLinesH) drawLinesH();
		if(bDrawLinesV) drawLinesV();
    	if(bDrawNativePointCloud) drawPointCloud();
#else
  //  post.begin();
        switch(appStatuses["escena"]){
                
            case EM:
                particleCloud.drawParticles();
            break;
                
            case GRAVEDAD:
                if(mgrid.status==mgrid.GRID){
                    mgrid.draw(&camera);
                }
                else if(mgrid.status==mgrid.BLACKHOLE){
                    mtunnel.draw();
                }
            break;
                
            case NUCLEAR_DEBIL:

            break;
                
            case NUCLEAR_FUERTE:
                
                break;
        }
    //post.end();
 /*    cout << "fov"<< ofToString(camera.getFov())<<endl;
    cout << "aspect"<< ofToString(camera.getAspectRatio())<<endl;
        cout << "near clip"<< ofToString(camera.getNearClip())<<endl;
    
   float hNear = 2 * tan(camera.getFov()*PI / 360) * camera.getNearClip(); // height
    float wNear = hNear * 0.56; // width
    tr=ofVec3f( wNear / 2, hNear / 2, -camera.getNearClip() );
    tl=ofVec3f( -wNear / 2, hNear / 2, -camera.getNearClip() );
    bl=ofVec3f( -wNear / 2, -hNear / 2, -camera.getNearClip() );
    br=ofVec3f( -wNear / 2, -hNear / 2, -camera.getNearClip() );
    
    tr=camera.cameraToWorld(tr);
    tl=camera.cameraToWorld(tl);
    bl=camera.cameraToWorld(bl);
    br=camera.cameraToWorld(br);/
    
    ofVec3f lookat=camera.getLookAtDir();
    ofVec3f v1=camera.getUpDir();
    
    ofVec3f v2= lookat*v1;
    v1=v1*700;
    v2=v2*700;
    ofPoint X=ofPoint(0,0,1000);
    ofPoint X2=ofPoint(X.x+v1.x, X.y+v1.y, X.z+v1.z);
    ofPoint X3=ofPoint(X.x+v1.x+v2.x, X.y+v1.y+v2.y, X.z+v1.z+v2.z);
    ofPoint X4=ofPoint(X.x+v2.x, X.y+v2.y, X.z+v2.z);
    //tr.z+=50;
    //tl.z+=50;
    ofMesh m1;
    glPointSize(30);
    glBegin(GL_QUADS);
    //m1.setMode(OF_PRIMITIVE_LINE_LOOP);
    glVertex3f(X.x, X.y, X.z);
    glVertex3f(X.x+v1.x, X.y+v1.y, X.z+v1.z);
    glVertex3f(X.x+v1.x+v2.x, X.y+v1.y+v2.y, X.z+v1.z+v2.z);
    glVertex3f(X.x+v2.x, X.y+v2.y, X.z+v2.z);
    glEnd();
    
    m1.addVertex(X);m1.addColor(ofColor::pink);
    m1.addVertex(X2);m1.addColor(ofColor::green);
    m1.addVertex(X3);m1.addColor(ofColor::blue);
    m1.addVertex(X4);m1.addColor(ofColor::pink);
    

    
    
    m1.setMode(OF_PRIMITIVE_POINTS);
        m1.addVertex(tl);
    m1.addColor(ofColor::red);
        m1.addVertex(tr);
        m1.addColor(ofColor::yellow);
        m1.addVertex(br);
            m1.addColor(ofColor::green);
        m1.addVertex(bl);
            m1.addColor(ofColor::pink);
    mesh1.setMode(OF_PRIMITIVE_LINES);*/
    //ofSetColor(ofColor::pink);
   //  ofEnableDepthTest();
   // m1.draw();
   // ofDisableDepthTest();

#endif
	ofPopMatrix();


#ifdef EASYCAM
	easyCam.end();
#else
//	camera.end();
    post.end();
#endif
    
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
                mmenu.draw();
                break;
                
            case GRAVEDAD:
                if(mgrid.status==mgrid.BLACKHOLE){
                    if(mcontour.v.size()>0 && mcontour.v[0].size()>0){
                        mcontour.draw(&(mcontour.v[0])); // TODO REPLACE WITH THE OTHER CONTOUR
                    }
                }
                else{
                    mgrid.draw2D();
                }
            break;
                
            case NUCLEAR_DEBIL:
                post.begin();
                mrayoSil.draw();
                post.end();
            break;
                
            case NUCLEAR_FUERTE:
                post.begin();
                    mcontour.draw();
                post.end();
                break;
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
    ofLogVerbose()<<"SCREEN ID:" << id<<endl;
}

    
#endif

