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

// Modos de presentacion. Ya les pondremos nombre.
int MODO_0 = 0;
int MODO_1 = 1;
int MODO_2 = 2;
int MODO_3 = 3;

int modo = MODO_0;


// Patron circulos a proyectar
// Consiste en varios circulos concentricos
// + un circulo negro en el centro de radio rMin

float rMax;
float rMin;
float[] rs;
int nCircs = 8;

color cInt, cExt;
color[] colors;

float fase = 0;

// Background anim
color bckColor = color(0);
color bckColorTgt = bckColor;
float f_bckColor = 0.2;

void setup() {
//  size(1280,800);
  size(1280,768);
  
  smooth();
  
  rMax = height/2;
  rMin = 100;
  
  rs = new float[nCircs];
  colors = new color[nCircs];

  colorMode(HSB, 360, 100,100);
  cInt = color(0,0,0);  // Negro  

  setupPatronCirculos();
  
  
}

void setupPatronCirculos() {
  
  for(int i=0; i<nCircs; i++) {
    rs[i] = rMin + (rMax-rMin)/(nCircs)*(i+1);
    colors[i] = color(360/(nCircs)*(i), 100,100);
  }
  
}

void draw() {
  background(0);
  
  pushMatrix();
  translate(width/2,height/2);
  noStroke();

  drawBckColor_circulo();


  drawPatronCirculos();  
  popMatrix();
  
  fill(255);
  text("fr: "+frameRate,10,15);


  float vMax = TWO_PI;
  fase += radians(map(mouseX, 0, width, -vMax,vMax));
}

void drawBckColor_circulo() {
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
  
  pushStyle();
    colorMode(HSB,255);
    color(hc,sc,bc);
    fill(bckColor);
    ellipse(0,0,2*rMax,2*rMax);
  popStyle();
}

float updateVar(float vAct, float vTgt, float f_v, float lim) {
  float vUpd = vAct;
  float dv = vAct-vTgt;
  if(dv<lim)   vUpd = vTgt;
  else         vUpd -= dv*f_v;
    
  return vUpd;
}

void drawPatronCirculos() {

  // Dibuja circulos desde el mayor hasta el pequeño
  for(int i=nCircs-1; i>=0; i--) {
    fill(colors[i]);
    ellipse(0,0,2*rs[i],2*rs[i]);  
  }

  // Dibuja circulo interior
  fill(cInt);
  ellipse(0,0,2*rMin,2*rMin);

  // Dibuja lineas radiales
  stroke(0,0,100);
  pushMatrix();
  rotate(fase);
  int nRadios = 36;
  for(int i=0; i<nRadios; i++) {
    float ang = TWO_PI/nRadios*i;
    line(0,0, rMax*cos(ang), rMax*sin(ang));
  }
  popMatrix();  
  
}

void drawPatronRadial() {
  // Dibuja una trat de colores
 
}

void drawPatronLineas() {
  // dibuja lineas horizontales y verticales que emanan del centro
  
}

void dibujaPatronPuntos() {
  
}

