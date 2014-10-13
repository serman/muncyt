//
//  rectangles.h
//  kinectExample
//
//  Created by Sergio Galan on 10/5/14.
//
//

#ifndef kinectExample_rectangles_h
#define kinectExample_rectangles_h
#include "ofxDelaunay.h"
class dela{    
public:
    enum colores{ROJOS=0, AZULES,VERDES,FLUOR};
    void setup(int _w, int _h, float *_zMin, float *_zMax, ofxOpenNI2Grabber *_oniCamGrabber, extendedDepthSource *_depthGenerator, ofCamera *_cam){
        w=_w;
        h=_h;
        oniCamGrabber=_oniCamGrabber;
        depthGenerator= _depthGenerator;
        zMin=_zMin;
        zMax=_zMax;
       // setupParticles();
        speed = 1.0;
        stopUmbral = 10000;
        alphaParticles = 255;
       // cam=_cam;
       // setupParticles();
        blob.allocate(640,480,OF_IMAGE_GRAYSCALE);
        setColours();
        palette.loadImage("palette1.png");
        paletteRed.loadImage("palette_red.png");
        paletteGreen.loadImage("palette_green.png");
        paletteBlue.loadImage("palette_blue.png");
        selectedPalette=&paletteBlue;
    }
    ofImage palette, paletteRed, paletteGreen,paletteBlue;
    ofImage *selectedPalette;
    void update(){
        del.reset();
        
		
		unsigned char* pix = new unsigned char[640*480];
		
		unsigned char* gpix = new unsigned char[640*480];
		
		for(int x=0;x<640;x+=1) {
			for(int y=0;y<480;y+=1) {
				float distance = depthGenerator->currentRawPixels->getPixels()[y * depthGenerator->width + x];
				
				int pIndex = x + y * 640;
				pix[pIndex] = 0;
                
				if(distance > *zMin && distance < *zMax) {
					pix[pIndex] = 255;
				}
                
			}
		}
		
		blob.setFromPixels(pix, 640, 480, OF_IMAGE_GRAYSCALE);
		
		int numPoints = 0;
		
		for(int x=0;x<640;x+=pointSkip*2) {
			for(int y=0;y<480;y+=pointSkip*2) {
				int pIndex = x + 640 * y;
                
				if(blob.getPixels()[pIndex]> 0) {
					ofVec3f wc = oniCamGrabber->convertDepthToWorld(x,y);
					//parece que los centra entorno al punto 0
					wc.x = x - 320.0;
					wc.y = y - 240.0;
					
					if(abs(wc.z) > 100 && abs(wc.z ) < 2000) {
						
						wc.z = -wc.z;
						
						wc.x += ofSignedNoise(wc.x,wc.z)*noiseAmount;
						wc.y += ofSignedNoise(wc.y,wc.z)*noiseAmount;
						
						wc.x = ofClamp(wc.x, -320,320);
						wc.y = ofClamp(wc.y, -240,240);
						
						del.addPoint(wc);
					}
					numPoints++;
				}
                
			}
		}
        
		
		if(numPoints >0)
			del.triangulate();
		
		for(int i=0;i<del.triangleMesh.getNumVertices();i++) {
			del.triangleMesh.addColor(ofColor(0,0,0));
		}
		
		for(int i=0;i<del.triangleMesh.getNumIndices()/3;i+=1) {
			ofVec3f v = del.triangleMesh.getVertex(del.triangleMesh.getIndex(i*3));
			
			v.x = ofClamp(v.x, -319,319);
			v.y = ofClamp(v.y, -239, 239);
            
			ofColor c = oniCamGrabber->rgbSource.currentPixels->getColor(v.x+320.0, v.y+240.0);
            
			if(!useRealColors){
//				c = ofColor(255,0,0);
                //c= colorFluor[ ofRandom(colorFluor.size())];
                ofVec3f punto=del.triangleMesh.getVertices()[i]; //getIndex(i*3)
                punto.x+=320;
                punto.y+=240;
                 changeColorConst+=changeColorFactor;
                int changeFactor1=(selectedPalette->getWidth()/6) * (ofNoise( changeColorConst)-0.5 );
                changeColorConst+=changeColorFactor;
                int changeFactor2=(selectedPalette->getHeight()/6) * (ofNoise( 10+changeColorConst)-0.5);
                
                punto.x+=changeFactor1;
                punto.y+=changeFactor2;
                punto.x=ofClamp(punto.x,0,selectedPalette->getWidth()-1);
                punto.y=ofClamp(punto.y,0,selectedPalette->getHeight()-1);
                
                c= selectedPalette->getColor(punto.x,punto.y);

            }
            
			c.a = colorAlpha;
			
			del.triangleMesh.setColor(del.triangleMesh.getIndex(i*3),c);
			del.triangleMesh.setColor(del.triangleMesh.getIndex(i*3+1),c);
			del.triangleMesh.setColor(del.triangleMesh.getIndex(i*3+2),c);
		}
        
        
        
        
        convertedMesh.clear();
        wireframeMesh.clear();
        wireframeMesh.setMode(OF_PRIMITIVE_TRIANGLES);
        for(int i=0;i<del.triangleMesh.getNumIndices()/3;i+=1) {
            
            int indx1 = del.triangleMesh.getIndex(i*3);
            ofVec3f p1 = del.triangleMesh.getVertex(indx1);
            int indx2 = del.triangleMesh.getIndex(i*3+1);
            ofVec3f p2 = del.triangleMesh.getVertex(indx2);
            int indx3 = del.triangleMesh.getIndex(i*3+2);
            ofVec3f p3 = del.triangleMesh.getVertex(indx3);
            
            ofVec3f triangleCenter = (p1+p2+p3)/3.0;
            triangleCenter.x += 320;
            triangleCenter.y += 240;
            
            triangleCenter.x = floor(ofClamp(triangleCenter.x, 0,640));
            triangleCenter.y = floor(ofClamp(triangleCenter.y, 0, 480));
            
            int pixIndex = triangleCenter.x + triangleCenter.y * 640;
            if(pix[pixIndex] > 0) {
                
                convertedMesh.addVertex(p1);
                convertedMesh.addColor(del.triangleMesh.getColor(indx1));
                
                convertedMesh.addVertex(p2);
                convertedMesh.addColor(del.triangleMesh.getColor(indx2));
                
                convertedMesh.addVertex(p3);
                convertedMesh.addColor(del.triangleMesh.getColor(indx3));
                
                wireframeMesh.addVertex(p1);
                wireframeMesh.addVertex(p2);
                wireframeMesh.addVertex(p3);
            }
        }
        
        delete pix;
        delete gpix;
        
    }

