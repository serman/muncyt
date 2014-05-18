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
    
    low=0.25;
    zMin = 200;
    zMax = 1800;
    
    incrDistance=0;
	setupParticles();

}

//--------------------------------------------------------------
void testApp::update() {
	
//	ofBackground(100, 100, 100);
	
	kinect.update();
	
	// there is a new frame and we are connected
	if(kinect.isFrameNew()) {
		// load grayscale depth image from the kinect source
		grayImage.setFromPixels(kinect.getDepthPixels(), kinect.width, kinect.height);
		
		// we do two thresholds - one for the far plane and one for the near plane
		// we then do a cvAnd to get the pixels which are a union of the two thresholds
		if(bThreshWithOpenCV) {
			grayThreshNear = grayImage;
			grayThreshFar = grayImage;
			grayThreshNear.threshold(nearThreshold, true);
			grayThreshFar.threshold(farThreshold);
			cvAnd(grayThreshNear.getCvImage(), grayThreshFar.getCvImage(), grayImage.getCvImage(), NULL);
		} else {
			// or we do it ourselves - show people how they can work with the pixels
			unsigned char * pix = grayImage.getPixels();
			
			int numPixels = grayImage.getWidth() * grayImage.getHeight();
			for(int i = 0; i < numPixels; i++) {
				if(pix[i] < nearThreshold && pix[i] > farThreshold) {
					pix[i] = 255;
				} else {
					pix[i] = 0;
				}
			}
		}
        updateParticles();
	}
    
}
void testApp::updateParticles() {
    meshParticles.clear();
    std::vector<Particle>::iterator p ;
    int counter=0;
    for ( p = particles.begin() ; p != particles.end() ; p++ )
    {
        if(kinect.getDistanceAt(p->_x, p->_y) > 200
           && kinect.getDistanceAt(p->_x, p->_y) < zMax) {
            ofVec3f mdestPoint;
            if(explosion==false){
             mdestPoint= kinect.getWorldCoordinateAt(p->_x, p->_y);// ofVec3f(ofRandom(900,1000),ofRandom(900,1000),ofRandom(900,1000));//
            }
            else {
                mdestPoint= kinect.getWorldCoordinateAt(320, 240);// ofVec3f(ofRandom(900,1000),ofRandom(900,1000),ofRandom(900,1000));//
            }            
                ofVec3f diff = mdestPoint- p->position;
                if(diff.length()>30){
                    diff *= 0.005;
                    p->addForce( diff );
                    p->velocity +=p->acceleration;
                    p->position +=p->velocity;
                }
            
            //if(counter%300==0)
            //cout << "counter: " << p->position << " ===...........=== "<< mvect << " x: " << x << " y " << y <<"\n";
        	p->color=ofColor(255,255,255,255);
            meshParticles.addVertex(p->position);
            meshParticles.addColor(ofColor::fromHsb(ofMap(p->_x, p->_y, zMin, zMax, 0, 360) , 255, 255, 50));
//            meshParticles.addColor(p->color);
        }
        else{
            p->color=ofColor(255,255,255,0);
        }

        counter++;
    }

    
}

//--------------------------------------------------------------
void testApp::draw() {
	ofBackground(10,10,10,100);

	ofSetColor(255, 255, 255);
	
	if(bDrawPointCloud) {
		easyCam.begin();
        ofPushMatrix();
        // ofRotateZ(90);
        ofPushMatrix();
            // the projected points are 'upside down' and 'backwards'
        	ofScale(1, -1, -1);
            ofTranslate(0, 0, -1000); // center the points a bit
            if(pulso==true)
                //drawPointCloud();
                drawParticles();
            else
                drawPointCloud();
//                drawLines();
            ofPopMatrix();
        ofPopMatrix();
		easyCam.end();
	} else {
		// draw from the live kinect
		kinect.drawDepth(10, 10, 400, 300);
		kinect.draw(420, 10, 400, 300);
		
		grayImage.draw(10, 320, 400, 300);
		contourFinder.draw(10, 320, 400, 300);
		
	}
	
	// draw instructions
	ofSetColor(255, 255, 255);
    //kinect.setDepthClipping(500,1000);
	stringstream reportStream;
        
    if(kinect.hasAccelControl()) {
        reportStream << "accel is: " << ofToString(kinect.getMksAccel().x, 2) << " / "
        << ofToString(kinect.getMksAccel().y, 2) << " / "
        << ofToString(kinect.getMksAccel().z, 2) << endl;
    } else {
        reportStream << "Note: this is a newer Xbox Kinect or Kinect For Windows device," << endl
		<< "motor / led / accel controls are not currently supported" << endl << endl;
    }
    
	reportStream << "press p to switch between images and point cloud, rotate the point cloud with the mouse" << endl
    << "zMax= "<<zMax << endl
    
	<< "using opencv threshold = " << bThreshWithOpenCV <<" (press spacebar)" << endl
	<< "set near threshold " << nearThreshold << " (press: + -)" << endl
	<< "set far threshold " << farThreshold << " (press: < >) num blobs found " << contourFinder.nBlobs
	<< ", fps: " << ofGetFrameRate() << endl
	<< "press c to close the connection and o to open it again, connection is: " << kinect.isConnected() << endl;

    if(kinect.hasCamTiltControl()) {
    	reportStream << "press UP and DOWN to change the tilt angle: " << angle << " degrees" << endl
        << "press 1-5 & 0 to change the led mode" << endl;
    }
	//ofDrawBitmapString(reportStream.str(), 20, 640);
}

