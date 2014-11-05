int[] data = { 
  0, 2, 3, 4, 6, 7, 8, 9, 10, 11, 12, 17, 17, 24, 25, 27, 28, 29, 30, 31, 32, 36, 39, 40, 41, 44, 45, 47, 52, 53, 54, 57, 68, 70, 71, 72, 73, 74, 75, 79, 82, 85, 84, 83, 82, 81, 80, 79, 78, 77, 76, 75, 74, 73, 72, 71, 70, 69, 68, 67, 66, 65, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 19, 18, 18, 17, 17, 16, 16, 15, 15, 14, 14, 13, 13, 12, 12, 11, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 2, 1, 1, 0, 0, 2, 3, 4, 6, 7, 10, 14, 17, 17, 18, 19, 20, 34, 42, 40, 50, 62, 64, 71, 81, 82, 83, 94, 96, 99, 104, 107, 108, 115, 120, 121, 122, 123, 126, 130, 131, 132, 140, 150, 151, 152, 153, 154, 155, 155, 159, 163, 159, 160, 160, 161, 162, 163, 163, 163, 164, 164, 165, 165, 166, 166, 167, 167, 168, 168, 170, 168, 167, 164, 164, 162, 155, 154, 153, 148, 148, 147, 145, 145, 142, 142, 136, 134, 125, 123, 123, 122, 119, 102, 102, 101, 101, 100, 100, 99, 99, 98, 97, 96, 95, 95, 94, 91, 91, 90, 88, 84, 83, 80, 80, 79, 78, 73, 73, 72, 72, 70, 66, 65, 64, 63, 62, 60, 60, 59, 59, 58, 58, 57, 56, 55, 53, 53, 52, 52, 51, 51, 50, 49, 48, 47, 43, 42, 41, 40, 40, 39, 39, 38, 37, 36, 36, 37, 38, 38, 40, 41, 42, 45, 46, 47, 48, 49, 51, 53, 56, 58, 58, 62, 63, 68, 69, 70, 71, 73, 74, 76, 78, 79, 80, 80, 83, 83, 84, 87, 88, 98, 100, 102, 111, 124, 129, 131, 133, 136, 136, 142, 143, 144, 145, 147, 150, 148, 148, 144, 143, 141, 141, 136, 136, 135, 135, 134, 131, 124, 124, 123, 120, 119, 119, 117, 115, 115, 109, 109, 107, 103, 102, 101, 100, 99, 99, 97, 96, 94, 93, 93, 92, 92, 91, 91, 90, 90, 88, 88, 87, 86, 86, 85, 85, 78, 77, 70, 70, 69, 65, 64, 63, 63, 62, 62, 61, 60, 59, 58, 57, 57, 56, 54, 53, 48, 46, 38, 36, 36, 30, 28, 27, 26, 25, 24, 24, 18, 17, 16, 15, 11, 9, 8, 7, 6, 0
};

void drawTest() {
  background(0);

  float dotSize = width * 0.2;
  //image(dot, mouseX - dotSize*5/2, mouseY - dotSize/2, dotSize*5, dotSize);
      // fill(color(0,255,0));
      tint(color(255,0,0));
      image(maskImage,mouseX, mouseY,100,10);
      //fill(50, 50, 50);
  //rect(mouseX, mouseY, 300, lengthTramo+10);
}

int rotatePos=initX;
void rotateDraw(float T) {
  background(0);

  //freq= rotaciones/seg;
  //T = periodo en segundos
  float pix_second=lengthPared/T;  
  int inc_pix=round( pix_second/frameRate ); 
  pushStyle();
  fill(255);
  tint(10, 100, 10);
  noStroke();
  image(maskImageVert,rotatePos-15, initY, 30, lengthTira+10);
  //rect(rotatePos, initY, 30, lengthTira+10);  
  if (rotatePos>endX) rotatePos=initX;
  rotatePos+=inc_pix;
  popStyle();
}



