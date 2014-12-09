import processing.event.KeyEvent;
import processing.event.KeyEvent;

import gab.opencv.*;
import java.awt.Rectangle;
import controlP5.*;
OpenCV opencv;

// ------------------------------------------------------------------------
// START Editable Parameters
// Cascade file
String cascadeFile = "haarcascade_frontalface_default.xml";

// Choose amount of white space around the image
int margin = 0;
// Color of the image background
color imgBgColor = color(200);

ControlP5 cp5;
// Interface elements

PFont font1, font2;
PFont courier;
// Logic
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
ArrayList<String> fotoFiles;//{"test640.jpg","test640_2.jpg","test640_3.jpg"}; //se reescribe con las nuevas fotos
int currentPhoto=0;
String path; // sketch path

PGraphics mgraphics; //haar features are drawn here

Rectangle[] faces; //faces detected in the "real algorightm"
ArrayList<Rectangle> fakeDetectedfaces; //faces detected with this fake algorithm
/******statuses ****/
boolean fileLoaded=false; //is xml haar file loaded
boolean isDebug=true;
boolean doProcess; //controls reproduction

boolean showInterface;

String statusMsg = "";
int idCur;

String photoFolderPath=System.getProperty("user.home")+"/fotoshaarviz";
File photoFolder;

int windowsize=100; //haar window size
int xposition=0; 
int yposition=0;
Rectangle mrect;
Rectangle faceRectangle;
int stagesCounter=0; //This variable is the counter used to know in which stage of the algorithm we are (0 to 26 for xxxfacexxx.xml)
int featuresCounter=0; //Each stage has a different number of features
//FAKE STUFF
int stopStage=20; //The algorithm takes too long for a full visualization. so we include a stop stage
int xAxeMovement=100; 
int yAxeMovement=100;
int slowOneOfEach= 1; 
float areaInterseccion=0;
PImage img2; //the image we represent on the right side
miniImages minimages;
PImage imagebg;
boolean amIOveraFace=false;
void setup() { 
  /******* inicializar arrays **************/
  fakeDetectedfaces=new ArrayList<Rectangle>();
  fotoFiles=new ArrayList<String>();  
  minimages= new miniImages();
  minimages.setup();
 /************************ inicio ***************/ 
  size(1280, 1024,P2D);
  cp5 = new ControlP5(this);
  /*** OPENCV STUFF ***/
  
  photoFolder = new File(System.getProperty("user.home")+"/fotoshaarviz");  
  listFilesForFolder(photoFolder);
  //println(System.getProperty("user.home")+"/fotoshaarviz"); 
  
  currentPhoto=0;  
  if(loadNewPhoto(fotoFiles.get(currentPhoto))==false) refresh();  
  setInitPositions();
  mrect.x=xposition;
  mrect.y=yposition;
  
  if (isDebug){  
    println("[main] using cascade: " + cascadeFile);
    cp5.addButton("bAll")
        .setPosition(marginL, marginT+50)
          .setSize(200, 19).setCaptionLabel("RENDER ALL STAGES")
            ;
    cp5.addButton("refresh")
        .setPosition(marginL, marginT+100)
          .setSize(200, 19).setCaptionLabel("refresh")
            ;
  }
  cp5.hide();
  path = sketchPath; // an environment variable 
  
  smooth();

  font1 = createFont("Helvetica-Light", 21);
  font2 = createFont("Helvetica", 11);
  courier= createFont("courier",15);
  mgraphics= createGraphics(100,100,P2D);

  setupCurrentIteration();
  frameRate(30);
  
  if (isDebug==false) doProcess=true;
  else{
    yposition+=yAxeMovement;yposition+=yAxeMovement;//yposition+=yAxeMovement;yposition+=yAxeMovement;
    xposition+=xAxeMovement;
  }
  imagebg=loadImage("background.jpg");
  doProcess=true;
} //end setup



