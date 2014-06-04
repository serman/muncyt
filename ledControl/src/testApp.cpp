#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofSetFrameRate(60);
	myOSCrcv.setup();
    ledNumber=0;
#ifndef NOARDUINO
    setupSerial();
#endif
    red=32;green=32;blue=32;
    mrect=ofRectangle(0,500,1024,50);
    mrect2=ofRectangle(0,100,1024,100);
    lastTimeSent=ofGetElapsedTimeMillis();
	lastNumberSent=-1;
    memset(protocolToSend, PKTLENGTH, sizeof(char));
    protocolToSend[0]='l';
	protocolToSend[13]='\n';
    gui1 = new ofxUICanvas(500, 500, 524,250);
    
    
    gui1->addIntSlider("pixel", 0, 120, &ledNumber);
    gui1->addIntSlider("red", 0, 255, &red);
    gui1->addIntSlider("green", 0, 255, &green);
    gui1->addIntSlider("blue", 0, 255, &blue);
    
    gui1->addToggle("mode", false);
    ofAddListener(gui1->newGUIEvent,this,&testApp::gui1Event);
    TO.setup(&mySerial, &protocolToSend[0] );
    TO.start();
}

void testApp::setupSerial(){
    bSendSerialMessage = false;
	mySerial.listDevices();
	vector <ofSerialDeviceInfo> deviceList = mySerial.getDeviceList();
	
	// this should be set to whatever com port your serial device is connected to.
	// (ie, COM4 on a pc, /dev/tty.... on linux, /dev/tty... on a mac)
	// arduino users check in arduino app....
	int baud = 57600;
	mySerial.setup(0, baud); //open the first device
	//serial.setup("COM4", baud); // windows example
	//serial.setup("/dev/tty.usbserial-A4001JEC", baud); // mac osx example
	//serial.setup("/dev/ttyUSB0", baud); //linux example
	
	nTimesRead = 0;
	nBytesRead = 0;
	readTime = 0;



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
    ofBackground(40,40,40);

    ofFill();
    ofSetColor(255);
    //ofRect(mrect);
    //ofRect(mrect2);
}

void testApp::light(char code, int number, ofColor mcolor){
    TO.lock();
    protocolToSend[0]=code;
	intToChar(&protocolToSend[1] ,number, 3 );
    intToChar(&protocolToSend[4] ,red, 3);
    intToChar(&protocolToSend[7] ,green, 3);
    intToChar(&protocolToSend[10] ,blue, 3);
    protocolToSend[13]='\n';
	TO.unlock();
/*    protocolToSend[1]= number & 0x00FF;
    protocolToSend[2]= (number>>8 )& 0x00FF;
    protocolToSend[3]= 64;
    protocolToSend[4]= 64;
    protocolToSend[5]= 64;//mcolor.b;
    protocolToSend[6]= 64;//mcolor.b;
    protocolToSend[7]= 0; //bytes para otra cosa
    protocolToSend[8]= 0;
    protocolToSend[9]= 0;
    protocolToSend[10]= 255; //codigo de fin de paquete 1 255
    protocolToSend[11]= 0x55; //codigo de fin de paquete 2 0x55
 */
    cout << protocolToSend;

#ifndef NOARDUINO
   // mySerial.writeBytes(&protocolToSend[0], PKTLENGTH);
  //  cout << ofToString(int(protocolToSend[1])) << " ____ " << ofToString(int(protocolToSend[2])) << " ____" <<  ofToString(int(protocolToSend[3])) <<  "____" << ofToString(int(protocolToSend[4])) << "___"<<  ofToString(int(protocolToSend[6])) <<"\n" ;
//    printf("%");
#else

    cout << ofToString(int(protocolToSend[1])) << " ____ " << ofToString(int(protocolToSend[2])) << " ____" <<  ofToString(int(protocolToSend[3])) <<  "____" << ofToString(int(protocolToSend[4])) << "___"<<  ofToString(int(protocolToSend[6])) <<"\n" ;
#endif
    
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	ledNumber=ofRandom(0, 80);
    if(((ofGetElapsedTimeMillis()-lastTimeSent)>40)
       && ledNumber != lastNumberSent ) {
        light(mode,ledNumber,ofColor(0,0,254) );
        lastTimeSent=ofGetElapsedTimeMillis();
        lastNumberSent=ledNumber;
    }

}

void testApp::intToChar(unsigned char *dst, int num, int charLength){
    string s = ofToString(num, 3, '0');
    //cout << s << "\n";
    for(int i=0; i<charLength;i++){
	    dst[i]= s[i];
	}
    //dst[charLength]=',';
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
 /*   int mapeado=ofMap(x, 0, 1024, 0, 60);
    if(mrect.inside(x,y) && (ofGetElapsedTimeMillis()-lastTimeSent>40)
       && mapeado != lastNumberSent ) {
        light('l',mapeado,ofColor(0,0,254) );
        lastTimeSent=ofGetElapsedTimeMillis();
        lastNumberSent=mapeado;
		
    }
    else if(mrect2.inside(x,y) && (ofGetElapsedTimeMillis()-lastTimeSent>40)
       && mapeado != lastNumberSent ) {
        light('s',mapeado,ofColor(0,0,254) );
        lastTimeSent=ofGetElapsedTimeMillis();
        lastNumberSent=mapeado;
		
    }
    */

}

void testApp::gui1Event(ofxUIEventArgs &e)
{
   // cout << "event";
	string name = e.widget->getName();
	int kind = e.widget->getKind();
    
    if(name == "mode")
    {
        if(mode=='l') mode='s';
        else mode='l';
        
    }
    else if(name== "pixel" || name== "red" || name== "green" || name== "blue"){
		if(((ofGetElapsedTimeMillis()-lastTimeSent)>40)
           /**&& ledNumber != lastNumberSent**/ ) {
            light(mode,ledNumber,ofColor(red,green,blue) );
            lastTimeSent=ofGetElapsedTimeMillis();
            lastNumberSent=ledNumber;
        }
        
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
