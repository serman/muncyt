//
//  contours.h
//  kinectExample
//
//  Created by Sergio Galan on 8/31/14.
//
//

#ifndef kinectExample_contours_h
#define kinectExample_contours_h
#include "ofxOpenCv.h"
#include "ofxCv.h"

using namespace ofxCv;
using namespace cv;
class contours{
public:
    
    vector<ofPolyline> v;
    
    void update(){
        
        depthImg.setFromPixels( oniCamGrabber->getDepthPixels());
        depthImg.update();

        v.clear();
        v= getSmoothSilhouete1(contourFinder, depthImg, (float)0.1/10);
        if(v.size()>0){
            resampledContour =v[0];
    }
    }
    void draw(){
        ofFill();
        ofSetColor(255,255,255);
        ofSetColor(255, 255, 0);
        ofSetLineWidth(2);
        if(resampledContour.size()>=3) {
            triangulation.reset();
            for(int i=0; i<resampledContour.size(); i++) {
                triangulation.addPoint(resampledContour[i]);
            }
            if(bAddPts) {
                ofRectangle bounds = resampledContour.getBoundingBox();
                
                for(int i=0; i<numPointsXtra; i++) {
                    float px = bounds.x+ofRandom(bounds.width);
                    float py = bounds.y+ofRandom(bounds.height);
                    if( resampledContour.inside(ofPoint(px, py)) ){
                        triangulation.addPoint(ofPoint(px, py));
                    }
                }
            }
            
            
            triangulation.triangulate();
            triangContMesh_old = triangContMesh;
            
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
                    // supongo el area de imagen dividido en 10 franjas divididas a su vez en
                    // colorFluor.size() bandas de distinto color
                    float hBandaPpal = ofGetHeight()/10;
                    
                    float hBanda = hBandaPpal/colorFluor.size();
                    
                    ofColor ctmpa, ctmpb, ctmpc;
                    if(modoFill==0) {
                        // RANDOM
                        ofColor ctmp = colorFluor[i%colorFluor.size()];
                        ctmpa = ctmp;
                        ctmpb = ctmp;
                        ctmpc = ctmp;
                    }
                    else if(modoFill==1) {
                        // asignar color segun la posicion del punto medio
                        int nBanda = floor(pm.y/hBanda);
                        ofColor ctmp = colorFluor[nBanda%colorFluor.size()];
                        ctmpa = ctmp;
                        ctmpb = ctmp;
                        ctmpc = ctmp;
                    }
                    else if(modoFill==2) {
                        int nBanda = floor(a.y/hBanda);
                        ctmpa = colorFluor[nBanda%colorFluor.size()];
                        nBanda = floor(b.y/hBanda);
                        ctmpb = colorFluor[nBanda%colorFluor.size()];
                        nBanda = floor(c.y/hBanda);
                        ctmpc = colorFluor[nBanda%colorFluor.size()];
                    }
                    triangContMesh.addVertex(a);
                    triangContMesh.addVertex(b);
                    triangContMesh.addVertex(c);
                    triangContMesh.addColor(ctmpa);
                    triangContMesh.addColor(ctmpb);
                    triangContMesh.addColor(ctmpc);
                }
                
                
            }
            
            
            // DIBUJAR
            
            ofPushMatrix();
            ofTranslate(ofGetWidth(), ofGetHeight());
            ofRotateZ(180);
            ofScale(3,3,3);
            
            if(doTriang) {
                ofNoFill();
                ofSetColor(ofColor::wheat);
                ofNoFill();
                if(!bSoloEnContorno) {
                    triangulation.draw();
                }
                else {
                    if(bFill)	{
                        triangContMesh.draw();
                        if(bDrawOld) {
                            ofEnableBlendMode(OF_BLENDMODE_ADD);
                            triangContMesh_old.draw();
                            ofDisableBlendMode();
                        }
                    }
                    else		triangContMesh.drawWireframe();
                }
                // tambien se puede dibujar con Mesh
                // Mola porque se puede texturizar y aplicar vertex_shaders
                // ofMesh triangulation.triangleMesh;
            }
            
