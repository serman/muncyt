//
//  tunnel.h
//  kinectExample
//
//  Created by Sergio Galan on 9/7/14.
//
//

#ifndef kinectExample_tunnel_h
#define kinectExample_tunnel_h



class tunnel  {
private:

    void enableFog(float near, float far) {
        glEnable(GL_FOG);
        glFogi(GL_FOG_MODE, GL_LINEAR);
        GLfloat fogColor[4]= {0, 0, 0, 1};
        glFogfv(GL_FOG_COLOR, fogColor);
        glHint(GL_FOG_HINT, GL_FASTEST);
        glFogf(GL_FOG_START, near);
        glFogf(GL_FOG_END, far);
    }
    
    void disableFog() {
        glDisable(GL_FOG);
    }
    
	ofxUICanvas *gui;
    int fogNear=400;
    int fogFar=8000;
    int tunnelLength = 64;
    float tunnelRadius = 400;
    float tunnelSeparation = 200;
    float rotationChange = 0.05;
    float moveSpeed = 2500;
    float xRotation = 10;
    float yRotation = 10;
    int circleResolution = 21;
    
public:
    void setUI(ofxUITabBar *guiTabBar ){
        gui = new ofxUICanvas(0,100,400,600);
        gui->setName("Tunnel");
		gui->addIntSlider("fogNear", 0, 1000, &fogNear);
		gui->addIntSlider("fogFar",  0, 10000,&fogFar);
		gui->addIntSlider("tunnelLength", 0, 1000, &tunnelLength);
		gui->addSlider("tunnelSeparation", 0, 1000,&tunnelSeparation);
		gui->addSlider("tunnelRadius", 0, 1000,&tunnelRadius);
		gui->addSlider("rotationChange", 0, 1,&rotationChange);
		gui->addSlider("xRotation",  0, 45, &xRotation);
		gui->addSlider("yRotation",  0, 45, &yRotation);
		gui->addSlider("moveSpeed",  0, 5000,&moveSpeed);
		gui->addIntSlider("circleResolution", 1, 30, &circleResolution);
        guiTabBar->addCanvas(gui);
    }
	
	void setup() {


	}
	
	void update() {
	}
	
	ofVec2f getOrientation(float t) {
        
		int choice = ofNoise(t, 0) * 5;
		switch(choice) {
			case 0: return ofVec2f(-xRotation, 0);
			case 1: return ofVec2f(+xRotation, 0);
			case 2: return ofVec2f(0, -yRotation);
			case 3: return ofVec2f(0, +yRotation);
		}
		return ofVec2f();
	}
	
	void draw() {

		ofPushMatrix();
        
        glEnable(GL_DEPTH_TEST);
   		ofNoFill();

		enableFog(fogNear, fogFar);
        
		float moveOffset = moveSpeed * ofGetElapsedTimef() / tunnelSeparation;
		int moveWhole = floor(moveOffset);
		float moveFraction = fmodf(moveOffset, 1);
		
		ofVboMesh tunnelSegment;
		tunnelSegment.setMode(OF_PRIMITIVE_LINES);
		ofVec3f segmentOffset(0, 0, tunnelSeparation);
		for(int i = 0; i < circleResolution; i++) {
			float theta0 = ofMap(i + 0, 0, circleResolution - 1, 0, 360);
			float theta1 = ofMap(i + 1, 0, circleResolution - 1, 0, 360);
			ofVec2f v0(tunnelRadius, 0), v1(tunnelRadius, 0);
			v0.rotate(theta0);
			v1.rotate(theta1);
			tunnelSegment.addVertex(v0);
			tunnelSegment.addVertex(v1);
			tunnelSegment.addVertex(v0);
			tunnelSegment.addVertex(segmentOffset + v0);
			tunnelSegment.addVertex(segmentOffset + v0);
			tunnelSegment.addVertex(segmentOffset + v1);
		}
		
		ofVec2f lerpOrientation = ofVec2f().interpolate(getOrientation(rotationChange * (1 + moveWhole)), moveFraction);
		ofRotateX(-lerpOrientation.x);
		ofRotateY(-lerpOrientation.y);
		ofTranslate(0, 0, -moveFraction * tunnelSeparation);
		
		for(int i = 0; i < tunnelLength; i++) {
			if(i > 0) {
				ofVec2f orientation = getOrientation(rotationChange * (i + moveWhole));
				ofRotateX(orientation.x);
				ofRotateY(orientation.y);
			}
			ofSetColor(255);
			tunnelSegment.draw();
			ofTranslate(0, 0, tunnelSeparation);
		}
        glDisable(GL_DEPTH_TEST);
        disableFog();
		ofPopMatrix();


	}
};



#endif
