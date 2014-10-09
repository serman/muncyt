//
//  gridView.h
//  kinectExample
//
//  Created by Sergio Galan on 9/2/14.
//
//

#ifndef kinectExample_gridView_h
#define kinectExample_gridView_h

class gridView{
    
public:
    enum {INIT, END, GRID, BLACKHOLE};
    int status=GRID;
    void setup(int _w, int _h, float *_zMin, float *_zMax, ofxOpenNI2Grabber *_oniCamGrabber, extendedDepthSource *_depthGenerator){
        w=_w;
        h=_h;
        temporary_w=0;
        oniCamGrabber=_oniCamGrabber;
        depthGenerator= _depthGenerator;
        zMin=_zMin;
        zMax=_zMax;
        alphaLines=255;
        distanciaLineasK=600;
        stepLines=2;
        shader.load("shaders/fisheye.vert", "shaders/fisheye.frag");
        fbo.allocate(1280, 720,GL_RGB);
        fbo.begin();
        ofClear(0, 0, 0, 0);
        fbo.end();
        mline.loadImage("images/lineGlow.png");
        
    }
    
    void update(){
        switch(status){
            case GRID:
                counter++;
                if(amplitude>0) amplitude-=0.5;
                if(elong_current>elong_goal ){
                    elong_current-=0.01;
                }
                else if(elong_current<elong_goal)
                    elong_current+=0.01;
            break;
        }
        
        
        
        
    }
    
