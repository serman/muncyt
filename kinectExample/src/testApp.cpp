#include "testApp.h"
#include <math.h>

//--------------------------------------------------------------
void testApp::setup() {
	ofSetLogLevel(OF_LOG_VERBOSE);
	
    ofSetFullscreen(true);
    
	// enable depth->video image calibration
	kinect.setRegistration(true);
    
	kinect.init();
	//kinect.init(true); // shows infrared instead of RGB video image
	//kinect.init(false, false); // disable video image (faster fps)
	
	kinect.open();		// opens first available kinect
	//kinect.open(1);	// open a kinect by id, starting with 0 (sorted by serial # lexicographically))
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
	
	nearThreshold = 230;
	farThreshold = 70;
	bThreshWithOpenCV = true;
	
	ofSetFrameRate(60);
	
	// zero the tilt on startup
	angle = 0;
	kinect.setCameraTiltAngle(angle);
	
	// start from the front
	bDrawPointCloud = true;

	// init variables
	explosion = false;
	numParticles = 0;
    speed = 1.0;
    stopUmbral = 10000;
    alphaParticles = 255;
	
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
	easyCam.setAutoDistance(true);
	
	setupParticles();
    
	setupGUI();

	setupShader();
    
    
    myOSCrcv.setup();
}



void testApp::setupGUI() {
	gui1 = new ofxUICanvas(0,100,400,600);
	
	gui1->addSlider("speed", 0.4, 5, &speed);
	gui1->addIntSlider("stopUmbral", 10, 10000, &stopUmbral) ;
	
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
	gui1->addToggle("noise", &boolDrawNoise);
	
	gui1->addSpacer();
	
	gui1->addIntSlider("stepPointCloud", 1,5, &stepCloudPoint);
	gui1->addIntSlider("stepLines", 2,10, &stepLines);
	
	gui1->addSpacer();
	
	gui1->addRangeSlider("RangoZ", 100, 3000, &zMin, &zMax);
	
	gui1->addSpacer();
	
	gui1->addToggle("DrawPoints", &bDrawPoints);
	gui1->addToggle("DrawLinesH", &bDrawLinesH);
	gui1->addToggle("DrawLinesV", &bDrawLinesV);
	gui1->addToggle("Explosion", &explosion);
	gui1->addSpacer();
	
	gui1->addButton("reset",true);
	
	
	
	gui1->addSpacer();
	
	ofAddListener(gui1->newGUIEvent,this,&testApp::guiEvent);
}



//--------------------------------------------------------------
void testApp::update() {
	
//	ofBackground(100, 100, 100);
	
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
	
	
	
	
	
	
	
	
	
    // Update OSC
    if(myOSCrcv.update()==true){
        //TODO compute position values and trigger the animations
        cout << "myOSCrcv.remotePosition"  << myOSCrcv.remotePosition << "\n";
    }

}