int ex1Pos;
int ex2Pos;
void exchangeDraw(float T,int section,color colour1 , color colour2) {
  background(0);

  float pix_second=(lengthPared/3)/T; 
  int inc_pix=round( pix_second/frameRate ); 
  //println ( section + " ex1Pos " + ex1Pos + "||| ex2Pos "+ ex2Pos +" ||| 1: "  +screen1Pos  +" |||| 2: "+  screen3Pos + "  incpix " + inc_pix + " pared: " +lengthPared);
  if(section==1){
    if((ex1Pos<screen2Pos) && (ex2Pos >screen1Pos)){
     // noStroke();
      //colour1=color(200,0,0);
     // fill(colour1);
     //movimiento izquierda a derecha
      tint(colour1);
      imageMode(CORNERS);
      image(maskImage,ex1Pos-10, initY-10,ex1Pos+10,initY+20);
      tint(colour2);
      image(maskImage,ex2Pos-10, initYTira2-10,ex2Pos+10,initYTira2+25);      
      ex1Pos+=inc_pix;
      ex2Pos-=inc_pix;
    }
  }
   if(section==2){
    if((ex1Pos<screen3Pos) && (ex2Pos >screen2Pos)){
       tint(colour1);
      imageMode(CORNERS);
      image(maskImage,ex1Pos-10, initY-10,ex1Pos+10,initY+20);
      tint(colour2);
      image(maskImage,ex2Pos-10, initYTira2-10,ex2Pos+10,initYTira2+25);     
      ex1Pos+=inc_pix;
      ex2Pos-=inc_pix;
    }
  }
  if(section==3){
    if( (ex1Pos>=screen3Pos && ex1Pos<=lengthPared+initX) || (ex1Pos>=0 && ex1Pos<screen1Pos) ){
       tint(colour1);
      imageMode(CORNERS);
      image(maskImage,ex1Pos-10, initY-10,ex1Pos+10,initY+20);
      tint(colour2);
      image(maskImage,ex2Pos-10, initYTira2-10,ex2Pos+10,initYTira2+25);   
      
      ex1Pos+=inc_pix;
      ex2Pos-=inc_pix;
      if(ex1Pos>=lengthPared+initX) ex1Pos=initX;
      if(ex2Pos<=initX) ex2Pos=lengthPared+initX;
    }
  }
  imageMode(CORNER);
  
}
void setupExchange(int section){
  if(section==1){
    ex1Pos=screen1Pos;
    ex2Pos=screen2Pos;
  }
  if(section==2){
    ex1Pos=screen2Pos;
    ex2Pos=screen3Pos;
  }
  if(section==3){
    ex1Pos=screen3Pos;
    ex2Pos=screen1Pos;
  }
}

void drawLine() {
  background(0);
  pushStyle();
    fill(255);
    
    //rect(initX+rotatePos, initY-3, 10, lengthTira+10);
    tint(10, 100, 10);
    noStroke();
    image(maskImageVert,initX+rotatePos-10, initY-3, 20, lengthTira+10);
  popStyle();
}



void noiseDraw() {
  frameRate(6);
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
      float ruido=random(120);
      if(ruido<100) fill(0);
      else fill(ruido);
      rect(initX+j*psize, initY+i*psize, psize, psize);
    }
  } 
  popStyle();
}
int explosionAngleInitValue=10;
void drawExplosionAngle(){
  println(explosionAngleInitValue);
   frameRate(6);
  background(0);
  int psize = 2;
  int x_count = 16; 
  int y_count = (lengthTira/psize);
  
  pushStyle();
  noStroke(); //commenting this out gives a neat effect

  //draw the "pixels"
  for (int i=0; i<=y_count; i++) {
    for (int j=explosionAngleInitValue/psize; j<=explosionAngleInitValue/psize+x_count; j++) {
      //pick a random grayscale value before drawing each rectangle. (thanks subpixel!)
      float ruido=random(120);
      if(ruido<100) fill(0);
      else fill(ruido,random(0,80),0);
      rect(initX+j*psize, initY+i*psize, psize, psize);
    }
  } 
  popStyle();
}

void drawRandomParticles(){
  background(0);
 for (int i=0; i<NUMPARTIC; i++){
   randomP[i].draw();
 //  println ("random partc " + i );
 } 
}


