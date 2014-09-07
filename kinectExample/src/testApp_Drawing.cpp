//
//  testApp_Drawing.cpp
//  kinectExample
//
//  Created by Sergio Galan on 7/15/14.
//
//

#include "testApp.h"





//native point cloud
void testApp::drawPointCloud(int step) {
mesh.clear();

int w = IRCAMERAWIDTH;
int h = IRCAMERAHEIGHT;

incrDistance+=1;
mesh.setMode(OF_PRIMITIVE_POINTS);
for(int y = 0; y < h; y += step) { //recorro los puntos bajando por las columnas
    for(int x = 0; x < w; x += step) { //recorro columnas

            int distance=depthGenerator.currentRawPixels->getPixels()[y * depthGenerator.width +x];
            if(distance> zMin && distance < zMax) {
                ofVec2f p2   = ofVec2f(x,y);
                ofVec3f vtmp=oniCamGrabber.convertDepthToWorld(x,y);
                mesh.addColor(ofColor(255,255,255,ofMap(mouseX,1,ofGetWidth(),100,255 ) ) );
                mesh.addVertex(vtmp);
                
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


/*void testApp::drawLinesH(float step){
    
	int w = IRCAMERAWIDTH;
	int h = IRCAMERAHEIGHT;
    
    incrDistance+=1;
    std::vector<ofPolyline> lineMesh;
    ofColor col ;
    float _time = ofGetElapsedTimef() ;
    float theta = sin ( ofGetElapsedTimef() ) ;
    ofVec2f v1=ofVec2f(IRCAMERAWIDTH,IRCAMERAWIDTH);
    ofPolyline lineMesh1;
    //    ofPoint lastPoint ;
    ofVec3f lastPoint ;
    
	for(int y = 0; y < h; y += step) { //recorro los puntos bajando por las columnas
        ofPath line ;
		bool bLastValid = false;
		int _xStep = step;
		for(int x = 0; x < w; x += step) { //recorro columnas
            
            int distance=depthGenerator.currentRawPixels->getPixels()[y * depthGenerator.width +x];
            if(distance> zMin && distance < zMax) {
                ofVec3f vtmp=oniCamGrabber.convertDepthToWorld(x,y);
                
                ofPoint _lastPoint = vtmp ;
                //                float dist = abs(vtmp.z - lastPoint.z) ;
                //                if (  dist < 30  )
				float dist2 = vtmp.squareDistance(lastPoint);
                if (  dist2 < distanciaLineasK  )
                {
                    if ( bLastValid == false )
                    {
                        line.moveTo( vtmp ) ;
                    }
                    else
                    {
                        line.lineTo( vtmp ) ;//addVertex( vertex ) ;
                    }
                    bLastValid = true ;
                    
                }
                else
                {
                    bLastValid = false ;
                }
                lastPoint = vtmp ;
            }
        }
        ofColor c =  ofColor( 35 , 255 , 24 , alphaLines) ;
		line.setColor( c ) ;
        line.setFilled( false ) ;
        line.setStrokeColor( c ) ;
        line.setStrokeWidth( 1);
        line.draw();
	}
}

void testApp::drawLinesV(float step){
    
    
    int w= oniSettings.width;
    int h= oniSettings.height;
    incrDistance+=1;
    std::vector<ofPolyline> lineMesh;
    ofColor col ;
    float _time = ofGetElapsedTimef() ;
    float theta = sin ( ofGetElapsedTimef() ) ;
    ofVec2f v1=ofVec2f(w,h);
    ofPolyline lineMesh1;
    //    ofPoint lastPoint ;
    ofVec3f lastPoint ;
    
	for(int x = 0; x < w; x += step) { // recorro la horizontal
        ofPath line;
		bool bLastValid = false;
        //		int _xStep = step;
		for(int y = 0; y < h; y += step) { // recorro columnas
            
            int distance=depthGenerator.currentRawPixels->getPixels()[y * depthGenerator.width +x];
            if(distance> zMin && distance < zMax) {
	            ofVec3f vtmp=oniCamGrabber.convertDepthToWorld(x,y);
				float dist2 = vtmp.squareDistance(lastPoint);
                if (  dist2 < distanciaLineasK  )
                {
                    if ( bLastValid == false )
                    {
                        line.moveTo( vtmp ) ;
                    }
                    else
                    {
                        line.lineTo( vtmp ) ;//addVertex( vertex ) ;
                    }
                    bLastValid = true ;
                }
                else
                {
                    bLastValid = false ;
                }
                lastPoint = vtmp;
            }
        }
		
        ofColor c =  ofColor( 35 , 255 , 24 , alphaLines) ;
		line.setColor( c ) ;
        line.setFilled( false ) ;
        line.setStrokeColor( c ) ;
        line.setStrokeWidth( 1);
        line.draw();
	}
}*/

void testApp::drawAxis(){

    ofSetColor (ofColor::blue);
    ofLine(0, 0, 2000, 0);
    ofSetColor (ofColor::white);
    ofLine(0, 0, 0, 2000);
    ofSetColor (ofColor::yellow); //z
    ofLine(0, 0, 0, 0, 0, 3000);
}

void testApp::setRandomBG(){
    colorfondo = ofColor::fromHsb(ofRandom(360),255,255);
}

void testApp::fadeBG() {
    float bb = colorfondo.getBrightness();
	if(bb<2) colorfondo.setBrightness(0);
    else colorfondo.setBrightness(bb*0.95);
    
}
