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
	
//    gui->addToggle("Modo GL", &swmodoGL);
//	gui->addToggle("Blending", bWarpBlending);
//	
//    gui->addToggle("Activar Seq", &swSeqActiva);
//	
//	gui->addSlider("FrameRate Sequence", 0.1, 10, fr);
//	
//	gui->addIntSlider("Map Image", 0, sampleMaps.size()-1, &currentSampleMapIndex);
//	
//	//    gui->autoSizeToFitWidgets();
	
    ofAddListener(gui->newGUIEvent, this, &scanner_faces::guiEvent); 
	//    gui->loadSettings("settings.xml");

	// posicionar
//	gui->getRect()->x = ofGetWidth() - gui->getRect()->getWidth();	
}

void scanner_faces::guiEvent(ofxUIEventArgs &e) {

//	if(e.getName() == "FrameRate Sequence")
//    {
//        ofxUISlider *slider = e.getSlider();
//		fr = slider->getScaledValue();
//		sequenceActiva.setFrameRate(fr);
//    }
//	else if(e.getName() == "Map Image") {
//		//		ofLogNotice("Map Image");
//		//       ofxUISlider *slider = e.getSlider();
//		warp.setDelayMap(*sampleMaps[currentSampleMapIndex]);			
//		scanner.setDelayMap(sampleMaps[currentSampleMapIndex]->getTextureReference());
//	}
//	
//	
//    else if(e.getName() == "Blending")
//    {
//        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
//		warp.toggleBlending();
//		bWarpBlending = warp.isBlending();
//    }
	
}

void scanner_faces::exit() {
//	gui->saveSettings("settings.xml");
//	delete gui;
}


