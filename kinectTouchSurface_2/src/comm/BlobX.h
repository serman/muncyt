/*
 *  BlobX.h
 *  G_KinectTouchSurface
 *
 *  Created by guillermo casado on 03/10/14.
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _BLOBX
#define _BLOBX

//#include "ofxCv.h"

class BlobX {
	
public:
	BlobX() {};
	
	int label;
	ofVec2f pos;
	ofVec2f vel;	// ult desplazamiento
	float   maccel;	// veloc = despl/dt
	ofVec2f	boundingRect;
	

/*	
 // Mas posibles atributos
 //
 vector <ofPoint>    pts;     // the contour of the blob
 int                 nPts;    // number of pts;
 int					id;
 float               area;
 float               length;
 float               angle;
 float				maccel;  //distance traveled since last frame
 float				age;     //how long the blob has been at war
 float				sitting; //how long hes been sitting in the same place
 float				downTime;
 float				lastTimeTimeWasChecked;
 ofRectangle         boundingRect;
 ofRectangle         angleBoundingRect;
 CvBox2D				angleBoundingBox;
 ofPoint             centroid, lastCentroid, D;
 bool				simulated;
 bool				isObject;
 bool                hole;
 int					color;	
 
 */
	
};

#endif