void testApp::drawPointCloud() {
    mesh.clear();
	int w = 640;
	int h = 480;
    incrDistance+=1;
	mesh.setMode(OF_PRIMITIVE_POINTS);
    int step;
	step = 4;
	for(int y = 0; y < h; y += step) { //recorro los puntos bajando por las columnas
		for(int x = 0; x < w; x += step) { //recorro columnas
			if(kinect.getDistanceAt(x, y) > 200
               && kinect.getDistanceAt(x, y) < zMax) {
                	ofVec2f p2= ofVec2f(x,y);
                    mesh.addColor(kinect.getColorAt(x,y));
                    mesh.addColor(ofColor::fromHsb(ofMap(kinect.getDistanceAt(x, y), zMin, zMax, 0, 360) , 255, 255, 50));
                    //mesh.addColor(ofColor::yellowGreen );
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
	ofEnableDepthTest();
	mesh.draw();	
	ofDisableDepthTest();
}

void testApp::setupParticles(){
   //particles.clear() ;
    int w= 640;
    int h= 480;
	int    sampling=2;
    //Loop through all the rows

    //Loop through all the columns
    for ( int y = 0 ; y < h ; y+=sampling )
    {
        for ( int x = 0 ; x < w ; x+=sampling )
        {
        particles.push_back(Particle(ofVec3f( 0,0,0),ofColor(255,255,255) ,x,y));
        numParticles++ ;
        }
    }
    
    cout<<"particulas"<<numParticles;
}

void testApp::drawParticles(){
    meshParticles.setMode(OF_PRIMITIVE_POINTS);
    glPointSize(2);
	ofEnableDepthTest();
	meshParticles.draw();
	ofDisableDepthTest();
    /*return
    glBegin(GL_POINTS);
    //glBegin(GL_LINES);
    //glBegin(GL_TRIANGLES);
    
    ofEnableAlphaBlending() ;
    
    //Create an iterator to cycle through the vector
    std::vector<Particle>::iterator p ;
    
    for ( p = particles.begin() ; p != particles.end() ; p++ )
    {
    	ofSetColor ( p->color ) ;
        glVertex3f(p->position.x , p->position.y , p->position.z );
    }
    
    //ofPopMatrix() ;
    glEnd();
    
    ofEnableAlphaBlending() ;
*/
}

void testApp::drawLines(){
	int w = 640;
	int h = 480;
    incrDistance+=1;
    int step;
    if(pulso==false)
        step = 3;
    else
        step=5;
    std::vector<ofPolyline> lineMesh;
    ofColor col ;
    float _time = ofGetElapsedTimef() ;
    float theta = sin ( ofGetElapsedTimef() ) ;
    ofVec2f v1=ofVec2f(640,480);
    ofPolyline lineMesh1;
    ofPoint lastPoint ;
    
	for(int y = 0; y < h; y += step) { //recorro los puntos bajando por las columnas
        ofPath line ;
		bool bLastValid = false;
		int _xStep = step;
		for(int x = 0; x < w; x += step) { //recorro columnas
			if(kinect.getDistanceAt(x, y) > 200 && kinect.getDistanceAt(x, y) < zMax) {
                ofVec2f p2= ofVec2f(x,y);
                ofVec3f vtmp = kinect.getWorldCoordinateAt(x , y);
                
                ofPoint _lastPoint = vtmp ;
                float dist = abs(vtmp.z - lastPoint.z) ;
                if (  dist < 30  )
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
            }//if
        } //for interior
        ofColor c=  ofColor( 35 , 255 , 24 ) ;
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
	
#ifdef USE_TWO_KINECTS
	kinect2.close();
#endif
}

//--------------------------------------------------------------
void testApp::keyPressed (int key) {
	switch (key) {
		case ' ':
			bThreshWithOpenCV = !bThreshWithOpenCV;
			break;
			
		case'p':
			bDrawPointCloud = !bDrawPointCloud;
			break;
			
		case '>':
		case '.':
			farThreshold ++;
			if (farThreshold > 255) farThreshold = 255;
			break;
			
		case '<':
		case ',':
			farThreshold --;
			if (farThreshold < 0) farThreshold = 0;
			break;
			
		case '+':
		case '=':
			nearThreshold ++;
			if (nearThreshold > 255) nearThreshold = 255;
			break;
			
		case '-':
			nearThreshold --;
			if (nearThreshold < 0) nearThreshold = 0;
			break;
			
		case 'w':
			kinect.enableDepthNearValueWhite(!kinect.isDepthNearValueWhite());
			break;
			
		case 'o':
			kinect.setCameraTiltAngle(angle); // go back to prev tilt
			kinect.open();
			break;
			
		case 'c':
			kinect.setCameraTiltAngle(0); // zero the tilt
			kinect.close();
			break;
			
		case '1':
			kinect.setLed(ofxKinect::LED_GREEN);
			break;
			
		case '2':
			kinect.setLed(ofxKinect::LED_YELLOW);
			break;
			
		case '3':
			kinect.setLed(ofxKinect::LED_RED);
			break;
			
		case '4':
			kinect.setLed(ofxKinect::LED_BLINK_GREEN);
			break;
			
		case '5':
			kinect.setLed(ofxKinect::LED_BLINK_YELLOW_RED);
			break;
			
		case '0':
			kinect.setLed(ofxKinect::LED_OFF);
			break;
		case 'm':
			pulso=!pulso;
			break;
		case 'n':
			explosion=!explosion;
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
            Img.grabScreen( 0, 0, ofGetWidth(), ofGetHeight() );
            Img.saveImage( "test.png" );
	}
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
