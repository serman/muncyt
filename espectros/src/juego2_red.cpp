//
//  juego2_red.cpp
//  mainScreen
//
//  Created by Sergio Galan on 8/14/14.
//
//

#include "juego2.h"
bool vertexExist(ofPolyline &l, ofPoint p1){
    ofPoint p2   = l.getClosestPoint(p1);
    if(p1.distance(p2)==0) return true;
    else return false;
}

void juego2::tuioAdded(ofxTuioCursor &tuioCursor){
    //nota aunque pone Xspeed lo que estoy enviando es el centroide con smooth
    ofPoint p= convertPoint(tuioCursor.getXSpeed(), tuioCursor.getYSpeed());
    addObstacle(p , tuioCursor.getSessionId() );
    if(p.x == -1 && p.y == -1){
        hideObstacle(tuioCursor.getSessionId() );
    }else{        
        updateObstacle( p , tuioCursor.getSessionId() );
    }
    return;

}

void juego2::tuioUpdated(ofxTuioCursor &tuioCursor){
    ofPoint p= convertPoint(tuioCursor.getXSpeed(), tuioCursor.getYSpeed());
    if(p.x == -1 && p.y == -1){
        hideObstacle(tuioCursor.getSessionId() );
    }else{
        updateObstacle( p , tuioCursor.getSessionId() );
    }
	
}

void juego2::tuioRemoved(ofxTuioCursor &tuioCursor){
	removeObstacle(tuioCursor.getSessionId());
}

//devuelve (-1, -1) si el punto está fuera de la pantalla



    void juego2::move(int dir){
        ofVec2f p=player.getPosition();
        
        if(appStatuses["isOnContour"]==false /*&& previousKey != currentKey*/){

            poli.lineTo(p);
        }
        
        switch(dir){
            case NONE: return;
            case UP:
                if(p.y>0 ){
                    p.y-=movingSpeed;
                }
            break;
            case DOWN:
                if( p.y<SCREEN_H-1){
                    p.y+=movingSpeed;

                   // poli.addVertex(p);
                }
            break;
            case LEFT:
                if( p.x>0){
                    p.x-=movingSpeed;

                  //  poli.addVertex(p);
                }
                break;
            case RIGHT:
                if( p.x<SCREEN_W){
                    p.x+=movingSpeed;
                   // poli.addVertex(p);
                }
                break;
                
        }
        if(poli.size()>3 && vertexExist(poli,p)){
            prepareReset=true;
            cout << "tocando al mismo vertice";
        }
        if(p.x>=SCREEN_W) p.x= SCREEN_W-1;
        if(p.y>=SCREEN_H) p.y= SCREEN_H-1;
        if(p.x<0) p.x=0;
        if(p.y<0) p.y=0;
        if(mask1.getColor(p.x, p.y)==ofColor::black){
            player.setPosition(p);
            appStatuses["isOnContour"]=false;
            poli.addVertex(prevPos);
            
        }
        else{ //estoy sobre un punto blanco            
            if(appStatuses["isOnContour"]==false){ //antes estaba fuera  del contorno ya
                appStatuses["isOnContour"]=true;
                poli.addVertex(p);
                player.setPosition(p);
                fillthis=true;
            }
        }
        prevPos=p;
        player.setPosition(p);

        appStatuses["lastMovement"]=dir;
    }
//TODO CONSEGUIR UN PUNTO INTERIOR DEL POLYLINE
ofPoint juego2::findRegionToPaint(){
    //Se recorren las areas del contour finder que serán 3.
    //1º el area negra, el area blanca y el área que se acaba de crear
    for (int i=0; i<contourFinder.size(); i++){
        ofPolyline l= contourFinder.getPolyline(i);
        if (l.size()<4 ) continue;
        //necesitamos encontrar un punto dentro del polyline que se ha creado
        
        ofRectangle r= l.getBoundingBox();
        ofPoint p1= r.getCenter();
        if( !l.inside(p1) ){
            //si el punto no está dentro del polígono hay que encontrar otro punto. Vamos a generar puntos alrededor del cursor y vamos viendo
            int radius=4;
            while(!l.inside(p1) && radius <2000){
                p1=ofPoint( ofRandom(player.getPosition().x-radius,player.getPosition().x+radius),
                           ofRandom(player.getPosition().y-radius,player.getPosition().y+radius));
                radius+=1;
            }
            
        }
        else{
            cout << "el punto está en el rectangulo";
        }
        ofColor c=mask1.getColor( p1.x,  p1.y) ;
        // al dividir un plano con una línea, quedan dos polígonos siempre
        //Cual de los dos se marca como "conquistado y cual no?"
        //condiciones: 1ª que el poligono no sea ya el "conquistado"
        // que el enemigo no esté dentro del poligono
        //una vez que tenemos un punto de dentro del poligono comprobamos si este poligono del polyline era
        if( mask1.getColor( p1.x,  p1.y) == ofColor::black && l.inside(bigEnemy.getPosition())==false){
            return p1;
        }
        
    }
    return ofPoint(-1,-1);
}

void juego2::explode(ofPoint d){
//    ofPoint d=ofPoint(ofRandom(0,SCREEN_W),SCREEN_H);
    l.clear();
    l.addVertex(player.getPosition());
    l.lineTo(d);
    for(int i=1;i<95;i+=5){
        l.insertVertex(l.getPointAtPercent((float)i/100),floor(i/5)+1);
    }
    drawExplodeb=true;
    start=0;
    end=2;
    mparticles.addEmitter(player.getPosition().x,player.getPosition().y);
}

void juego2::drawExplode(){
    ofSetLineWidth(2);
    ofSetColor(255, 0,0);
    if(l.size()<1) return;
    for(int i=start; i<end; i++){
        ofLine(l.getVertices()[i],l.getVertices()[i+1]);
    }
    end++;
    if(end>=l.size()-1) {
        end=l.size()-1;
        start+=1;
        if(start>=l.size()-2) drawExplodeb=false;
    }    
}

void juego2::drawPlayer(int _x, int _y){
    int _size=10;
    ofPushMatrix();

    
    ofPushStyle();
    ofSetColor(0,200,0);;
    ofTranslate(_x, _y);
    if(    appStatuses["isOnContour"]==true){
        ofCircle(0, 0, _size);
         glowcircle.draw(-glowcircle.width/2,-glowcircle.height/2);
    }
    ofSetColor(255,150+155*sin(ofGetElapsedTimef()/6.0*TWO_PI));
   
    //ofRotateX(45);
    //    ofSetColor(255, 200, 0);
    //ofRect(_size/2,_size/2,_size,_size);
    ofPopStyle();
    ofPopMatrix();
    
}


