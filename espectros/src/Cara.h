/*
 *  Cara.h
 *  cargaFaceTracker
 *
 *  Created by guillermo casado on 26/10/14.
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef CARA
#define CARA

class Cara {
public:
	string nombre;
	ofMesh	mesh, objMesh, meanObjMesh;
	ofImage	img;
	
	ofVbo imgVbo, objVbo, meanObjVbo;
	
	Cara() {}
    
	void setup(string _name) {
		nombre = _name;
		
		// cargar caras
		img.loadImage(nombre+".jpg");
		mesh.load(nombre+"_ImageMesh.ply");
		objMesh.load(nombre+"_ObjectMesh.ply");
		meanObjMesh.load(nombre+"_MeanObjectMesh.ply");
		
		imgVbo.setMesh(mesh, GL_STATIC_DRAW);
		objVbo.setMesh(objMesh, GL_STATIC_DRAW);
		meanObjVbo.setMesh(meanObjMesh, GL_STATIC_DRAW);
		
	}
	
	
	
	
};

#endif