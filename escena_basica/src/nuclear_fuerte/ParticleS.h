//
//  ParticleS.h
//  PixelForces
//
//  Created by Ben McChesney on 7/2/11.
//  Copyright 2011 Helios Interactive. All rights reserved.
//
//  A Simple Particle Class
//
// evol: peripecio 2014
//
// Particle + mass, q (charge), spin)
//

#ifndef _PARTICLE_S
#define _PARTICLE_S

#define MAX_SPEED 30.f
#define MAX_STEER 0.9f

#define NPOS_MAX 10;

#include "ofMain.h"

//#include "Emisor.h"
#include "ParticleTipos.h"

class ParticleS
{
    public :
	
	int id;
	
	float m;	// mass
	float q;	// carga
    ofVec3f position , velocity ;
    ofVec3f position_prev;	
	vector<ofVec3f> positions;
    ofVec3f acceleration ;          //smoothing applied to velocity
	
    ofVec3f spawnPoint ;            //original location to line up the picture
    ofColor color, color_orig, colorExcited, colorTail ;
    char recentlyUsed;
	
	int		tipoPart;
	float	amortV;
	float	tLife;
	
	// 26Nov
	int mode_MESH;

	float	angle;
	
	float	dt;
	
	// para chequear colision con ofPolyline
	bool inside;
	
	// esto crea una linea cuando este dentro de un polyline
	vector<ofPolyline> memoPaths;
	
	// esto acumula todos los puntos por los que pasa
	ofPolyline	lifePath;

	// 26Nov
	ofMesh		meshPath;
	ofVbo		vboPath;
	float	grosor;
	
	bool	bDrawLife;
	bool	bDrawMemoPath;
	
	int		idEmitter;
	
	float	longTail;
	
	bool	swActivaB;
	
    ParticleS() ;
    
//    ParticleS( ofVec3f _position , ofColor _color, float mass = 1.0, float charge = 0.0 )
//    {
//        position = _position ;
//        setColor(_color, _color, _color);
//		m = mass;
//		q = charge;
//				
//		position_prev = position;
//		positions.push_back(position);
//		
//        velocity = ofVec3f ( ofRandom ( -2 , 2 ) , ofRandom ( -2 , 2 ) , ofRandom ( -2 , -2 ) ) ;
//
//		amortV = 1.0;
//		tLife = 2.0*10;	
//		
//		setupCommon();
//		
//    }
//	
//    ParticleS( ofVec3f _position , ofVec3f _vel , ofColor _color, float mass = 1.0, float charge = 0.0 )
//    {
//        position = _position ;
//        setColor(_color, _color, _color);
//		m = mass;
//		q = charge;
//		
//		position_prev = position;
//		positions.push_back(position);
//        velocity = _vel;
//		
//		amortV = 1.0;
//		tLife = 2.0*10;	
//		
//		setupCommon();
//		
//    }
	
    ParticleS( ParticleData pData ) {
		
        position = pData.position;
        velocity = pData.velocity;
		m = pData.m;
		q = pData.q;
		tipoPart = pData.tpPartic;
		amortV = pData.amortV;
		tLife = pData.tLife;	
		// por ahora pd solo tiene .color
        setColor(pData.color, pData.color, pData.color);
		setupCommon();
		
	}
	
	void setColor(ofColor cc, ofColor cc1, ofColor cc2) {
        color = cc ;
		color_orig = cc1;
		colorExcited = cc2;
		colorTail = color;
	}
	
	void setupCommon() {
		angle = 0;
        spawnPoint = position ;
        recentlyUsed=0;
		
		swActivaB = false;
		
		setModoMesh();	
		
		setupPaths();
		
		dt = 0.5;
		
	}

	void setupPaths() {
		
		inside = false;
		
		bDrawLife = false;
		bDrawMemoPath = true;
		

		longTail = 10;
		
		//		lifeMemoPath.setStrokeColor(ofColor::blueSteel);
		//		lifeMemoPath.setColor(ofColor::blueSteel);
		//		lifeMemoPath.moveTo(position.x, position.y);
	}
	
