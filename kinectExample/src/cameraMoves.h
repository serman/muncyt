//
//  cameraMoves.h
//  kinectExample
//
//  Created by Sergio Galan on 10/21/14.
//
//

#ifndef kinectExample_cameraMoves_h
#define kinectExample_cameraMoves_h
#include <ofMain.h>


class cameraMoves{
    
public:
    void setup(ofCamera * _cam);
    void zoomInAndCenter();
    void reset();
        bool shouldRestartCamera;
    static cameraMoves *getInstance(){
        if (!m_pInstance)   // Only allow one instance of class to be generated.
            m_pInstance = new cameraMoves;
        return m_pInstance;
    }
    
    
private:
    cameraMoves();
    cameraMoves(cameraMoves const&);              // Don't Implement
    void operator=(cameraMoves const&); // Don't implement
    static cameraMoves* m_pInstance;
    bool zoomInFirstTime;
    int zoomInCounter;
    int tiltCounter=10;
    ofCamera *cam;
    float zoomSpeed;

};
#endif