void draw() {
 // println(xposition);
  //actualizacion del rectangulo
    if(stagesCounter>stopStage){ //fin de una etapa toca mover cuadradito
      stagesCounter=0;
      featuresCounter=0;
      xposition+=xAxeMovement;
      if(xposition>img.width-windowsize){ //row jump needed
        xposition=0;
        yposition+=yAxeMovement;
      }
      if(yposition>img.height-windowsize){ //current image has finished
        refresh(); 
        setInitPositions();
      }      
      mrect.x=xposition;
      mrect.y=yposition;
      setupCurrentIteration();
    }

  // Draw
  background(255);
  image(imagebg,0,0,width, height);

  if(frameCount%40==0) slowMotion(false );
   if (doProcess && (frameCount% slowOneOfEach)==0) {
     renderStages(windowsize);
   }
  /****DRAW MAIN PHOTO****/
  pushMatrix();
    translate(740,80);
    image(img, 0, 0);
    noFill();
    stroke(0, 255, 0,10);
    strokeWeight(2);
    for (int i = 0; i < faces.length; i++) {
      rect(faces[i].x, faces[i].y, faces[i].width, faces[i].height);
    }

  //if(frameCount%30==0)
    stroke(0, 255, 0,255);
    strokeWeight(2);
    for (int i = 0; i< fakeDetectedfaces.size(); i++) {
      rect(fakeDetectedfaces.get(i).x, fakeDetectedfaces.get(i).y, fakeDetectedfaces.get(i).width, fakeDetectedfaces.get(i).height);
    }
    
    image(mgraphics,xposition,yposition); //dibuja el canvas con las features haar
  
  
  
    stroke(255,0,0);
    rect(mrect.x,mrect.y,mrect.width,mrect.height);
        

  popMatrix();
 /************/

// Draw zoomed version   
  pushMatrix();  
    translate(55,391);
    img2.filter(GRAY);
    image(img2,0,0,min(faceRectangle.width*2,250),min(faceRectangle.height*2,250));    
    image(mgraphics,0,0,min(faceRectangle.width*2,250),min(faceRectangle.height*2,250));
  popMatrix();
  
if(showInterface){
  int yPos = marginT;
  int xPos = marginL;
  yPos+= 20;
  textFont(font2);
  fill(60);
  text("\"" + cascadeFile + "\"", xPos, yPos);
  fill(255, 0, 0 );
  yPos += 15;
 
}
  textFont(courier);
  fill(255,255,255);
  text(statusMsg, 376, 428);
 /* textFont(font1);
  fill(20);
  text(" El sistema recorre toda la imagen \n buscando patrones de luminosidad \n cada vez en mas detalle ",xPos,yPos+250);
  text(" Compara la intensidad de color bajo las zonas \n blancas y negras y busca que la diferencia \n supere cierto umbral",xPos,yPos+350);
  text(" Para un ordenador, una cara humana sólo es un conjunto \n de contrastes dentro de una imagen",xPos,yPos+460);
 */
  if(amIOveraFace){ 
    if( stagesCounter==10 && (featuresCounter==2 || featuresCounter==6)){
      minimages.addImage(img2,mgraphics);
    }
    if( stagesCounter==11 && (featuresCounter==6 ||  featuresCounter==37) ) {
      minimages.addImage(img2,mgraphics); 
    }
  } 
  
  
  pushMatrix();
  translate(746,836);
  minimages.draw();
  popMatrix();
  fill(80);
  if (isDebug==true && showInterface)  debugInfo();
 
}
void debugInfo(){
  textFont(font2);
  pushMatrix();
  fill(255);
  translate(100,200);
   text("framerater+ " + frameRate, 0, 00);
  text("area interseccion" + areaInterseccion,0,20);
  text("faceRectangle size" + windowsize + " face on screen width:" + faces[0].width ,0,40);  
  popMatrix();
}

boolean loadNewPhoto(String name){
  img = loadImage(photoFolderPath+"/"+name);
  opencv = new OpenCV(this, img);  
  opencv.loadCascade(cascadeFile);
 // println("despues de opencv" ); 
  faces = opencv.detect(1.4, 3, 0,100,250);  //(scale, min_neighbors, flags, min_width, min_height);
  if( faces.length<1 ) return false;
 // println("despues de opencv 2" );  
  
  faceRectangle=faces[0];
 // println("despues de opencv 3" ); 
  faces = opencv.detect(1.4,0,0,100,250);
 // println("despues de opencv 4" ); 
  mrect=new Rectangle(0,0,100,100);
 // println("despues de opencv 5" );
  return true;
  

}

