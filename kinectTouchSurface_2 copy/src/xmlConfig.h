/*
 *  xmlConfig.h
 *  G_KinectTouchSurface
 *
 *  Created by guillermo casado on 03/10/14.
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#include "testApp.h"

void testApp::loadXMLSettings()
{
	//	// TODO: a seperate XML to map keyboard commands to action
	//	message = "Loading config.xml...";
	//	// Can this load via http?
	//	if ( XML.loadFile("config.xml"))
	//		message = "Settings Loaded!\n\n";
	//	else
	//		message = "No Settings Found...\n\n"; //FAIL
	//	
	//	//--------------------------------------------------------------
	//	//  START BINDING XML TO VARS
	//	//--------------------------------------------------------------
	//	winWidth					= XML.getValue("CONFIG:WINDOW:WIDTH", 950);
	//	winHeight					= XML.getValue("CONFIG:WINDOW:HEIGHT", 600);
	//	maxBlobs					= XML.getValue("CONFIG:BLOBS:MAXNUMBER", 20);
	//	bShowLabels					= XML.getValue("CONFIG:BOOLEAN:LABELS",0);
	//	bDrawOutlines				= XML.getValue("CONFIG:BOOLEAN:OUTLINES",0);
	//	bUndistort					= XML.getValue("CONFIG:BOOLEAN:UNDISTORT", 0);
	//	filter->bLearnBakground		= XML.getValue("CONFIG:BOOLEAN:LEARNBG",0);
	//	filter->bVerticalMirror		= XML.getValue("CONFIG:BOOLEAN:VMIRROR",0);
	//	filter->bHorizontalMirror	= XML.getValue("CONFIG:BOOLEAN:HMIRROR",0);
	//	
	//	nearThreshold				= XML.getValue("CONFIG:KINECT:NEAR",0);
	//	farThreshold				= XML.getValue("CONFIG:KINECT:FAR",100);
	//	angle						= XML.getValue("CONFIG:KINECT:ANGLE",0);
	//	
	//	//Filters
	//	filter->bTrackDark			= XML.getValue("CONFIG:BOOLEAN:TRACKDARK", 0);
	//	filter->bHighpass			= XML.getValue("CONFIG:BOOLEAN:HIGHPASS",1);
	//	filter->bAmplify			= XML.getValue("CONFIG:BOOLEAN:AMPLIFY", 1);
	//	filter->bSmooth				= XML.getValue("CONFIG:BOOLEAN:SMOOTH", 1);
	//	filter->bDynamicBG			= XML.getValue("CONFIG:BOOLEAN:DYNAMICBG", 1);
	//	//MODES
	//	bGPUMode					= XML.getValue("CONFIG:BOOLEAN:GPU", 0);
	//	bMiniMode                   = XML.getValue("CONFIG:BOOLEAN:MINIMODE",0);
	//	//CONTROLS
	//	tracker.MOVEMENT_FILTERING	= XML.getValue("CONFIG:INT:MINMOVEMENT",0);
	//	MIN_BLOB_SIZE				= XML.getValue("CONFIG:INT:MINBLOBSIZE",2);
	//	MAX_BLOB_SIZE				= XML.getValue("CONFIG:INT:MAXBLOBSIZE",100);
	//	backgroundLearnRate			= XML.getValue("CONFIG:INT:BGLEARNRATE", 0.01f);
	//	//Filter Settings
	//	filter->threshold			= XML.getValue("CONFIG:INT:THRESHOLD",0);
	//	filter->highpassBlur		= XML.getValue("CONFIG:INT:HIGHPASSBLUR",0);
	//	filter->highpassNoise		= XML.getValue("CONFIG:INT:HIGHPASSNOISE",0);
	//	filter->highpassAmp			= XML.getValue("CONFIG:INT:HIGHPASSAMP",0);
	//	filter->smooth				= XML.getValue("CONFIG:INT:SMOOTH",0);
	//	minTempArea					= XML.getValue("CONFIG:INT:MINTEMPAREA",0);
	//	maxTempArea					= XML.getValue("CONFIG:INT:MAXTEMPAREA",0);
	//	hullPress					= XML.getValue("CONFIG:INT:HULLPRESS",20.0);
	//	
	//	//Tracking Options
	//	contourFinder.bTrackBlobs	= XML.getValue("CONFIG:BOOLEAN:TRACKBLOBS",0);
	//	contourFinder.bTrackFingers	= XML.getValue("CONFIG:BOOLEAN:TRACKFINGERS",0);
	//	contourFinder.bTrackObjects	= XML.getValue("CONFIG:BOOLEAN:TRACKOBJECTS",0);
	//	
	//	//NETWORK SETTINGS
	//	bTUIOMode					= XML.getValue("CONFIG:BOOLEAN:TUIO",0);
	//	myTUIO.bOSCMode				= XML.getValue("CONFIG:BOOLEAN:OSCMODE",1);
	//	myTUIO.bTCPMode				= XML.getValue("CONFIG:BOOLEAN:TCPMODE",1);
	//	myTUIO.bBinaryMode			= XML.getValue("CONFIG:BOOLEAN:BINMODE",1);
	//	myTUIO.bHeightWidth			= XML.getValue("CONFIG:BOOLEAN:HEIGHTWIDTH",0);
	//	tmpLocalHost				= XML.getValue("CONFIG:NETWORK:LOCALHOST", "localhost");
	//	tmpPort						= XML.getValue("CONFIG:NETWORK:TUIOPORT_OUT", 3333);
	//	tmpFlashPort				= XML.getValue("CONFIG:NETWORK:TUIOFLASHPORT_OUT", 3000);
	//	myTUIO.setup(tmpLocalHost.c_str(), tmpPort, tmpFlashPort); //have to convert tmpLocalHost to a const char*
	//	//--------------------------------------------------------------
	//	//  END XML SETUP
}

void testApp::saveSettings(){
	//	XML.setValue("CONFIG:KINECT:NEAR", nearThreshold);
	//	XML.setValue("CONFIG:KINECT:FAR", farThreshold);
	//	XML.setValue("CONFIG:KINECT:ANGLE", angle);
	//	
	//	XML.setValue("CONFIG:BOOLEAN:PRESSURE",bShowPressure);
	//	XML.setValue("CONFIG:BOOLEAN:LABELS",bShowLabels);
	//	XML.setValue("CONFIG:BOOLEAN:OUTLINES",bDrawOutlines);
	//	XML.setValue("CONFIG:BOOLEAN:UNDISTORT",bUndistort);
	//	XML.setValue("CONFIG:BOOLEAN:LEARNBG", filter->bLearnBakground);
	//	XML.setValue("CONFIG:BOOLEAN:VMIRROR", filter->bVerticalMirror);
	//	XML.setValue("CONFIG:BOOLEAN:HMIRROR", filter->bHorizontalMirror);
	//	XML.setValue("CONFIG:BOOLEAN:TRACKDARK", filter->bTrackDark);
	//	XML.setValue("CONFIG:BOOLEAN:HIGHPASS", filter->bHighpass);
	//	XML.setValue("CONFIG:BOOLEAN:AMPLIFY", filter->bAmplify);
	//	XML.setValue("CONFIG:BOOLEAN:SMOOTH", filter->bSmooth);
	//	XML.setValue("CONFIG:BOOLEAN:DYNAMICBG", filter->bDynamicBG);
	//	XML.setValue("CONFIG:INT:MINMOVEMENT", tracker.MOVEMENT_FILTERING);
	//	XML.setValue("CONFIG:INT:MINBLOBSIZE", MIN_BLOB_SIZE);
	//	XML.setValue("CONFIG:INT:MAXBLOBSIZE", MAX_BLOB_SIZE);
	//	XML.setValue("CONFIG:INT:BGLEARNRATE", backgroundLearnRate);
	//	XML.setValue("CONFIG:INT:THRESHOLD", filter->threshold);
	//	XML.setValue("CONFIG:INT:HIGHPASSBLUR", filter->highpassBlur);
	//	XML.setValue("CONFIG:INT:HIGHPASSNOISE", filter->highpassNoise);
	//	XML.setValue("CONFIG:INT:HIGHPASSAMP", filter->highpassAmp);
	//	XML.setValue("CONFIG:INT:SMOOTH", filter->smooth);
	//	XML.setValue("CONFIG:INT:MINTEMPAREA", minTempArea);
	//	XML.setValue("CONFIG:INT:MAXTEMPAREA", maxTempArea);
	//	XML.setValue("CONFIG:INT:HULLPRESS", hullPress);
	//	XML.setValue("CONFIG:BOOLEAN:MINIMODE", bMiniMode);
	//	XML.setValue("CONFIG:BOOLEAN:TUIO",bTUIOMode);
	//	XML.setValue("CONFIG:BOOLEAN:TRACKBLOBS",contourFinder.bTrackBlobs);
	//	XML.setValue("CONFIG:BOOLEAN:TRACKFINGERS",contourFinder.bTrackFingers);
	//	XML.setValue("CONFIG:BOOLEAN:TRACKOBJECTS",contourFinder.bTrackObjects);
	//	XML.setValue("CONFIG:BOOLEAN:HEIGHTWIDTH", myTUIO.bHeightWidth);
	//	XML.setValue("CONFIG:BOOLEAN:OSCMODE", myTUIO.bOSCMode);
	//	XML.setValue("CONFIG:BOOLEAN:TCPMODE", myTUIO.bTCPMode);
	//	XML.setValue("CONFIG:BOOLEAN:BINMODE", myTUIO.bBinaryMode);
	//	XML.saveFile("config.xml");
}