            //    else triangContMesh.drawWireframe();
            ofPopMatrix();
        }

    }
    
    void setup(ofxOpenNI2Grabber *_oniCamGrabber){
        cargaColores();
        bFill = false;
        bSoloEnContorno = true;
        bDrawOld = false;
        oniCamGrabber=_oniCamGrabber;

        
    }
    
    void cargaColores() {
        // paleta de colores fluor
        // http://www.colourlovers.com/palette/970972/FLUOR
        //
        colorFluor.clear();
        //	colorFluor.push_back(ofColor::red);	// rojo
        //	colorFluor.push_back(ofColor::green);	// green
        //	colorFluor.push_back(ofColor::blue);	// blue
        colorFluor.push_back(ofColor::fromHex(0x0DE0FC));	// melting flowers
        colorFluor.push_back(ofColor::fromHex(0x38FC48));	// Dead Nuclear
        colorFluor.push_back(ofColor::fromHex(0xF938FC));	// Calcinha
        colorFluor.push_back(ofColor::fromHex(0xFA00AB));	// ow!
        colorFluor.push_back(ofColor::fromHex(0xDFFC00));	// Limei Green
        
    }
    
    void setUI(ofxUITabBar *guiTabBar ){
        gui1 = new ofxUICanvas(0,100,400,800);
        gui1->setName("Contours" );
        gui1->addToggle("bFill", &bFill);

        gui1->addToggle("bAddPts", &bAddPts);
        gui1->addIntSlider("num Pts", 0, 500, &numPointsXtra) ;
       // ofAddListener(gui->newGUIEvent,this,&particleClouds::guiEvent);
        gui1->addToggle("doTriang", &doTriang);
        gui1->addToggle("doTessel", &doTessel);
        gui1->addToggle("bAddPts", &bAddPts);
        gui1->addToggle("bFill", &bFill);
        gui1->addIntSlider("modoFill", 0,2, &modoFill);
        gui1->addToggle("bDrawOld", &bDrawOld);
        guiTabBar->addCanvas(gui1);
        
    }

    

    
    
private:
        

        
    bool doTriang;
	ofxDelaunay triangulation;
	bool	bAddPts;		// si añade puntos a la triangulacion
	bool	bSoloEnContorno;	// si muestra solo los triangulos de dentro del contorno
	ofMesh	triangContMesh;
    ofxCv::ContourFinder contourFinder;
    ofPolyline resampledContour;
    
	bool doTessel;
	ofTessellator tessel;
	ofMesh contornoTes;	// contorno teselado
    ofMesh	triangContMesh_old;
    int numPointsXtra = 100;
    bool bFill;
    
    int modoFill;
    // Colores
    vector<ofColor> colorFluor;

    ofImage depthImg;
    ofxOpenNI2Grabber *oniCamGrabber;
    
    bool bDrawOld;
    ofxUICanvas *gui1;
    ofxUITabBar *guiTabBar;
        
    vector<ofPolyline> getSmoothSilhouete1( ofxCv::ContourFinder &contourFinder, ofImage &grayimg,float threshold ){
        
        vector<ofPolyline> silhouettes;
        contourFinder.setSortBySize(true);
        contourFinder.setThreshold(threshold);
        contourFinder.findContours(grayimg);
        ofPolyline convexHull;
        int n = contourFinder.size();
        for(int i = 0; i < n; i++) {
            ofSetColor(yellowPrint);
            convexHull = contourFinder.getPolyline(i);
            convexHull.simplify();
            convexHull=convexHull.getResampledBySpacing(30);
            silhouettes.push_back (convexHull);
            //convexHull.draw();
            
        }
        return silhouettes;
        
    }
};


#endif