	void setTipoPart(int tipo) {
		//	// devuelve un ParticleData para un tipo de particula.
		//	// No da valor a posicion ni velocidad
		//
		tipoPart = tipo;
		if(tipo==0) {
			m = 1.0;
			q = 0;
			amortV = 1.0;
			tLife = 4.0;
		}
		else if(tipo==1) {
			m = 1.5;
			q = 0.1;
			amortV = 2.0;
			tLife = 4.0;
		}
		else if(tipo==2) {
			m = 1.5;
			q = -1.0;
			amortV = 5.0;
			tLife = 1.2;
		}
		else if(tipo==3) {
			m = 2.0;
			q = -1.5;
			amortV = 2.0;
			tLife = 2.0;
		}
		else if(tipo==4) {
			m = 2.2;
			q = 1.6;
			amortV = 3.0;
			tLife = 2.0;
		}
		else if(tipo==5) {
			m = 3.0;
			q = -1.8;
			amortV = 4.0;
			tLife = 1.0;
		}
		tLife*=5;
		
	}
	
	
	void setModoMesh() {
		grosor = 1.0;
		switch(tipoPart) {
			case 0:
				mode_MESH = OF_PRIMITIVE_LINE_STRIP;//GL_LINE_STRIP;
				break;
			case 1:
				mode_MESH = OF_PRIMITIVE_LINES;//GL_LINES;
				break;
			case 2:
				mode_MESH = OF_PRIMITIVE_LINE_STRIP;//GL_LINE_STRIP;
				grosor = 2.0;
				break;
			case 3:
				mode_MESH = OF_PRIMITIVE_POINTS;//GL_LINE_STRIP;
				grosor = 2.0;
				break;
			case 4:
				mode_MESH = OF_PRIMITIVE_LINE_STRIP;//GL_LINE_STRIP;
				break;
			case 5:
				mode_MESH = OF_PRIMITIVE_LINES;//GL_LINE_STRIP;
				grosor = 2.0;
				break;
			default:
				mode_MESH = OF_PRIMITIVE_LINE_STRIP;//GL_LINE_STRIP;
				break;
		}	
		meshPath.setMode((ofPrimitiveMode)mode_MESH);
		
	}	
	
	
	void setEmitter(int id) {
		idEmitter = id;
	}
	
	

