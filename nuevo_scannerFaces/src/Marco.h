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
	
	float		gap;
	
	Marco() {
//		setFillColor(ofColor(127,127,127));
		setFillColor(ofColor(0));
		setFillAlpha(255);
		
		setupBorde();
		
		gap = 20;
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
		
		// dibujar marcas
		int	  lTick = 4;
		int	  sep = 4;
		ofPushStyle();
		// UP 
		ofSetColor(255);
		ofSetLineWidth(1.5);
		ofLine(rect.x, rect.y-gap, rect.x+rect.width, rect.y-gap);
		ofLine(rect.x, rect.y+rect.height+gap, rect.x+rect.width, rect.y+rect.height+gap);
		ofLine(rect.x-gap, rect.y, rect.x-gap, rect.y+rect.height);
		ofLine(rect.x+rect.width+gap, rect.y, rect.x+rect.width+gap, rect.y+rect.height);
		ofSetLineWidth(1.0);
		int x0 = rect.x-gap;
		int x1 = rect.x+rect.width+gap;
		int y0 = rect.y-gap;
		int y1 = rect.y+rect.height+gap;
		for(int i=0; i<rect.width; i+=sep) {
			ofLine(rect.x+i, y0, rect.x+i, y0+lTick);
			ofLine(rect.x+i, y1, rect.x+i, y1-lTick);
		}		
		
		for(int i=0; i<rect.height; i+=sep) {
			ofLine(x0, rect.y+i, x0+lTick, rect.y+i);
			ofLine(x1, rect.y+i, x1-lTick, rect.y+i);
		}
		
		
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