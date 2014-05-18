//
//  mosaicRecorder.h
//  mainScreen
//
//  Created by Sergio Galan on 5/13/14.
//
//

#ifndef __mainScreen__mosaicPlayer__
#define __mainScreen__mosaicPlayer__

#include "ofMain.h"
#include "ofQTKitPlayer.h"
#include <iostream>
#define MAXVIDEOS 9

class mosaicPlayer {
public:
    
    void setup();
    mosaicPlayer();
    void update();
    void draw(int index);
    void drawAll(int x0, int y0);
    void replaceVideo(int index);
private:
    ofQTKitPlayer videos[MAXVIDEOS];
    string path="/Users/sergiogalan/MultimediaProgramming/of_v0.8.0_osx_release/apps/muncyt/panel1_tracking/bin/data/";
    string filename="videoRecorded_";
    string ext=".mp4";
    
};

#endif /* defined(__mainScreen__mosaicPlayer__) */