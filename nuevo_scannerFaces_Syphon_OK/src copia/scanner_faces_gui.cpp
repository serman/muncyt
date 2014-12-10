/*
 *  escaner_faces_gui.cpp
 *  nuevo_scannerFaces
 *
 *  Created by guillermo casado on 13/07/14.
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */
#include "scanner_faces.h"


void scanner_faces::setupGUI() {
    gui = new ofxUICanvas();
    
    gui->addLabel("Face Scanner", OFX_UI_FONT_LARGE);
	
    gui->addToggle("(t) FullScreen", &swFullScreen);

	gui->addIntSlider("Modo Draw FT", 0, 2, &modoDrawFT);
	
	gui->addSpacer();
	
	gui->addButton("Finder_Fast", false);
	gui->addButton("Finder_Accurate", false);
	gui->addButton("Finder_Sensitive", false);

	gui->addSpacer();
	gui->addToggle("Do Haar Face", &doHaarFace);
	gui->addToggle("Do FaceTracker", &doFaceTracker);
	gui->addToggle("Draw HaarFace", &doDrawHaarFace);
	gui->addToggle("Draw FaceTracker", &doDrawFaceTracker);
	
	gui->addSpacer();
	gui->addButton("Hacer Foto", false);
	
//	gui->addToggle("Blending", bWarpBlending);
//	
//  gui->addToggle("Activar Seq", &swSeqActiva);
//	
//	gui->addSlider("FrameRate Sequence", 0.1, 10, fr);
//	
//	
//	//    gui->autoSizeToFitWidgets();
	
    ofAddListener(gui->newGUIEvent, this, &scanner_faces::guiEvent); 
	//    gui->loadSettings("settings.xml");

	// posicionar
//	gui->getRect()->x = ofGetWidth() - gui->getRect()->getWidth();	
}

void scanner_faces::guiEvent(ofxUIEventArgs &e) {

	string name = e.getName();
	int kind = e.getKind();
	//	cout << "got event from: " << name << endl;
	
	if(name == "(t) FullScreen")
	{	// swOrtho
		//		ofxUIToggle *toggle = (ofxUIToggle *) e.getToggle();
		//		swOrtho = toggle->getValue();
		//		if(swOrtho) cam.enableOrtho();
		//		else cam.disableOrtho();
		ofSetFullscreen(swFullScreen);
	}
//	else if(name=="Finder_Fast") {
//		finder.setPreset(ObjectFinder::Fast);
//	}
//	else if(name=="Finder_Accurate") {
//		finder.setPreset(ObjectFinder::Accurate);
//	}
//	else if(name=="Finder_Sensitive") {
//		finder.setPreset(ObjectFinder::Sensitive);
//	}
	else if(name == "Hacer Foto") {
		hacerFoto();
	}
	
	else if(name == "radio_Sol")
	{
//		ofxUISlider *slider = (ofxUISlider *) e.widget;		
//		sol.setRadio(slider->getScaledValue());
	}
	else if(name == "cam_z")
	{
//		ofxUISlider *slider = (ofxUISlider *) e.widget;
//		zCam = slider->getScaledValue();
//		cam.setDistance(zCam);
	}
	
	
	
}


