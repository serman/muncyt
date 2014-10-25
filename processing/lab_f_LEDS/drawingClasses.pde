class randomParticle {
  int particlePosX;
  int particlePosY;
  color mcolor;
  float incr=0.1;
  void setup() {
     particlePosX=initX+(int)random(0,endX-20);
     particlePosY=initY+(int)random(0,lengthTira);
    colorMode(HSB);  
     mcolor=color(random(90,210), random(90,210), random(90,210));
    colorMode(RGB);
  }

  void draw() {
    pushStyle();
    noStroke(); 
     //  colorMode(HSB); 
  fill(255);   
      tint(mcolor);
      //rect(particlePosX, particlePosY, 5, 5);
      image(maskImage,particlePosX, particlePosY, 20, 5);
      particlePosX+=4*(noise(100+incr)-0.5);
      particlePosY+=4*(noise(100+incr)-0.5);
      incr+=0.01;
      if (particlePosX>endX) { 
        particlePosX=initX; 
        particlePosY=initY+(int)random(0,lengthTira);
        mcolor=color(random(100,200), 200, 200);
      }
      popStyle();
    }
}


class circles{
 int size=1;
 void draw(){
   size+=3;
   ellipseMode(CENTER);
   fill(100);
   ellipse(initX+lengthPared/2, initY+lengthTira/2,size,size);
   
   if(size>40){
     fill(0);
     ellipse(initX+lengthPared/2, initY+lengthTira/2,size-40,size-40);
   }
   if(size>80){
     fill(100);
     ellipse(initX+lengthPared/2, initY+lengthTira/2,size-80,size-80);
   }
   if(size>110){
     fill(0);
     ellipse(initX+lengthPared/2, initY+lengthTira/2,size-110,size-110);
   }
   
      if(size>140){
     fill(100);
     ellipse(initX+lengthPared/2, initY+lengthTira/2,size-140,size-140);
   }
   
   if(size>170){
     fill(0);
     ellipse(initX+lengthPared/2, initY+lengthTira/2,size-170,size-170);
   }
   
   if(size>210){
     fill(100);
     ellipse(initX+lengthPared/2, initY+lengthTira/2,size-210,size-210);
   }
   
   if (size>300) size=0;
 }
  
}

