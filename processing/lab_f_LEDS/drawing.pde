void drawTest() {
  background(0);
  float dotSize = width * 0.2;
  image(dot, mouseX - dotSize/2, mouseY - dotSize/2, dotSize, dotSize);
}

int rotatePos=initX;
void rotateDraw(float T) {
  background(0);
  //freq= rotaciones/seg;
  //T = periodo en segundos
  float pix_second=lengthPared/T;  
  int inc_pix=round( pix_second/frameRate ); 
  pushStyle();
  fill(0, 200, 000);
  noStroke();
  rect(rotatePos, initY, 3, lengthTira);
  if (rotatePos>endX) rotatePos=initX;
  rotatePos+=inc_pix;
  popStyle();
}


void noiseDraw() {
  background(0);
  int psize = 4;
  int x_count = (lengthPared/psize); 
  int y_count = (lengthTira/psize);
  pushStyle();
  noStroke(); //commenting this out gives a neat effect

  //draw the "pixels"
  for (int i=0; i<=y_count; i++) {
    for (int j=0; j<=x_count; j++) {
      //pick a random grayscale value before drawing each rectangle. (thanks subpixel!)
      fill(int(random(200)));
      rect(initX+j*psize, initY+i*psize, psize, psize);
    }
  } 

  popStyle();
}

int particlePosX=initX;
int particlePosY=initY+40;
void randomParticleDraw() {
  fill(0,10);
  rect(0,0,width,height);
  noStroke();
  fill(0,200,0);
  rect(particlePosX, particlePosY, 5, separacionV);
  particlePosX+=random(5, 8);
  particlePosY+=random(-2, 3);
  if (particlePosX>endX){ particlePosX=initX; particlePosY=initY+40;}
}


float phi =  0; // the phase shift offset amount
float amplitude = lengthTira/2; // the amplitude of our wave
float frequency = 1.0/32; // slow down our wave so we can see it better
int numPoints = 100; // the granularity of our drawing
void waveDraw() {
  stroke(0,100,200);
  noFill();
  background(0);
  pushMatrix();
  translate(initX,initY+lengthTira/2);
  float x = 0;
  float y = 0;
   
  // if we consider 1 pixel to be 1 radian, then this is the number
  // of radians between points
  //float angle = lengthPared+initX/numPoints;probar esto!!!!!!
  float angle = (lengthPared+initX)/numPoints;
  beginShape();
  for (int i = 0; i < numPoints; i++) {
     
    // calculate where we are along x
    x = angle*i;
     
    // y is determined by our function
    y = sin(x*frequency+phi)* amplitude;
    vertex(x, y);
  }
  endShape();
  phi -= .2;
  popMatrix();
}


void moveColor() {
}

void off() {
  background(0);
}

void breath() {
}


