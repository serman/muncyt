void drawTest(){
float dotSize = width * 0.2;
  image(dot, mouseX - dotSize/2, mouseY - dotSize/2, dotSize, dotSize);

}

int rotatePos=initX;
void rotateDraw(float T){
  //freq= rotaciones/seg;
  //T = periodo en segundos
  float pix_second=lengthPared/T;  
  int inc_pix=round( pix_second/frameRate ); 
  pushStyle();
    fill(0,200,000);
    noStroke();
    rect(rotatePos,initY,3,lengthTira);
    if(rotatePos>endX) rotatePos=initX;
    rotatePos+=inc_pix;
  popStyle();
}
    
      
void noiseDraw(){
  int psize = 4;
   int x_count = (lengthPared/psize); 
   int y_count = (lengthTira/psize);
   pushStyle();
    noStroke(); //commenting this out gives a neat effect
   
    //draw the "pixels"
    for(int i=0; i<=y_count; i++){
      for(int j=0; j<=x_count; j++){
        //pick a random grayscale value before drawing each rectangle. (thanks subpixel!)
        fill(int(random(200)));
        rect(initX+j*psize, initY+i*psize, psize, psize);
      }
    } 
  
  popStyle();
  
}


void randomParticleDraw(){
  
}

void waveDraw(){
  
}


void moveColor(){
  
}

void off(){
  
}

void breath(){

}
  
  
