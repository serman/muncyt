#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofSetFrameRate(20);
	myOSCrcv.setup();
#ifndef NOARDUINO
    setupSerial();
#endif
    
    mrect=ofRectangle(0,500,1024,50);
    mrect2=ofRectangle(0,100,1024,100);
    lastTimeSent=ofGetElapsedTimeMillis();
	lastNumberSent=-1;
}

void testApp::setupSerial(){
    bSendSerialMessage = false;
	mySerial.listDevices();
	vector <ofSerialDeviceInfo> deviceList = mySerial.getDeviceList();
	
	// this should be set to whatever com port your serial device is connected to.
	// (ie, COM4 on a pc, /dev/tty.... on linux, /dev/tty... on a mac)
	// arduino users check in arduino app....
	int baud = 9600;
	mySerial.setup(0, baud); //open the first device
	//serial.setup("COM4", baud); // windows example
	//serial.setup("/dev/tty.usbserial-A4001JEC", baud); // mac osx example
	//serial.setup("/dev/ttyUSB0", baud); //linux example
	
	nTimesRead = 0;
	nBytesRead = 0;
	readTime = 0;
	memset(protocolToSend, 0, 7);


}

//--------------------------------------------------------------
void testApp::update(){
    if(myOSCrcv.update()==true){
        //TODO compute position values and trigger the animations
        cout << "myOSCrcv.remotePosition"  << myOSCrcv.remotePosition << "\n";
    }
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackground(0,0,0);

    ofFill();
    ofSetColor(255);
    ofRect(mrect);
    ofRect(mrect2);
}

void testApp::light(char code, int number, ofColor mcolor){
    protocolToSend[0]=code;
    protocolToSend[1]= number & 0x00FF;
    protocolToSend[2]= (number>>8 )& 0x00FF;
    protocolToSend[3]= 128;
    protocolToSend[4]= 128;
    protocolToSend[5]= 128;//mcolor.b;
    protocolToSend[6]= 255;

#ifndef NOARDUINO
    mySerial.writeBytes(&protocolToSend[0], 7);
    cout << ofToString(int(protocolToSend[1])) << " ____ " << ofToString(int(protocolToSend[2])) << " ____" <<  ofToString(int(protocolToSend[3])) <<  "____" << ofToString(int(protocolToSend[4])) << "___"<<  ofToString(int(protocolToSend[6])) <<"\n" ;
//    printf("%");
#else
    cout << protocolToSend << "\n";
#endif
    
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
    int mapeado=ofMap(x, 0, 1024, 0, 120);
    if(mrect.inside(x,y) && (ofGetElapsedTimeMillis()-lastTimeSent>20)
       && mapeado != lastNumberSent ) {
        light('l',mapeado,ofColor(0,0,254) );
        lastTimeSent=ofGetElapsedTimeMillis();
        lastNumberSent=mapeado;
		
    }
    else if(mrect2.inside(x,y) && (ofGetElapsedTimeMillis()-lastTimeSent>20)
       && mapeado != lastNumberSent ) {
        light('s',mapeado,ofColor(0,0,254) );
        lastTimeSent=ofGetElapsedTimeMillis();
        lastNumberSent=mapeado;
		
    }
    

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}