    //lineas que van de la cabeza a los pies
    void drawH(float step = 5.0){
        
        incrDistance+=1;
        std::vector<ofPolyline> lineMesh;


        //    ofPoint lastPoint ;
        ofVec3f lastPoint ;
        
        for(int y = 0; y < h; y += step) { //recorro los puntos bajando por las columnas
            ofPath line ;
            bool bLastValid = false;
            int _xStep = step;
            for(int x = 0; x < temporary_w; x += step) { //recorro columnas
                int distance=depthGenerator->currentRawPixels->getPixels()[y * depthGenerator->width +x];
                if(distance> *zMin && distance < *zMax) {
                    ofVec3f vtmp=oniCamGrabber->convertDepthToWorld(x,y);
                    float xtra=amplitude *
                    cos(waveSeparation* x/(2*PI)+ counter/(2*PI*speed))
                    *cos( waveSeparation*y/(2*PI) + counter / (2*PI*speed) );
                    
                   // ofPoint _lastPoint = vtmp ;
                    //                float dist = abs(vtmp.z - lastPoint.z) ;
                    //                if (  dist < 30  )
                    float dist2 = vtmp.squareDistance(lastPoint);
                    if (  dist2 < distanciaLineasK  )
                    {
                        vtmp.z -= xtra;
                        if ( bLastValid == false )
                        {
                            line.moveTo( vtmp ) ;
                        }
                        else
                        {
                            line.lineTo( vtmp ) ;//addVertex( vertex ) ;
                        }
                        bLastValid = true ;
                            vtmp.z +=xtra;
                            lastPoint = vtmp ;
                    }
                    else
                    {
                        bLastValid = false ;
                        lastPoint = vtmp;
                    }
                    
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
    
    void drawV(float step = 5.0){
        incrDistance+=1;
        std::vector<ofPolyline> lineMesh;
        //    ofPoint lastPoint ;
        ofVec3f lastPoint ;
        
        for(int x = 0; x < temporary_w; x += step) { // recorro la horizontal
            ofPath line;
            bool bLastValid = false;
            //		int _xStep = step;
            for(int y = 0; y < h; y += step) { // recorro columnas
                int distance=depthGenerator->currentRawPixels->getPixels()[y * depthGenerator->width +x];
                if(distance> *zMin && distance < *zMax) {
                    ofVec3f vtmp=oniCamGrabber->convertDepthToWorld(x,y);
                    //if (counter %40==0 && amplitude >490 && x>(w/4) )
                        //cout << ofToString(amplitude * cos( (counter*40+ (x*2+y*2)*2*PI)/300 +1 ) / ((counter*40+ (x*2+y*2)*2*PI)/300)) << "   :::z:: "<< vtmp.z;
                    float xtra=amplitude *
                    cos(waveSeparation* x/(2*PI)+ counter/(2*PI*speed))
                    *cos( waveSeparation*y/(2*PI) + counter / (2*PI*speed) );
                    
                    float dist2 = vtmp.squareDistance(lastPoint);
                    if (  dist2 < distanciaLineasK  )
                    {
                        vtmp.z-=xtra;
                        if ( bLastValid == false )
                        {
                            line.moveTo( vtmp ) ;
                        }
                        else
                        {
                            line.lineTo( vtmp ) ;//addVertex( vertex ) ;
                        }
                        bLastValid = true ;
                        vtmp.z +=xtra;
                        lastPoint = vtmp ;
                    }
                    else
                    {
                        bLastValid = false ;
                        lastPoint = vtmp;
                    }
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
    
    void draw(ofCamera *cam){
       temporary_w= temporary_w<=w ? temporary_w+4: w;

       if(status!=GRID)
           return;
       // fbo.begin();
       // ofClear(0,0,0,0);
      //  ofSetMatrixMode(OF_MATRIX_PROJECTION);
      //  ofLoadMatrix(cam->getProjectionMatrix());
      //  ofSetMatrixMode(OF_MATRIX_MODELVIEW);
        
       // ofLoadMatrix(cam->getModelViewMatrix());
       // ofBackground(255, 0, 0);
       // ofSetColor(255,255,255);
       // ofRect(0,0,400,400);

        ofPushMatrix();
        ofScale(1,elong_current,1);
        ofScale(elong_current,elong_current,elong_current);
            drawV();
            drawH();
        ofPopMatrix();
     //   fbo.end();
 /*
       shader.begin();
      //shader.setUniform3f("camera_pos",p[0],p[1],p[2]);
        shader.setUniform3f("iResolution", ofGetWidth(), ofGetHeight(),0);
        shader.setUniform2f("iMouse",ofGetMouseX(),ofGetMouseY());
        shader.setUniformTexture("maskTex", fbo.getTextureReference(), 1);
        shader.end();
        
        fbo.draw(0,0);
*/
        return;
        ofMesh m;
        ofVbo v;
        m.addVertex(ofVec3f(0,0,0));
        m.addTexCoord(ofVec2f(0,0));
        
        m.addVertex(ofVec3f(fbo.getWidth(),0,0));                 m.addTexCoord(ofVec2f(fbo.getWidth(),0));
        
        m.addVertex(ofVec3f(fbo.getWidth(),fbo.getHeight(),0));     m.addTexCoord(ofVec2f(fbo.getWidth(),fbo.getHeight()));
        m.addVertex(ofVec3f(0,fbo.getHeight(),0));           m.addTexCoord(ofVec2f(0,fbo.getHeight()));
        
        v.setMesh(m, GL_STATIC_DRAW);
        
        fbo.begin();
        ofClear(0,0,0,0);
        
        ofPushMatrix();
        //    ofScale(1,elong_current,1);
        //  ofScale(elong_current,elong_current,elong_current);
        //            drawV();
        //          drawH();
        
        ofSetColor(0,255,0);
        ofLine(0,0,fbo.getWidth(), fbo.getHeight());
        ofNoFill();
        ofRect(0,0,500,500);
        ofPopMatrix();
        fbo.end();
        
        shader.begin();
        //shader.setUniform3f("camera_pos",p[0],p[1],p[2]);
        shader.setUniform2f("iResolution", ofGetWidth(), ofGetHeight());
        shader.setUniform2f("iMouse",ofGetMouseX(),ofGetMouseY());
        shader.setUniformTexture("tex0", fbo.getTextureReference(), 1);
        //        fbo.draw(0,0);
        v.draw(GL_QUADS,0,4);
        shader.end();
        
        fbo.draw(0,0);
        
        
    }
    void draw2D(){
        ofPushStyle();
        ofEnableAlphaBlending();
        ofSetColor(ofColor::white);
        if(temporary_w<w){
            mline.draw(ofMap(temporary_w,0,w,0,ofGetWidth()),0,30,ofGetHeight());
        }
        ofDisableAlphaBlending();
        ofPopStyle();
    }

    void setUI(ofxUITabBar *guiTabBar ){
        gui1 = new ofxUICanvas(0,100,400,800);
        gui1->setName("Grid" );
        gui1->addIntSlider("alpha Lines", 1, 255, &alphaLines) ;
        gui1->addIntSlider("Distance Lines", 50, 600, &distanciaLineasK) ;
        gui1->addIntSlider("stepLines", 2,10, &stepLines);
        gui1->addSlider("elongation", 0.3,3, &elong_goal);
        
        vector<string> names;
        names.push_back("INIT");
        names.push_back("END");
        names.push_back("GRID");
        names.push_back("BLACKHOLE");
        
        gui1->addRadio("MODO_grid", names, OFX_UI_ORIENTATION_HORIZONTAL);
        ofAddListener(gui1->newGUIEvent,this,&gridView::guiEvent);

        
        guiTabBar->addCanvas(gui1);
    }
    
    void guiEvent(ofxUIEventArgs &e)
    {
        string name = e.widget->getName();
        int kind = e.widget->getKind();
        
        if(name == "MODO_grid")
        {
            ofxUIRadio *  wr = (ofxUIRadio *) e.widget;
            ofLogNotice("MODO_grid. " + wr ->getActiveName() + " = " + ofToString(wr->getValue()));
            status = wr->getValue();
        }
        
    }

    
    void setVibration(){
        amplitude=60;
    }
    
    void elongation(float _elong){
        elong_goal=_elong;
    }
    
    void fadeIn(){
        status=GRID;
        temporary_w=0;
        elong_goal=1.0;
        elong_current=1.0;
    }
    
    void initBlackHole(){
        status = BLACKHOLE;
    }
    
    void endBlackHole(){
        
    }
    
    
private:
    float elong_goal=1;
    float elong_current=1;
    int w;
    int h;
    int temporary_w;
    float *zMin;
    float *zMax;
    ofxOpenNI2Grabber *oniCamGrabber;
    extendedDepthSource *depthGenerator;
    int    incrDistance;
    ofxUICanvas *gui1;
    int alphaLines, distanciaLineasK, stepLines;
    int counter=0;
    int amplitude;
    float waveSeparation=0.6; // influye sobre cuantas ondas se ven mas grand emas ondas
    float speed=1.2; //influye sobre la velocidad de las ondas mas pequeño mas rapido
    ofShader shader;
    ofFbo fbo;
                ofImage mline;
};

#endif
