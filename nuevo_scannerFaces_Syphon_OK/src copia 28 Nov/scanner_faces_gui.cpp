/*
 *  escaner_faces_gui.cpp
 *  nuevo_scannerFaces
 *
 *  Created by guillermo casado on 13/07/14.
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */
#include "scanner_faces.h"

//--------------------------------------------------------------
void scanner_faces::keyPressed(int key) {
	
	//	if(key == 'v') {
	//		cam.videoSettings();
	//	}
	//	else
    if(key == 'x') ofToggleFullscreen();
	
	// Presets ObjectFinder
	if(key=='1') {
		modoDrawFT = FT_LINES;
	}
	else if(key=='2') {
		modoDrawFT = FT_WIREFRAME;		
	}
	else if(key=='3') {
		modoDrawFT = FT_TEXTURE;
	}
	
	
	if(key==' ') {
		idMsgShow++;
		idMsgShow%=mensajesT.size();
	}
	   
	//	if(key=='i') bFlipH = !bFlipH;
	//	if(key=='o') bFlipV = !bFlipV;
	
	
	if(key=='6') {
		finder.setPreset(ofxCv::ObjectFinder::Fast);
	}
	else if(key=='7') {
		finder.setPreset(ofxCv::ObjectFinder::Accurate);
	}
	else if(key=='8') {
		finder.setPreset(ofxCv::ObjectFinder::Sensitive);
	}
	
	else if(key=='d') doHaarFace=!doHaarFace;
	else if(key=='f') doFaceTracker=!doFaceTracker;
	else if(key=='e') doDrawHaarFace=!doDrawHaarFace;
	else if(key=='r') doDrawFaceTracker=!doDrawFaceTracker;
	
	if(key=='q') {
		nextEscena();
	}
	if(key=='a') {
		scnAct--;
		if(scnAct<0) scnAct+=4;
		setEscena(scnAct);
	}
	
    if(key=='t') {
		bSendSerialMessage = true;
		charSend = 'q';
	}
	else if(key=='g') {
		bSendSerialMessage = true;
		charSend = 'a';
	}
	if(key=='y') {
		bSendSerialMessage = true;
		charSend = 'w';
	}
	else if(key=='h') {
		bSendSerialMessage = true;
		charSend = 's';
	}
	else if(key=='i') {
        setLight(true);
	}
	else if(key=='o') {
        setLight(false);
	}

	else if(key=='m') {
        swShowInfo = !swShowInfo;
	}
    
	//	if(key=='w') {
	//		id_modo_haar_act++;
	//		id_modo_haar_act%=modos_haar.size();
	//		finder.setup(modos_haar[id_modo_haar_act]);
	//	}
	//	else if(key=='s') {
	//		id_modo_haar_act--;
	//		if(id_modo_haar_act<0) id_modo_haar_act+=modos_haar.size();;
	//		finder.setup(modos_haar[id_modo_haar_act]);
	//	}
	
	else if(key=='p') {
		// grabar imagen
		hacerFoto(); // Ya suma nZonaAct++ si se hace la foto
	}
	
	
}

// - - - 


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
	gui->addToggle("Proyecta FT 3D", &doProjectFT);
	
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


