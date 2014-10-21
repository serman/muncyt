OPC opc;
PImage dot;
int NUMPARTIC = 4;
int separacion =10;
int separacionV =3;
int OPC_counter=0;
int INTERTIRA=50;
int initX=50;
int initY=50; //esquina superior izquierda

// Map one 64-LED strip to the center of the window
//PANELES 1 2 3 4 
int NUM_TIRAS=10*2+3*2; // consideramos las tiras individuales como caso especial de la tira doble
int endX= (NUM_TIRAS-1)*separacion+initX;
int lengthTramo=separacionV*8;
int lengthTira=separacionV*24+INTERTIRA;
int lengthPared=endX-initX;
int endY=lengthTira+initY;

int initYTira2=initY+INTERTIRA+lengthTramo*2;
int initYTira1=initY+INTERTIRA+lengthTramo;

int screen1Pos=initX+separacion*5;
int screen2Pos=initX+separacion*12;
int screen3Pos=initX+separacion*21;

import oscP5.*;
import netP5.*;
import processing.video.*;
boolean debugOSC=true;
Movie movieExp;
char fade=0; // -1 0 o 1
boolean bdrawNoise=false;
boolean   bdrawWaves=false;
boolean bDrawRandomParticles=false;
boolean bDrawMovie=false;
boolean bDrawGridImage=false;
boolean bDrawGridCircles, bDrawLine,bExchange, bdrawOff=false;

randomParticle randomP[]=new randomParticle[NUMPARTIC];
circles mcircle;
PImage gridImg;


float exchangeTime; int exchangeSection;
color exchangeC1, exchangeC2;
void setup()
{
  size(400, 400, P2D);
  // Load a sample image
  dot = loadImage("color-dot.png");
  frameRate(30);
  // Connect to the local instance of fcserver
  opc = new OPC(this, "127.0.0.1", 7890);
  int i=0;
  // void ledStrip(int index, int count, float x, float y, float spacing, float angle, boolean reversed)
  for (i=0; i<NUM_TIRAS; i++) {
    opc.ledStrip(OPC_counter, 8, initX+separacion*i, initY+lengthTramo/2 +INTERTIRA*2, separacionV, -PI/2, false);
    OPC_counter+=8;
    opc.ledStrip(OPC_counter, 8, initX+separacion*i, initY+lengthTramo/2 +INTERTIRA, separacionV, -PI/2, false);
    OPC_counter+=8;
    opc.ledStrip(OPC_counter, 8, initX+separacion*i, initY+lengthTramo/2, separacionV, -PI/2, false);
    OPC_counter+=8;  

    i++;
    opc.ledStrip(OPC_counter, 8, initX+separacion*i, initY+lengthTramo/2, separacionV, -PI/2, true);
    OPC_counter+=8;
    opc.ledStrip(OPC_counter, 8, initX+separacion*i, initY+lengthTramo/2+INTERTIRA, separacionV, -PI/2, true);
    OPC_counter+=8;
    opc.ledStrip(OPC_counter, 8, initX+separacion*i, initY+lengthTramo/2+INTERTIRA*2, separacionV, -PI/2, true);
    OPC_counter+=8;
  } 
  println(i);
  setupOSC();   
  movieExp = new Movie(this, "explosion.mov");
  movieExp.play();
  movieExp.stop();

  gridImg=loadImage("grid.png");
  //class setup
  // smooth();
  myBall = new Ball[ballAmount];
  for (int ii = 0; ii < ballAmount; ii++) {
    myBall[ii] = new Ball();
    myBall[ii].setup();
  }

  for (int jj=0; jj< NUMPARTIC; jj++) {
    randomP[jj]=new randomParticle();
    randomP[jj].setup();
  }
  mcircle=new circles();
  /****/
  
}

void draw()
{
  background(0);
  /* stroke(255,0,0);
   fill(0,0,200);
   line(endX,initY,endX,endY);
   rect(initY,initX,2,2);*/

  //  rotateDraw(1);
  if (bdrawNoise) noiseDraw();
  //randomParticleDraw();
  if (bdrawWaves) waveDraw();
  if (bDrawRandomParticles) drawRandomParticles();

  //breath();
  if (bDrawMovie)  drawMovie();
  if (bDrawGridImage) drawGridImage();
  if (bDrawGridCircles) mcircle.draw();
  // drawWeird1();
  //drawTest();
  if (bdrawOff) off();
  if (bDrawLine) drawLine();
  if(bExchange)  exchangeDraw(exchangeTime, exchangeSection, exchangeC1, exchangeC2);
  drawScreens();
}


void movieEvent(Movie m) {
  m.read();
}

void allOff() {
  bdrawNoise=false;
  bdrawWaves=false;
  bDrawRandomParticles=false;
  bDrawMovie=false;
  bDrawGridImage=false;
  bDrawGridCircles=false;
  bDrawLine=false; 
  bdrawOff=false;
  bExchange=false;
}
