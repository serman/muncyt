//
//  enemy.h
//  mainScreen
//
//  Created by Sergio Galan on 8/26/14.
//
//

#ifndef mainScreen_enemy_h
#define mainScreen_enemy_h

typedef enum tipos {BALL=1, GOAL =2, OBSTACLE} tipoObj;

class datoObjeto  {
    public:
        tipoObj tipo;
        int id ;
        bool isHidden;
} ;

class enemy : public datoObjeto{
private:
    ofMesh m;
    int rot=0;
    float xoff = 0.0;
    int RadiEnemy=20;
    int NPOINTS=20;
//    int x0,y0=0;


public:
    void create(){
        for(int i=0; i<NPOINTS; i++){
            m.addVertex(ofVec3f(ofRandom(RadiEnemy,RadiEnemy),ofRandom(-RadiEnemy,RadiEnemy) ,ofRandom(-RadiEnemy,RadiEnemy) ));
            ofColor  c = ofColor::fromHsb(ofRandom(0,128), 250, 250);
            m.addColor(c);
        }
        m.setMode(OF_PRIMITIVE_POINTS);
    }

    void destroy(){
        
    }
/*    void setPosition(int _x,int _y){
        x0=_x;
        y0=_y;
    }*/
    
    void draw(int x0, int y0){

        glPointSize(2);
        glEnable(GL_POINT_SMOOTH);
        ofPushMatrix();
            ofTranslate(x0,y0);
            ofRotateX(rot);
            ofRotateZ(rot);
            ofRotateY(rot/2);
            //ofEnableDepthTest();
            m.draw();
            //ofDisableDepthTest();
        ofPopMatrix();
  //ACTUALIZAR PUNTOS
        rot+=4  ;
        
        ofVec3f * v=m.getVerticesPointer();
        for (int i=0; i<NPOINTS; i++){
            float n,n1=0;
            float x1=v[i].x;
            float y1=v[i].y;
            float z1=v[i].z;
            n = cos(x1) * 5;
            n1=ofNoise(xoff);
            n1=(n1-0.5)*2;
            //cout << "Valores= " <<ofToString(x1) << ofToString(y1) << endl;
            float newx=x1;
            float newz=z1;
       //para no alejar mucho las particulas de donde deben de estar:
            if(x1+n1<RadiEnemy && x1+n1>-RadiEnemy ){
                newx=x1+n1;
            }
            if(z1+n1<RadiEnemy && z1+n1>-RadiEnemy ){
                newz=z1-n1;
            }
            
            v[i].set(newx, n, newz);
            xoff+=0.1;
        }
        
    }//fin draw
    
    
    void drawBIGENEMY(int x0, int y0){
        m.setMode(OF_PRIMITIVE_LINE_STRIP);
        glPointSize(2);
        glEnable(GL_POINT_SMOOTH);
        ofPushMatrix();
        ofTranslate(x0,y0);
        ofRotateX(rot);
        ofRotateZ(rot);
        ofRotateY(rot/2);
        //ofEnableDepthTest();
        m.draw();
        //ofDisableDepthTest();
        ofPopMatrix();
        //ACTUALIZAR PUNTOS
        rot+=4  ;
        
        ofVec3f * v=m.getVerticesPointer();
        for (int i=0; i<NPOINTS; i++){
            float n,n1=0;
            float x1=v[i].x;
            float y1=v[i].y;
            float z1=v[i].z;
            n = cos(x1) * 5;
            n1=ofNoise(xoff);
            n1=(n1-0.5)*2;
            //cout << "Valores= " <<ofToString(x1) << ofToString(y1) << endl;
            float newx=x1;
            float newz=z1;
            //para no alejar mucho las particulas de donde deben de estar:
            if(x1+n1<RadiEnemy && x1+n1>-RadiEnemy ){
                newx=x1+n1;
            }
            if(z1+n1<RadiEnemy && z1+n1>-RadiEnemy ){
                newz=z1-n1;
            }
            
            v[i].set(newx, n, newz);
            xoff+=0.1;
        }
        
    }//fin draw
    
};

#endif
