
#include "fantasmas.h"


void fantasmas::selectNextPhantom(){/*
// 1º recorremos todos los blobs actuales y buscamos el video grabado que no colisiona y que está mas lejos
    int MIN_DISTANCE=150;
    for (int i=0; i< videoQueue.size(); i++){
        int index= videoQueue.front();
        bool validVideo=true;
        //recorro todos los puntos y los comparo con la posición de  los blobs.
        vector<ofPoint> thisPoints= points[index];
        if(thisPoints.size()<maxFrame) break; // Si la secuencia de puntos no esta completa por algun error saltamos
        
        for (int j=0; j< thisPoints.size(); j++){
        // si no están a menos de MIN_DISTANCE pixels me quedo con el video actual y lo reproduzco
            list<ofxTuioCursor*>::iterator tobj;
            list<ofxTuioCursor*> objectList = tuioclient->getTuioCursors();
            for (tobj=objectList.begin(); tobj != objectList.end(); tobj++) {
                ofxTuioCursor *blob = (*tobj);
                 ofPoint p2 = convertPoint(blob->getX(), blob->getY());
                if(thisPoints[j].distance(p2)<MIN_DISTANCE){
                    validVideo=false;
                    break;
                }
            }
            if(validVideo==true) { // Si el video cumple las condiciones:
                secuence_to_play=index; //     elije ese video y se pasa al final de la cola
                return;
            }
        videoQueue.push(index); // si no se pasa el video al final y se continua
    }
        //Aqui solo se llega si ningun video ha valido
    secuence_to_play=-1;
    return;
    
*/
    
}