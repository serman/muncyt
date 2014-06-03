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

  drawPatronCirculos();  
  popMatrix();
  
  fill(255);
  text("fr: "+frameRate,10,15);


  float vMax = TWO_PI;
  fase += radians(map(mouseX, 0, width, -vMax,vMax));
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

