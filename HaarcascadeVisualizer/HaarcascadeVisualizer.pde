
import gab.opencv.*;
import java.awt.Rectangle;
OpenCV opencv;

// ------------------------------------------------------------------------
// START Editable Parameters
// Cascade file
String cascadeFile = "haarcascade_frontalface_default.xml";

// Choose amount of white space around the image
int margin = 0;
// Color of the image background
color imgBgColor = color(200);


import java.awt.Rectangle;
import controlP5.*;
ControlP5 cp5;
// Interface elements


PFont font1, font2;

// Logic
boolean doProcess;

String statusMsg = "";
int idCur;

// GUI

int marginL = 40;
int marginT = 60;

// Graphics
int[] sampleSize = new int[2];
int minSize = 20;
PImage img, resizedImg;
//int rows, cols;
int imgSize;

// File I/O
ArrayList stages;
float stageScale;
String[] xmlFiles;

String path; // sketch path

PGraphics mgraphics;

Rectangle[] faces;
/******statuses ****/
boolean fileLoaded=false;
int windowsize=100; //haar window size
int xposition=0;
int yposition=0;
Rectangle mrect;
Rectangle faceRectangle;
int stagesCounter=0;
int featuresCounter=0;
int stopStage=20;
int xAxeMovement=100;
int yAxeMovement=100;
PImage img2;
void setup() { 
  size(1280, 720,P2D);
  cp5 = new ControlP5(this);
  /*** OPENCV STUFF ***/
  loadNewPhoto("test640.jpg");
  
  println("[main] using cascade: " + cascadeFile);
  cp5.addButton("bAll")
      .setPosition(marginL, marginT+50)
        .setSize(200, 19).setCaptionLabel("RENDER ALL STAGES")
          ;
cp5.addButton("refresh")
      .setPosition(marginL, marginT+100)
        .setSize(200, 19).setCaptionLabel("refresh")
          ;
  path = sketchPath; // an environment variable
  


  smooth();

  font1 = createFont("Helvetica-Light", 25);
  font2 = createFont("Helvetica", 11);
  mgraphics= createGraphics(100,100,P2D);
  setupCurrentIteration();
  frameRate(60);
}
float areaInterseccion=0;
void draw() {
  
    if(stagesCounter>stopStage){ //fin de una etapa toca mover cuadradito
      stagesCounter=0;
      featuresCounter=0;
      xposition+=xAxeMovement;
      if(xposition>img.width-windowsize){
        xposition=0;
        yposition+=yAxeMovement;
      }
      if(yposition>img.height-windowsize){
        yposition=0; 
        //TODO hemos terminado!  
      }
      mrect.x=xposition;
      mrect.y=yposition;
      setupCurrentIteration();
    }

  // Draw
  background(255);


  if (doProcess) {
    renderStages(windowsize);
  }
  /********/
  pushMatrix();
    translate(800,0);
    image(img, 0, 0);
    noFill();
    stroke(0, 255, 0,50);
    strokeWeight(2);
    for (int i = 0; i < faces.length; i++) {
      rect(faces[i].x, faces[i].y, faces[i].width, faces[i].height);
    }
  //if(frameCount%30==0)
    image(mgraphics,xposition,yposition); //dibuja el canvas con las features haar
  
    stroke(255,0,0);
    rect(mrect.x,mrect.y,mrect.width,mrect.height);
  popMatrix();
 /************/
   
  pushMatrix();
  
    translate(400,20);
    scale(2);
    image(img2,0,0);
    img2.filter(GRAY);
    image(mgraphics,0,0);
  popMatrix();
  

  int yPos = marginT;
  int xPos = marginL;
  yPos+= 20;
  textFont(font2);
  fill(60);
  text("\"" + cascadeFile + "\"", xPos, yPos);
  fill(255, 0, 0 );
  yPos += 15;
  text(statusMsg, xPos, yPos);
  
  textFont(font1);
  fill(20);
  text("El sistema recorre toda la imagen \n buscando patrones de luminosidad \n cada vez en mas detalle ",xPos,yPos+300);
  text("Compara la intensidad de color bajo las zonas \n blancas y negras y busca que la diferencia \n supere cierto umbral",xPos,yPos+500);

  fill(80);
  debugInfo();
 
}
void debugInfo(){
  textFont(font2);
  pushMatrix();
  translate(100,20);
   text("framerater+ " + frameRate, 0, 00);
  text("area interseccion" + areaInterseccion,0,20);
  text("faceRectangle size" + windowsize + " face on screen width:" + faces[0].width ,0,40);  
  popMatrix();
}

