//
//  puntosFormas.h
//  kinectExample
//
//  Created by Sergio Galan on 10/24/14.
//
//

#ifndef kinectExample_puntosFormas_h
#define kinectExample_puntosFormas_h
class puntosFormas{
public:
    ofImage palette, paletteRed, paletteGreen,paletteBlue;
    ofImage *selectedPalette;
    enum colores{ROJOS=0, VERDES, AZULES, FLUOR};
    void setup(int _w, int _h, float *_zMin, float *_zMax, ofxOpenNI2Grabber *_oniCamGrabber, extendedDepthSource *_depthGenerator, ofCamera *_cam){
        w=_w;
        h=_h;
        oniCamGrabber=_oniCamGrabber;
        depthGenerator= _depthGenerator;
        zMin=_zMin;
        zMax=_zMax;
        speed = 1.0;
        stopUmbral = 10000;
        alphaParticles = 255;
        cam=_cam;
        palette.loadImage("palette1.png");
        paletteRed.loadImage("palette_red.png");
        paletteGreen.loadImage("palette_green.png");
        paletteBlue.loadImage("palette_blue.png");
        selectedPalette=&paletteBlue;
    }

    void drawWithRectangles(){
                cameraMoves::getInstance()->zoomInAndCenter();
        for ( int y = 0 ; y < h ; y+=5 ){
            for ( int x = w ; x > 0 ; x-=5 ){
                int distance=depthGenerator->currentRawPixels->getPixels()[y * depthGenerator->width + x];
                if(distance> *zMin && distance < *zMax) {
                    ofVec3f mdestPoint=  oniCamGrabber->convertDepthToWorld(x,y);
                    //ofColor c= oniCamGrabber->rgbSource.currentPixels->getColor(x,y);
                    //ofSetColor(ofColor::fromHsb(150+ 100* ofNoise(0.4+ofGetElapsedTimeMillis()/100000), 200, 200));
                    ofNoFill();
                    ofSetColor(ofColor::white);
                    ofPushMatrix();
                    ofTranslate(mdestPoint.x,mdestPoint.y,mdestPoint.z);
                    if((x+y)%10==0){
                        ofRotateY(ofGetElapsedTimeMillis()/3);
                    }
                    ofRect(-4,-4,8,8);
                    ofFill();
                    //                     ofSetColor(c);
                    //                     ofSetColor(ofColor::fromHsb(50+ 100* ofNoise(0.4+ofGetElapsedTimeMillis()/1000+(x+y)), 200, 200));
                    ofSetColor(
                               selectedPalette->getColor(x+100*ofNoise( (float) ofGetElapsedTimeMillis()/1000 ),y+50*ofNoise( (float) ofGetElapsedTimeMillis()/1000 ))
                               );
                    ofRect(-4,-4,8,8);
                    ofPopMatrix();
                }
            }
        }
    }

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

private:
    ofxOpenNI2Grabber *oniCamGrabber;
    extendedDepthSource *depthGenerator;
    int numParticles;
    int mode;
    int w;
    int h;
        ofCamera *cam;
};

#endif
