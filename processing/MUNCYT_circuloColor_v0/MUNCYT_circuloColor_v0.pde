import controlP5.*;

import oscP5.*;
import netP5.*;

// Parametros del mov que pueden llegar por OSC
float    angulo;
float    wAng;
float    accAng;
boolean  swBang;

// inica si el control de la posicion angular lo lleva
// las ordenes OSC o mediante un mando en el programa
boolean swCtrlOSC = false;

boolean swRota = true;

// Modos de presentacion. Ya les pondremos nombre.
int MODO_0 = 0;  // Concentrico
int MODO_1 = 1;  // Radial
int MODO_2 = 2;  // Lineas
int MODO_3 = 3;  // Puntos

int modoDraw = MODO_0;

PShape ps;
float radMax;

// params patron circulos
int nRads = 5;//10;
int nAngs = 8;//30;

// params patron radial
float angRango_rad;
int nAngs_rad;
float nRamas_rad;
color cFill_rad;

// rotacion
float rot_ps = 0.0;

PShape  psConcentrico;
PShape  psRadial;
PShape  psLines;
PShape  psPuntos;



// Background anim
color bckColor = color(200,200,10);
color bckColorTgt = color(0);
float f_bckColor = 0.2;
color  cBackground;
float  hTgt, sTgt, bTgt;
float fBckColor = 0.00001;
int tOrden;
int tFull = 200;

void setup() {
  size(1280,800,P2D);
  
  smooth();
  
  radMax = height/2;

  crearPatronConcentrico(radMax, nRads, nAngs);

  
  angRango_rad = PI/10;
  nAngs_rad = 6;
  nRamas_rad = 4;
  
  colorMode(HSB,360,100,100);
  cFill_rad = color(90,100,100);
  colorMode(RGB,255);
// creaPatronRadial(float radMax, float angRango, int nAngs, float nRamas, color colorFill) {
  creaPatronRadial(radMax, angRango_rad, nAngs_rad, nRamas_rad, cFill_rad);

//void creaPatronLineas(float radMax, float ancho, float sep) {
//void creaPatronPuntos(float radMax, int nPts, float rPts) {
  creaPatronLineas(radMax, 30, 20);
  creaPatronPuntos(radMax, 100, 4);
  
  
  setupOSC();
  
}



void draw() {
  background(0);

  pushMatrix();
  translate(width/2,height/2);

  drawBckColor_circulo();
  
  rotate(rot_ps);
  if(modoDraw==MODO_0)  shape(psConcentrico);
  else if(modoDraw==MODO_1)  shape(psRadial);
  else if(modoDraw==MODO_2)  shape(psLines);
  else if(modoDraw==MODO_3)  shape(psPuntos);
//  shape(psRadial, mouseX, mouseY);

  pushStyle();
  noStroke();
  fill(0);
  ellipse(0,0, 80,80);

  popMatrix();
    
  text("fr: "+frameRate,10,10);
  text("1-4 modosDraw " + (modoDraw+1),10,25);
  text("q/a nRads: "+nRads,10,40);
  text("w/s nAngs: "+nAngs,10,55);

//  rot_ps+=0.003;
  if(swRota) rot_ps+=wAng;
}


void drawBckColor_circulo() {
  if(tOrden<millis()-tFull) {
    pushStyle();
      colorMode(HSB,255);
      // update color. Solo Brillo: 255->0
      
      float hc = hue(bckColor);
  //    float hct = hue(bckColorTgt);
  //    hc = updateVar(hc,hct, f_bckColor, 2.0);
  
      float sc = saturation(bckColor);
  //    float sct = saturation(bckColorTgt);
  //    sc = updateVar(sc,sct, f_bckColor, 2.0);
    
      float bc = brightness(bckColor);
      float bct = brightness(bckColorTgt);
      bc = updateVar(bc,bct, f_bckColor, 2.0);
    
      bckColor = color(hc,sc,bc);
    popStyle();    
  }
  fill(bckColor);
  noStroke();
  ellipse(0,0,2*radMax,2*radMax);

}

float updateVar(float vAct, float vTgt, float f_v, float lim) {
  float vUpd = vAct;
  float dv = vAct-vTgt;
  if(dv<lim)   vUpd = vTgt;
  else         vUpd -= dv*f_v;
  
  float reducc = dv*f_v;
    
  println("updtVar de "+vAct+" a "+vUpd + " . . . "+reducc);
  
  return vUpd;
}

void setBckColor() {
  // random color
  pushStyle();
  colorMode(HSB,255);
  bckColor = color(random(255),255,255);
  popStyle();
  tOrden = millis();
}


void mousePressed() {
  if(swRota) wAng=map(mouseX, 0, width, -PI, PI);
}

void mouseDragged() {
  if(swRota) wAng=map(mouseX, 0, width, -PI, PI);
}

void keyPressed() {
  if(key=='1') modoDraw = MODO_0;
  if(key=='2') modoDraw = MODO_1;
  if(key=='3') modoDraw = MODO_2;
  if(key=='4') modoDraw = MODO_3;
  
  if(key=='r') swRota = !swRota;
  
  if(key=='q') {
    if(modoDraw==MODO_0) {
      nRads++;
      crearPatronConcentrico(radMax, nRads, nAngs);
    }
    else if(modoDraw==MODO_1) {
      nRamas_rad++;
//      angRango_rad = PI/10;
//      nAngs_rad = 6;
//      nRamas_rad = 4;
      creaPatronRadial(radMax, angRango_rad, nAngs_rad, nRamas_rad, cFill_rad);
    }
  }
  
  else if(key=='a') {
    if(modoDraw==MODO_0) {
      nRads--;
      if(nRads<0) nRads=0;
      crearPatronConcentrico(radMax, nRads, nAngs);         
    }
    else if(modoDraw==MODO_1) {
      nRamas_rad--;
      if(nRamas_rad<0) nRamas_rad=0;
//      angRango_rad = PI/10;
//      nAngs_rad = 6;
//      nRamas_rad = 4;
      creaPatronRadial(radMax, angRango_rad, nAngs_rad, nRamas_rad, cFill_rad);
    }
  }
  
  else if(key=='w') {
    if(modoDraw==MODO_0) {
      nAngs++;
      crearPatronConcentrico(radMax, nRads, nAngs);
    }
    else if(modoDraw==MODO_1) {
      angRango_rad+=radians(5);
      creaPatronRadial(radMax, angRango_rad, nAngs_rad, nRamas_rad, cFill_rad);
    }
    
  }
  else if(key=='s') {
    if(modoDraw==MODO_0) {
      nAngs--;
      if(nAngs<0) nAngs=0;
      crearPatronConcentrico(radMax, nRads, nAngs);
    }
    else if(modoDraw==MODO_1) {
      angRango_rad-=radians(5);
      creaPatronRadial(radMax, angRango_rad, nAngs_rad, nRamas_rad, cFill_rad);
    }    
  }
  
  else if(key=='b') {
    setBckColor();
  }
  
}



