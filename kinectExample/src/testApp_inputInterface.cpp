//
//  testApp_inputInterface.cpp
//  kinectExample
//
//  Created by Sergio Galan on 7/15/14.
//
//

#include "testApp.h"

//--------------------------------------------------------------




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
    gui1->addToggle("bFill", &bFill);
	gui1->addIntSlider("", 0, 2, &modoFill) ;
    gui1->addToggle("bAddPts", &bAddPts);
	gui1->addIntSlider("num Pts", 0, 500, &numPointsXtra) ;
    gui1->addToggle("bDrawOld", &bDrawOld);
    
//case 'f': modoFill++; modoFill%=3;break;
	
	gui1->addSpacer();
	
	ofAddListener(gui1->newGUIEvent,this,&testApp::guiEvent);
    
}

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
			
#endif
		case 'm':
			bDrawPoints=!bDrawPoints;
			break;
		case '1':
			appStatuses["escena"]=EM;
             cambioEscena();
			break;
        case '2':
			appStatuses["escena"]=NUCLEAR_DEBIL;
             cambioEscena();
			break;
        case '9':
			appStatuses["em_ruido"]=true;
			break;
        case '8':
			appStatuses["em_ruido"]=false;
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
        case '3': doTriang=!doTriang;break;
        case '4': doTessel=!doTessel; break;
            
//        case '5': bSoloEnContorno=!bSoloEnContorno;break;
        case '6': bAddPts=!bAddPts;break;
        case '7': bFill=!bFill;break;
        case 'f': modoFill++; modoFill%=3;break;
			
	}
	
#ifndef EASYCAM
    if(key=='Q')	saveCameraPose();
    else if(key=='W') 	loadCameraPose();
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
    else if(name=="RangoZ"){
        cout << "cambio rango \n";
        oniCamGrabber.depthSource.setDepthClipping(zMin,zMax);
    }
}