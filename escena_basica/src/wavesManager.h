//
//  wavesManager.h
//  escena_basica
//
//  Created by Sergio Galan on 7/13/14.
//
//

#ifndef escena_basica_wavesManager_h
#define escena_basica_wavesManager_h
#include "waves.h"
#include "tangibleObject.h"
#include "cheapComm.h"

#define MAX_ONDAS 6
struct trIndices{
    
    int n_wave;
    int new_index;
  };

class wavesManager : public tangibleObject{
    public:
    vector<waves> waveslist;
    waves *singleWave;
//    int waves_id_counter;

    int howManyWaves(){
        return num_waves;
    }
    
    void countWaves(){
        int count=0;
        for(int i=0; i<waveslist.size(); i++){
            if(waveslist[i].isCompleted())
                count++;
        }
        num_waves= count;
    }

    void setup(){
        singleWave= new waves(0);
        singleWave->setup();
        for(int i=0; i<waveslist.size(); i++){
            waveslist[i].setup(); // esta funcion descarta el id si no lo tiene
        }
    }
    
    void reset(){
        waveslist.clear();
        delete singleWave;
        singleWave=NULL;
        setup();
                countWaves();
    }

    void draw(){
        for(int i=0; i<waveslist.size(); i++){
            waveslist[i].draw(); // esta funcion descarta el id si no lo tiene
        }
    }
    void update(){
        countWaves();
        for(int i=0; i<waveslist.size(); i++){
            waveslist[i].update(); // esta funcion descarta el id si no lo tiene
        }
        
        
        puntos_ondas=0; //va a almacenar la cantidad de puntos totales que hay para asignar a las distintas particulas
        for(int i=0; i<howManyWaves(); i++){
            puntos_ondas+=waveslist[i].points.size();
        }
    };
    
    void touch(float x, float y, int s_id){
        //if(singleWave->isCompleted==false ){ //creamos una onda
            singleWave->addPoint(x,y,s_id);
            if(singleWave->isCompleted()==true){
                waveslist.push_back(*singleWave);
                cheapComm::getInstance()->sendAudio1("/audio/electromagnetism/create_wave_event",singleWave->waveID);
                
                cheapComm::getInstance()->sendSync0("/sync/electromagnetism/create_wave_event");
                
                singleWave= new waves(getAvailableId()); //esta onda se queda como incompleta hasta que se le añadan 2 puntos
                
            }
        //}
    }
    void slide(float x, float y, int s_id, int acc){
        for(int i=0; i<waveslist.size(); i++){
            waveslist[i].updateCursor(s_id,x,y); // esta funcion descarta el id si no lo tiene
        }
        
    }
    
    void touchUp(int s_id){
        if(singleWave->contains(s_id))
            singleWave->remove_id(s_id);
        for(int i=0; i<waveslist.size(); i++){
            if(waveslist[i].contains(s_id)){
                cheapComm::getInstance()->sendAudio1("/audio/electromagnetism/destroy_wave_event", waveslist[i].waveID);
                cheapComm::getInstance()->sendSync0("/sync/electromagnetism/destroy_wave_event");
                
                waveslist[i].remove_id(s_id);
                if(singleWave->isHalf() ){// si hay otro cursor soltero los uno y creo una onda entre ellos
                    waveslist[i].addPoint(singleWave->getSinglePoint().x,singleWave->getSinglePoint().y,singleWave->getSinglePointID());
                    singleWave->clear();
                }
                else{ //Si no hay otro cursor la onda actual es la soltera
                    if( singleWave != NULL)
                        //delete singleWave;
                    singleWave->p1= waveslist[i].p1;
                    singleWave->p2= waveslist[i].p2;
                    singleWave->p2_id= waveslist[i].p2_id;
                    singleWave->p1_id= waveslist[i].p1_id;
                    singleWave->p1set= waveslist[i].p1set;
                    singleWave->p2set= waveslist[i].p2set;
                    waveslist.erase(waveslist.begin()+i);
                }
                break;
            }
        }
        
    }
    void touchUpAll(){
        
    }
    


    /*
     ind es el numero de onda al que tiene que ir un punto
     
     devuelve una onda a la que ir y un indice dentro de esa onda
     */
    trIndices num_onda(int ind,int i){ // i siempre va a ser un multiplo de ind
        int onda_asignada=ind;
        if(ind>=howManyWaves()){
            onda_asignada=ind%howManyWaves();
        }
        int npuntos=waveslist[onda_asignada].points.size();
        // tengo npuntos que asignar a un cierto numero de particulas. Los asigno en orden
        trIndices trindex;
        int indicereal=i/(ind+1);
        int puntoAsignado=indicereal%npuntos;
        
        trindex.n_wave=onda_asignada;
        trindex.new_index=puntoAsignado;
        return trindex;
        /*
        int counter=0;
        for(int i=0; i<howManyWaves(); i++){ //recorro cada onda La condicion para asignarle el punto a la onda es que esté 
            if(ind>=counter && (ind < (counter+waveslist[i].points.size() )) ){
                trindex.n_wave=i;
                trindex.new_index=ind-counter;
                return trindex;
            }
            counter += waveslist[i].points.size();
        }
        trindex.n_wave=-1;
        return trindex;*/
    }

    void debugInfo(){
        if(ofGetFrameNum()%30==0){
            ofLogNotice() << "Numero de ondas" << ofToString(howManyWaves()) ;
            ofLogNotice() << "onda single half: " << ofToString(singleWave->isHalf()) ;
            if(singleWave->p1set){
                ofLogNotice() << "p1 " << ofToString(singleWave->p1_id) ;
            }
            if(singleWave->p2set){
                ofLogNotice() << "___ p2: " << ofToString(singleWave->p2_id) << endl;
            }
            for(int i; i<waveslist.size(); i++){
               ofLogNotice() << "onda "<< ofToString(i) << ": " <<  ofToString(waveslist[i].p1_id )     << "p2: " <<  ofToString( waveslist[i].p2_id )<< endl;
                
            }
        }
        
    }
    
    int getSingleWaveId(){
        if(singleWave->isHalf()==true){
            return singleWave->getSinglePointID();
        }
        else return -1;
    }
    
    int getAvailableId(){
        for(int idToCheck=0; idToCheck<8; idToCheck++){
            int i=0;
            for(i=0; i<waveslist.size(); i++){
                if(waveslist[i].waveID==idToCheck) break;
            }
            if(i==waveslist.size()){
                return idToCheck;
            }
        }
        return -1;
    }

private:
    int puntos_ondas;
    int num_waves;

};


#endif
