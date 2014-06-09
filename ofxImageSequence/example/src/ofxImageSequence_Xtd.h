/*
 *  ofxImageSequence_Xtd.h
 *  imageSequence
 *
 *  Created by guillermo casado on 11/09/13.
 *  Copyright 2013 __MyCompanyName__. All rights reserved.
 *
 *
 *  Como ofxImageSequence pero que permite 
 *  cargar todas las imagenes de un directorio con 
 *  el mismo nomrbe
 *  Y asocia un nombre a la secuencia.
 * 
 */



#pragma once

#include "ofxImageSequence.h"

class ofxImageSequence_Xtd : public ofxImageSequence 
{
  public:
	// add un nombre a la sequencia
	string name;
	
	// add un metodo para cargar una lista de nombres de ficheros
	void loadSequence_Files(vector<string> namesFiles) 
	{
		filenames.reserve(namesFiles.size());
		sequence.reserve(namesFiles.size());
		ofLogVerbose("CARGA DE FICHEROS");
		
		for(int i = 0; i < namesFiles.size(); i++){
			ofLogVerbose("FICHERO: "+ofToString(i)+"-->"+ofToString(namesFiles[i]));

			filenames.push_back(new string(namesFiles[i]));
			sequence.push_back(NULL);
		}

		ofLogVerbose("CARGADOS NOMBRES DE FICHEROS");

		loaded = true;
		
		lastFrameLoaded = -1;

		ofLogVerbose("CARGAR EL FICHERO 0");

		loadFrame(0);
		ofLogVerbose("CARGADO FICHERO 0");
		
		width  = sequence[0]->getWidth();
		height = sequence[0]->getHeight();
		
	}

};


