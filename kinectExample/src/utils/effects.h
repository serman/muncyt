//
//  Header.h
//  mainScreen
//
//  Created by Sergio Galan on 9/11/14.
//
//

#ifndef mainScreen_Header_h
#define mainScreen_Header_h
enum kindEffects{COLL};

class effects{
    
    public:
        effects(){
            duration=800;
            setToDelete=false;
            initTime=ofGetElapsedTimeMillis();
        }

    
    void update(){
        if(ofGetElapsedTimeMillis()-initTime>duration){
            setToDelete=true;
        }
    }
            virtual void draw(){}
    bool setToDelete;
    private:
        long initTime;
        long duration;

        
};

class explosion: public effects{
public:
    enum {TRI, RECT};
    int _x, _y;
    int crece=0;
    int rotate=0;
    int tipo=0;
    ofColor c;
    void draw(){
        ofPushMatrix();
        ofPushStyle();
        ofTranslate(_x,_y);
        ofRotate(rotate);
        ofNoFill();
        ofSetColor(c);
        ofSetLineWidth(1);
        if(tipo==RECT)
            ofRect(-5-crece/2,-5-crece/2,5+crece,5+crece);
        if(tipo==TRI)
            ofTriangle(-5-crece,-5+crece,5+crece,-5+crece,0,10+crece*2);
        crece+=20;
        rotate+=3;
  //      cout << "draw"<< endl;
        ofPopStyle();
        ofPopMatrix();
    }
    
    void setPosition(int px, int py){
        _x=px;
        _y=py;
        if(ofRandom(0,2)>1){
            tipo=TRI;
        }else
            tipo=RECT;
        float r=ofRandom(0,1);
        if(r<0.4){
            c=ofColor(248,88,152);
        }else if( r>=0.4<=0.6)
            c=ofColor(88,216,84);
        else
            c=ofColor(0,252,252);
        
    }
};


class effectsHandler{

    public:
      ofPtr<effects> addEffect(int effectType){
        if(effectType==COLL){
            ofPtr<effects> ef(new explosion());
            effectsv.push_back(ef);
            return ef;
        }
    }
    
    void update(){
        for(int i=0; i<effectsv.size(); i++) {
            effectsv[i]->update();
        }
        
        for(int i=0; i<effectsv.size(); i++) {
            if(effectsv[i]->setToDelete==true){
                effectsv.erase(effectsv.begin()+i);
                break;
            }
        }
        
    }
    
    void draw(){
        //cout << "size"<< effectsv.size() <<endl;
        
        for(int i=0; i<effectsv.size(); i++) {
            effectsv[i]->draw();
        }
    }

    private:
        vector < ofPtr<effects> > effectsv;
};
#endif
