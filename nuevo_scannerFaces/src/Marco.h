/*
 *  Marco.h
 *  scaner_faces
 *
 *  Created by guillermo casado on 16/06/14.
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 
 Clase que presenta un marco que ocupa toda la pantalla y deja un rectangulo abierto.
 
 El cuadro puede cambiar de color y transparencia
 
 
 */
#pragma once

#include "ofMain.h"

#ifndef MARCO
#define MARCO

class Marco {
public:
	ofRectangle	rect, rectExt;
	ofColor		fillColor;
	float		fillAlpha;
	
	ofPath		borde;
	
	Marco() {
		setFillColor(ofColor(127,127,127));
		setFillAlpha(255);
		
		setupBorde();
	}
	
	void setRect(ofRectangle _rect) {
		rect = _rect;
		setupBorde();
	}
	
	void setRectExt(ofRectangle _rectExt) {
		rectExt = _rectExt;
		setupBorde();
	}
	
	void setFillColor(ofColor _fillColor) {
		fillColor = _fillColor;
		setupBorde();
	}
	void setFillAlpha(float _fillAlpha) {
		fillAlpha = _fillAlpha;
		setupBorde();
	}
	
	void setupBorde() {
		borde.clear();
		ofColor ctmp = ofColor(fillColor, fillAlpha);
		borde.setFillColor(ctmp);
		//http://www.openframeworks.cc/documentation/graphics/ofPath.html#show_setPolyWindingMode
		borde.setPolyWindingMode(OF_POLY_WINDING_ODD);
		// rectangulo 
		borde.rectangle(rectExt);
		borde.rectangle(rect);
		
	}
	
	void draw() {
		ofPushStyle();
			borde.draw();
		ofPopStyle();		
	}
	
	bool isOut(ofRectangle rr) {
		// dice si algun lado del rectangulo rr esta fuera del marco
		bool isOut = false;
		if (rr.x<rect.x || rr.y<rect.y || 
			(rr.x+rr.width)>(rect.x+rect.width) || (rr.y+rr.height)>(rect.y+rect.height) ) {
			
			isOut = true;
		}
		return isOut;
	}
	

};
#endif