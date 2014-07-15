#include "cvFunctions.h"

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
        convexHull=convexHull.getResampledBySpacing(30);
        silhouettes.push_back (convexHull);
        //convexHull.draw();
        
    }
    return silhouettes;
    
}


//drawDelaunay(ofPolyline contour){
    


//}
