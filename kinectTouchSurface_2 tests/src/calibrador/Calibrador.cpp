/*
 *  Calibrador.cpp
 *  homography_calibracion
 *
 *  Created by guillermo casado on 15/10/14.
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#include "Calibrador.h"

Calibrador::Calibrador() {
	homographyReady= false;
	movingPoint = false;
}

void Calibrador::setAreaRef(ofRectangle	_areaRef) {
	areaRef = _areaRef;
}

//vector <ofVec2f> ptsSrc;
//vector <ofVec2f> ptsDst;
//cv::Mat homography, homography_inv;
//cv::Mat	map_matrix;
//bool homographyReady;

void Calibrador::addPts(ofVec2f ptS, ofVec2f ptD) {
	// Referencia desde un punto de la imagen, no de la pantalla.
	// ptsSrc: los de la Dcha
	// ptsDst: los de la Izda
	
	ptsSrc.push_back(ptS);
	ptsDst.push_back(ptD);
	
	if(ptsSrc.size()>=4) {
		calcHomography();
	}
}

int Calibrador::size() {
	return ptsSrc.size();
}

void Calibrador::reset() {
	ptsSrc.clear();
	ptsDst.clear();
	
	homographyReady = false;
	
	
}

void Calibrador::calcHomography() {
	if(ptsSrc.size() >= 4) {
		vector<Point2f> srcPoints, dstPoints;
		for(int i = 0; i < ptsSrc.size(); i++) {
			srcPoints.push_back(Point2f(ptsSrc[i].x, ptsSrc[i].y));
			dstPoints.push_back(Point2f(ptsDst[i].x, ptsDst[i].y));
		}
		
		// generate a homography from the two sets of points
		homography = findHomography(Mat(srcPoints), Mat(dstPoints));
		homographyReady = true;
		cv::invert(homography, homography_inv);
		
		// getPerspective
		// esto necesita 4 puntos (ni mas ni menos)
		srcPoints.erase(srcPoints.begin()+4,srcPoints.end());
		dstPoints.erase(dstPoints.begin()+4,dstPoints.end());
		
		map_matrix = getPerspectiveTransform(srcPoints, dstPoints);
	}
}


ofVec2f	Calibrador::transf_Punto(ofVec2f pt, bool inv) {
	// el punto esta en la referencia de la imagen Dst
	ofVec2f ppt = ofVec2f(0,0);
	if(homographyReady) { 
		vector<cv::Vec2f> psSrc, psDst;
		psSrc.push_back(ofxCv::toCv(pt));
		psSrc.push_back(cv::Vec2f(0,0));
		
		// invertir!
		//// metodo de matriz con puntos
		// http://stackoverflow.com/questions/17852182/runnig-cvwarpperspective-on-points
		//	perspectiveTransform(psSrc, psDst, homography.inv());// map_matrix);
		//	perspectiveTransform(psSrc, psDst, map_matrix.inv());
		if(inv) 
			perspectiveTransform(psSrc, psDst, homography_inv);// map_matrix);
		else 
			perspectiveTransform(psSrc, psDst, homography);// map_matrix);
		
		// para varios puntos:
		// void perspectiveTransform(vector<Point2f> origPoints, vector<Point2f> transformedPoints, Mat h)		
		// http://answers.opencv.org/question/1858/homography-for-cordinates-of-poins/#1859
		
		ppt = ofxCv::toOf(psDst[0]);
	}
	else {
		ppt = pt;
	}
	return ppt;
	
	// otros http://stackoverflow.com/questions/17852182/runnig-cvwarpperspective-on-points		
}

vector<ofVec2f> Calibrador::transf_Puntos(vector<ofVec2f> pts, bool inv) {
	vector<ofVec2f> ppts;
	if(homographyReady) { 
		vector<cv::Vec2f> psSrc, psDst;
		for(int i=0; i<ptsSrc.size(); i++) {
			psSrc.push_back(ofxCv::toCv(ptsSrc[i]));
		}
		
		// invertir!
//		perspectiveTransform(psSrc, psDst, homography_inv);// map_matrix);
		if(inv) 
			perspectiveTransform(psSrc, psDst, homography_inv);// map_matrix);
		else 
			perspectiveTransform(psSrc, psDst, homography);// map_matrix);
		
		for(int i=0; i<psDst.size(); i++) {
			ppts.push_back(ofxCv::toOf(psDst[i]));
		}
	}
	else {
		ppts = ptsSrc;
	}
	return ppts;
}


void Calibrador::warpImage(ofImage imgSrc, ofImage& warpedImage) {
	
	if(homographyReady) {
//		// this is how you warp one ofImage into another ofImage given the homography matrix
//		// CV INTER NN is 113 fps, CV_INTER_LINEAR is 93 fps
//		warpPerspective(imgSrc, warpedImage, homography, CV_INTER_LINEAR);
//		warpedImage.update();
	}
	
}

// - - - - 

void Calibrador::drawPoints(ofRectangle rDst, ofRectangle rSrc) {

	ofPushStyle();
	ofSetColor(ofColor::red);
	drawPoints(ptsDst, rDst);
	ofSetColor(ofColor::blue);
	drawPoints(ptsSrc, rSrc);
	
	ofSetColor(128);
	for(int i = 0; i < ptsDst.size(); i++) {
		ofLine(ptsDst[i]+ofVec2f(rDst.x,rDst.y), ptsSrc[i]+ofVec2f(rSrc.x,rSrc.y));
	}
	ofPopStyle();
	
}

void Calibrador::drawPoints(vector<ofVec2f>& points, ofRectangle rr) {
	ofNoFill();
	for(int i = 0; i < points.size(); i++) {
		ofCircle(points[i]+ofVec2f(rr.x, rr.y), 10);
		ofCircle(points[i]+ofVec2f(rr.x, rr.y), 1);
	}
}


// - - - - 

void Calibrador::loadMatrix(String fileName) {
	bool ok1 = false;
	bool ok2 = false;
	bool ok3 = false;
	//	ofLogNotice("LOAD HOMOGR");
	// load the previous homography if it's available
	ofFile previous(fileName+"_homography.yml");
	if(previous.exists()) {
		FileStorage fs(ofToDataPath(fileName+"_homography.yml"), FileStorage::READ);
		fs["homography"] >> homography;
		//		calcHomography();
		homographyReady = true;
		
		ok1 = true;
		
		ofFile previous_Inv(fileName+"_homography_inv.yml");
		if(previous_Inv.exists()) {
			FileStorage fsInv(ofToDataPath(fileName+"_homography_inv.yml"), FileStorage::READ);
			fsInv["homography_inv"] >> homography_inv;
		}
		
	}

	//
	// Puntos para calcular la homografia
	// Las coordenadas vienen dadas con referencia al area de la imagen, no de la pantalla
	//
	
	ofFile fileL(fileName+"_ptsHomogL.txt");
	if(fileL.exists()) {
		ptsDst.clear();
		vector<float>coords;
		while(fileL) {
			string svv;
			fileL >> svv;
			vector<string> vals = ofSplitString(svv,", ");
			coords.push_back(ofToFloat(vals[0]));
		}
		if(coords.size()>0) {
			for(int i=0; i<coords.size()-1; i+=2) {
				ofVec2f ptL = ofVec2f(coords[i], coords[i+1]);
				ptsDst.push_back(ptL);
			}
		}
		ok2=true;
	}
	
	
	ofFile fileR(fileName+"_ptsHomogR.txt");
	if(fileR.exists()) {
		ptsSrc.clear();
		vector<float>coords;
		while(fileR) {
			string svv;
			fileR >> svv;
			vector<string> vals = ofSplitString(svv,", ");
			coords.push_back(ofToFloat(vals[0]));
		}
		if(coords.size()>0) {
			for(int i=0; i<coords.size()-1; i+=2) {
				ofVec2f ptR = ofVec2f(coords[i], coords[i+1]);
				ptsSrc.push_back(ptR);
			}
		}
		ok3=true;
	}
	if(ok1 && ok2 && ok3) 		calcHomography();
	
	
	
	// 
	// 
	// 
	ofFile fileDimL(fileName+"_dimsL.txt");
	if(fileDimL.exists()) {
		vector<float>coords;
		string svv;
		while(fileDimL) {
			fileDimL >> svv;
			ofLogNotice("Carga DimL: " + svv);
			vector<string> vals = ofSplitString(svv,", ");
			coords.push_back(ofToFloat(vals[0]));		
		}
		rectDst = ofRectangle(coords[0], coords[1], coords[2], coords[3]);
		ofLogNotice("Cargo RectDst: " + ofToString(rectDst));
	}
	
	ofFile fileDimR(fileName+"_dimsR.txt");
	if(fileDimR.exists()) {
		vector<float>coords;
		string svv;
		while(fileDimR) {
			fileDimR >> svv;
			vector<string> vals = ofSplitString(svv,", ");
			coords.push_back(ofToFloat(vals[0]));		
		}
		rectSrc = ofRectangle(coords[0], coords[1], coords[2], coords[3]);
		ofLogNotice("Cargo RectSrc: " + ofToString(rectSrc));
	}
	
	
	
	
}

void Calibrador::saveMatrix(String fileName) {
	if(homographyReady) {
		FileStorage fs(ofToDataPath(fileName+"_homography.yml"), FileStorage::WRITE);
		fs << "homography" << homography;
		
		FileStorage fsInv(ofToDataPath(fileName+"_homography_inv.yml"), FileStorage::WRITE);
		fsInv << "homography_inv" << homography_inv;
		
		ofFile fileL(fileName+"_ptsHomogL.txt", ofFile::WriteOnly);
		for(int i=0; i<ptsDst.size(); i++) {
			fileL << ptsDst[i] << endl;
		}
		ofFile fileR(fileName+"_ptsHomogR.txt", ofFile::WriteOnly);
		for(int i=0; i<ptsSrc.size(); i++) {
			fileR << ptsSrc[i] << endl;
		}
		
		
		// 
		// 
		// 
		ofFile fileDimL(fileName+"_dimsL.txt", ofFile::WriteOnly);
		{
			fileDimL << rectDst.x << endl;
			fileDimL << rectDst.y << endl;
			fileDimL << rectDst.width << endl;
			fileDimL << rectDst.height << endl;
		}
		ofFile fileDimR(fileName+"_dimsR.txt", ofFile::WriteOnly);
		{
			fileDimR << rectSrc.x << endl;
			fileDimR << rectSrc.y << endl;
			fileDimR << rectSrc.width << endl;
			fileDimR << rectSrc.height << endl;
		}
	}
}

void Calibrador::logHomogr() {
	if(homographyReady) {
		ofLogNotice("homography");
		int col = homography.cols;
		int row = homography.rows;
		for(int j=0; j<row; j++) {
			string rr = "";
			for(int i=0; i<col; i++) {
				rr+=ofToString(homography.at<float>(i,j)) + "   " ;
			}
			ofLogNotice() << rr;
		}	
		
		ofLogNotice("map_matrix");
		col = map_matrix.cols;
		row = map_matrix.rows;
		ofLogNotice() << col << "," << row;;
		for(int j=0; j<row; j++) {
			string rr = "";
			for(int i=0; i<col; i++) {
				rr+=ofToString(map_matrix.at<float>(i,j)) + "   " ;
				//				rr+=ofToString(map_matrix.at<float>(i,j)) + "   " ;
			}
			ofLogNotice() << rr;
		}	
		
	}
}