    void update(){
		position_prev = position;
		
    	velocity += acceleration/m*dt;
		
		velocity-=velocity*amortV/300.0;

		// muy graciosete:
		//	https://sites.google.com/site/ofauckland/examples/curly-moving-particles		
		//		angle += ofSignedNoise(position.x, position.y)*PI;
		//		velocity.rotate(angle, ofVec3f(0,0,1));		
		
        position += velocity*dt;
		
		
		// acumular puntos
		positions.push_back(position);
		if(positions.size() > longTail) {	
			positions.erase(positions.begin());
		}
		
		// todos los puntos por donde pasa (igual consume mucho)
		lifePath.addVertex(position);
		
		// 26Nov
		// dependiendo del tipo puede añadir mas puntos para tener trayectorias diferentes
		ofColor ct = colorTail;//Excited;
		ct.a = 100.0;
		//		meshPath.addVertex(position);
		//		meshPath.addColor(ct);
		if(tipoPart==0) {
			// Línea Simple
			meshPath.addVertex(position);
			meshPath.addColor(ct);
			
		}
		else if(tipoPart==1) {
			// Línea Doble
			// calcular direccion y situar ptos a un lado
			ofVec3f posR = position;
			ofVec3f posL = position;
			ofVec3f dir = position-position_prev;
			
			ofVec3f dirZ = ofVec3f(0,0,1);			
			
			ofVec3f dirL = dirZ.getCrossed(dir);
			dirL.normalize();
			
			
			meshPath.addVertex(position+dirL*5.0);
			meshPath.addColor(ct);
			meshPath.addVertex(position-dirL*5.0);
			meshPath.addColor(ct);
			
		}
		else if(tipoPart==2) {
			// Línea gruesa
			meshPath.addVertex(position);
			meshPath.addColor(ct);
			
		}
		else if(tipoPart==3) {
			// Puntos
			meshPath.addVertex(position);
			meshPath.addColor(ct);
			
		}
		else if(tipoPart==4) {
			// Línea discontínua
			meshPath.addVertex(position);
			meshPath.addColor(ct);
			
		}
		else if(tipoPart==5) {
			// zigzag o ++++
			meshPath.addVertex(position);
			meshPath.addColor(ct);
			
		}
		
		
		// puntos dentro de un polyline
		if(inside) {
			int nn = memoPaths.size();
			if(nn>0) {
				if(memoPaths[nn-1].size()==0) {
					//				memoPaths
					// si ya hay paths, creados
					// coger el ultimo y poner el punto en el penultimo punto
					memoPaths[nn-1].addVertex(position);
				}
			}
		}
		
        acceleration=acceleration*0;
		
		tLife -= 0.1;
    }	
	
	
	// Los modos de dibujo variarán dependiendo del tipo	
	void draw() {
		ofPushStyle();
		
		ofColor ct = colorTail;//Excited;
		ct.a = 100.0;
		ofSetColor(ct);
		ofNoFill();
		ofSetLineWidth(grosor);//1.5);
		
		if(bDrawLife) {
			lifePath.draw();			
		}
		
//		if(bDrawMemoPath) {
//			drawMemoPath();
//		}		
		
		// Dibujar Trails:
		meshPath.draw();
		//		if(false) {
		//			if(!inside) {
		//				ofBeginShape();
		//				for( int i=0; i<positions.size(); i++)  {
		//					ofVertex(positions[i].x, positions[i].y);
		//				}
		//				ofEndShape(false);
		//				//ofLine(position_prev.x, position_prev.y, position.x, position.y);
		//			}	
		//		}
		
		ofPushStyle();
		
		//
		// Dibujar Cabezas
		//
		ofSetColor(color);
		//		ofLogNotice(ofToString(color));
		ofFill();
		ofPushMatrix();
		ofTranslate(position.x, position.y);		
		if(tipoPart==0) {
			ofEllipse(0,0, 4,4);
		}
		else if(tipoPart==1) {
			ofRotate(ofGetElapsedTimeMillis());	// degrees
			ofSetLineWidth(2);
			ofLine(-4, 0, 4, 0);
		}
		else if(tipoPart==2) {
			ofRect(-2, -2, 4,4);
		}
		else if(tipoPart==3) {
			ofTriangle(3,3, -3,-3, 0,3);
			//			ofRect(-1, -3, 2,6);
		}
		else if(tipoPart==4) {
			ofEllipse(0, 0, 4,4);
		}
		else if(tipoPart==5) {
			ofRect(-4, -4, 8,8);
		}
		ofPopMatrix();		
		ofPopStyle();
		
		if(inside) {
			ofPushStyle();
			ofNoFill();
			ofEllipse(position.x, position.y, 4,4);			
			ofPopStyle();
		}
		
		//		ofPopMatrix();
		ofPopStyle();
	}
	
	void drawMemoPath() {
		ofPushStyle();
		ofSetLineWidth(2);
//		ofSetColor(colorExcited);
		ofColor ce = color;//Excited;
		ce.a = 100.0;
		ofSetColor(ce);
		for(int i=0; i<memoPaths.size(); i++) {
			memoPaths[i].draw();
		}
		ofPopStyle();
	}
	
	ofVec3f getMomentoP() {
		
		return m*velocity;
	}

	float getEnergyK() {
		return m*velocity.lengthSquared()/2.0;
	}
	
	
    