public void listFilesForFolder(final File folder) {
  fotoFiles.clear();
    for (final File fileEntry : folder.listFiles()) {
        if (fileEntry.isDirectory()) {
            listFilesForFolder(fileEntry);
        } else {
         
        String extension = "";    
        String filename  = fileEntry.getName();    
          int i = filename.lastIndexOf('.');
          if (i > 0) {
              extension = filename.substring(i+1);
            //  System.out.println("exten "+ extension);
          }
          if(extension.equals("png") || extension.equals("jpg") ){
            fotoFiles.add(fileEntry.getName());
            System.out.println(fileEntry.getName());
          }
        }
    }
}

//loads new image and define how fast the rectangles will move
void refresh(){  
  currentPhoto++;
  println("refrech" +  currentPhoto); 
  if(currentPhoto>=fotoFiles.size()){    
    listFilesForFolder(photoFolder);
    currentPhoto=0;
  }
  if(loadNewPhoto(fotoFiles.get(currentPhoto)) == true ){  //TODO TAKE NEW PHOTO FROM LISTS OR WHATEVER
    fakeDetectedfaces.clear();
    xposition=0;
    yposition=0;
    //setupCurrentIteration();
    minimages.reset();
  }else{
    refresh();
  }
  
}

void setInitPositions(){
  if(faceRectangle.y >100) {
    xAxeMovement=faceRectangle.x/2;
    yAxeMovement=faceRectangle.y/2;
  }
  else{  
      yAxeMovement=100;  
      yposition=faceRectangle.y; 
      xposition=0;
      xAxeMovement=faceRectangle.x/2; 
  }
}

//set what happens when the finding window is over one position of the screen.
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
  stopStage= ceil(map(areaInterseccion,0,1,1,9));
  if(areaInterseccion>0.94) { //This iteration is over a face
    stopStage=16;
    fakeDetectedfaces.add(new Rectangle( mrect));
    amIOveraFace=true;
  }
  else amIOveraFace=false;
  
  if(stopStage>10) stagesCounter=stopStage-10;   
   img2=createImage(mrect.width,mrect.height,RGB); 
   img2=img.get( mrect.x, mrect.y, mrect.width,mrect.height);
   img2.updatePixels();
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
      //statusMsg = "[main] Processing tree # " + j + " in stage " + (i+1);
      statusMsg = "ETAPA " + (i+1) + " TEST: " + j + "/"+stage.getNumItems();
     // redraw();
        //mgraphics.clear();
        mgraphics.beginDraw();
        mgraphics.clear();
     //  mgraphics.rect(0,0,100,100);
       mgraphics.endDraw();
       tree.render(imgSize, stageScale, 1,mgraphics);
      
    
    if(featuresCounter<stage.getNumItems() -1) {
      featuresCounter++;
      //println (featuresCounter);
    }
    else{
      featuresCounter=0;      
      if(stagesCounter<=stages.size()-1){ 
        stagesCounter++;      
       // println("cambio a la etapa "+ stagesCounter);
     }
      else{
          //TODO final del proceso
          exit();
      }
    }
}

boolean slowmoDOWN=false;
void slowMotion(boolean start){
//  println("startslowmo" + slowOneOfEach);
  if(start) slowOneOfEach=2;
  
  if (slowOneOfEach > 1 ){    
    if (slowmoDOWN==false){
      if(slowOneOfEach<7)      slowOneOfEach++;
      else {slowmoDOWN=true;}
    }  else if(slowmoDOWN==true){
      slowOneOfEach--;
    }
  }
  
  if(slowmoDOWN==true){
     if( slowOneOfEach <=1) { 
       slowmoDOWN=false;
     println("end slowmodown");
     }
  }
} 

//carga archivo haar
void loadFile() {
  String name = cascadeFile;
  String path = sketchPath +"/";
  println(path+name);
  stages = doLoadXML(path+name);
  String[] pieces = split(name, '.');
  name = pieces[0];

 fileLoaded=true;
}

void keyPressed( ){
    
  if(key== 'g'  ){
    println("key pressed");
    showInterface =! showInterface;
    if(showInterface) cp5.setAutoDraw(true);
    else cp5.setAutoDraw(false);
  }if(key== 's' ){
   slowMotion(true); 
  }
  
}
