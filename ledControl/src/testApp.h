#pragma once

#include "ofMain.h"
#include "cheapCommRcv.h"
#include "ofxUI.h"
// this h file has the definition for an object that uses the threaded addon:
#include "threadedObject.h"

#define PKTLENGTH 14
//#define NOARDUINO 1
class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    	void setupSerial();
    	void light(char code, int number, ofColor mcolor);
    
    	cheapCommRcv myOSCrcv;
    ofSerial 	mySerial;
    
    bool		bSendSerialMessage;			// a flag for sending serial
    char		bytesRead[3];				// data from serial, we will be trying to read 3
    char		bytesReadString[4];			// a string needs a null terminator, so we need 3 + 1 bytes
    unsigned char 		protocolToSend[PKTLENGTH];
    int			nBytesRead;					// how much did we read?
    int			nTimesRead;					// how many times did we read?
    float		readTime;					// when did we last read?
		
    threadedObject	TO;
    
    ofRectangle mrect;
    ofRectangle mrect2;
    long lastTimeSent;
    int lastNumberSent;
    void intToChar(unsigned char *dst,int num,int charLength);
    ofxUICanvas *gui1;
	void gui1Event(ofxUIEventArgs &e);
    int ledNumber;
    char mode;
    int red, green, blue;
};

//c command code: l= light one; u= light until;
//00 pixel number
// r
// g
// b
// 255 eof

//c command code
//00 pixel number
// r
// g
// b
// 255 eof