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
int nRads = 5;//10;
int nAngs = 8;//30;
float radMax;

float rot_ps = 0.0;


PShape  psConcentrico;
PShape  psRadial;
PShape  psLines;
PShape  psPuntos;

// Background anim
color bckColor = color(200,200,10);
color bckColorTgt = bckColor;
float f_bckColor = 0.2;
color  cBackground;
float  hTgt, sTgt, bTgt;
float fBckColor = 0.02;

void setup() {
  size(1280,800,P2D);
  
  smooth();
  
  radMax = height/2;

  crearPatronConcentrico(radMax, nRads, nAngs);
  
  colorMode(HSB,360,100,100);
  color cfillRadial = color(90,100,100);
  colorMode(RGB,255);
// creaPatronRadial(float radMax, float angRango, int nAngs, float nRamas, color colorFill) {
  creaPatronRadial(radMax, PI/10, 6, 4, cfillRadial);

//void creaPatronLineas(float radMax, float ancho, float sep) {
//void creaPatronPuntos(float radMax, int nPts, float rPts) {
  creaPatronLineas(radMax, 30, 20);
  creaPatronPuntos(radMax, 100, 4);
  
  colorMode(HSB,360,100,100);
  hTgt = 60;
  sTgt = 50;
  bTgt = 0;
  cBackground = color(hTgt, sTgt, bTgt);
  colorMode(RGB,255);
  
  
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
  popMatrix();
    
  text("fr: "+frameRate,10,10);
  text("1-4 modosDraw " + (modoDraw+1),10,25);
  text("q/a nRads: "+nRads,10,40);
  text("w/s nAngs: "+nAngs,10,55);

//  rot_ps+=0.003;
  if(swRota) rot_ps+=wAng;
}


void drawBckColor_circulo() {
  pushStyle();
    colorMode(HSB,255);
    // update color
    float hc = hue(bckColor);
    float hct = hue(bckColorTgt);
    hc = updateVar(hc,hct, f_bckColor, 2.0);
  
    float sc = saturation(bckColor);
    float sct = saturation(bckColorTgt);
    sc = updateVar(sc,sct, f_bckColor, 2.0);
  
    float bc = brightness(bckColor);
    float bct = brightness(bckColorTgt);
    bc = updateVar(bc,bct, f_bckColor, 2.0);
  
    color(hc,sc,bc);
    fill(bckColor);
    noStroke();
    ellipse(0,0,2*radMax,2*radMax);
  popStyle();
}

float updateVar(float vAct, float vTgt, float f_v, float lim) {
  float vUpd = vAct;
  float dv = vAct-vTgt;
  if(dv<lim)   vUpd = vTgt;
  else         vUpd -= dv*f_v;
    
  return vUpd;
}




void mousePressed() {
  if(swRota) wAng=map(mouseX, 0, width, -TWO_PI, TWO_PI);
}

void mouseDragged() {
  if(swRota) wAng=map(mouseX, 0, width, -TWO_PI, TWO_PI);
}

void keyPressed() {
  if(key=='1') modoDraw = MODO_0;
  if(key=='2') modoDraw = MODO_1;
  if(key=='3') modoDraw = MODO_2;
  if(key=='4') modoDraw = MODO_3;
  
  if(key=='r') swRota = !swRota;
  
  if(key=='q') {
    nRads++;
    crearPatronConcentrico(radMax, nRads, nAngs);     
  }
  else if(key=='a') {
    nRads--;
    if(nRads<0) nRads=0;
    crearPatronConcentrico(radMax, nRads, nAngs);         
  }
  
  else if(key=='w') {
    nAngs++;
    crearPatronConcentrico(radMax, nRads, nAngs);     
  }
  else if(key=='s') {
    nAngs--;
    if(nAngs<0) nAngs=0;
    crearPatronConcentrico(radMax, nRads, nAngs);     
  }
  
}



