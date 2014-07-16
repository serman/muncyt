//
//  testApp_Drawing.cpp
//  kinectExample
//
//  Created by Sergio Galan on 7/15/14.
//
//

#include "testApp.h"



void testApp::drawCVSilhouettes(){
ofFill();
ofSetColor(255,255,255);
ofSetColor(255, 255, 0);
ofSetLineWidth(2);
if(resampledContour.size()>=3) {
    triangulation.reset();
    for(int i=0; i<resampledContour.size(); i++) {
        triangulation.addPoint(resampledContour[i]);
    }
    triangulation.triangulate();
    triangContMesh.clear();
    
    triangContMesh.setMode(OF_PRIMITIVE_TRIANGLES);
    ofMesh tt = triangulation.triangleMesh;
    vector<ofMeshFace> faces = tt.getUniqueFaces();
    for(int i=0; i<faces.size(); i++) {
        ofVec3f a = faces[i].getVertex(0);
        ofVec3f b = faces[i].getVertex(1);
        ofVec3f c = faces[i].getVertex(2);
        ofVec3f pm = (a+b+c)/3.0;
        
        if( resampledContour.inside(ofPoint(pm.x,pm.y)) ) {
            triangContMesh.addVertex(a);
            triangContMesh.addVertex(b);
            triangContMesh.addVertex(c);
        }
    }
    ofPushMatrix();
    ofTranslate(ofGetWidth(), ofGetHeight());
    ofRotateZ(180);
    ofScale(3,3,3);
    triangContMesh.drawWireframe();
    ofPopMatrix();
}
}

void testApp::drawPointCloud(int step) {
mesh.clear();

int w = IRCAMERAWIDTH;
int h = IRCAMERAHEIGHT;

incrDistance+=1;
mesh.setMode(OF_PRIMITIVE_POINTS);
for(int y = 0; y < h; y += step) { //recorro los puntos bajando por las columnas
    for(int x = 0; x < w; x += step) { //recorro columnas
#ifndef ASUS
        if(kinect.getDistanceAt(x, y) > zMin
           && kinect.getDistanceAt(x, y) < zMax) {
            ofVec2f p2= ofVec2f(x,y);
            
            mesh.addColor(ofColor(255,255,255,ofMap(mouseX,1,ofGetWidth(),100,255 ) ) );
            ofVec3f vtmp = kinect.getWorldCoordinateAt(x , y);
            mesh.addVertex(vtmp);
            
#else
            int distance=depthGenerator.currentRawPixels->getPixels()[y * depthGenerator.width +x];
            if(distance> zMin && distance < zMax) {
                ofVec2f p2   = ofVec2f(x,y);
                ofVec3f vtmp=oniCamGrabber.convertDepthToWorld(x,y);
                mesh.addColor(ofColor(255,255,255,ofMap(mouseX,1,ofGetWidth(),100,255 ) ) );
                mesh.addVertex(vtmp);
                
#endif
                //}
                
                // lineMesh1.addColor(ofColor::yellowGreen );
                // lineMesh1.addVertex(vtmp);
            }//if
            
        } //for interior
        // cout << p1.distance(vtmp) << "\n";
        //vtmp.x +=20*sin(vtmp.y/10.0*PI+ofGetElapsedTimeMillis())*sin(ofGetElapsedTimeMillis()/1000.0*TWO_PI);
    }
    glPointSize(2);
    glEnable(GL_POINT_SMOOTH);	// Para que sean puntos redondos
    ofEnableDepthTest();
    mesh.draw();	
    ofDisableDepthTest();
}


void testApp::drawNoise(){
    ofPushStyle();
    ofFill();
    ofSetColor(255,255,255,40);
         ofSetColor(255,255,255,40);
    
    shader.begin();
    shader.setUniform3f("iResolution", ofGetWidth(), ofGetHeight(),0);
    shader.setUniform1f("iGlobalTime", ofRandomf());
        shader.setUniform1f("alpha", appStatuses["alpha_ruido"]);
    shader.setUniform2f("iMouse",ofGetMouseX(),ofGetMouseY());

    
    //mtexture.loadScreenData(600,600,200,200);
    //mtexture.loadData(mimage.getPixelsRef());
    ///shader.setUniformTexture("iChannel0",mtexture,2 );
    
    ofRect(0, 0, ofGetWidth(), ofGetHeight());
    shader.end();
    ofPopStyle();
    //mimage.getTextureReference().unbind();
}

void testApp::drawCountours(){
    ofNoFill();
    ofSetColor(255,255,255);
    ofTexture& depth = oniCamGrabber.getDepthTextureReference();
    depth.draw(ofGetWidth()-480*2,ofGetHeight()-360,480,360);
    ofPushMatrix();
    ofRotateZ(-90);
    //   rgbGenerator.texture.draw(ofGetWidth()-480,ofGetHeight()-360,480,360);
    ofPopMatrix();
}


