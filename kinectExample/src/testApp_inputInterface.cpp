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
    guiTabBar = new ofxUITabBar();
    
	gui1 = new ofxUICanvas(0,100,400,800);
    gui1->setName("General");
    
    particleCloud.setUI(guiTabBar);
    mcontour.setUI(guiTabBar);
    mgrid.setUI(guiTabBar);
    mtunnel.setUI(guiTabBar);
    mrayoSil.setUI(guiTabBar);
    mdela.setUI(guiTabBar);
	gui1->addSpacer();
	gui1->addButton("save",true);
	gui1->addButton("load",true);
	gui1->addSpacer();
    
	gui1->addIntSlider("Distance Lines", 50, 600, &distanciaLineasK) ;
	
	gui1->addSpacer();
	
	gui1->addRangeSlider("RangoZ", 100, 4000, &zMin, &zMax);

#ifdef TESTMODE
	gui1->addSpacer();
    gui1->addToggle("noise", &boolDrawNoise);
	gui1->addToggle("DrawPoints", &bDrawPoints);
	gui1->addToggle("DrawLinesH", &bDrawLinesH);
	gui1->addToggle("DrawLinesV", &bDrawLinesV);
    gui1->addToggle("DrawNativePointCloud", &bDrawNativePointCloud);
	//gui1->addToggle("Explosion", &explosion);
    gui1->addToggle("bDrawContours", &bDrawContours);
    //gui1->addToggle("camera Colours", &bRealColors);
    
#else
    vector<string> names;
    names.push_back("EM");
    names.push_back("DEBIL");
    names.push_back("FUERTE");
    names.push_back("GRAVEDAD");
    names.push_back("MENU");
    gui1->addRadio("MODO", names, OFX_UI_ORIENTATION_HORIZONTAL);
    gui1->addIntSlider("lx", -700, 700, &lx) ;
    gui1->addIntSlider("ly", -700, 700, &ly) ;
    gui1->addIntSlider("lz", -700, 700, &lz) ;

    
#endif
	gui1->addSpacer();
	
	gui1->addButton("reset",true);
	gui1->addSpacer();
    
    
//case 'f': modoFill++; modoFill%=3;break;
	
	gui1->addSpacer();
	
    
	ofAddListener(gui1->newGUIEvent,this,&testApp::guiEvent);
    guiTabBar->addCanvas(gui1);
   // guiTabBar->toggleVisible();
    
    
}

void testApp::keyPressed (int key) {
	switch (key) {
        case 'g':
            debug=!debug;
			gui1->toggleVisible();
			break;

        case 'o':
            if(oniCamrecorder.isRecording==true)
                oniCamrecorder.stopRecording();
            else
                oniCamrecorder.startRecording();
            break;
        case 'm':
            mgrid.setVibration();
        break;
        case 'l':
            if(light.getIsEnabled()) light.disable();
            else light.enable();
            
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
			guiTabBar->saveSettings("./config/gui/","espejo_");
			break;
        case 'k':
            camera.toggleMouseActions();
            break;
        case ' ':
            if(guiTabBar->isEnabled() )
	            guiTabBar->disable();
            else
				guiTabBar->enable();
            break;


	}
    unsigned idx = key - '0';
    if (idx < post.size()) post[idx]->setEnabled(!post[idx]->getEnabled());
	
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
{

}

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
        particleCloud.resetParticles();
    }
	else if(name == "save")
    {
        cout << "save";
					guiTabBar->saveSettings("./config/gui/","espejo_");
#ifndef EASYCAM
		saveCameraPose();
#endif
    }
	else if(name == "load")
    {
        cout << "load";
		guiTabBar->loadSettings("./config/gui/","espejo_");
#ifndef EASYCAM
		loadCameraPose();
#endif
		
    }
	else if(name == "MODO")
    {
		ofxUIRadio *  wr = (ofxUIRadio *) e.widget;
		ofLogNotice("MODO" + wr ->getActiveName() + " = " + ofToString(wr->getValue()));
        status_mode=wr->getValue();
        appStatuses["escena"]=wr->getValue();
        //		gui1->loadSettings("./config/gui/gui_kinect.xml");
    }
    else if(name=="RangoZ"){
        oniCamGrabber.depthSource.setDepthClipping(zMin,zMax);
    }
}