	bool setInside(bool bInside) {	// devuelve si ha cambiado de estado in/out u out/in
		bool bChange = false;
		bool inside_prev = inside;
		inside = bInside;
		if(inside) { 
			if(!inside_prev) {
				// nuevo camino
				ofPolyline memopl;
				memopl.addVertex(position);				
				memoPaths.push_back(memopl);
				
				bChange = true;
			}
		}
		else {
			if(inside_prev) {
				// sale de polyline
				// ofLogNotice("cierro");
				bChange = true;
			}
		}
		return bChange;
	}
	
	void insertPtChoque(ofVec2f ptChoque) {
		lifePath.insertVertex(ptChoque, lifePath.size()-1);
		
		int nn = memoPaths.size();
		if(memoPaths[nn-1].size()>0) {
			// si ya hay paths, creados
			// coger el ultimo y poner el punto en el penultimo punto
			int mm = memoPaths[nn-1].size();
			memoPaths[nn-1].insertVertex(ptChoque, mm-1);
		}
	}
	
    void applyForce(float a_x, float a_y, float a_z) {
        acceleration.x += a_x/m;
        acceleration.y += a_y/m;
        acceleration.z += a_z/m;
    };
    
    void applyForce(ofVec3f a_loc) {
        acceleration += a_loc/m;
    }
    
    void stopAcc() {
        acceleration.x = 0;
        acceleration.y = 0;
        acceleration.z = 0;
    };
    
    
//ready made forces
    void steer(ofVec3f& a_target, bool a_slowdown, float a_scale, float a_minDist ) {
        ofVec3f steer;
        ofVec3f desired = a_target - position;
        float d = desired.length();
        
        // If the distance is greater than 0, calc steering (otherwise return zero vector)
        if (d > 0.f) {
            desired.normalize();
            if(a_minDist == 0) a_slowdown = false;
            // Two options for desired vector magnitude (1 -- based on distance, 2 -- maxspeed)
            if ((a_slowdown) && (d < a_minDist)) {desired *= (MAX_SPEED * (d/a_minDist));} // This damping is somewhat arbitrary
            else {desired *= (MAX_SPEED); }
            
            steer = (desired - velocity);
            steer.limit(MAX_STEER);
            steer *= a_scale;
        } else {
            steer.set(0.f, 0.f, 0.f);
        }
        acceleration += steer;
    }
    
    void gravityTowards(ofVec3f& a_target,float a_minDist, float speed){
            ofVec3f diff =  a_target - position;
            int sqdistance =  diff.length();
            if(sqdistance>a_minDist){
              diff.normalize();
            int force=sqdistance/speed;
            diff *= force;
            applyForce( diff );
         }//espejo
        else{
         diff.normalize();
         int force=sqdistance/speed;
         diff = diff*-1;
         diff= diff*force;
         applyForce(diff);
        }
    }
    
    bool sandDown(int acc,int floor){
        ofVec3f dPoint = ofVec3f( floor,position.y,position.z);
        ofVec3f diff =  dPoint - position;
        if(position.x<floor){ //no point can be "below our floor
        	position.x=floor;
            return false;
        }        
        
        if(abs(diff.x)>1){
        	diff.normalize();
        	applyForce(diff*acc);
            return true;
        }else{
            velocity*=0;
            acceleration*=0;
        	return false;
        }
    }
    // Añade una fuerza en al direccion de las agujas del reloj (si str>0) respecto de un punto a_target
    void forceRadAround(ofVec3f& a_target, float fVal, float a_minDist){
        ofVec3f dir = position - a_target;
        float d = dir.lengthSquared();
        if (d < (a_minDist*a_minDist) && d > 0) {
			
//			velocity.rotate(dirGiro*90.0, ofVec3f(0,0,1.0));
			
        }
    }

	void forceMagnetZ(float val_bMagnet){
		ofVec3f bMag = ofVec3f(0,0,val_bMagnet);
		forceMagnet(bMag);
	}
	
    void forceMagnet(ofVec3f& bMagnet){
		ofVec3f fMag = q*bMagnet.cross(velocity);
		applyForce(fMag);
		
	}
};
#endif

