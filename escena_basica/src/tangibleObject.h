//
//  tangibleObject.h
//  example
//
//  Created by Sergio Galan on 7/8/14.
//

#include <ofMain.h>
#ifndef example_tangibleObject_h
#define example_tangibleObject_h

class tangibleObject : public ofRectangle{
public:
 //   virtual ~tangibleObject()= 0;
    int cursor_id;
    ofColor mcolor;
    long m_id;

	ofVec2f getCentroid() {return ofVec2f(x+width/2.0, y+height/2.0);};

    virtual void setup()= 0;
    virtual void draw()= 0;
    virtual void update()= 0;
    virtual void touch(float x, float y, int s_id)= 0;
    virtual void slide(float x, float y, int s_id, int acc)= 0;
    virtual void touchUp(int s_id)= 0;
};

#endif
