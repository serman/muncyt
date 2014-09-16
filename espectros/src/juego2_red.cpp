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
    ofPoint p= convertPoint(tuioCursor.getX(), tuioCursor.getY());
    if(p.x == -1 && p.y == -1){
        return;
    }else{
        addObstacle(p , tuioCursor.getSessionId() );
    }
}

void juego2::tuioUpdated(ofxTuioCursor &tuioCursor){
    ofPoint p= convertPoint(tuioCursor.getX(), tuioCursor.getY());
    if(p.x == -1 && p.y == -1){
        hideObstacle(tuioCursor.getSessionId() );
    }else{
        updateObstacle( p , tuioCursor.getSessionId() );
    }
	
}

void juego2::tuioRemoved(ofxTuioCursor &tuioCursor){
	removeObstacle(tuioCursor.getSessionId());
}

ofPoint juego2::convertPoint(float x1, float y1){
    if(y1*VIDEO_H*VIDEO_scale>(VIDEO_offset-30)
       && y1*VIDEO_H*VIDEO_scale>VIDEO_offset <  (VIDEO_offset + SCREEN_H+30) ){
        ofPoint p1=ofPoint((float)x1*SCREEN_W, (float)y1*VIDEO_H*VIDEO_scale-VIDEO_offset);
        return p1;
    }
    else return ofPoint(-1,-1);
}


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
    for (int i=0; i<contourFinder.size(); i++){
        
        ofPolyline l= contourFinder.getPolyline(i);
        if (l.size()<4 ) continue;
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
        //una vez que tenemos un punto de dentro del poligono comprobamos si es del tipo que queremos colorear
        if( mask1.getColor( p1.x,  p1.y) == ofColor::black && l.inside(600,100)==false){
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


