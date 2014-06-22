class miniImages{
  int initX;
  int initY;
  int imgWidth;
  int imgHeight;
  int maxImages;
  ArrayList <PImage> images;
 
 miniImages(){
     initX=50;
     initY=620;
     imgWidth=100;
     imgHeight=imgWidth;
      maxImages=10;
 }
 
 void setup(){
   images = new ArrayList<PImage> ();
 }
 
 
 void reset(){
   images.clear();
   
 }
 
 void addImage(PImage mImg, PGraphics mImg2){
   if(images.size()>maxImages) return;
   
   PGraphics mgraphic=createGraphics(imgWidth,imgHeight,P2D);
   mgraphic.beginDraw();
   mgraphic.image(mImg,0,0,imgWidth,imgHeight);
   mgraphic.image(mImg2,0,0,imgWidth,imgHeight);
   mgraphic.endDraw();
   images.add(mgraphic );
   println("images size" + images.size());
 }
 
 void draw(){
   for(int i=0; i< images.size(); i++){
    // image(images.get(i),50+ i*imgWidth,initY,imgWidth,imgHeight);   
     image(images.get(i),50+ i*(imgWidth+10),initY,imgWidth,imgHeight);   
   } 
  
  }

}