void loadNewPhoto(String name){
  opencv = new OpenCV(this, name);  
  opencv.loadCascade(OpenCV.CASCADE_FRONTALFACE); 
  faces = opencv.detect(1.4,3,0,100,250);
  faceRectangle=faces[0];
  faces = opencv.detect(1.4,4,0,100,250);
  mrect=new Rectangle(0,0,100,100);
  img = loadImage(name);

}

void refresh(){
loadNewPhoto("test640_2.jpg");
setupCurrentIteration();
}
void setupCurrentIteration(){    
  windowsize=faceRectangle.width;
  if(mgraphics.width!=windowsize)  mgraphics = createGraphics(windowsize,windowsize,P2D);
  mrect.width=windowsize;
  mrect.height=windowsize;
  if(mrect.intersects(faceRectangle)){
    Rectangle interseccion=mrect.intersection(faceRectangle);
    areaInterseccion=(float) (interseccion.width*interseccion.height/2) / (float)(windowsize*windowsize/2); //normalized intersection area
  } 
  else{
    areaInterseccion=0;
  }
  /*** stop  Stage ***/
  stopStage= ceil(map(areaInterseccion,0,1,1,13));
  if(areaInterseccion>0.94) stopStage=18;
  if(stopStage>8) stagesCounter=stopStage-8;
  
  xAxeMovement=faceRectangle.x/2;
  yAxeMovement=faceRectangle.y/2;
    println(mrect.x);
  println(mrect.y);
  println(mrect.width);
   img2=createImage(mrect.width,mrect.height,RGB); 
   img2=img.get( mrect.x, mrect.y, mrect.width,mrect.height);
  img2.updatePixels();
  println("actualizado");
}

public void  bAll(){
  if(doProcess)
  doProcess = false;
   else doProcess = true;
}

void renderStages(int imgSize) {  
 
  if(fileLoaded==false)
    loadFile();
 // imgSize = img.width;//calucated from XML file
  minSize = sampleSize[0];
  stageScale = (float)imgSize/(float)minSize;
     int i=stagesCounter;
    Stage stage = (Stage) stages.get(i);
   
    //println("trees in this stage" + stage.getNumItems() );
    // Render stage
      int j=featuresCounter; 
      Tree tree = stage.getTree(j);
     // println("[main] Processing tree # " + j + " in stage " + (i+1));
      statusMsg = "[main] Processing tree # " + j + " in stage " + (i+1);
     // redraw();
        //mgraphics.clear();
        mgraphics.beginDraw();
        mgraphics.clear();
     //  mgraphics.rect(0,0,100,100);
       mgraphics.endDraw();
       tree.render(imgSize, stageScale, 1,mgraphics);
      
    
    if(featuresCounter<stage.getNumItems() -1) {
      featuresCounter++;
      println (featuresCounter);
    }
    else{
      featuresCounter=0;      
      if(stagesCounter<=stages.size()-1){ 
        stagesCounter++;      
        println("cambio a la etapa "+ stagesCounter);}
      else{
          //TODO final del proceso
          exit();
      }
    }
}

void slowMotion(){

}
void loadFile() {
  String name = cascadeFile;
  String path = sketchPath +"/";
  println(path+name);
  stages = doLoadXML(path+name);
  String[] pieces = split(name, '.');
  name = pieces[0];

 fileLoaded=true;
}
