/*
 *  utilsGeom.cpp
 *  nuclear_Fuerte
 *
 *  Created by guillermo casado on 03/09/14.
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#include "nuclear_fuerte.h"


namespace GeometryUtils {
    
    //-------------------------------------------------------------------  
    static ofVec2f getLineIntersection(const ofVec2f &pt1, const ofVec2f &pt2, const ofVec2f &pt3, const ofVec2f &pt4, bool doInfinite=true) {
        float bx = pt2.x - pt1.x;
        float by = pt2.y - pt1.y;
        float dx = pt4.x - pt3.x;
        float dy = pt4.y - pt3.y; 
        float b_dot_d_perp = bx * dy - by * dx;
        if(b_dot_d_perp == 0) {
            return ofVec2f(0,0);
        }
        float cx = pt3.x-pt1.x; 
        float cy = pt3.y-pt1.y;
        float t = (cx*dy - cy*dx) / b_dot_d_perp; 
        if(!doInfinite) {
            if(t < 0 || t > 1) {
                return ofVec2f(0,0);
            }
            float u = (cx * by - cy * bx) / b_dot_d_perp;
            if(u < 0 || u > 1) { 
                return ofVec2f(0,0);
            }
        }
        return ofVec2f(pt1.x+t*bx, pt1.y+t*by); 
    }
    
	
	// GGGG
	static bool estaDentro(const ofVec2f &pt, const ofPolyline &pl) {
		return pl.inside(pt);
	}
	
	static ofVec2f getIntersectionPoly(const ofVec2f &pt1, const ofVec2f &pt2, const ofPolyline &poly) {
		ofVec3f inters= ofVec2f(0,0);
		ofVec2f pl1,pl2;
		int N = poly.size();
		int i=0;
		for(i=0; i<N-1; i++) {
			pl1 = poly[i];
			pl2 = poly[i+1];
			inters = GeometryUtils::getLineIntersection(pt1, pt2, pl1, pl2, false);
			if(inters!=ofVec2f(0,0)) {
				break;
			}	
		}
		
		// ademas se puede calcular el angulo de incidencia y devolverlo en un struct con el pto de interseccion

		return inters;
	}
	
	
	// FIN GGGG
	
    //-------------------------------------------------------------------
    static float getTriangleRadius(ofVec2f v1, ofVec2f v2, ofVec2f v3) {
        
        float a = v1.distance(v2);
        float b = v2.distance(v3);
        float c = v3.distance(v1);
        
        float k = 0.5 * (a+b+c);
        
        float r = sqrt( k * ((k-a)*(k-b)*(k-c)) ) / k;
        return r;
    }
    
    //-------------------------------------------------------------------
    static ofVec2f getTriangleCenter(ofVec2f v1, ofVec2f v2, ofVec2f v3) {
        
        float a = v2.distance(v3);
        float b = v1.distance(v3);
        float c = v1.distance(v2);
        float d = a+b+c;
        
        float ix = ((a * v1.x) + (b * v2.x) + (c * v3.x)) / d;
        float iy = ((a * v1.y) + (b * v2.y) + (c * v3.y)) / d;
        
        return ofVec2f(ix, iy);
    }
    
    
    //-------------------------------------------------------------------
    static float getArea(const vector <ofVec2f> pts) {
        int i, j, n = pts.size();
        float polyArea = 0;
        for (i = 0; i < n; i++) {
            j = (i + 1) % n;
            polyArea += pts[i].x * pts[j].y;
            polyArea -= pts[j].x * pts[i].y;
        }
        polyArea /= 2.0;
        return polyArea;
    }
    
    //-------------------------------------------------------------------
    static float getTriangleArea(ofVec2f &a, ofVec2f &b, ofVec2f &c) {
        vector <ofVec2f> pts;
        pts.push_back(a);
        pts.push_back(b);
        pts.push_back(c);
        int i, j, n = pts.size();
        float polyArea = 0;
        for (i = 0; i < n; i++) {
            j = (i + 1) % n;
            polyArea += pts[i].x * pts[j].y;
            polyArea -= pts[j].x * pts[i].y;
        }
        polyArea /= 2.0;
        return polyArea;
    }
    
    
}


