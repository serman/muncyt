
#include "fantasmas.h"


void fantasmas::selectNextPhantom(){
  //  ofLog()<< "fantasmas::selectNextPhantom " <<endl;
// 1º recorremos todos los blobs actuales y buscamos el video grabado que no colisiona y que está mas lejos
    int MIN_DISTANCE=150;
    for (int i=0; i< videoQueue.size(); i++){
        int index= videoQueue.front();
        videoQueue.pop();
        bool validVideo=true;
        
        //recorro todos los puntos y los comparo con la posición de  los blobs.
        vector<ofPoint> thisPoints= points[index];
        if(thisPoints.size()<maxFrame) {
            ofLogError()<< "fantasmas::selectNextPhantom Numero de puntos cargados  del archivo  menor de los debidos." <<endl;
            break; // Si la secuencia de puntos no esta completa por algun error saltamos
        }
        
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
            if(validVideo==false) break;
        }
        videoQueue.push(index); // En cualquier caso se pasa el video al final y se continua
        if(validVideo==true) { // Si al terminar el bucle doble el video cumple las condiciones:
            secuence_to_play=index; //     elije ese video
            return;
        }
        
        
    }
        //Aqui solo se llega si ningun video ha valido
    secuence_to_play=-1;
    return;
}

/*returns true when collision detected
 False otherwise
 */
bool fantasmas::checkPosibleCollision(){

    // 1º recorremos todos los blobs actuales y buscamos el video grabado que no colisiona y que está mas lejos
    int MIN_DISTANCE=150;
  
    int index= secuence_to_play;
    bool validVideo=true;
    
    //recorro todos los puntos y los comparo con la posición de  los blobs.
    vector<ofPoint> thisPoints= points[index];
    if(thisPoints.size()<maxFrame) {
        ofLogError()<< "fantasmas::checkPotentialCollision Numero de puntos cargados  del archivo  menor de los debidos. Esto no deberia pasar" <<endl;
        return false;
    }
    
    for (int j=0; j< thisPoints.size(); j++){
        // si alguno están a menos de MIN_DISTANCE pixels de un blob actual cierro
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
        if(validVideo==false) break;
    }
    if(validVideo==true) { // Si al terminar el bucle doble el video cumple las condiciones:
        return false; // no hace falta cambiar
    } else
        return true; //hay que cambiar
}

ofxTuioCursor* fantasmas::getNextBlob(list<ofxTuioCursor*>::iterator &tobj, list<ofxTuioCursor*> &objectList  ){
    // Si hay algun elemento después devuelve ese elemento
    for(tobj; tobj != objectList.end(); ++tobj){
        if((*tobj)->getSessionId() != selectedBlobId)
            return (*tobj);
    }
    //si no lo hay devuelve el inicial
    tobj=objectList.begin();
    return (*tobj);
}

void fantasmas::gui2Event(ofxUIEventArgs &e)
{
    cout << "event";
	string name = e.widget->getName();
	int kind = e.widget->getKind();
    cout << "name" << name << "\n";
    cout << "kind" << kind << "\n";
    if(kind==2){
        if(name == "2")
        {
            currentImg=2;
        }
        if(name == "1")
        {
            currentImg=1;
        }
        if(name == "0")
        {
            currentImg=0;
        }
    }
    if(kind==OFX_UI_WIDGET_TOGGLE && name=="grabacion"){
        if(appStatuses["mode"]==CAPTURE)
            appStatuses["mode"]=REPLAYING;
        else appStatuses["mode"]=CAPTURE;
    }
}




//scene notifications
void fantasmas::sceneWillAppear( ofxScene * fromScreen ){  // reset our scene when we appear
    init_Escena();
};


//scene notifications
void fantasmas::sceneWillDisappear( ofxScene * toScreen ){
    
	exit_Escena();
};

void fantasmas::init_Escena(){
    //gui2->enable();
    cheapComm::getInstance()->showVideoFront();
    
    selectedBlobId=-1;
    ofAddListener(gui2->newGUIEvent,this,&fantasmas::gui2Event);
    ofAddListener(finishedRecordingEvent,this, &fantasmas::onRecordingFinished);
    ofSetFrameRate(20);
    recordThisBlob=false;
    showSuccessRecordUntil=0;
    showAbortRecordUntil=0;
    secuence_to_play=-1;
    selectNextPhantom();
    if(secuence_to_play==-1)
        secuence_to_play=0;

}

void fantasmas::exit_Escena(){
    ofRemoveListener(finishedRecordingEvent,this, &fantasmas::onRecordingFinished);
    ofRemoveListener(gui2->newGUIEvent,this,&fantasmas::gui2Event);
    gui2->disable();
    
    
}