void testApp::updateParticles() {
    meshParticles.clear();
    ofVec3f mdestPoint;
    ofVec3f diff ;
    std::vector<Particle>::iterator p ;
	
//	float xm = 9999;
//	float xM = -9999;
//	float ym = 9999;
//	float yM = -9999;
//	float zm = 9999999;
//	float zM = -9999999;
    for ( p = particles.begin() ; p != particles.end() ; p++ )
    {
//		ofVec3f pt = kinect.getWorldCoordinateAt(p->_x, p->_y);
//		if(xm>pt.x) xm = pt.x;
//		if(xM<pt.x) xM = pt.x;
//		if(ym>pt.y) ym = pt.y;
//		if(yM<pt.y) yM = pt.y;
//		if(zm>pt.z) zm = pt.z;
//		if(zM<pt.z) zM = pt.z;
		
        if(kinect.getDistanceAt(p->_x, p->_y) > 200 && kinect.getDistanceAt(p->_x, p->_y) < zMax) {

            if(particleMode==ESPEJO){
                p->stopAcc();
             	mdestPoint= kinect.getWorldCoordinateAt(p->_x, p->_y);//getWorldCoordinateAt() returns the position in millimeters with kinect at the origin.
                diff = mdestPoint- p->position;
                if(diff.lengthSquared()>stopUmbral){
                    diff.normalize();
                    //diff *= 0.05;
                    diff *= (speed / diff.lengthSquared());
                    p->addForce( diff );
                    p->update();
                }//espejo
                else{
                    p->stopAcc();
                    p->velocity = ofVec3f(0,0,0);
                }
            }
            else if(particleMode==NUBE) {
                mdestPoint= ofVec3f(p->position.x + ofRandom(-ofGetFrameNum()%300,ofGetFrameNum()%300),p->position.y + ofRandom(-ofGetFrameNum()%300,ofGetFrameNum()%300),p->position.z +
                                    ofRandom(-ofGetFrameNum()%300,ofGetFrameNum()%300)); //kinect.getWorldCoordinateAt(ofRandom(0,400), ofRandom(200,400));// ofVec3f(ofRandom(900,1000),ofRandom(900,1000),ofRandom(900,1000));//
                 	diff = mdestPoint- p->position;
                    diff *= 0.00001;
                    p->addForce( diff );
	                p->update();
               // p->position+=ofNoise( p->position.x,p->position.y,p->position.z)*30;
            }
            //if(counter%300==0)
            //cout << "counter: " << p->position << " ===...........=== "<< mvect << " x: " << x << " y " << y <<"\n";
        	//p->color=ofColor(255,255,255,255);
//            p->color=ofColor::fromHsb(ofMap(kinect.getDistanceAt(p->_x, p->_y), zMin, zMax, 4, 255) , 255, 255, 50);
            //p->color=ofColor(255,255,255,ofMap(mouseX,1,ofGetWidth(),1,255 ) );
            //p->color=ofColor::fromHsb(ofMap(kinect.getDistanceAt(p->_x, p->_y), zMin, zMax, 0, 360) , 255, 255, 200);
           // p->color=ofColor( kinect.getColorAt(p->_x, p->_y) );
            p->color=ofColor(255,255,255,alphaParticles);
            if((p->_x+p->_x)%400==0 ){ //820
                        	//p->color=ofColor(0,255,0,255);
                //if( ofGetFrameNum()%1==0) cout <<  " ::: "	<< diff.lengthSquared() <<  " .... "<< p->velocity <<" ... " << p->acceleration;
                
            }
            meshParticles.addVertex(p->position);
           // meshParticles.addColor(ofColor::fromHsb(ofMap(p->_x, p->_y, zMin, zMax, 1, 360) , 255, 255, 50));
            meshParticles.addColor(p->color);
        }else{
           // meshParticles.addVertex(p->position);
            p->color=ofColor(255,0,0,40);
            
            //meshParticles.addColor(ofColor(255,0,0,40));
        }
    }//end for all points within vector
    //if((ofGetFrameNum()%1==0))
    //cout << " \n ----------------------------------------------- \n";

//    ofLogNotice("World Coords: xyz m: " + ofToString(ofVec3f(xm,ym,zm)));
//    ofLogNotice("World Coords: xyz M: " + ofToString(ofVec3f(xM,yM,zM)));
}

// - - - - - - - - - - - - - - - - - - -- -- -- -- -- -- -- -- -- -- -- -- -- --- --- --- --- --- ---

void testApp::draw() {
	ofBackground(10,10,10,100);

	ofSetColor(255, 255, 255);
    if(boolDrawNoise)  	drawNoise();
    ofEnableAlphaBlending();
	easyCam.begin();
	ofPushMatrix();
		// the projected points are 'upside down' and 'backwards'
		ofScale(1, -1, -1);
		ofTranslate(0, 0, -1000); // Acerca a camara la nube de puntos
		
		// Superponemos modos de dibujo en 3D
		if(bDrawPoints) {
			drawParticles();
		}
		if(bDrawLinesH) drawLinesH();
		if(bDrawLinesV) drawLinesV();
		
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
	easyCam.end();
	
	// Dibujar imagen Profundidad
	float ww = 300;
	float hh = (float)300/kinect.width * kinect.height;
	ofNoFill();
	ofSetColor(0,0,255);
	ofRect(ofGetWidth()-ww,ofGetHeight()-hh,ww,hh);
	ofSetColor(255,255,255);
	kinect.drawDepth(ofGetWidth()-ww,ofGetHeight()-hh,ww,hh);
		
    if(debug) showDebug();
	
	
	
}





