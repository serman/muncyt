class randomParticle {
  int particlePosX;
  int particlePosY;
  color mcolor;

  void setup() {
     particlePosX=initX+(int)random(0,endX-20);
     particlePosY=initY+(int)random(0,lengthTira);
    colorMode(HSB);  
     mcolor=color(random(100,200), 200, 200);
    colorMode(RGB);
  }

  void draw() {
    pushStyle();
    noStroke(); 
     //  colorMode(HSB);    
      fill(mcolor);
      rect(particlePosX, particlePosY, 5, 10);
      particlePosX+=random(5, 8);
      particlePosY+=random(-2, 3);
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