    void draw(){
        ofSetColor(255,255,255);
        ofScale(1,-1,-1);
//        ofTranslate(0, 0,-1000);
        ofFill();
       // glPushAttrib(GL_ALL_ATTRIB_BITS);
       // glShadeModel(GL_FLAT);
       // glProvokingVertex(GL_FIRST_VERTEX_CONVENTION);
                ofSetColor(200);
        convertedMesh.drawFaces();
       // glShadeModel(GL_SMOOTH);
       // glPopAttrib();
        
        /*if(useRealColors) {
            ofSetColor(30,30,30, 255);
        } else
            ofSetColor(124,136,128,255);
        */
        ofPushMatrix();
        ofTranslate(0, 0,0.5);
        ofSetColor(200);
        wireframeMesh.drawWireframe();
        ofPopMatrix();
       // cam.end();
    }
    
    void setColours(){
        colorFluor.clear();
        colorFluor.push_back(ofColor::fromHex(0x0DE0FC));	// melting flowers
        colorFluor.push_back(ofColor::fromHex(0x38FC48));	// Dead Nuclear
        colorFluor.push_back(ofColor::fromHex(0xF938FC));	// Calcinha
        colorFluor.push_back(ofColor::fromHex(0xFA00AB));	// ow!
        colorFluor.push_back(ofColor::fromHex(0xDFFC00));	// Limei Green
    }
    
    void setUI(ofxUITabBar *guiTabBar ){
        gui = new ofxUICanvas(0,100,400,800);
        gui->setName("Delaunay 3D" );
        
        gui->addIntSlider("pointSkip", 1, 30,  &(pointSkip));
        gui->addIntSlider("colorAlpha", 1, 255,  &(colorAlpha));
        gui->addSlider("changeColorFactor", 0, 1.0,  &(changeColorFactor));
        gui->addSlider("noiseAmount", 0, 10,  &(noiseAmount));
        gui->addToggle("useRealColors",&useRealColors);
        guiTabBar->addCanvas(gui);
    }
    int		cloudState;
    float *zMin, *zMax;
    float speed;
    float acceleration;
    int stopUmbral;
    int alphaParticles;
    bool bRealColors;
    float rotateY=0; float rotateZ=0; float rotateX=0;
    float tranX=0;
    float tranY=0;
    float tranZ=0;
    
    void setColor( int tipoColor){
        switch(tipoColor){
            case ROJOS:
                selectedPalette=&paletteRed;
                break;
            case VERDES:
                selectedPalette=&paletteGreen;
                break;
            case AZULES:
                selectedPalette=&paletteBlue;
                break;
            case FLUOR:
                selectedPalette=&palette;
                break;
        }
    }
    
private:
    int numParticles;
    int mode;
    int w;
    int h;
    int sampling;
    vector<Particle> particles ;
    ofMesh meshParticles;
    ofxUICanvas *gui;
    bool fadeIn_b;
    ofxOpenNI2Grabber *oniCamGrabber;
    extendedDepthSource *depthGenerator;
    
    ofMesh convertedMesh;
	ofMesh wireframeMesh;
	ofxDelaunay del;
	ofImage blob;
    int pointSkip=6;
    bool useRealColors=false;
    float noiseAmount=3;
    int colorAlpha=255;
    vector<ofColor> colorFluor;
    float changeColorConst=0.0;
    float changeColorFactor=0.01;
};


#endif