void testApp::drawPointCloud(int step) {
    mesh.clear();
	int w = 640;
	int h = 480;
    incrDistance+=1;
	mesh.setMode(OF_PRIMITIVE_POINTS);
	for(int y = 0; y < h; y += step) { //recorro los puntos bajando por las columnas
		for(int x = 0; x < w; x += step) { //recorro columnas
			if(kinect.getDistanceAt(x, y) > 200
               && kinect.getDistanceAt(x, y) < zMax) {
                	ofVec2f p2= ofVec2f(x,y);
                    //mesh.addColor(kinect.getColorAt(x,y));
                    //mesh.addColor(ofColor::fromHsb(ofMap(kinect.getDistanceAt(x, y), zMin, zMax, 0, 360) , 255, 255, 50));
                    //mesh.addColor(ofColor::yellowGreen );
                	mesh.addColor(ofColor(255,255,255,ofMap(mouseX,1,ofGetWidth(),1,255 ) ) );
                    //				mesh.addVertex(kinect.getWorldCoordinateAt(x, y));
                    ofVec3f vtmp = kinect.getWorldCoordinateAt(x , y);
                    mesh.addVertex(vtmp);
            
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
    int w= 640;
    int h= 480;
	int    sampling=2;
    //Loop through all the rows

	numParticles = 0;
    //Loop through all the columns
	// Distribuir las particulas por la escena
    for ( int y = 0 ; y < h ; y+=sampling ){
        for ( int x = 0 ; x < w ; x+=sampling ){
//			particles.push_back(Particle(ofVec3f(0,0,0),ofColor(255,255,255) ,x,y));
			float px = ofRandom(-1000,1000);
			float py = ofRandom(-1000,1000);
			float pz = ofRandom(0,5000);
			particles.push_back(Particle(ofVec3f(px,py,pz) ,ofColor(255,255,255) ,x,y));
        numParticles++ ;
        }
    }
    
    cout<<"particulas"<<numParticles;
}

void testApp::resetParticles(){
    particles.clear();
    setupParticles();
}

void testApp::drawParticles(){
    meshParticles.setMode(OF_PRIMITIVE_POINTS);
    glPointSize(1);
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
    ofDrawBitmapString("EasyCam - distance: " + ofToString(easyCam.getDistance()), ofGetWidth()-200, 20);
    ofDrawBitmapString("drag: " + ofToString(easyCam.getDrag()), ofGetWidth()-200, 35);
	
	ofNode nodeCam = easyCam.getTarget();
//    ofDrawBitmapString("EasyCam - posCam: " + ofToString(nodeCam.getX()) + "," + ofToString(nodeCam.getY()) + "," + ofToString(nodeCam.getZ()), ofGetWidth()-200, 50);
    ofDrawBitmapString("posCam: " + ofToString(nodeCam.getPosition()), ofGetWidth()-200, 50);
    ofDrawBitmapString("globalposCam: " + ofToString(nodeCam.getGlobalPosition()), ofGetWidth()-200, 65);
    ofDrawBitmapString("spitch: " + ofToString(nodeCam.getPitch()), ofGetWidth()-200, 80);
//    ofDrawBitmapString("EasyCam - distance: " + ofToString(easyCam.getDistance()), ofGetWidth()-200, 20);
//    ofDrawBitmapString("EasyCam - distance: " + ofToString(easyCam.getDistance()), ofGetWidth()-200, 20);
}

void testApp::drawLinesH(float step){
	int w = 640;
	int h = 480;
    incrDistance+=1;
    std::vector<ofPolyline> lineMesh;
    ofColor col ;
    float _time = ofGetElapsedTimef() ;
    float theta = sin ( ofGetElapsedTimef() ) ;
    ofVec2f v1=ofVec2f(640,480);
    ofPolyline lineMesh1;
//    ofPoint lastPoint ;
    ofVec3f lastPoint ;
    
	for(int y = 0; y < h; y += step) { //recorro los puntos bajando por las columnas
        ofPath line ;
		bool bLastValid = false;
		int _xStep = step;
		for(int x = 0; x < w; x += step) { //recorro columnas
			if(kinect.getDistanceAt(x, y) > zMin && kinect.getDistanceAt(x, y) < zMax) {
                ofVec2f p2= ofVec2f(x,y);
                ofVec3f vtmp = kinect.getWorldCoordinateAt(x , y);
                
                ofPoint _lastPoint = vtmp ;
//                float dist = abs(vtmp.z - lastPoint.z) ;
//                if (  dist < 30  )
				float dist2 = vtmp.squareDistance(lastPoint);
                if (  dist2 < 120  )
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
	int w = kinect.width;
	int h = kinect.height;
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
			if(kinect.getDistanceAt(x, y) > zMin && kinect.getDistanceAt(x, y) < zMax) {
                ofVec2f p2   = ofVec2f(x,y);
                ofVec3f vtmp = kinect.getWorldCoordinateAt(x , y);
                
//                ofPoint _lastPoint = vtmp ;
//                float dist = abs(vtmp.z - lastPoint.z) ;
//                if (  dist < 30  )
				float dist2 = vtmp.squareDistance(lastPoint);
                if (  dist2 < 120  )
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
	kinect.setCameraTiltAngle(0); // zero the tilt on exit
	kinect.close();
	
	//gui1->saveSettings("gui_kinect.xml");
    
	delete gui1;
	
	
}

//--------------------------------------------------------------
void testApp::keyPressed (int key) {
	switch (key) {

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
		case 'm':
			bDrawPoints=!bDrawPoints;
			break;
		case 'n':
			explosion=!explosion;
			break;
        case 'g':
            debug=!debug;
            			break;
        case 'p':
            if(particleMode==NUBE)
                particleMode=ESPEJO;
            else particleMode=NUBE;
            break;
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
			
	}
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
    }
	else if(name == "load")
    {
        cout << "load";
		gui1->loadSettings("./config/gui/gui_kinect.xml");
    }
	else if(name == "MODO_Partics")
    {
		ofxUIRadio *  wr = (ofxUIRadio *) e.widget;
		ofLogNotice("MODO_Partics. " + wr ->getActiveName() + " = " + ofToString(wr->getValue()));
		particleMode = wr->getValue();
//		gui1->loadSettings("./config/gui/gui_kinect.xml");
    }
}
