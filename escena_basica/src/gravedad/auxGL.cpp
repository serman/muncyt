/*
 *  auxGL.cpp
 *  shaderExample
 *
 *  Created by guillermo casado on 26/08/14.
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */


#include "testApp.h"

/*
 These functions are for adding quads and triangles to an ofMesh -- either
 vertices, or texture coordinates.
 */
//--------------------------------------------------------------
void testApp::addFace(ofMesh& mesh, ofVec3f a, ofVec3f b, ofVec3f c) {
	mesh.addVertex(a);
	mesh.addVertex(b);
	mesh.addVertex(c);
	
	//	mesh.addColor(ofColor(a.x/planoWidth*255.0, a.y/planoWidth*255.0, 255.0));
	mesh.addColor(ofColor::white);
	mesh.addColor(ofColor::white);
	mesh.addColor(ofColor::white);
	
	//	ofVec3f vv = ofVec3f(a.x,a.y,0);
	//	ofVec3f ref = ofVec3f(planoWidth/2.0,planoHeight/2.0,30);
	//	vv-=ref;
	//	vv.normalize();
//	ofVec3f vv = ofVec3f(0,ofRandom(1),ofRandom(1));
//	vv.normalize();
//	mesh.addNormal(vv);
//	mesh.addNormal(vv);
//	mesh.addNormal(vv);
	
	ofLogNotice("addFace::faceAdded. numIndices:"+ofToString(mesh.getNumIndices()) + "   num Vertices: " + ofToString(mesh.getNumVertices()));
}

//--------------------------------------------------------------
void testApp::addFace(ofMesh& mesh, ofVec3f a, ofVec3f b, ofVec3f c, ofVec3f d) {
	addFace(mesh, a, b, c);
	addFace(mesh, a, c, d);
}

//--------------------------------------------------------------
void testApp::addTexCoords(ofMesh& mesh, ofVec2f a, ofVec2f b, ofVec2f c) {
	mesh.addTexCoord(a);
	mesh.addTexCoord(b);
	mesh.addTexCoord(c);
}

//--------------------------------------------------------------
void testApp::addTexCoords(ofMesh& mesh, ofVec2f a, ofVec2f b, ofVec2f c, ofVec2f d) {
	addTexCoords(mesh, a, b, c);
	addTexCoords(mesh, a, c, d);
}

/*
 The 3d data is stored in an image where alpha represents depth. Here we create
 a 3d point from the current x,y image position.
 */
//--------------------------------------------------------------
ofVec3f testApp::getVertexFromImg(ofImage& img, int x, int y) {
	ofColor color = img.getColor(x, y);
	if(color.a > 0) {
		float z = ofMap(color.a, 0, 255, -480, 480);
		return ofVec3f(x - img.getWidth() / 2, y - img.getHeight() / 2, z);
	} else {
		return ofVec3f(0, 0, 0);
	}
}

//Universal function which sets normals for the triangle mesh 
void testApp::setNormals( ofMesh &mesh ){
//	ofLogNotice("dir: ****************** testApp::setNormals ******************");
	
	//The number of the vertices 
	int nV = mesh.getNumVertices();
	//The number of the triangles 
	int nT = mesh.getNumIndices() / 3;
//	ofLogNotice("dir: ****************** testApp::setNormals ****************** nt: " + ofToString(nT));
	
	vector<ofPoint> norm( nV ); //Array for the normals
	//Scan all the triangles. For each triangle add its 
	//normal to norm's vectors of triangle's vertices 
	for (int t=0; t<nT; t++) {
		
		//Get indices of the triangle t 
		int i1 = mesh.getIndex( 3 * t ); 
		int i2 = mesh.getIndex( 3 * t + 1 ); 
		int i3 = mesh.getIndex( 3 * t + 2 );
		
		//Get vertices of the triangle 
		const ofPoint &v1 = mesh.getVertex( i1 ); 
		const ofPoint &v2 = mesh.getVertex( i2 ); 
		const ofPoint &v3 = mesh.getVertex( i3 );
		
		//Compute the triangle's normal 
		ofPoint dir = ( (v2 - v1).crossed( v3 - v1 ) ).normalized();
		
		//Accumulate it to norm array for i1, i2, i3 
		norm[ i1 ] += dir; 
		norm[ i2 ] += dir; 
		norm[ i3 ] += dir;
	//	ofLogNotice("dir: " + ofToString(dir));
	}
	
	//Normalize the normal's length 
	for (int i=0; i<nV; i++) { 
		norm[i].normalize();	
	}
	
	//Set the normals to mesh 
	mesh.clearNormals(); 
	mesh.addNormals( norm );
}
//--------------------------------------------------------------