float phi =  0; // the phase shift offset amount
float amplitude = lengthTramo/2; // the amplitude of our wave
float frequency = 1.0/32; // slow down our wave so we can see it better
int numPoints = 30; // the granularity of our drawing
int nWaves=0;
color waveColor2, waveColor1, waveColor0=color(200,255,255);;
void waveDraw() {
  /*stroke(0, 40, 100);
  fill(0, 200);
  rect(0, 0, width, height);
  noFill();
  smooth();*/
  background(0);

  
  float x = 0;
  float y = 0;

  // if we consider 1 pixel to be 1 radian, then this is the number
  // of radians between points
  //float angle = lengthPared+initX/numPoints;probar esto!!!!!!
  float angle = (lengthPared+initX)/numPoints;
  smooth();
  imageMode(CENTER);
  colorMode(HSB);
  strokeWeight(3);
  for (int n=1; n<=min (nWaves, 3); n++) {
     pushMatrix();
     fill(255);
    if (n==1){ translate(initX, initY+ lengthTira/2); tint(waveColor0); }
    if (n==2){ translate(initX, initY+ lengthTramo/2);  tint(waveColor1); }
    if (n==3){  translate(initX, initY+ lengthTramo*2+ INTERTIRA+ lengthTramo/2);  tint(waveColor2); }

    //beginShape();
noStroke();

    for (int i = 0; i < numPoints; i++) {     
      // calculate where we are along x
      x = angle*i;     
      // y is determined by our function
      y = sin(x*frequency+phi+PI*n)* amplitude;
      //vertex(x, y, i, y);
      image(maskImageVertSmall,x,y);
      
    }
    //endShape();    
    popMatrix();
  }
  phi -= .10;
imageMode(CORNER);
colorMode(RGB);
}


void moveColor() {
}

void off() {
  background(0);
}

int loopCount=0;
void breath() {
  background(0);
  int temp = round(map(data[loopCount], 0, 170, 0, 200));
  fill(temp);
  rect(initX, initY, lengthPared, lengthTira);
  if (loopCount > data.length-2) {
    loopCount = 0;
  } else {
    loopCount++;
  }
}

float angle=0;
void drawGridImage(){
  pushMatrix();
  translate(initX+lengthPared/2,initY+lengthTira/2);
  rotate(angle);
  //
  tint(255,100);
   image(gridImg,-gridImg.width/2,-gridImg.width/2); 
  popMatrix();
  angle+=0.02;
   
}
void drawMovie() {

  background(0);
  fill(255, 100);
  image(movieExp, initX-10, initY-10, lengthPared*1.5, lengthTira*1.5);

}

float lengthFading=0;
float initFadingX=0;
float initFadingY=0;
float HueFading=100;
float fadingSpeed=1;
void drawFading(){
  background(0);
  imageMode(CORNERS);
  colorMode(HSB,255);
  tint(HueFading,200,160);
  if(fadingSpeed>0)
    image(maskImage2,initFadingX,initY+initFadingY,initFadingX+lengthFading,initFadingY+lengthTramo*1.8+initY);
  else{

    pushMatrix();
    scale(-1,1);
    translate(-400,0);
    //image(maskImage2,initFadingX-lengthFading,initY+initFadingY,lengthFading,lengthTramo*1.8);
    image(maskImage2,initFadingX,initY+initFadingY,initFadingX-lengthFading,initFadingY+lengthTramo*1.8+initY);
    popMatrix();
  }
  lengthFading+=fadingSpeed;
  if(abs(lengthFading)>200){
    initFadingX=random(0,lengthPared-50);
    lengthFading=0;
    float signo=random(-2,2);
    if(signo>0) fadingSpeed=8; else fadingSpeed=-8;
    initFadingY=random(0,lengthTira-lengthTramo);
    HueFading=random(70,150);
  }
  imageMode(CORNER);
  colorMode(RGB);
}

Ball[] myBall;
int ballAmount = 115;
int distance = 115;
int radius = 0;
void drawWeird1() {
  background(0);
  for (int i = 0; i < ballAmount; i++) {
    myBall[i].step();

    for (int j = 0; j < i; j++) {
      if (i != j) {
        float distBalls = dist(myBall[i].x, myBall[i].y, myBall[j].x, myBall[j].y);

        if (distBalls <= distance) {
          float lineWeight = 100/distBalls;
          stroke(lineWeight*100, lineWeight*100, lineWeight*100);
          strokeWeight(1);
          line(myBall[i].x, myBall[i].y, myBall[j].x, myBall[j].y);
        }

        if (distBalls <= radius) {
          myBall[i].setInc();
          myBall[j].setInc();
        }
      }
    }
  }
}
int bottomTopCounter=200;
void drawBottomTop(){
  tint(30,200,100);
 image(maskImageHlong,initX-10,bottomTopCounter,300,30);
  bottomTopCounter-=3;
 if(bottomTopCounter<-20){
 bottomTopCounter=200;
 }
  
}

void drawScreens(){
  stroke(0,255,0);
  line(screen1Pos,initY,screen1Pos,initY+lengthTira);
  line(screen2Pos,initY,screen2Pos,initY+lengthTira);
  line(screen3Pos,initY,screen3Pos,initY+lengthTira);
}

