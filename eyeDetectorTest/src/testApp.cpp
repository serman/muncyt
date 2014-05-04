#include "testApp.h"
#include "constants.h"
//PARTE DEL CODIGO LO SACO DE : https://github.com/trishume/eyeLike/tree/master/src PARA DETECTAR LA PUPILA.
//Explicacion: http://thume.ca/projects/2012/11/04/simple-accurate-eye-center-tracking-in-opencv/#fn:2

// HAY VARIOS METODOS MEZCLADOS

using namespace ofxCv;
using namespace cv;

void testApp::setup() {
	ofSetVerticalSync(true);
	ofSetFrameRate(120);
    finder.setup("haarcascade_frontalface_alt2.xml");
	finderEyeL.setup("haarcascade_eye.xml");
	finder.setPreset(ObjectFinder::Fast);
    finder.setMinNeighbors(2);
    finderEyeL.setPreset(ObjectFinder::Accurate);
    finderEyeL.setMaxSizeScale(0.1);
    finderEyeL.setMinNeighbors(2);
    
    finderEyeR.setup("haarcascade_righteye_2splits.xml");
    finderEyeR.setPreset(ObjectFinder::Accurate);
    finderEyes.setup("haarcascade_mcs_eyepair_small.xml");
    finderEyes.setPreset(ObjectFinder::Accurate);
    
	cam.initGrabber(640, 480);
    
    eye1.allocate(ofGetWidth(), ofGetHeight(),OF_IMAGE_COLOR);
    eye2.allocate(ofGetWidth(), ofGetHeight(),OF_IMAGE_COLOR);
    eyeGray.allocate(ofGetWidth(), ofGetHeight(),OF_IMAGE_GRAYSCALE);
    
    thresholdI=60;
}
cv::Mat eyes[2];

void testApp::update() {
	cam.update();
	if(cam.isFrameNew()) {
		finder.update(cam);
        for(int i = 0; i < finder.size(); i++) {
            ofRectangle object = finder.getObjectSmoothed(i);
            cv::Mat imgMat = toCv(cam);
            cv::Mat face(imgMat,cv::Rect(object.getX(), object.getY(), object.getWidth(), object.getHeight()) );
            finderEyes.update(face);
            cv::Rect faceR=toCv(object);
            if(finderEyes.size()>0){
                cv::Rect roiEyes = toCv(finderEyes.getObjectSmoothed(0));
                cv::Mat eyes(face,roiEyes);
                //detect eyes within face
                finderEyeL.update(face);
                finderEyeR.update(face);
                //if(finderEyeL.size()>0){
                    ofRectangle eyesRect=finderEyes.getObjectSmoothed(0);
                
                
                    //ESTA PARTE LA USO PARA DIVIDOR LOS DOS OJOS PERO ME LA HE SACADO DE LA MANGA (DIVIDO LA IMG EN 3 TERCIOS OJO NARIZ OJO(
                    ofRectangle eyel(0,0,floor(eyesRect.width/3),eyesRect.height);
                    ofRectangle eyer(floor(2 * (eyesRect.width/3)), 0 , floor(eyesRect.width/3), eyesRect.height);

                    cv::Rect roil = toCv(eyel);
                    cv::Mat eyeM1(eyes,roil) ;
                    resize(eyeM1, eye1); eye1.update();
                    
                    cv::Rect roir = toCv(eyer);
                    cv::Mat eyeM2(eyes,roir) ;
                    resize(eyeM2, eye2); eye2.update();
                
                //ESTA ES LA FORMA EN LA QUE SE HACE SEGUN EL ALGORITMO DE DETECCION POR GRADIENTE
                    int eye_region_width = faceR.width * (kEyePercentWidth/100.0);
                    int eye_region_height = faceR.width * (kEyePercentHeight/100.0);
                    int eye_region_top = faceR.height * (kEyePercentTop/100.0);
                    cv::Rect leftEyeRegion(faceR.width*(kEyePercentSide/100.0),
                                           eye_region_top,eye_region_width,eye_region_height);
		            cv:Mat image_gray1;
                	cv::cvtColor(face, image_gray1, CV_RGB2GRAY);
	                leftPupil = findEyeCenter(image_gray1,leftEyeRegion,"Left Eye");
                    

            }
        }
	}
}

void testApp::draw() {
	cam.draw(0, 0);
	finder.draw();
    for(int i = 0; i < finder.size(); i++) {
        ofPushMatrix();
   	 	ofTranslate(finder.getObjectSmoothed(0).getX(), finder.getObjectSmoothed(0).getY()) ;
        finderEyes.draw();
    	//finderEyeL.draw();
        //finderEyeR.draw();
        ofPoint pupila=toOf(leftPupil);
        ofPushMatrix();
        ofTranslate(finderEyeL.getObjectSmoothed(0).getX(),finderEyeL.getObjectSmoothed(0).getY());
        ofRect(pupila.x,pupila.y,2,2);
        ofPopMatrix();
    	ofPopMatrix();
    }
    
    Mat img2=toCv(eye1);
    
    // initialize the output matrix with zeros
    Mat new_image;// = Mat::zeros( img2.size(), img2.type() );
    Mat image_gray;// = Mat::zeros( img2.size(), img2.type() );
    Mat image_binary;// = Mat::zeros( img2.size(), img2.type() );
    //bitwise_not(img2,new_image); //invert

    cv::cvtColor(img2, image_gray, CV_RGB2GRAY); //CONVERTIR UNA IMG A GRIS
    threshold(image_gray, image_binary, thresholdI); //INVERTIR IMAGEN ESTO NO LO ESTOY USANDO AL FIAN
//    cv:: adaptiveThreshold(image_gray, image_binary, 255, CV_ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY_INV,3,5 );
    toOf(image_binary,eyeGray);
    
    eyeGray.update();
    eye1.draw(400,0,eye1.width/6,eye1.height/6);
    eye2.draw(500,0,eye2.width/6,eye2.height/6);
    eyeGray.draw(400,110,eyeGray.width/6,eyeGray.height/6);
    
	ofDrawBitmapStringHighlight(ofToString(finder.size()), 10, 20);
}

void testApp::keyPressed(int key){
    if(key == 'm'){
        thresholdI+=10;
    }else if(key=='l'){
        thresholdI-=10;
    }
    cout<<thresholdI;
}



