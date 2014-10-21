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
    enum colores{ROJOS=0, AZULES,VERDES,FLUOR};
    ofImage paletteRadio, paletteBlackhole;
    vector<ofPolyline> v;
    int numPointsXtra;
        bool bFill;
        int modoFill;
    void setup(ofxOpenNI2Grabber *_oniCamGrabber){
        cargaColores();
        bFill = false;
        bSoloEnContorno = true;
        bDrawOld = false;
        oniCamGrabber=_oniCamGrabber;
        selectedColors=colorFluor;
        paletteRadio.loadImage("palette_radiactive.png");
        paletteBlackhole.loadImage("palette_Blackhole.png");
        numPointsXtra = 5;

    }
    
    ofPolyline *getMainSilhouette(){
        if(v.size()>0){
            if(v[0].size()>5)
                return &(v[0]);
        }
        return NULL;
    }
    
    void update(){
        depthImg.setFromPixels( oniCamGrabber->getDepthPixels());
        depthImg.update();

        v.clear();
        v= getSmoothSilhouete1(contourFinder, depthImg, (float)0.1/10);
        if(v.size()>0){
            resampledContour =v[0];
        }
        //decrease();
        
        //Set drawing status according to the state of the scene
        
        if(numPointsXtra<10){ bAddPts=false; doTriang=false;}
        if(numPointsXtra>10) {bAddPts=true; doTriang=true;}
        if(numPointsXtra<100) bFill=false;
        else bFill=true;
        
    }
    
    void reset(){
        numPointsXtra = 5;
    }
    
    void draw(ofPolyline *mcontour=NULL ){        
        //NIAPILLA. SI NO LE PASAS NADA DIBUJA POR DEFECTO LA SILUETA LOCAL. SI LE PASAS OTRO POLYLINE ( EL REMOTO)
        //LO DIBUJA
        if(mcontour==NULL)
            mcontour=&resampledContour;
        if(mcontour->size()>=3) {
            triangulation.reset();
            for(int i=0; i<mcontour->size(); i++) {
                triangulation.addPoint((*mcontour)[i]);
            }
            if(bAddPts) {
                if(mcontour->size()>umbralPoints && bRegeneratePoints==true){
                    puntosExtraInit(mcontour);
                    bRegeneratePoints=false;
                    ofDrawBitmapString("REGENERATING POINTS: ", ofGetWidth()-300,500 );
                }
                if(mcontour->size()<umbralPoints-3 && bRegeneratePoints==false){
                    bRegeneratePoints=true;
                }
                
                ofRectangle bounds = mcontour->getBoundingBox();
                puntosExtraUpdate(mcontour);
                for(int i=0; i<extraPoints.size(); i++) {
                    if( mcontour->inside(extraPoints[i]) ){
                        triangulation.addPoint(extraPoints[i]);
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
                
                if( mcontour->inside(ofPoint(pm.x,pm.y)) ) {
                    // supongo el area de imagen dividido en 10 franjas divididas a su vez en
                    // colorFluor.size() bandas de distinto color
                    float hBandaPpal = ofGetHeight()/10;
                    
                    float hBanda = hBandaPpal/selectedColors.size();
                    
                    ofColor ctmpa, ctmpb, ctmpc;
                    if(modoFill==0) {
                        // RANDOM
                        ofColor ctmp= paletteRadio.getColor( ofClamp(pm.x,0,paletteRadio.getWidth() ) , ofClamp(pm.y,0,paletteRadio.getHeight())
                                                            );
//                        ofColor ctmp = selectedColors[i%selectedColors.size()];
                        ctmpa = ctmp;
                        ctmpb = ctmp;
                        ctmpc = ctmp;
                    }
                    else if(modoFill==1) {
                        // asignar color segun la posicion del punto medio
                        int nBanda = floor(pm.y/hBanda);
                        ofColor ctmp = selectedColors[nBanda%selectedColors.size()];
                        ctmpa = ctmp;
                        ctmpb = ctmp;
                        ctmpc = ctmp;
                    }
                    else if(modoFill==2) {
                        int nBanda = floor(a.y/hBanda);
                        ctmpa = selectedColors[nBanda%selectedColors.size()];
                        nBanda = floor(b.y/hBanda);
                        ctmpb = selectedColors[nBanda%selectedColors.size()];
                        nBanda = floor(c.y/hBanda);
                        ctmpc = selectedColors[nBanda%selectedColors.size()];
                    }
                    if(modoFill==3) {
                        // RANDOM
                        ofColor ctmp= paletteBlackhole.getColor( ofClamp(pm.x,0,paletteRadio.getWidth() ) , ofClamp(pm.y,0,paletteRadio.getHeight())
                                                            );
                        ctmpa = ctmp;
                        ctmpa.a=150;
                        ctmpb = ctmp;
                        ctmpb.a=150;
                        ctmpc = ctmp;
                        ctmpc.a=150;
                        
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
           // ofTranslate(ofGetWidth(), ofGetHeight());
            //ofRotateZ(180);
                    //ofRect(0,0,ofGetWidth(), ofGetHeight());
            ofFill();            
            ofSetColor(255, 255, 0);
            ofSetLineWidth(1);
            ofScale(1,1,1);
            
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
            ofDrawBitmapString("tamanio contour: " + ofToString( mcontour->size()) + "\n", ofGetWidth()-300,300 );
        }

    }
    
    void drawSimple(ofPolyline *mcontour=NULL ){
        if(mcontour==NULL) return;
        bAddPts=true;
        numPointsXtra=100;
        modoFill=3;
        bFill=true;
        ofRectangle bounds =mcontour->getBoundingBox();
        addImpacts(bounds );
        draw(mcontour);
        
    }
    

    
    
    
    void addImpacts(ofRectangle &bounds){
        int pointGoals=100;
        if( numPointsXtra<pointGoals){
            numPointsXtra+=5;
            for(int i=0; i<5; i++){
                float px = bounds.x+ofRandom(bounds.width);
                float py = bounds.y+ofRandom(bounds.height);
                extraPoints.push_back(ofPoint(px, py));
            }
        }
    }
    void addImpact(int px, int py){
       
        if(numPointsXtra<100){
             numPointsXtra+=1;
            extraPoints.push_back(ofPoint(px, py));
        }

    }
    
    void decrease(){
        int pointGoals=5;
        if( numPointsXtra>pointGoals)
        {
            numPointsXtra--;
            if(extraPoints.size()>pointGoals)
                extraPoints.erase(extraPoints.begin(),extraPoints.begin()+3);
        }
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
        
        colorFluorGreen.push_back(ofColor::fromHex(0x9CFE30));
        colorFluorGreen.push_back(ofColor::fromHex(0x39EDC0));
        colorFluorGreen.push_back(ofColor::fromHex(0xBBC848));
        colorFluorGreen.push_back(ofColor::fromHex(0x00E331));
        colorFluorGreen.push_back(ofColor::fromHex(0xDDFF54));
        
        colorFluorBlue.push_back(ofColor::fromHex(0x04F2FF));
        colorFluorBlue.push_back(ofColor::fromHex(0x3C3FFC));
        colorFluorBlue.push_back(ofColor::fromHex(0x0902A8));
        colorFluorBlue.push_back(ofColor::fromHex(0x2A8AC4));
        colorFluorBlue.push_back(ofColor::fromHex(0x596BA2));
        colorFluorBlue.push_back(ofColor::fromHex(0x1CB5C5));
        
        colorFluorRed.push_back(ofColor::fromHex(0xFF5988));
        colorFluorRed.push_back(ofColor::fromHex(0xCF3ED6));
        colorFluorRed.push_back(ofColor::fromHex(0xFF7A1F));
        colorFluorRed.push_back(ofColor::fromHex(0xFF0000));
        colorFluorRed.push_back(ofColor::fromHex(0xFF965C));
        colorFluorRed.push_back(ofColor::fromHex(0xE426AA));
        
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
        gui1->addIntSlider("modoFill", 0,2, &modoFill);
        gui1->addToggle("bDrawOld", &bDrawOld);
        guiTabBar->addCanvas(gui1);
        
    }
    
    void setColor( int tipoColor){
        switch(tipoColor){
            case ROJOS:
                selectedColors=colorFluorRed;
                break;
            case VERDES:
                selectedColors=colorFluorGreen;
                break;
            case AZULES:
                selectedColors=colorFluorBlue;
                break;
            case FLUOR:
                selectedColors=colorFluor;
                break;
        }
    }

    void puntosExtraReset(){
        extraPoints.clear();
    }
    
    void puntosExtraInit(ofPolyline *mcontour){
        ofRectangle bounds = mcontour->getBoundingBox();
        for(int i=0; i<numPointsXtra; i++) {
            float px = bounds.x+ofRandom(bounds.width);
            float py = bounds.y+ofRandom(bounds.height);
            extraPoints.push_back(ofPoint(px, py));
        }
    }
    
    void puntosExtraUpdate(ofPolyline *mcontour){
        ofRectangle bounds = mcontour->getBoundingBox();
        for(int i=0; i<extraPoints.size(); i++) {
            if(mcontour->inside(extraPoints[i]) ){
                extraPoints[i].x+=40*(ofNoise(dt)-0.5);
                extraPoints[i].y+=40*(ofNoise(50+dt)-0.5);
            }
            else{
                extraPoints[i].x = bounds.x+ofRandom(bounds.width);
                extraPoints[i].y = bounds.y+ofRandom(bounds.height);
            }
            dt+=0.01;
        }
    }
    
    
private:
    
    vector<ofPoint> extraPoints;
    int umbralPoints=30;
    int bRegeneratePoints=true;
    
    float dt=0.05;
    bool doTriang;
	ofxDelaunay triangulation;
	bool	bAddPts;		// si añade puntos a la triangulacion
	bool	bSoloEnContorno;	// si muestra solo los triangulos de dentro del contorno
	ofMesh	triangContMesh;
    ofxCv::ContourFinder contourFinder;
    ofPolyline resampledContour; //local contour
    
	bool doTessel;
	ofTessellator tessel;
	ofMesh contornoTes;	// contorno teselado
    ofMesh	triangContMesh_old;
    

    // Colores
    vector<ofColor> colorFluor;
    vector<ofColor> colorFluorGreen;
    vector<ofColor> colorFluorRed;
    vector<ofColor> colorFluorBlue;

    vector<ofColor> selectedColors;
    
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
            convexHull=convexHull.getResampledBySpacing(50);
            silhouettes.push_back (convexHull);
            //convexHull.draw();
            
        }
        return silhouettes;
        
    }
};


#endif
